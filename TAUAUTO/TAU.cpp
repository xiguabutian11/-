#include "TAU.h"


using namespace std;



PowerResult HuZuoYong(double fre, double pin, double voltage)
{
    // 结构体初始化对应更新后的字段
    PowerResult result{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };

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
                   // ========== 1. 查找第一个超过阈值的极大值点 ==========
                    double threshold = 0.0;

                    if (guanzi_type == 1)
                    {
                        threshold = pin * pow(10, 5 / 10.0);
                    }
                    else if (guanzi_type == 2)
                    {
                        threshold = pin * pow(10, two_point(Gain * 0.45) / 10.0);
                    }
                    else if (guanzi_type == 3)
                    {
                        threshold = pin * pow(10, two_point(Gain * 0.65) / 10.0);
                    }

                    int targetPeakPointIdx = -1; // 0开始
                    double targetPeakPower = 0.0;

                    // 遍历找第一个符合条件的极大值点
                    for (int i = 0; i < result.totalPoints; i++) {
                        bool isPeak = false;
                        double currentPower = poutArray[i];

                        if (i > 0 && i < result.totalPoints - 1) {
                            // 核心修改：左侧> 右侧>=，同时加后续趋势校验
                            bool basicPeak = (currentPower > poutArray[i - 1]) && (currentPower >= poutArray[i + 1]);
                            if (basicPeak) {
                                // 额外校验：如果右侧是相等的平缓点，需检查平缓后是否爬升
                                int j = i + 1;
                                // 跳过连续相等的平缓点
                                while (j < result.totalPoints && poutArray[j] == currentPower) {
                                    j++;
                                }
                                // 若平缓后继续爬升 → 不是极大值；若平缓后下降/到末尾 → 是极大值
                                if (j >= result.totalPoints || poutArray[j] < currentPower) {
                                    isPeak = true;
                                }
                            }
                        }
                        // 最后一个点：左侧> （原有逻辑，无需改）
                        else if (i == result.totalPoints - 1 && result.totalPoints > 1) {
                            isPeak = (currentPower > poutArray[i - 1]);
                        }
                        // 只有1个点：默认是极大值
                        else if (result.totalPoints == 1) {
                            isPeak = true;
                        }

                        // 找到第一个符合阈值的极大值点，立即停止遍历
                        if (isPeak && currentPower > threshold) {
                            targetPeakPointIdx = i;
                            targetPeakPower = currentPower;
                            break;
                        }
                    }

                    // 兼容逻辑：没找到符合条件的极大值点，使用【最后一个点】
                    if (targetPeakPointIdx == -1) {
                        targetPeakPointIdx = result.totalPoints - 1;  
                        targetPeakPower = poutArray[targetPeakPointIdx];
                    }

                    // ========== 2. 提取全局最大值（原有逻辑） ==========
                    auto globalMaxIt = std::max_element(poutArray.begin(), poutArray.end());
                    double globalMaxPower = *globalMaxIt;
                    int globalMaxPointIdx = std::distance(poutArray.begin(), globalMaxIt); // 0开始

                    // ========== 3. 新增：提取两点之间的极小值 ==========
                    double valleyPower = 0.0;    // 极小值大小
                    int valleyPointIdx = -1;     // 极小值位置（0开始）

                    // 判断：全局最大值点和极大值点不是同一个点
                    if (globalMaxPointIdx != targetPeakPointIdx) {
                        // 确定两点之间的区间（取最小索引为起点，最大索引为终点）
                        int startIdx = std::min(globalMaxPointIdx, targetPeakPointIdx);
                        int endIdx = std::max(globalMaxPointIdx, targetPeakPointIdx);

                        // 查找区间内的最小值（极小值）
                        auto valleyIt = std::min_element(poutArray.begin() + startIdx, poutArray.begin() + endIdx + 1);
                        valleyPower = *valleyIt;
                        valleyPointIdx = std::distance(poutArray.begin(), valleyIt); 
                    }
                    else {
                        // 两点相同，极小值赋值为0
                        valleyPower = 0.0;
                        valleyPointIdx = -1;
                    }

                    // ========== 4. 赋值所有字段 ==========
                    // 原有目标极大值（带阈值）
                    result.targetPeakPower = targetPeakPower;
                    result.targetPeakPoint = targetPeakPointIdx + 1; // 转1开始
                    // 全局最大值
                    result.globalMaxPower = globalMaxPower;
                    result.globalMaxPoint = globalMaxPointIdx + 1; // 转1开始
                    // 新增极小值字段
                    result.valleyPower = valleyPower;
                    result.valleyPoint = valleyPointIdx + 1; // 转1开始（-1则为0）
                    // 原有其他字段
                    int midPoint = result.totalPoints / 2;
                    result.midOutputPower = poutArray[midPoint];
                    result.endOutputPower = poutArray.back();

                    // ========== 调试输出 ==========
                    std::cout << "总点数: " << result.totalPoints << std::endl;
                    std::cout << "第一个符合阈值的极大值: " << targetPeakPower << " W (第" << result.targetPeakPoint << "点)" << std::endl;
                    std::cout << "全局最大值: " << globalMaxPower << " W (第" << result.globalMaxPoint << "点)" << std::endl;
                    if (valleyPointIdx != -1) {
                        std::cout << "两点间极小值: " << valleyPower << " W (第" << result.valleyPoint << "点)" << std::endl;
                    }
                    std::cout << "中点输出功率: " << result.midOutputPower << " W (第" << midPoint + 1 << "点)" << std::endl;
                    std::cout << "末端输出功率: " << result.endOutputPower << " W (第" << result.totalPoints << "点)" << std::endl;
                }
            }
        }
    }

    return result;
}

