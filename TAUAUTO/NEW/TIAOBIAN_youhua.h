#pragma once
#include "../TAU.h"


struct SweepData
{
	double fre;
	double gain;
	double outpower;
};

struct SpeedPoint
{
	int index;      // 原始点的位置 i
	double value;   // 速度值
};

struct SweepStat
{
    // 增益
    double gain_max;
    double gain_mean;
    double gain_var;
    double gain_std;

    // 输出功率
    double power_max;
    double power_mean;
    double power_var;
    double power_std;
};

vector<SweepData> fre_sweep(double dot_numble,double work_V, L_YOUHUA L) ;

vector<double> Get_speed(double fre, double pin, double voltage);  //互作用计算，得出归一化速度数据组

vector<SpeedPoint> Speed_need(const vector<double>& speed, double endcut_percent, int need);  
//对归一化数据组进行提取，阈值为截断后平均差值的一半，点的个数为need确认

void plotWithPython(const vector<SweepData>& data, int index);   //把扫频数据保存并调用python画图

double tiaobian_score(const vector<SweepData>& junyun, const vector<SweepData>& tiaobian);

SweepStat calcSweepStat(const vector<SweepData>& data);