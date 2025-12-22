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
    double kkk = (fre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
    judge.f = guanzi[n1 - 1].f + (guanzi[n1].f - guanzi[n1 - 1].f) * kkk;
    judge.f_end = guanzi.back().f;
    judge.vp = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;
    judge.kc = guanzi[n1 - 1].kc + (guanzi[n1].kc - guanzi[n1 - 1].kc) * kkk;
    judge.alpha = guanzi[n1 - 1].alpha + (guanzi[n1].alpha - guanzi[n1 - 1].alpha) * kkk;
    return judge;
};

double Vp_range(std::vector<LXsesan>guanzi, double minfre,double maxfre)
{
    LXsesan min;
    LXsesan max;
    double nn = 0;  
    double nnn = 0;  

    for (const auto& item : guanzi) {
        nn++;
        if (item.f >= minfre) { nnn++; }
    }
    double n1 = nn - nnn;
    double kkk = (minfre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
    min.vp = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;
    
    nn = 0;  
    nnn = 0;  

    for (const auto& item : guanzi) {
        nn++;
        if (item.f >= maxfre) { nnn++; }
    }
    n1 = nn - nnn;
    kkk = (maxfre - guanzi[n1 - 1].f) / (guanzi[n1].f - guanzi[n1 - 1].f);
    max.vp = guanzi[n1 - 1].vp + (guanzi[n1].vp - guanzi[n1 - 1].vp) * kkk;

    return std::abs(max.vp - min.vp);

};