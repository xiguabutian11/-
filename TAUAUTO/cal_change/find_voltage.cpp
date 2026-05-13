#include "find_voltage.h"

double BEST::bestvoltage2(double V, double fre, double I, double Vjiange) {

    datachange::changecalsetting("i", I);
    datachange::changecalsetting("frequency", fre);

    // 分段生成电压序列
    double lowV1 = V - 400;
    double highV1 = V + 400;
    double step1 = 50;

    double lowV2 = highV1;
    double highV2 = V + 5000;
    double step2 = (highV2 - lowV2) / Vjiange;

    // 组装完整电压数组
    std::vector<double> voltageList;
    for (double voltage = lowV1; voltage <= highV1; voltage += step1)
    {
        voltageList.push_back(voltage);
    }
    for (double voltage = lowV2 + step2; voltage <= highV2; voltage += step2)
    {
        voltageList.push_back(voltage);
    }

    // 一次性写入电压数组 批量扫参
    datachange::changecalsetting_array("v1", voltageList);

    filesystem::path projectPath = Projectpath;
    usrData& data = usrData::getInstance();

    if (!projManage::openProj(projectPath.string())) {
        for (auto& msg : data.curCalGroup.message) {
            std::cerr << msg.str << std::endl;
        }
        return V;
    }

    // 只计算一次 批量出所有扫参结果
    calculation::seqCalculate();
    calculation::waitForAllTasks();

    std::vector<SimulationResult> firstResults;

    // 读取结果：从 res.params["v1"] 取真实绑定电压，杜绝顺序错乱
    for (auto& seq : data.curCalGroup.res.reses) {
        for (auto& res : seq.second) {
            SimulationResult result;
            // 关键：取当前结果绑定的真实v1，不依赖遍历顺序
            result.voltage = res.params["v1"];
            result.frequency = res.freqy.freq;
            result.inputPower = res.freqy.pin;

            if (!res.result.Pout.empty() && !res.result.Pout[0].empty()) {
                result.outputPower = res.result.Pout[0].back();
                result.avg = 10 * log10(result.outputPower / result.inputPower);
            }
            else {
                result.outputPower = -1;
                result.avg = -1;
            }
            firstResults.push_back(result);
        }
    }

    // ========== 新增：按电压 从小到大 排序 ==========
    std::sort(firstResults.begin(), firstResults.end(),
        [](const SimulationResult& a, const SimulationResult& b) {
            return a.voltage < b.voltage;
        });

    /*===================== 有序输出 =====================*/
    std::cout << "\n\n=============== 扫压结果（按电压升序）===============" << std::endl;
    std::cout << std::left << std::setw(10) << "电压(V)"
        << std::setw(12) << "频率(GHz)"
        << std::setw(12) << "输入功率(W)"
        << std::setw(12) << "输出功率(W)"
        << std::setw(12) << "增益(db)"
        << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (const auto& result : firstResults) {
        if (result.outputPower >= 0) {
            std::cout << std::left << std::setw(10) << result.voltage
                << std::setw(12) << result.frequency
                << std::setw(12) << result.inputPower
                << std::setw(12) << result.outputPower
                << std::setw(12) << result.avg
                << std::endl;
        }
    }

    std::vector<SimulationResult> BestPoints;
    if (!firstResults.empty()) {
        auto bestIt = max_element(firstResults.begin(), firstResults.end(),
            [](const auto& a, const auto& b) { return a.outputPower < b.outputPower; });

        BestPoints.push_back(*bestIt);
    }

    cout << "\n============== 输出功率最大的电压 ==============" << endl;
    cout << left << setw(10) << "电压(V)"
        << setw(12) << "频率(GHz)"
        << setw(12) << "输入功率(W)"
        << setw(12) << "输出功率(W)"
        << setw(12) << "增益(dB)"
        << endl;
    cout << "------------------------------------------------" << endl;

    double bestV = V;
    for (const auto& point : BestPoints) {
        cout << setw(10) << point.voltage
            << setw(12) << point.frequency
            << setw(12) << point.inputPower
            << setw(12) << point.outputPower
            << setw(12) << point.avg
            << endl;
        datachange::changecalsetting("v1", point.voltage);
        bestV = point.voltage;
    }

    return bestV;
}

double BEST::bestfre()
{
    std::vector<FreqResult> freqResults;

    for (double frequency : {minfre, maxfre, (minfre + maxfre) / 2.0}) {
        datachange::changecalsetting("frequency", frequency);

        filesystem::path projectPath = Projectpath;
        usrData& data = usrData::getInstance();

        if (!projManage::openProj(projectPath.string())) {
            for (auto& msg : data.curCalGroup.message) {
                std::cerr << msg.str << std::endl;
            }
            continue;
        }

        // 执行计算
        calculation::seqCalculate();
        calculation::waitForAllTasks();

        // 获取当前频点结果
        for (auto& seq : data.curCalGroup.res.reses) {
            for (auto& res : seq.second) {
                if (!res.result.Pout.empty()) {
                    for (auto& pout : res.result.Pout[0]) {
                        std::cout << pout << " ";
                    }
                    std::cout << std::endl;
                    const std::vector<double>& poutArray = res.result.Pout[0];
                    int lastIndex = poutArray.size() - 1;

                    // 找到最大值和位置
                    auto maxIt = std::max_element(poutArray.begin(), poutArray.end());
                    double maxPower = *maxIt;
                    int maxPos = std::distance(poutArray.begin(), maxIt);

                    // 判断是否过饱和：最大值不在最后一个点就是过饱和
                    bool isOverSaturated = (maxPos != lastIndex);

                    freqResults.push_back({ frequency, maxPower, maxPos, isOverSaturated });
                }
            }
        }
    }

    // 分析频点数据
    double bestFrequency = 0;
    
    if (!freqResults.empty()) {
        // 分离过饱和和不过饱和的频点
        std::vector<FreqResult> overSaturated;
        std::vector<FreqResult> notSaturated;

        for (const auto& res : freqResults) {
            if (res.isOverSaturated) {
                overSaturated.push_back(res);
            }
            else {
                notSaturated.push_back(res);
            }
        }

        // 决策逻辑
        if (!notSaturated.empty()) {
            // 存在不过饱和的频点，取最大值最小的
            auto bestIt = std::min_element(notSaturated.begin(), notSaturated.end(),
                [](const auto& a, const auto& b) { return a.maxPower < b.maxPower; });
            bestFrequency = bestIt->frequency;
        }
        else {
            // 都过饱和，取最大值位置最大的（离终点最远）
            auto bestIt = std::max_element(overSaturated.begin(), overSaturated.end(),
                [](const auto& a, const auto& b) { return a.maxPosition < b.maxPosition; });
            bestFrequency = bestIt->frequency;
        }
    }

    // 输出结果
    std::cout << "\n============== 频点扫描结果 ==============" << std::endl;
    for (const auto& res : freqResults) {
        std::cout << "频点: " << res.frequency << "GHz, "
            << "最大功率: " << res.maxPower << "W, "
            << "位置: " << res.maxPosition << ", "
            << (res.isOverSaturated ? "过饱和" : "不过饱和") << std::endl;
    }

    std::cout << "\n最终选择频点: " << bestFrequency << " GHz" << std::endl;

    return bestFrequency;
}