int pout_yes(double fre, double pin, double voltage)
{
    // 1. 设置参数（触发计算的必要步骤）
    datachange::changecalsetting("frequency", fre);
    datachange::changecalsetting("pin", pin);
    datachange::changecalsetting("v1", voltage);

    // 2. 打开项目并执行计算
    filesystem::path projectPath = Projectpath;
    usrData& data = usrData::getInstance();
    if (!projManage::openProj(projectPath.string())) {
        for (auto& msg : data.curCalGroup.message) {
            std::cerr << msg.str << std::endl;
        }
    }// 忽略返回值，仅执行
    calculation::seqCalculate();
    calculation::waitForAllTasks();

    // 3. 核心判断：res中是否有有效数值
    bool hasValidValue = false;
    for (auto& seq : data.curCalGroup.res.reses) {
        for (auto& res : seq.second) {
            // 检查Pout是否非空，且至少有一个非0数值
            if (!res.result.Pout.empty()) {
                for (auto& pout : res.result.Pout[0]) {
                    if (pout != 0.0) { // 判定有效数值：非0
                        hasValidValue = true;
                        break; // 找到有效数值，终止内层循环
                    }
                }
                if (hasValidValue) break; // 终止中层循环
            }
        }
        if (hasValidValue) break; // 终止外层循环
    }

    // 4. 返回结果：有有效数值返回1，否则返回0
    return hasValidValue ? 1 : 0;
}

double smallpin(double L)
{
    double A = 0;
    double pin = 0.1;
    const double MIN_PIN_LIMIT = 1e-5; // 功率下限阈值10^-5 W
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
        else if (guanzi_type == 2 ) {
            // 类型2：取第二个极值点
            MAXPOINT = extremePoints[0];
            std::cout << ", 管子类型2取第二个极值点, 位置=" << MAXPOINT;
        }
        else if (guanzi_type == 3 ) {
            // 类型2：取第二个极值点
            MAXPOINT = extremePoints[0];
            std::cout << ", 管子类型2取第二个极值点, 位置=" << MAXPOINT;
        }

        double Lbaohe = (MAXPOINT * 2.0 * L) / totalPoints; // L1：功率最大值位置的管长
        std::cout << ", L=" << L << ", Lbaohe(功率最大值管长)=" << Lbaohe << std::endl;

        // 核心逻辑修改：修正记录管长的计算
        if (L < 0.7 * Lbaohe) {
            // 满足条件：正常返回，更新为原始L
            A = 1;
            datachange::tubeDataChange("tubeLength", L);
            return pin;
        }
        else if (pin <= MIN_PIN_LIMIT+ 1e-6) {
            // 功率降至10^-5仍不满足条件，记录0.7*Lbaohe并更新
            double record_tube_length = 0.7 * Lbaohe; // 要记录的管长：0.7*功率最大值位置的管长
            std::cout << "警告：功率已降至" << MIN_PIN_LIMIT << "W，仍未满足条件！" << std::endl;
            std::cout << "当前记录 - 功率最大值位置管长(L1)：" << Lbaohe
                << "，最终记录管长(0.7*L1)：" << record_tube_length
                << "，输入功率：" << pin << std::endl;
            A = 1; // 终止循环
            datachange::tubeDataChange("tubeLength", record_tube_length); // 更新为0.7*L1
            return pin; // 返回当前功率
        }
        else {
            // 未满足条件且功率未到下限，继续缩小
            pin = 0.1 * pin;
        }
    }

    return pin;
}

