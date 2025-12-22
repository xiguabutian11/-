#include "write.h"
using namespace std;

void changeinputvalue(const string& filename, const string& paramName, double newValue) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    bool found = false;
    for (auto& line : lines) {
        if (line.find(paramName) == 0) {  // 确保参数名在行首
            string searchPattern;
            if (paramName == "a") searchPattern = "- internal radius of helix,";
            else if (paramName == "b") searchPattern = "-externa radius of helix,";
            else if (paramName == "L") searchPattern = "-helix pitch,";
            else if (paramName == "c") searchPattern = "-wall radius,";
            else if (paramName == "g") searchPattern = "-radius of load,";
            else if (paramName == "del") searchPattern = "- helix width,";
            else if (paramName == "eps") searchPattern = ",";
            else if (paramName == "ratio") searchPattern = ",";
            else if (paramName == "fir") searchPattern = ",";
            else if (paramName == "correction factor") searchPattern = ",";
            else if (paramName == "number of loads") searchPattern = ",";
            else if (paramName == "m") searchPattern = "- number of points for description rods,";

            size_t commaPos = line.find(searchPattern);
            if (commaPos != string::npos) {
                // 查找逗号后的数字开始位置
                size_t numStart = line.find_first_not_of(" \t", commaPos + searchPattern.length());
                if (numStart != string::npos) {
                    // 查找数字结束位置
                    size_t numEnd = line.find_first_not_of("0123456789.", numStart);
                    if (numEnd == string::npos) numEnd = line.length();

                    // 替换数字部分
                    string newValueStr = to_string(newValue);
                    // 去除多余的零和小数点
                    newValueStr.erase(newValueStr.find_last_not_of('0') + 1, string::npos);
                    newValueStr.erase(newValueStr.find_last_not_of('.') + 1, string::npos);

                    line.replace(numStart, numEnd - numStart, newValueStr);
                    found = true;
                }
            }
        }
    }

    if (!found) {
        cerr << "Warning: Parameter '" << paramName << "' not found" << endl;
        return;
    }

    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Cannot write to file " << filename << endl;
        return;
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();
}

void modifyCoordinates(const string& filename, const vector<vector<double>>& newCoordinates) {
    // 读取文件内容
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // 找到坐标数据起始位置
    size_t coordHeaderIndex = 0;
    for (; coordHeaderIndex < lines.size(); coordHeaderIndex++) {
        if (lines[coordHeaderIndex].find("coordinats of points") != string::npos) {
            break;
        }
    }

    if (coordHeaderIndex == lines.size()) {
        cerr << "Error: 'coordinats of points' header not found" << endl;
        return;
    }

    // 替换坐标数据
    size_t coordStartIndex = coordHeaderIndex + 1;

    // 用新坐标替换旧坐标
    for (size_t i = 0; i < newCoordinates.size() && coordStartIndex + i < lines.size(); i++) {
        ostringstream oss;
        for (size_t j = 0; j < newCoordinates[i].size(); j++) {
            if (j > 0) oss << "\t";
            oss << fixed << setprecision(6) << newCoordinates[i][j];
        }
        lines[coordStartIndex + i] = oss.str();
    }

    // 写回文件
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Cannot write to file " << filename << endl;
        return;
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();
}

void renew_input(LXjiegou jiegou)
{
    changeinputvalue(inputPath, "a", m_mm(jiegou.Ra));
    changeinputvalue(inputPath, "b", m_mm(jiegou.Rb));
    changeinputvalue(inputPath, "c", m_mm(jiegou.Rc));
    changeinputvalue(inputPath, "g", m_mm(jiegou.Rg));
    changeinputvalue(inputPath, "L", m_mm(jiegou.L));
    changeinputvalue(inputPath, "del", m_mm(jiegou.del));
    changeinputvalue(inputPath, "fir", jiegou.fir);

    
    vector<vector<double>> newCoords = {
        {0.0,      m_mm(jiegou.Rb), 0.0},
        {m_mm(0.19*jiegou.Rb),m_mm(jiegou.Rb), 0.0},
        {m_mm(0.19*jiegou.Rb),m_mm(0.6*jiegou.Rc+0.4*jiegou.Rb),     0.0},
        {m_mm(0.44*jiegou.Rb),m_mm(0.6 * jiegou.Rc + 0.4 * jiegou.Rb),     0.0},
        {m_mm(0.44*jiegou.Rb),m_mm(jiegou.Rc), 0.0},
        {0.0,m_mm(jiegou.Rc), 0.0}
    };
    modifyCoordinates(inputPath, newCoords);
};