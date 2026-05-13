#include "calsetting_change.h"


bool datachange::changecalsetting(const std::string& targetParam, double numble) {
    calSetting calSet;

    if (!fileReading::calSetRead(calSet, jsonPath)) {
        std::cerr << "读取配置文件失败" << std::endl;
        return false;
    }

    if (calSet.calSet.find("sequence_1") == calSet.calSet.end() ||
        calSet.calSet["sequence_1"].seq.find(targetParam) == calSet.calSet["sequence_1"].seq.end()) {
        std::cerr << "未找到参数: " << targetParam << std::endl;
        return false;
    }

    double val = four_point(numble);
    calSet.calSet["sequence_1"].seq[targetParam].array = { val };

    if (!fileWriting::calSetWrite(calSet, jsonPath)) {
        std::cerr << "保存配置失败" << std::endl;
        return false;
    }

    return true;
}


bool datachange::changecalsetting_array(const std::string& targetParam, const std::vector<double>& numArray)
{
    calSetting calSet;

    if (!fileReading::calSetRead(calSet, jsonPath)) {
        std::cerr << "读取配置文件失败" << std::endl;
        return false;
    }

    if (calSet.calSet.find("sequence_1") == calSet.calSet.end() ||
        calSet.calSet["sequence_1"].seq.find(targetParam) == calSet.calSet["sequence_1"].seq.end()) {
        std::cerr << "未找到参数: " << targetParam << std::endl;
        return false;
    }

    std::vector<double> out;
    for (double d : numArray) {
        out.push_back(four_point(d));
    }

    calSet.calSet["sequence_1"].seq[targetParam].array = out;

    if (!fileWriting::calSetWrite(calSet, jsonPath)) {
        std::cerr << "保存配置失败" << std::endl;
        return false;
    }

    return true;
}


bool datachange::tubeDataChange(const std::string& fieldName,double newValue) {

    
    std::ifstream inFile(tubeDataPath);
    Json::Value root;
    inFile >> root; // 直接解析（省略错误处理）
    inFile.close();


    root[fieldName] = newValue;


    std::ofstream outFile(tubeDataPath);
    outFile << root; // 直接输出（保持原格式）
    outFile.close();

    return true;
}

bool datachange::magDataChange(const std::string& fieldName, double newValue) {

    std::ifstream inFile(magDataPath);
    Json::Value root;
    inFile >> root;
    inFile.close();


    double roundedValue = std::round(newValue * 1000) / 1000.0;
    root["magnetic"][0][fieldName] = roundedValue;


    std::ofstream outFile(magDataPath);
    Json::StreamWriterBuilder builder;
    builder.settings_["precision"] = 3;  // 设置精度为3位小数
    builder.settings_["indentation"] = "\t";  // 保持缩进格式
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &outFile);
    outFile.close();

    return true;
}

bool datachange::mag(double A, double P)
{
    datachange::magDataChange("AMP", A);
    datachange::magDataChange("period", P);
    return true;
}

bool datachange::beamDataChange(const std::string& fieldName, const Json::Value& newValue) {
  
    std::ifstream inFile(beamDataPath);
    Json::Value root;
    inFile >> root;
    inFile.close();


    root[fieldName] = newValue;


    std::ofstream outFile(beamDataPath);
    outFile << root;
    outFile.close();

    return true;
}


bool datachange::r(double r1, double r2)
{
    datachange::beamDataChange("outerR", r1);
    datachange::beamDataChange("tunnelR", r2);
    return true;
}

bool datachange::lossDataChange(int mode, double firstBegin, double firstEnd, double secondBegin, double secondEnd)
{
    std::ofstream file(lossDataPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << lossDataPath << std::endl;
        return 1;
    }

    if (mode == 0) {
        // 写入null
        file << "null";
        return 1;
    }
    else if (mode == 1) {
        // 使用stringstream构建JSON
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);  // 保持高精度

        oss << "{\n";
        oss << "   \"cutOffs\" : [\n";
        oss << "      {\n";
        oss << "         \"begin\" : " << firstEnd << ",\n";
        oss << "         \"end\" : " << secondBegin << "\n";
        oss << "      }\n";
        oss << "   ],\n";
        oss << "   \"lossSegs\" : [\n";
        oss << "      {\n";
        oss << "         \"begin\" : " << firstBegin << ",\n";
        oss << "         \"defaultDistriType\" : 0,\n";
        oss << "         \"defaultdB\" : 24.0,\n";
        oss << "         \"end\" : " << firstEnd << ",\n";
        oss << "         \"lossType\" : 0\n";
        oss << "      },\n";
        oss << "      {\n";
        oss << "         \"begin\" : " << secondBegin << ",\n";
        oss << "         \"defaultDistriType\" : 1,\n";
        oss << "         \"defaultdB\" : 24.0,\n";
        oss << "         \"end\" : " << secondEnd << ",\n";
        oss << "         \"lossType\" : 0\n";
        oss << "      }\n";
        oss << "   ]\n";
        oss << "}";

        file << oss.str();
    }
    else {
        std::cerr << "Error: Invalid mode. Use 0 for null or 1 for content." << std::endl;
        return 1;
    }

    file.close();
    return 1;
}


bool datachange::lossDataChange_2(
    double A, double A1,
    double B, double B2,
    double C, double C3,
    double D, double D4
)
{
    std::ofstream file(lossDataPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << lossDataPath << std::endl;
        return false;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "{\n";
    oss << "   \"cutOffs\" : [\n";
    oss << "      {\n";
    oss << "         \"begin\" : " << A1 << ",\n";
    oss << "         \"end\" : " << B << "\n";
    oss << "      },\n";
    oss << "      {\n";
    oss << "         \"begin\" : " << C3 << ",\n";
    oss << "         \"end\" : " << D << "\n";
    oss << "      }\n";
    oss << "   ],\n";
    oss << "   \"lossSegs\" : [\n";

    // 第一段有效区域
    oss << "      {\n";
    oss << "         \"begin\" : " << A << ",\n";
    oss << "         \"defaultDistriType\" : 0,\n";
    oss << "         \"defaultdB\" : 24.0,\n";
    oss << "         \"end\" : " << A1 << ",\n";
    oss << "         \"lossType\" : 0\n";
    oss << "      },\n";

    // 第二段有效区域
    oss << "      {\n";
    oss << "         \"begin\" : " << B << ",\n";
    oss << "         \"defaultDistriType\" : 1,\n";
    oss << "         \"defaultdB\" : 24.0,\n";
    oss << "         \"end\" : " << B2 << ",\n";
    oss << "         \"lossType\" : 0\n";
    oss << "      },\n";
    
    oss << "      {\n";
    oss << "         \"begin\" : " << C << ",\n";
    oss << "         \"defaultDistriType\" : 0,\n";
    oss << "         \"defaultdB\" : 24.0,\n";
    oss << "         \"end\" : " << C3 << ",\n";
    oss << "         \"lossType\" : 0\n";
    oss << "      },\n";

    oss << "      {\n";
    oss << "         \"begin\" : " << D << ",\n";
    oss << "         \"defaultDistriType\" : 1,\n";
    oss << "         \"defaultdB\" : 24.0,\n";
    oss << "         \"end\" : " << D4 << ",\n";
    oss << "         \"lossType\" : 0\n";
    oss << "      }\n";

    oss << "   ]\n";
    oss << "}";

    file << oss.str();
    file.close();
    return true;
}