SaturationResult best_pin1(double fre, double V, double initialPin, double L)
{
    double Gainmax = 25;
    if (Gain > 20 && Gain < 45) { Gainmax = 50; }
    else if (Gain < 70) { Gainmax = 75; }

    datachange::tubeDataChange("tubeLength", 2 * L);
    double currentPin = initialPin;
    double step_up = initialPin;
    double step_down = initialPin;
    const int maxIterations = 50;

    std::cout << "=== 寻找最佳输入功率 ===" << std::endl;

    int lastDirection = 0;
    int totalPoints = 0;

    for (int iteration = 0; iteration < maxIterations; iteration++) {
        PowerResult powerInfo = HuZuoYong(fre, currentPin, V);
        int currentMaxPoint = powerInfo.targetPeakPoint;
        totalPoints = powerInfo.totalPoints;

        // ===================== 【新增：增益超限判断】 =====================
        // 计算当前增益：全局最大输出功率 / 当前输入功率
        double currentGain = 10 * log10(powerInfo.globalMaxPower / currentPin);

        bool isDecreasing = (lastDirection == -1);  // 正在减小功率
        if (isDecreasing && currentGain > Gainmax)
        {
            std::cout << "\n【保护触发】减小功率时增益超过上限 Gainmax = " << Gainmax << " dB" << std::endl;
            std::cout << "当前增益 = " << currentGain << " dB，停止迭代" << std::endl;

            // 按 Gainmax 反算输出功率
            double limitedPout = currentPin * pow(10, Gainmax / 10.0);

            datachange::tubeDataChange("tubeLength", L);
            return {
                currentPin,          // 真实输入功率
                limitedPout,         // 按Gainmax计算的最大输出功率
                powerInfo.midOutputPower,
                0.0,                 // 过饱和度无意义
                fre,
                currentMaxPoint,
                totalPoints
            };
        }
        // =================================================================

        int targetMin, targetMax;
        if (powerInfo.valleyPower > 1e-10) {
            if (powerInfo.valleyPower >= powerInfo.targetPeakPower * 0.9) {
                double kuoda = powerInfo.globalMaxPoint - powerInfo.targetPeakPoint;
                targetMin = (totalPoints / 2) - kuoda;
                targetMax = (totalPoints / 2) + kuoda;
            }
            else {
                targetMin = (totalPoints / 2) - 2;
                targetMax = (totalPoints / 2) + 2;
            }
        }
        else {
            targetMin = (totalPoints / 2) - 2;
            targetMax = (totalPoints / 2) + 2;
        }

        std::cout << "迭代 " << iteration + 1 << ": pin=" << currentPin << "W, "
            << "饱和点位置=" << currentMaxPoint << "/" << totalPoints
            << " 目标区间:[" << targetMin << "," << targetMax << "]";

        if (currentMaxPoint >= targetMin && currentMaxPoint <= targetMax) {
            std::cout << " 符合要求" << std::endl;

            double oversaturation = 0;
            if (powerInfo.targetPeakPower > powerInfo.midOutputPower) {
                oversaturation = (powerInfo.targetPeakPower - powerInfo.midOutputPower) / powerInfo.targetPeakPower;
            }
            std::cout << "\n最佳工作点: " << currentPin << " W" << std::endl;
            std::cout << "全局最大输出功率: " << powerInfo.globalMaxPower << " W (第" << powerInfo.globalMaxPoint << "点)" << std::endl;
            std::cout << "第一个符合阈值的极大值功率: " << powerInfo.targetPeakPower << " W (第" << powerInfo.targetPeakPoint << "点)" << std::endl;
            if (powerInfo.valleyPower > 1e-10) {
                std::cout << "极小值功率: " << powerInfo.valleyPower << " W (第" << powerInfo.valleyPoint << "点)" << std::endl;
                double valleyRatio = (powerInfo.valleyPower / powerInfo.targetPeakPower) * 100;
                std::cout << "极小值/极大值占比: " << valleyRatio << "%" << std::endl;
            }
            std::cout << "中点输出功率: " << powerInfo.midOutputPower << " W (第" << totalPoints / 2 << "点)" << std::endl;
            std::cout << "过饱和度: " << oversaturation * 100 << "%" << std::endl;

            datachange::tubeDataChange("tubeLength", L);
            return {
                currentPin,
                powerInfo.targetPeakPower,
                powerInfo.midOutputPower,
                oversaturation,
                fre,
                currentMaxPoint,
                totalPoints
            };
        }

        int newDirection = 0;
        if (currentMaxPoint > targetMax) {
            std::cout << " → 最大值在末端，增大功率" << std::endl;
            if (lastDirection == -1) {
                step_down /= 2;
                step_up /= 2;
                currentPin = currentPin + step_down;
            }
            else {
                currentPin += step_up;
                newDirection = 1;
            }
        }
        else if (currentMaxPoint < targetMin) {
            std::cout << " → 最大值点太靠前，减小功率" << std::endl;
            if (lastDirection == 1) {
                step_up /= 2;
                step_down /= 2;
                currentPin = currentPin - step_up;
            }
            else {
                step_up /= 2;
                step_down /= 2;
                currentPin -= step_down;
                newDirection = -1;
            }
        }

        lastDirection = newDirection;

        if (currentPin < 1e-10) {
            std::cout << "警告：功率过小，重置为最小值" << std::endl;
            currentPin = 1e-10;
        }
    }

    datachange::tubeDataChange("tubeLength", L);
    std::cout << "未找到最佳工作点，达到最大迭代次数。" << std::endl;
    return {
        currentPin, 0, 0, 0, 0,0, 0
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
    int liutong_judge = 0;
    bool baoluo_judge = false;

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
                    else{
                        liutong_judge = 1;
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
                        else{
                            baoluo_judge = true;
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

LXjiegou voltage_YOUHUA_Brief(double startV, double& start_voltage,
    double& mag_A, double& mag_period)
{
    double F = 0;
    double fre = (minfre + maxfre) / 2;
    double I = DianLiu::way_1(Pout, V, miu);
    BEST liu;
	jieduan L_L = { 0,0,0,0 };
    LXjiegou jiegou = {0,0,0,0,0,0,0};
    if (V < 6000) { V_change = 400; }
	else if (V < 4000) { V_change = 200; }
    while (startV < V- 500 || startV > V + 500)
    {
        if (startV <V - 500)
        {
            std::cout << "最佳电压低于目标范围" << std::endl;
            if (F == 1) { V_change = V_change / 2; }
            start_voltage = start_voltage + V_change;

            jiegou = YOUHUA_sesan(minfre, maxfre, start_voltage, Pout, 0);//调整色散结构
            double r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);
            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L);//传入色散数据

            //--------------磁场优化------------
            while (mag_judge(fre, 0.001 * mostpin, V, mag_A, mag_period) == 0)
            {
                mag_A += 0.01;
                mag_period = mag2(V, r / 2, I);
                datachange::mag(mag_A, mag_period);
            }
            mag_A = mag1(V, 0.5 * r, I, 1.8);
            //----------------------------------
            startV = liu.bestvoltage3(V, fre, I, Vjiange);//寻找最佳电压
            F = -1;
        }
        else if (startV > V + 500)
        {
            std::cout << "最佳电压高于目标范围" << std::endl;
            if (F == -1) { V_change = V_change / 2; }
            start_voltage = start_voltage - V_change;

            jiegou = YOUHUA_sesan(minfre, maxfre, start_voltage, Pout, 0);
            double r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);

            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L);

            //--------------磁场优化------------
            while (mag_judge(fre, 0.001 * mostpin, V, mag_A, mag_period) == 0)
            {
                mag_A += 0.01;
                mag_period = mag2(V, r / 2, I);
                datachange::mag(mag_A, mag_period);
            }
            mag_A = mag1(V, 0.5 * r, I, 1.8);
            //----------------------------------
            startV = liu.bestvoltage3(V, fre, I, Vjiange);
            F = 1;
        }
    }
    return jiegou;
};

