#pragma once

#include "tauLib/headers/TauInteraction.h"
#include "tauLib/headers/constpar.h"
#include "usrDataInfo/usrDataSet.h"
#include "usrDataInfo/usrDataCheck.h"
#include "paramScan/paramScan.h"
#include "calculation/calculation.h"
#include "disperDataProcess/dispDtProcess.h"
#include "lossDataProcess/lossDataProcess.h"
#include "resultProcess/resultPro.h"
#include "projManage/projManage.h"
#include "cal_change/calsetting_change.h"
#include "cal_change/DataGet.h"
#include "filesReadAndWrite/fileReading.h"
#include "filesReadAndWrite/fileWriting.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include"DATA/data_start.h"

#include"SeSanJieGou.h"
#include"NEW/tube_length.h"
#include"NEW/gongshi.h"
#include"cal_change/find_voltage.h"

struct PowerResult {
    double maxOutputPower;      // 最大输出功率
    int maxPowerPoint;          // 最大功率对应的点数
    double endOutputPower;      // 末端输出功率  
    int totalPoints;            // 总点数
};

struct SaturationResult {
    double optimalPin;          // 最佳输入功率
    double maxOutputPower;      // 最大输出功率
    double endOutputPower;      // 末端输出功率  
    double oversaturation;      // 过饱和程度 (0-1)
    int maxPowerPoint;          // 最大功率点位置
    int iterations;             // 迭代次数
};

PowerResult HuZuoYong(double fre, double pin, double voltage);

double smallpin(double guanzi_type, double L);   //返回小信号输入功率，同时更改了计算文件中的输入功率

SaturationResult best_pin1(double fre, double V, double initialPin, double L);

double mag_judge(double fre, double pin, double voltage, double mag_A, double mag_period);

double voltage_YOUHUA(double bestV,double test_voltage);