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