LXjiegou voltage_YOUHUA(double bestV, double test_voltage,double length,double mag_A,double mag_period) {
    double F = 0;
    double r = 0;
    double small_pin = 0;
    double fre = (minfre + maxfre) / 2;
    double I = DianLiu::way_1(Pout, V, miu);
    BEST liu;
	jieduan L_L = { 0,0,0,0 };
    LXjiegou jiegou = { 0,0,0,0,0,0,0 };
    while (bestV < V - Vcha || bestV > V + Vcha)
    {
        if (bestV < V - Vcha)
        {
            std::cout << "最佳电压低于目标范围" << std::endl;
            if (F == 1) { V_change = V_change / 2; }
            test_voltage = test_voltage + V_change;

            jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);//调整色散结构
            r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);
            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L);//传入色散数据

            small_pin = smallpin(length);//小信号
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

            jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);
            r = 1000 * jiegou.Ra;
            datachange::beamDataChange("outerR", r / 2);
            datachange::beamDataChange("tunnelR", r);

            convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L);
            small_pin = smallpin(length);
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
    return jiegou;
}

L_YOUHUA L_from_Gain(double Gain1, double m, double L)  // 仅新增Pout_const参数
{
    L_YOUHUA liu1 = { 0,0,0,0 };
    double length_enough = 0;          //用来判断管长是否足够满足全频段增益达到目标值
    double test_pin = Pout / pow(10, Gain1 / 10.0);
    double test_length = L;
    double C = 0;
    double A1 = test_pin;
    double A2 = test_pin;
    double D = 0;
    double maxpout = 0;
    double threshold = 0.0;

    if (guanzi_type == 1)
    {
        threshold = test_pin * pow(10, 5 / 10.0);
    }
    else if (guanzi_type == 2)
    {
        threshold = test_pin * pow(10, two_point(Gain * 0.45) / 10.0);
    }
    else if (guanzi_type == 3)
    {
        threshold = test_pin * pow(10, two_point(Gain * 0.65) / 10.0);
    }

    while (length_enough == 0)
    {
        double now_Gain = 0;
        double correspondingLength = 0;
        double enough = 1;               //用来判断管子是否过饱和，0为过饱和，1为不饱和
        std::cout << "============================================================== " << std::endl;
        std::cout << "输入功率:" << test_pin << std::endl;
        datachange::changecalsetting("pin", test_pin);
        while (enough == 1)                //选择管长保证当前管子过饱和
        {
            datachange::tubeDataChange("tubeLength", test_length);

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

            for (auto& seq : data.curCalGroup.res.reses) {

                for (auto& res : seq.second) {
                    if (!res.result.Pout.empty()) {

                        auto& powerSequence = res.result.Pout[0];
                        std::cout << "频率 " << res.freqy.freq << ": ";
                        for (auto& out : powerSequence) {
                            std::cout << out << " ";
                        }
                        std::cout << std::endl;  // 新增：优化输出格式

                        // ========== 核心修改开始 ==========
                        size_t targetIndex = -1;  // 目标极大值点索引
                        double targetPout = 0;    // 目标极大值点功率

                        // 遍历找第一个超过阈值的极大值点（跳过首尾避免越界）
                        for (size_t i = 1; i < powerSequence.size() - 1; ++i) {
                            // 判定极大值点：当前点 > 左右相邻点，且超过阈值
                            bool isPeak = (powerSequence[i] > powerSequence[i - 1]) && (powerSequence[i] > powerSequence[i + 1]);
                            if (isPeak && powerSequence[i] > threshold) {
                                targetIndex = i;
                                targetPout = powerSequence[i];
                                break;  // 找到第一个符合条件的点就退出
                            }
                        }

                        if (targetIndex == -1) {
                            targetIndex = powerSequence.size() - 1;
                            targetPout = powerSequence[targetIndex];
                        }
                        // ========== 核心修改结束 ==========

                        if (targetIndex == powerSequence.size() - 1) {
                            now_Gain = 10 * log10(targetPout / test_pin);
                            enough = 1;
                            test_length = test_length + test_length;
                        }
                        else {
                            now_Gain = 10 * log10(targetPout / test_pin);
                            correspondingLength = static_cast<double>(targetIndex) / powerSequence.size() * test_length;
                            maxpout = targetPout;  // 替换为目标极大值点功率
                            enough = 0;
                        }
                    }
                }
            }
        }
        if (Gain1 < now_Gain && now_Gain < Gain1 + m) { length_enough = 1; }
        if (now_Gain < Gain1 && C != 2)
        {
            A2 = A2 / 2;
            test_pin -= A2;
            C = 1;
        }
        else if (now_Gain > Gain1 + m && C != 1)
        {
            if (D == 1) { A1 = A1 / 2; }
            test_pin += A1;
            C = 2;
        }
        else if (now_Gain < Gain1 && C == 2)
        {
            A1 = A1 / 2;
            test_pin -= A1;
            D = 1;
        }
        else if (now_Gain > Gain1 + m && C == 1)
        {
            A2 = A2 / 2;
            test_pin += A2;
        }
        std::cout << "增益" << now_Gain << "对应管长" << correspondingLength << "目标极大值功率" << maxpout << std::endl;
        liu1.gain = now_Gain;
        liu1.tubeLength = correspondingLength;
        liu1.maxPout = maxpout;  // 存储目标极大值点功率
        liu1.optimalPin = test_pin;
    };

    return liu1;
}


