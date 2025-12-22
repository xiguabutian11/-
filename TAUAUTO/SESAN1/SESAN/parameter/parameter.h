#pragma once
#include <iostream>

//****************用户初始数据***************
static double first_fir = 60;
//****************数据预处理修正数据***************
static double miu = 0.15;
static double AA = 0;
static double MM = 0;
//****************计算常量*******************
static double Vc = 2.99792458e8;
static double Me = 9.10938356e-31;
static double e = 1.602176634e-19;
static double PI = std::acos(-1.0);
//****************预设电流密度***************
static double I_density_MaiChong =0 ;
static double I_density_LianXu = 0;


struct Cycle_initial
{
    double k=0.05;

};

struct LXjiegou {
    double Ra;
    double Rb;
    double L;
    double Rc;
    double Rg;
    double del;
    double fir;
};

struct LXsesan {
    double f_end;
    double fmax;
    double fmin;
    double f;
    double vp;
    double phi;
    double kc;
    double alpha;
    double I;
	double guanzi_type;   //0为窄带管，1为宽带管，2为倍频管
};

struct order { 
    double n=0;        //频率判断
    double m=0;        //相速判断 
    double p=0;        //阻抗判断
    double t=0;        //频带判断
    double j=0;        //电流密度判断
};

