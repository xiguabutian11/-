#pragma once
#include "cal_change/DataGet.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include"DATA/data_start.h"
#pragma once

struct SimulationResult {               //正常数据存储的结构体
    double voltage;
    double frequency;
    double inputPower;
    double outputPower;
    double avg;
};

class BEST {
    double D = 1;
    double A = 1;
    double Bc = 0.01;
    double BI = 0.01;
    double Ba = 1E-15;
    double PI = std::acos(-1.0);
    double Vc = 2.99792458e8;
    double Me = 9.10938356e-31;     //电子质量
    double e = 1.602176634e-19;     //电子电荷


public:
    double bestvoltage3(double V, double fre, double I, double Vjiange);
};