double L_from_smallGain(double targetGain, double L)
{
    double test_pin = 0.001;
    datachange::changecalsetting("pin", test_pin);

    double test_length = two_point(pre_length(L));
    double correspondingLength = 0;
    int enough = 1;  
    while (enough == 1)
    {
        datachange::tubeDataChange("tubeLength", test_length);

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

        for (auto& seq : data.curCalGroup.res.reses) {
            for (auto& res : seq.second) {
                if (!res.result.Pout.empty()) {
                    auto& powerSequence = res.result.Pout[0];
                    bool foundGain = false;

                    std::cout << "频率 " << res.freqy.freq << " GHz 功率序列: ";
                    for (auto& out : powerSequence) {
                        std::cout << out << " ";
                    }
                    std::cout << std::endl;

                    for (size_t i = 0; i < powerSequence.size(); ++i) {
                        double pout = powerSequence[i];
                        double gain = 10 * log10(pout / test_pin);

                        if (gain >= targetGain) {
                            // 找到增益点 → 计算长度
                            correspondingLength = (double)i / powerSequence.size() * test_length;
                            foundGain = true;
                            enough = 0;

                            std::cout << "→ 找到目标增益: " << gain << " dB" << std::endl;
                            std::cout << "→ 对应管长: " << correspondingLength << std::endl;
                            break;
                        }
                    }
                    if (!foundGain) {
                        test_length *= 2;
                        std::cout << "→ 长度不足，自动加倍为: " << test_length << std::endl;
                    }
                }
            }
        }
    }

    return correspondingLength;
}


