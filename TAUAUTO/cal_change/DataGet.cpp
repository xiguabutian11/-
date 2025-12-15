#include "DataGet.h"

double dataget::dispDataGet(int line) {
    dispDataInfo disp;
    std::string filePath = "C:/Users/liu/Desktop/project_5/interaction_1/tubeData/dispdata.json"; // 您的JSON文件路径

    // 调用您的函数读取数据
    if (fileReading::dispDataRead(disp, filePath)) {

        std::string targetName = "position_1";

        try {
            // 使用 .at() 访问 map，如果键不存在会抛出 std::out_of_range 异常
            const auto& dispEntry = disp.dispInfo.at(targetName);

            // 获取 vp map
            const auto& vpMap = dispEntry.vp;

            // 查找 vp map 中的最大值
            double maxVpVal = std::numeric_limits<double>::lowest(); // 初始化为最小可能双精度值


            if (!vpMap.empty()) {
                for (const auto& pair : vpMap) {
                    // pair.first 是频率，pair.second 是值
                    maxVpVal = std::max(maxVpVal, pair.second);
                }
                if (line == 1) {
                    return maxVpVal;
                };
            }
            else {
                std::cout << "The 'vp' map for '" << targetName << "' is empty." << std::endl;
            }

            double minKcVal = std::numeric_limits<double>::max(); // 初始化为最小可能双精度值
            double maxKcVal = std::numeric_limits<double>::min();
            // 获取 kc map
            const auto& kcMap = dispEntry.kc;

            if (!kcMap.empty()) {
                for (const auto& pair : kcMap) {
                    minKcVal = std::min(minKcVal, pair.second); // 使用 std::min 查找最大值
                    maxKcVal = std::max(maxKcVal, pair.second);
                }
                if (line == 2) {
                    return minKcVal;
                };
                if (line == 3) {
                    return maxKcVal;
                };
            }
            else {
                std::cout << "The 'kc' map for '" << targetName << "' is empty." << std::endl;
            }
        }
        catch (const std::out_of_range& oor) {
            // 如果 disp.dispInfo 中没有 targetName 这个键
            std::cerr << "Error: Could not find '" << targetName << "' in dispInfo map." << std::endl;
            std::cerr << "Details: " << oor.what() << std::endl;
            return 0; // 表示错误
        }
        catch (const std::exception& e) {
            // 捕获其他可能的异常
            std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
            return 0; // 表示错误
        }
     
    }
    else {
        std::cerr << "Failed to read dispersion data from " << filePath << std::endl;
        // 处理文件读取失败的情况
        return 0; // 表示错误
    }
}

double dataget::tubeDataGet(const std::string& fieldName) {
    const std::string filePath = "C:/Users/liu/Desktop/project_5/interaction_1/tubeData/tubeData.json";

    try {
        // 1. 读取JSON文件
        std::ifstream inFile(filePath);
        if (!inFile.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            return 0.0;
        }

        Json::Value root;
        inFile >> root;
        inFile.close();

        // 2. 检查字段是否存在
        if (!root.isMember(fieldName)) {
            std::cerr << "Error: Field '" << fieldName << "' not found in JSON" << std::endl;
            return 0.0;
        }

        // 3. 返回字段值
        return root[fieldName].asDouble();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 0.0;
    }
}