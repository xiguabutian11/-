#include"tube_length.h"

lengthstruct youhua_length(double guanzi_type,double L, double Gain, double fre, double small_pin, double V)
{
    double target = Gain;
    lengthstruct liu;

    // 初始设置
    liu.first_lentgh = L;
    datachange::tubeDataChange("tubeLength", liu.first_lentgh);
    PowerResult result = HuZuoYong(fre, small_pin, V);
    liu.fisrt_gain = 10 * log10(result.endOutputPower / small_pin);

    // 优化参数
    double now_length = L;
    double now_gain = liu.fisrt_gain;
    double step = 50;  // 初始步长倍数
    const int max_iterations = 50; // 最大迭代次数防止无限循环
    int iteration = 0;

    // 目标区间
    double low= target + 3;
    double up = target + 8;

    std::cout << "开始优化长度，目标增益区间: [" << low << ", " << up << "]" << std::endl;
    std::cout << "初始长度: " << now_length << ", 初始增益: " << now_gain << std::endl;

    while (iteration < max_iterations) {
        iteration++;

        // 检查当前增益是否在目标区间内
        if (now_gain >= low && now_gain <= up) {
            std::cout << "第 " << iteration << " 次迭代: 增益 " << now_gain
                << " 在目标区间内，优化完成。" << std::endl;
            break;
        }

        double pre_length = now_length;
        double pre_gain = now_gain;

        // 调整策略
        if (now_gain < low) {
            // 增益太小，增加长度
            now_length = now_length+step;
        }
        else if (now_gain > up) {
            // 增益太大，减小长度
            now_length = now_length-step;
        }

        // 更新物理模型
        datachange::tubeDataChange("tubeLength", now_length);
        small_pin = smallpin(guanzi_type,now_length);              
        result = HuZuoYong(fre, small_pin, V);
        now_gain = 10 * log10(result.endOutputPower / small_pin);

        std::cout << "更新后增益: " << now_gain << std::endl;

        // 检查是否跳过目标区间
        bool cross = false;
        if ((pre_gain < low && now_gain > up) ||
            (pre_gain > up && now_gain < low)) {
            cross = true;
        }

        // 如果跳过区间，减小调整步长
        if (cross) {
            std::cout << "检测到跳过目标区间，调整倍数减半: " <<  std::endl;
            now_length = pre_length;// 回退
			now_gain = pre_gain;
            step *= 0.5;
        }
    }

    if (iteration >= max_iterations) {
        std::cout << "达到最大迭代次数，优化结束。" << std::endl;
    }

    // 设置最终结果
    liu.final_length = now_length;
    liu.final_gain = now_gain;

    std::cout << "优化结果 - 最终长度: " << liu.final_length
        << ", 最终增益: " << liu.final_gain << std::endl;

    return liu;
}