void writeDataToFile(const std::string& filename, LXjiegou jiegou, double L,jieduan LL,jieduan LL2,
    SaturationResult NN_1,
    SaturationResult NN_2,
    SaturationResult NN_3,
    SaturationResult NN_21,
    SaturationResult NN_22,
    SaturationResult NN_23,
    SaturationResult NN_11,
    SaturationResult NN_12,
    SaturationResult NN_13
    ) {

    // 创建输出文件流
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "错误：无法打开文件 " << filename << " 进行写入！" << std::endl;
        return;
    }
    outfile << "#设计参数:" << std::endl;
    outfile << "工作电压:" << V << "V" << std::endl;
    outfile << "工作频段:" << minfre << "GHz - " << maxfre << "GHz" << std::endl;
    outfile << "增益:" << Gain <<"db" << std::endl;
    outfile << "输出功率:" << Pout <<"W" << std::endl;
    outfile << std::endl;  
    outfile << "# 色散结构参数:" << std::endl;
    outfile << "螺旋线内半径=" << jiegou.Ra*1000 << " mm" << std::endl;
    outfile << "螺旋线外半径=" << jiegou.Rb * 1000 << " mm" << std::endl;
    outfile << "螺距=" << jiegou.L * 1000 << " mm" << std::endl;
    outfile << "屏蔽壳内径=" << jiegou.Rc * 1000 << " mm" << std::endl;
    outfile << "翼片内半径=" << jiegou.Rg * 1000 << " mm" << std::endl;
    outfile << "螺旋线宽度=" << jiegou.del * 1000 << " mm" << std::endl;
    outfile << "翼片角度=" << jiegou.fir << std::endl;
    outfile << std::endl;  
    outfile << "最终管长:" << L << "mm" << std::endl;
	outfile << "一段衰减:" << LL.A << "mm--" <<LL.B<<"mm" << std::endl;
    outfile << "二段衰减:" << LL.C << "mm--" << LL.D << "mm" << std::endl;
    outfile << "三段衰减:" << LL2.A << "mm--" << LL2.B << "mm" << std::endl;
    outfile << "四段衰减:" << LL2.C << "mm--" << LL2.D << "mm" << std::endl;
	outfile << "截断" << LL.B << "mm--" << LL.C << "mm" << std::endl;
    outfile << "第二个截断" << LL2.B << "mm--" << LL2.C << "mm" << std::endl;
    outfile << std::endl;
    outfile << "# 工作频率(Hz)\t增益(dB)\t最佳输入功率(W)\t输出功率(W)" << std::endl;

    // 设置输出格式
    outfile << std::fixed << std::setprecision(6);  // 保留6位小数

    // 原来的三个主要频率点
    double gain1 = 10 * log10(NN_2.maxOutputPower / NN_2.optimalPin);
    outfile << NN_2.workfre << "\t" << gain1 << "\t" << NN_2.optimalPin << "\t" << NN_2.maxOutputPower << "\n";

    // NN_21 到 NN_23 (低频段到中心频率之间)
    double gain21 = 10 * log10(NN_21.maxOutputPower / NN_21.optimalPin);
    outfile << NN_21.workfre << "\t" << gain21 << "\t"<< NN_21.optimalPin << "\t" << NN_21.maxOutputPower << "\n";

    double gain22 = 10 * log10(NN_22.maxOutputPower / NN_22.optimalPin);
    outfile << NN_22.workfre << "\t" << gain22 << "\t" << NN_22.optimalPin << "\t" << NN_22.maxOutputPower << "\n";

    double gain23 = 10 * log10(NN_23.maxOutputPower / NN_23.optimalPin);
    outfile << NN_23.workfre << "\t" << gain23 << "\t" << NN_23.optimalPin << "\t" << NN_23.maxOutputPower << "\n";

    double gain2 = 10 * log10(NN_1.maxOutputPower / NN_1.optimalPin);
    outfile << NN_1.workfre << "\t" << gain2 << "\t" << NN_1.optimalPin << "\t" << NN_1.maxOutputPower << "\n";

    // NN_11 到 NN_13 (中心频率到高频段之间)
    double gain11 = 10 * log10(NN_11.maxOutputPower / NN_11.optimalPin);
    outfile << NN_11.workfre << "\t" << gain11 << "\t" << NN_11.optimalPin << "\t" << NN_11.maxOutputPower << "\n";

    double gain12 = 10 * log10(NN_12.maxOutputPower / NN_12.optimalPin);
    outfile << NN_12.workfre << "\t" << gain12 << "\t" << NN_12.optimalPin << "\t" << NN_12.maxOutputPower << "\n";

    double gain13 = 10 * log10(NN_13.maxOutputPower / NN_13.optimalPin);
    outfile << NN_13.workfre << "\t" << gain13 << "\t" << NN_13.optimalPin << "\t" << NN_13.maxOutputPower << "\n";

    double gain3 = 10 * log10(NN_3.maxOutputPower / NN_3.optimalPin);
    outfile << NN_3.workfre << "\t" << gain3 << "\t" << NN_3.optimalPin << "\t" << NN_3.maxOutputPower << "\n";

    // 关闭文件
    outfile.close();

    std::cout << "数据已成功写入文件: " << filename << std::endl;
}


