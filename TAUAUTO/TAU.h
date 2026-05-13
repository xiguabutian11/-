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
#include"NEW/gongshi.h"
#include"cal_change/find_voltage.h"
#include"XIGUA.h"
#include "json.hpp"


struct PowerResult {
    double midOutputPower;       // 中点输出功率
    double endOutputPower;       // 末端输出功率
    int totalPoints;             // 总点数

    // 带阈值的极大值
    double targetPeakPower;      // 第一个符合阈值的极大值功率
    int targetPeakPoint;         // 第一个符合阈值的极大值点位置（1开始）

    // 全局最大值
    double globalMaxPower;       // 全局最大值功率
    int globalMaxPoint;          // 全局最大值点位置（1开始）

    // 新增：两点间极小值
    double valleyPower;          // 极小值大小
    int valleyPoint;             // 极小值位置（1开始）
};

struct SaturationResult {
    double optimalPin;          // 最佳输入功率
    double maxOutputPower;      // 最大输出功率
    double endOutputPower;      // 末端输出功率  
    double oversaturation;      // 过饱和程度 (0-1)
	double workfre;             // 工作频率
    int maxPowerPoint;          // 最大功率点位置
    int iterations;             // 迭代次数
};

struct L_YOUHUA {
    double gain;           // 实际增益 (dB)
    double tubeLength;     // 对应管长 (单位与输入一致)
    double maxPout;        // 最大输出功率
    double optimalPin;     // 最优输入功率
};


PowerResult HuZuoYong(double fre, double pin, double voltage);//进行计算 并且提取计算结果中需要的数据

vector<PowerResult> Many_HuZuoYong(std::vector<double> freList, std::vector<double> pinList, double voltage);//并行计算 并且提取计算结果中需要的数据

int pout_yes(double fre, double pin, double voltage);//判断是否存在输出

double smallpin(double L);   //返回小信号输入功率，同时更改了计算文件中的输入功率

SaturationResult best_pin1(double fre, double V, double initialPin, double L);  //寻找固定管长下的饱和输入功率点

vector<SaturationResult> best_pin2(vector<double> freList, double V, double initialPin, double L);

double mag_judge(double fre, double pin, double voltage, double mag_A, double mag_period);//判断磁场

LXjiegou voltage_YOUHUA_Brief(double startV, double& start_voltage,double& mag_A, double& mag_period);//粗糙的 不需要在小信号

LXjiegou voltage_YOUHUA( double test_voltage, double length, double mag_A, double mag_period);//小信号情况下准确的电压优化

L_YOUHUA L_from_Gain(double Gain1, double m, double L);   //Gain1为目标增益，m为精度大小，L为初始搜索管长,该函数由大信号增益确立管长

double L_from_smallGain(double targetGain, double L);  //小信号增益确立管长

void writeDataToFile(const std::string& filename,LXjiegou jiegou,double L, jieduan LL,jieduan LL2,
    SaturationResult NN_1,
    SaturationResult NN_2,
    SaturationResult NN_3,
    SaturationResult NN_21,
    SaturationResult NN_22,
    SaturationResult NN_23,
    SaturationResult NN_11,
    SaturationResult NN_12,
    SaturationResult NN_13);

void updateDispJson(double newPointZ);

void revertLastDispJson();