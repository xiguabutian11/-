#include "TAU.h"


using namespace std;



PowerResult HuZuoYong(double fre, double pin, double voltage) {
    PowerResult result{ 0, 0, 0, 0 };

    std::cout << "Setting fre to: " << fre << std::endl;
    datachange::changecalsetting("frequency", fre);
    std::cout << "Setting pin to: " << pin << std::endl;
    datachange::changecalsetting("pin", pin);
    std::cout << "Setting voltage to: " << voltage << std::endl;
    datachange::changecalsetting("v1", voltage);

    filesystem::path projectPath = Projectpath;
    usrData& data = usrData::getInstance();

    //-------------------------计算代码-----------------------
    if (!projManage::openProj(projectPath.string())) {
        for (auto& msg : data.curCalGroup.message) {
            std::cerr << msg.str << std::endl;
        }
    }

    calculation::seqCalculate();
    calculation::waitForAllTasks();

    // ===================== 处理计算结果 =====================
    for (auto& seq : data.curCalGroup.res.reses) {
        for (auto& res : seq.second) {
            if (!res.result.Pout.empty()) {
                // 输出所有功率点用于调试
                for (auto& pout : res.result.Pout[0]) {
                    std::cout << pout << " ";
                }
                std::cout << std::endl;

                // 获取输出功率数组
                const auto& poutArray = res.result.Pout[0];
                result.totalPoints = poutArray.size();

                if (result.totalPoints > 0) {
                    // 找到最大输出功率及其位置
                    auto maxIt = std::max_element(poutArray.begin(), poutArray.end());
                    result.maxOutputPower = *maxIt;
                    result.maxPowerPoint = std::distance(poutArray.begin(), maxIt) + 1; // 从1开始计数

                    // 获取末端输出功率（最后一个点）
                    result.endOutputPower = poutArray.back();

                    std::cout << "总点数: " << result.totalPoints << std::endl;
                    std::cout << "最大输出功率: " << result.maxOutputPower << " W (第" << result.maxPowerPoint << "点)" << std::endl;
                    std::cout << "末端输出功率: " << result.endOutputPower << " W (第" << result.totalPoints << "点)" << std::endl;
                }
            }
        }
    }

    return result;
}



double smallpin(double guanzi_type, double L)
{
    double A = 0;
    double pin = 0.1;
    datachange::tubeDataChange("tubeLength", 2 * L);

    while (A == 0) {
        double MAXPOINT = 0;
        double currentPoint = 1;
        std::cout << "输入功率: " << pin << "W" << std::endl;
        datachange::changecalsetting("pin", pin);

        //-------------------------计算代码-----------------------
        filesystem::path projectPath = Projectpath;
        usrData& data = usrData::getInstance();
        if (!projManage::openProj(projectPath.string())) {
            for (auto& msg : data.curCalGroup.message) {
                std::cerr << msg.str << std::endl;
            }
        }
        calculation::seqCalculate();
        calculation::waitForAllTasks();

        std::vector<double> poutValues;
        for (auto& seq : data.curCalGroup.res.reses) {
            for (auto& res : seq.second) {
                if (!res.result.Pout.empty()) {
                    for (auto& pout : res.result.Pout[0]) {
                        std::cout << pout << " ";
                    }
                    std::cout << std::endl;
                    for (const auto& value : res.result.Pout[0]) {
                        poutValues.push_back(value);
                        currentPoint++;
                    }
                }
            }
        }

        // 寻找极大值点（包含右端点）
        std::vector<int> extremePoints;
        int totalPoints = poutValues.size();

        // 检查中间点 (i = 1 到 totalPoints-2)
        for (int i = 1; i < totalPoints - 1; i++) {
            if (poutValues[i] > poutValues[i - 1] && poutValues[i] > poutValues[i + 1]) {
                extremePoints.push_back(i + 1);
            }
        }

        // 检查右端点 (i = totalPoints-1)
        if (totalPoints > 1 && poutValues[totalPoints - 1] > poutValues[totalPoints - 2]) {
            extremePoints.push_back(totalPoints); // 最后一个点
        }

        // 输出关键信息
        std::cout << "总点数: " << totalPoints << ", 极值点数量: " << extremePoints.size();

        // 根据管子类型选择极值点
        if (guanzi_type == 1 && !extremePoints.empty()) {
            // 类型1：取第一个极值点
            MAXPOINT = extremePoints[0];
            std::cout << ", 管子类型1取第一个极值点, 位置=" << MAXPOINT;
        }
        else if (guanzi_type == 2 && extremePoints.size() >= 2) {
            // 类型2：取第二个极值点
            MAXPOINT = extremePoints[1];
            std::cout << ", 管子类型2取第二个极值点, 位置=" << MAXPOINT;
        }

        double Lbaohe = (MAXPOINT * 2.0 * L) / totalPoints;
        std::cout << ", L=" << L << ", Lbaohe=" << Lbaohe << std::endl;

        if ( L < 0.7 * Lbaohe) {
            A = 1;
            datachange::tubeDataChange("tubeLength", L);
            return pin;
        }
        else  {
            pin = 0.1 * pin;
        }
    }

    return pin;
}

