#include "TIAOBIAN_youhua.h"


vector<SweepData> fre_sweep(double dot_numble, double work_V, L_YOUHUA L)
{
    vector<SweepData> sweepResults;

    // 生成所有频率点数组
    vector<double> freList;
    double jiange = (maxfre - minfre) / (dot_numble - 1);
    for (int i = 0; i < dot_numble; i++)
    {
        double fre = minfre + i * jiange;
        freList.push_back(fre);
    }

    // 批量并行寻找最优Pin（一次仿真跑所有频率）
    vector<SaturationResult> batchResults = best_pin2(freList, work_V, L.optimalPin, L.tubeLength);

    sort(batchResults.begin(), batchResults.end(), [](const SaturationResult& a, const SaturationResult& b) {
        return a.workfre < b.workfre;
        });

    // 组装最终输出
    for (auto& res : batchResults)
    {
        double now_gain = 10 * log10(res.maxOutputPower / res.optimalPin);
        sweepResults.push_back({ res.workfre, now_gain, res.maxOutputPower });
    }

    return sweepResults;
}

vector<double> Get_speed(double fre, double pin, double voltage)
{
    vector<double> speed;
    datachange::changecalsetting("frequency", fre);
    datachange::changecalsetting("pin", pin);
    datachange::changecalsetting("v1", voltage);

    filesystem::path projectPath = Projectpath;
    usrData& data = usrData::getInstance();

    // -------------- 执行计算 --------------
    if (!projManage::openProj(projectPath.string())) {
        for (auto& msg : data.curCalGroup.message) {
            cerr << msg.str << endl;
        }
    }

    calculation::seqCalculate();
    calculation::waitForAllTasks();

    // -------------- 只提取 speed --------------
    for (auto& seq : data.curCalGroup.res.reses) {
        for (auto& res : seq.second) {
            if (!res.result.speed.empty()) {
                speed = res.result.speed;
            }
        }
    }

    // -------------- 返回速度数组 --------------
    return speed;
}

vector<SpeedPoint> Speed_need(const vector<double>& speed, double endcut_percent, int need)
{
    vector<SpeedPoint> filtered;
    int total = (int)speed.size();

    if (total < 10)
        return filtered;

    // 1. 截取 endcut 之后的段
    vector<SpeedPoint> after_cut;
    for (int i = 0; i < total; ++i)
    {
        double pos = (double)i / (total - 1);
        if (pos > endcut_percent)
        {
            after_cut.push_back({ i, speed[i] });
        }
    }

    // 打印截取后的点
    int M = (int)after_cut.size();
    for (int i = 0; i < M; i++)
    {
        std::cout << "Point: " << after_cut[i].index << ", Speed: " << after_cut[i].value << std::endl;
    }

    if (M < 2)
        return after_cut;

    // 2. 计算所有差值，求全局平均
    vector<double> delta;
    double sum_delta = 0;

    for (int i = 0; i < M - 1; ++i)
    {
        double d = after_cut[i].value - after_cut[i + 1].value;
        delta.push_back(d);
        sum_delta += d;
    }

    double avg_delta = sum_delta / delta.size();
    double threshold = avg_delta * 0.5;    // 全局平均值的一半

    // 3. 找第一个超过阈值的位置（下降沿开始）
    int local_start = 0;
    for (int i = 0; i < (int)delta.size(); ++i)
    {
        if (delta[i] > threshold)
        {
            local_start = i + 1;
            break;
        }
    }

    // 4. 从开始点向后取
    for (int i = local_start; i < M; ++i)
    {
        filtered.push_back(after_cut[i]);
    }

    // 5. 均匀取 need 个点
    vector<SpeedPoint> result;
    int N = (int)filtered.size();

    if (N <= need)
        return filtered;

    int interval = N / need;
    for (int k = 0; k < need; ++k)
    {
        int idx = k * interval;
        if (idx < N)
            result.push_back(filtered[idx]);
    }

    return result;
}


void plotWithPython(const vector<SweepData>& data, int index)
{
    
    CreateDirectoryA("tiaobian_try", NULL);

    
    char filename[256];
    sprintf_s(filename, sizeof(filename), "tiaobian_try/sweep_data_%d.txt", index);

    ofstream f(filename);
    for (const auto& d : data) {
        f << d.fre << " " << d.gain << " " << d.outpower << endl;
    }
    f.close();

    
    string cmd = "python " + TAUAUTO_PATH + "/plot_sweep.py ";
    cmd += filename;
    cmd += " ";
    cmd += to_string(index);

    system(cmd.c_str());
}


SweepStat calcSweepStat(const vector<SweepData>& data)
{
    SweepStat st{};
    int n = (int)data.size();

    if (n == 0) return st;

    // 最大值
    st.gain_max = data[0].gain;
    st.power_max = data[0].outpower;

    for (int i = 1; i < n; ++i)
    {
        if (data[i].gain > st.gain_max)
            st.gain_max = data[i].gain;
        if (data[i].outpower > st.power_max)
            st.power_max = data[i].outpower;
    }

    // 均值
    double sum_g = 0, sum_p = 0;
    for (int i = 0; i < n; ++i)
    {
        sum_g += data[i].gain;
        sum_p += data[i].outpower;
    }
    st.gain_mean = sum_g / n;
    st.power_mean = sum_p / n;

    // 方差、标准差
    double sum_g2 = 0, sum_p2 = 0;
    for (int i = 0; i < n; ++i)
    {
        double dg = data[i].gain - st.gain_mean;
        sum_g2 += dg * dg;

        double dp = data[i].outpower - st.power_mean;
        sum_p2 += dp * dp;
    }

    st.gain_var = sum_g2 / n;
    st.gain_std = sqrt(st.gain_var);

    st.power_var = sum_p2 / n;
    st.power_std = sqrt(st.power_var);

    return st;
}


double tiaobian_score(const vector<SweepData>& junyun, const vector<SweepData>& tiaobian)
{
    int size = junyun.size();
    SweepStat junyun_stat = calcSweepStat(junyun);
    SweepStat tiaobian_stat = calcSweepStat(tiaobian);

    // ==================== 功率相对偏差 ====================
    double cha_power_max = (tiaobian_stat.power_max - junyun_stat.power_max) / junyun_stat.power_max;
    double cha_power_mean = (tiaobian_stat.power_mean - junyun_stat.power_mean) / junyun_stat.power_mean;
    double cha_power_var = (tiaobian_stat.power_var - junyun_stat.power_var) / junyun_stat.power_var;
    double cha_power_std = (tiaobian_stat.power_std - junyun_stat.power_std) / junyun_stat.power_std;

    // ==================== 增益相对偏差 ====================
    double cha_gain_var = (tiaobian_stat.gain_var - junyun_stat.gain_var) / junyun_stat.gain_var;
    double cha_gain_std = (tiaobian_stat.gain_std - junyun_stat.gain_std) / junyun_stat.gain_std;

    // 综合评分
    double score =
        1 * cha_power_max
        + 1 * cha_power_mean
        //- 0.01 * cha_power_var
        - 0.01 * cha_power_std
        //- 0.01 * cha_gain_var
        - 0.01 * cha_gain_std;

    return score;
}