#pragma once
#include "tauLib/headers/params.h"

using namespace std;

//电压的单位选择
static map<int, pair<string, double>> VUnit = {
    {1,{"V",1}},
    {2,{"kV",1000}}
};

//电流的单位选择
static map<int, pair<string, double>> IUnit = {
    {1,{"A",1}},
    {2,{"mA",1e-3}},
};

//频率的单位选择
static map<int, pair<string, double>> freqUnit = {
    {1,{"Hz",1e-9}},
    {2,{"kHz",1e-6}},
    {3,{"MHz",1e-3}},
    {4,{"GHz",1}},
    {5,{"THz",1000}}
};

//长度的单位选择
static map<int, pair<string, double>> lengthUnit = {
    {1,{"m",1000}},
    {2,{"cm",10}},
    {3,{"mm",1}},
    {4,{"um",1e-3}},
};

//默认单位数据（索引值），注意！！必须与constInfo.h中unitKeys中各物理量的排序保持一致
static map<int, int>defaultUnitIndex = {
    {1,1},  //键1代表电压
    {2,1},  //键2代表电流
    {3,4},  //键3代表频率
    {4,3}   //键4代表长度
};

//某些数据的默认值
static map<string, double> defaultValue = {
    {"pin",0.3}
};