SaturationResult best_pin1(double fre, double V, double initialPin, double L)
{
    datachange::tubeDataChange("tubeLength", 2 * L);
    double currentPin = initialPin;
    double step_up = initialPin * 0.5;
    double step_down = initialPin * 0.5;
    const int maxIterations = 50;  //最大迭代次数

    std::cout << "=== 寻找最佳输入功率 ===" << std::endl;

    int lastDirection = 0; // 改用lastDirection记录方向
    int totalPoints = 0;

    for (int iteration = 0; iteration < maxIterations; iteration++) {
        PowerResult powerInfo = HuZuoYong(fre, currentPin, V);
        int currentMaxPoint = powerInfo.maxPowerPoint;
        totalPoints = powerInfo.totalPoints;

        int targetMin = totalPoints * 0.5 - 5;
        int targetMax = totalPoints * 0.5 + 5;

        std::cout << "迭代 " << iteration + 1 << ": pin=" << currentPin << "W, "
            << "最大值点位置=" << currentMaxPoint << "/" << totalPoints
            << " 目标区间:[" << targetMin << "," << targetMax << "]";

        // 检查是否在目标区间内
        if (currentMaxPoint >= targetMin && currentMaxPoint <= targetMax) {
            std::cout << " ✓ 符合要求" << std::endl;

            double oversaturation = 0;
            if (powerInfo.maxOutputPower > powerInfo.endOutputPower) {
                oversaturation = (powerInfo.maxOutputPower - powerInfo.endOutputPower) / powerInfo.maxOutputPower;
            }

            std::cout << "\n最佳工作点: " << currentPin << " W" << std::endl;
            datachange::tubeDataChange("tubeLength", L); // 恢复原值
            return {
                currentPin, powerInfo.maxOutputPower, powerInfo.endOutputPower,
                oversaturation, currentMaxPoint, totalPoints
            };
        }

        int newDirection = 0;
        if (currentMaxPoint > targetMax) {
            std::cout << " → 最大值在末端，增大功率" << std::endl;
            if (lastDirection == -1) { // 方向改变
                step_up /= 2;
                std::cout << "  增长步长减半: " << step_up << "W" << std::endl;
                currentPin = currentPin + step_up; // Corrected: Add step_up
            }
            else {
                currentPin += step_up;
            }
            newDirection = 1;
        }
        else if (currentMaxPoint < targetMin) {
            std::cout << " → 最大值点太靠前，减小功率" << std::endl;
            if (lastDirection == 1) { // 方向改变
                step_down /= 2;
                std::cout << "  减少步长减半: " << step_down << "W" << std::endl;
                currentPin = currentPin - step_down;
            }
            else {
                currentPin -= step_down;
            }
            newDirection = -1;
        }

        lastDirection = newDirection;

        // 防止功率为负
        if (currentPin < 1e-10) {
            std::cout << "警告：功率过小，重置为最小值" << std::endl;
            currentPin = 1e-10;
        }
    }

    datachange::tubeDataChange("tubeLength", L); // 恢复原值
    std::cout << "未找到最佳工作点，达到最大迭代次数。" << std::endl;
    return {
        currentPin, 0, 0, 0, 0, 0 // 返回默认值或错误值
    };
}

