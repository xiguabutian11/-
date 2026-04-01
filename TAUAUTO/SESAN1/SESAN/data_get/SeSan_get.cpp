#include"SeSan_get.h"


LXsesan centerfre(std::vector<LXsesan>guanzi, double f_phi)
{
    LXsesan judge;
    double nn = 0;  //记录第几组色散数据
    double nnn = 0;  //记录相位角过了的组数
    //获得中心频率相位角
    for (const auto& item : guanzi) {
        nn++;
        if (item.phi >= f_phi) { nnn++; }
    }
    double n1 = nn - nnn;
    double kkk = (f_phi - guanzi[n1 - 1].phi) / (guanzi[n1].phi - guanzi[n1 - 1].phi);
    judge.f = guanzi[n1 - 1].f + (guanzi[n1].f - guanzi[n1 - 1].f) * kkk;
    judge.vp = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;
    judge.kc = guanzi[n1 - 1].kc + (guanzi[n1].kc - guanzi[n1 - 1].kc) * kkk;
    judge.alpha = guanzi[n1 - 1].alpha + (guanzi[n1].alpha - guanzi[n1 - 1].alpha) * kkk;
    return judge;
};

LXsesan fre_data(std::vector<LXsesan>guanzi, double fre)
{
    LXsesan judge;
    double nn = 0;  //记录第几组色散数据
    double nnn = 0;  //记录相位角过了的组数

    for (const auto& item : guanzi) {
        nn++;
        if (item.f >= fre) { nnn++; }
    }
    double n1 = nn - nnn;
    if (n1 == 0) { 
        judge = guanzi[0];
        judge.f_end = guanzi.back().f;
        return judge;
    }
    double kkk = (fre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
    judge.f = guanzi[n1 - 1].f + (guanzi[n1].f - guanzi[n1 - 1].f) * kkk;
    judge.f_end = guanzi.back().f;
    judge.vp = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;
    judge.kc = guanzi[n1 - 1].kc + (guanzi[n1].kc - guanzi[n1 - 1].kc) * kkk;
    judge.alpha = guanzi[n1 - 1].alpha + (guanzi[n1].alpha - guanzi[n1 - 1].alpha) * kkk;
    return judge;
};

VpRangeResult Vp_range(std::vector<LXsesan> guanzi, double minfre, double maxfre)
{
    VpRangeResult result;

    // 快速检查
    if (guanzi.empty()) {
        std::cerr << "错误：guanzi向量为空！" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 扫描数据点...
    double maxdata = -1e100;
    double mindata = 1e100;
    int maxpoint = -1;
    int minpoint = -1;
    int leftpoint = -1;
    int rightpoint = -1;

    for (int i = 0; i < (int)guanzi.size(); i++) {
        if (guanzi[i].f >= minfre && guanzi[i].f <= maxfre) {
            if (leftpoint == -1) leftpoint = i;
            rightpoint = i;

            if (guanzi[i].vp > maxdata) {
                maxdata = guanzi[i].vp;
                maxpoint = i;
            }
            if (guanzi[i].vp < mindata) {
                mindata = guanzi[i].vp;
                minpoint = i;
            }
        }
    }

    // 检查是否有数据点在区间内
    if (leftpoint == -1) {
        std::cerr << "错误：在频率区间 [" << minfre << ", " << maxfre
            << "] 内没有找到数据点！" << std::endl;
        exit(EXIT_FAILURE);
    }

    // ===================== 只返回最大/最小值 =====================
    result.max_vp = maxdata;
    result.min_vp = mindata;
    // ==============================================================

    // 设置状态值
    if (maxpoint == leftpoint) {
        result.status = 1;
        std::cout << "相速最大值在区间左边界！" << std::endl;
    }
    else if (maxpoint == rightpoint) {
        result.status = 3;
        std::cout << "相速最大值在区间右边界！" << std::endl;
    }
    else {
        std::cout << "相速最大值在中间！" << std::endl;
        result.status = 2;
    }

    // 根据最大值点位置决定计算方法
    // 情况1或3：最大值在区间左边界或右边界
    if (result.status == 1 || result.status == 3) {
        // 计算minfre处的vp（左端点）
        double vp_minfre;
        double nn = 0, nnn = 0;
        for (const auto& item : guanzi) {
            nn++;
            if (item.f >= minfre) { nnn++; }
        }
        double n1 = nn - nnn;
        if (n1 == 0) {
            vp_minfre = guanzi[0].vp;
        }
        else {
            if (n1 - 1 < 0 || n1 >= (int)guanzi.size()) {
                std::cerr << "错误：计算minfre插值时索引越界！" << std::endl;
                exit(EXIT_FAILURE);
            }
            double kkk = (minfre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
            vp_minfre = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;
        }

        // 计算maxfre处的vp（右端点）
        double vp_maxfre;
        nn = 0, nnn = 0;
        for (const auto& item : guanzi) {
            nn++;
            if (item.f >= maxfre) { nnn++; }
        }
        n1 = nn - nnn;

        if (n1 - 1 < 0 || n1 >= (int)guanzi.size()) {
            std::cerr << "错误：计算maxfre插值时索引越界！" << std::endl;
            exit(EXIT_FAILURE);
        }
        double kkk = (maxfre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
        vp_maxfre = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;

        // 左端点减右端点（这样通常得到正值）
        result.value = vp_minfre - vp_maxfre;
    }
    // 情况2：最大值在区间中间
    else {
        if (minpoint == -1) {
            std::cerr << "错误：找到了最大值点但未找到最小值点！" << std::endl;
            exit(EXIT_FAILURE);
        }
        // 最大值减最小值
        result.value = maxdata - mindata;
    }

    return result;
}