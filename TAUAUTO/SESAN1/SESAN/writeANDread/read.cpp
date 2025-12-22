#include "read.h"


std::vector<LXsesan> readTWTData(const std::string& filename) {
    std::vector<LXsesan> results;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "文件打开失败" << std::endl;
        return results;
    }

    std::string line;
    std::getline(file, line); // 跳过标题

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        LXsesan d;
        if (iss >> d.f >> d.vp >> d.phi >> d.kc >> d.alpha) {
            results.push_back(d);
        }
    }

    return results;
}