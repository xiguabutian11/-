#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include"parameter/parameter.h"


//**********************求出目标相速以及耦合阻抗以及电流**********************

class XiangSu {
public:
    static double way_1(double voltage, double A);
};

class DianLiu
{public:
    static double way_1(double Pmax, double voltage,double miu);
};

class ZuKang
{public:
    static double way_1(double voltage, double current, double Pmax,double M);
};


//**********************求出满足目标频段和相速的几何结构**********************

class YuShe_jiegou
{public:
    static LXjiegou way_1(double fre, double vp);
};
