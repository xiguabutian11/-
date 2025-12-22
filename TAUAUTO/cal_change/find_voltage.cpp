#include "find_voltage.h"



double BEST::bestvoltage3(double V,double fre, double I, double Vjiange) {

    datachange::changecalsetting("i", I);
    datachange::changecalsetting("frequency", fre);
	double minVguess = V - 600;      //电压扫描范围下限
	double maxVguess = V + 600;      //电压扫描范围上限
    double jiange = (maxVguess - minVguess) / Vjiange;                //采集20个点在电压范围内

    // 存储所有结果的容器
    std::vector<SimulationResult> firstResults;
    std::vector<SimulationResult> BestPoints;


    for (double voltage = minVguess; voltage <= maxVguess; voltage += jiange)             //电压扫描
    {
		std::cout << "扫描电压范围" << minVguess << "V-" << maxVguess << "V" << std::endl;
        datachange::changecalsetting("v1", voltage);   //修改calsetting中的电压参数
        std::cout << "扫描电压: " << voltage << " V..." << std::endl;        //产生电压序列，上下限200步长
        double M1 = Me * Vc * Vc / (Me * Vc * Vc + voltage * e);
        double Ve = Vc * std::pow((1 - M1 * M1), 0.5);


        
        //-------------------------计算代码-----------------------
        filesystem::path projectPath = Projectpath;
        usrData& data = usrData::getInstance();

        if (!projManage::openProj(projectPath.string())) {
            for (auto& msg : data.curCalGroup.message) {
                std::cerr << msg.str << std::endl;
            }
            continue;
        }

        calculation::seqCalculate();
        calculation::waitForAllTasks();
        // ===================== 将计算结果进行处理并保存到结构体result中 =====================
        for (auto& seq : data.curCalGroup.res.reses) {
            for (auto& res : seq.second) {
                SimulationResult result;
                result.voltage = voltage;                           //电压
                result.frequency = res.freqy.freq;                 //频率
                result.inputPower = res.freqy.pin;                 //输入功率

                if (!res.result.Pout.empty()) {
                    result.outputPower = res.result.Pout[0].back();                     //输出功率
                    std::cout << res.freqy.freq << std::endl;
                }
                else {
                    result.outputPower = -1; // 标记无效结果
                }
                result.avg = 10 * log10(result.outputPower / result.inputPower);                  //增益
                firstResults.push_back(result);

            }
        }

    }
    /*===================== 输出 =====================*/
    std::cout << "\n\n=============== 扫压结果 ===============" << std::endl;
    std::cout << std::left << std::setw(10) << "电压(V)"
        << std::setw(12) << "频率(GHz)"
        << std::setw(12) << "输入功率(W)"
        << std::setw(12) << "输出功率(W)"
        << std::setw(12) << "增益(db)"
        << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (const auto& result : firstResults) {
        // 只输出有效结果（outputPower >= 0）
        if (result.outputPower >= 0) {

            std::cout << std::left << std::setw(10) << result.voltage
                << std::setw(12) << result.frequency
                << std::setw(12) << result.inputPower
                << std::setw(12) << result.outputPower
                << std::setw(12) << result.avg
                << std::endl;
        }
    }


    if (!firstResults.empty()) {
        auto bestIt = max_element(firstResults.begin(), firstResults.end(),
            [](const auto& a, const auto& b) { return a.outputPower < b.outputPower; });

        BestPoints.push_back(*bestIt);  // BestPoints里面保存了每个电压的评估参数

    }

    cout << "\n============== 输出功率最大的电压 ==============" << endl;
    cout << left << setw(10) << "电压(V)"
        << setw(12) << "频率(GHz)"
        << setw(12) << "输入功率(W)"
        << setw(12) << "输出功率(W)"
        << setw(12) << "增益(dB)"
        << endl;
    cout << "------------------------------------------------" << endl;

    for (const auto& point : BestPoints) {
        cout << setw(10) << point.voltage
            << setw(12) << point.frequency
            << setw(12) << point.inputPower
            << setw(12) << point.outputPower
            << setw(12) << point.avg
            << endl;
        datachange::changecalsetting("v1", point.voltage);
        return point.voltage;
    }

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