double mag_judge(double fre, double pin, double voltage,double mag_A,double mag_period) {
    // 保存当前的cout格式状态
    std::ios_base::fmtflags original_flags = std::cout.flags();
    std::streamsize original_precision = std::cout.precision();

    datachange::mag(mag_A, mag_period);
    datachange::changecalsetting("frequency", fre);
    datachange::changecalsetting("pin", pin);
    datachange::changecalsetting("v1", voltage);

    filesystem::path projectPath = Projectpath;
    usrData& data = usrData::getInstance();

    if (!projManage::openProj(projectPath.string())) {
        for (auto& msg : data.curCalGroup.message) {
            std::cerr << msg.str << std::endl;
        }
    }

    calculation::seqCalculate();
    calculation::waitForAllTasks();

    // ===================== 检查电子流通率 =====================
    int liutong_judge = 1;
    bool baoluo_judge = true;

    for (auto& seq : data.curCalGroup.res.reses) {
        for (auto& res : seq.second) {
            // 检查电子流通率
            if (!res.result.FlowRate.empty()) {
                const auto& flowRate = res.result.FlowRate;
                std::cout << "\n电子流通率: ";
                for (const auto& rate : flowRate) {
                    std::cout << rate << " ";
                    if (rate < 1.0) {
                        liutong_judge = 0;
                    }
                }
                std::cout << std::endl;
            }

            // 检查电子束包络
            if (!res.result.tra.empty()) {
                std::cout << "\n电子轨迹数据分析:" << std::endl;

                // 输出最大值
                std::cout << "最大值: ";
                for (int pos_idx = 0; pos_idx < res.result.tra.size(); pos_idx++) {
                    if (!res.result.tra[pos_idx].empty()) {
                        double max_abs_value = 0.0;
                        for (int elec_idx = 0; elec_idx < res.result.tra[pos_idx].size(); elec_idx++) {
                            double value = res.result.tra[pos_idx][elec_idx];
                            double abs_value = std::abs(value);
                            if (abs_value > max_abs_value) {
                                max_abs_value = abs_value;
                            }
                        }
                        std::cout << max_abs_value << " ";
                    }
                }
                std::cout << std::endl;

                // 输出比例
                std::cout << "比例: ";
                for (int pos_idx = 0; pos_idx < res.result.tra.size(); pos_idx++) {
                    if (!res.result.tra[pos_idx].empty()) {
                        int count_over_one = 0;
                        int total_count = res.result.tra[pos_idx].size();
                        for (int elec_idx = 0; elec_idx < res.result.tra[pos_idx].size(); elec_idx++) {
                            double value = res.result.tra[pos_idx][elec_idx];
                            double abs_value = std::abs(value);
                            if (abs_value > 1) {
                                count_over_one++;
                            }
                        }
                        double percentage = (static_cast<double>(count_over_one) / total_count) * 100.0;
                        std::cout << std::fixed << std::setprecision(1) << percentage << "% ";

                        if (percentage >= 30.0) {
                            baoluo_judge = false;
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
    }

    // 恢复cout的原始格式设置
    std::cout.flags(original_flags);
    std::cout.precision(original_precision);

    // 输出检查结果
    if (liutong_judge) {
        std::cout << "流通合格" << std::endl;
    }
    else {
        std::cout << "流通不合格" << std::endl;
    }

    if (baoluo_judge) {
        std::cout << "包络合适" << std::endl;
    }
    else {
        std::cout << "包络不合适" << std::endl;
    }

    if (liutong_judge && baoluo_judge) {
        std::cout << "磁场合格" << std::endl;
        return 1.0;
    }
    else {
        std::cout << "磁场不合格" << std::endl;
        return 0.0;
    }
}

double voltage_YOUHUA(double bestV, double test_voltage,double length,double mag_A,double mag_period) {
    double F = 0;
    double r = 0;
    double small_pin = 0;
    double fre = (minfre + maxfre) / 2;
    double I = DianLiu::way_1(Pout, V, miu);
    BEST liu;
    while (bestV < V - Vcha || bestV > V + Vcha)
    {
        if (bestV < V - Vcha)
        {
            std::cout << "最佳电压低于目标范围" << std::endl;
            if (F == 1) { V_change = V_change / 2; }
            test_voltage = test_voltage + V_change;

            LXjiegou jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);//调整色散结构
            r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);
            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);//传入色散数据

            small_pin = smallpin(1, length);//小信号
            //--------------磁场优化------------
            while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0)
            { 
                mag_A += 0.01;
                mag_period = mag2(V, r / 2, I);
                datachange::mag(mag_A, mag_period);
            }
            mag_A = mag1(V, 0.5 * r, I, 1.8);
            //----------------------------------
            bestV = liu.bestvoltage3(V, fre, I, Vjiange);//寻找最佳电压
            F = -1;
        }
        else if (bestV > V + Vcha)
        {
            std::cout << "最佳电压高于目标范围" << std::endl;
            if (F == -1) { V_change = V_change / 2; }
            test_voltage = test_voltage - V_change;

            LXjiegou jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);
            r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);

            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
            small_pin = smallpin(1, length);
            //--------------磁场优化------------
            while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0)
            {
                mag_A += 0.01;
                mag_period = mag2(V, r / 2, I);
                datachange::mag(mag_A, mag_period);
            }
            mag_A = mag1(V, 0.5 * r, I, 1.8);
            //----------------------------------
            bestV = liu.bestvoltage3(V, fre, I, Vjiange);
            F = 1;
        }
    }
    std::cout << "最佳电压处于范围内" << std::endl;
    return 1;
}