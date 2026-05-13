#pragma once
#include <iostream>
#pragma once

static double com_fmin = 1; // 色散结构最小频率
static double hf = 0.5; // 频率步进
static double sweep_point = 8;// 扫频点数
//-------------------------目标初始需求--------------------------
static double minfre = 12;
static double maxfre = 14;
static double V = 10000;
static double Pout = 400;
static double Gain = 30;
static double mostpin = Pout / pow(10, Gain / 10); // 达到目标的最大输入功率
extern double guanzi_type;
extern double bestV;
//-------------------------最佳电压扫描参数-----------------------
static double Vjiange = 10;    // 电压扫描时的采样间隔数
static double Vcha = 100;      // 目标电压允许的半偏差范围
static double V_change = 1000; // 每次调整的电压变化量，当电压分别小于6000和4000时，调整量分别为400和200

// 基础路径
const std::string TAUAUTO_PATH = "C:/Users/liu/Desktop/TAUAUTO/TAUAUTO/SESAN1/SESAN";
const std::string Projectpath = "C:/Users/liu/Desktop/project_5";

// input和output.txt的路径
const std::string inputPath = TAUAUTO_PATH + "/input.txt";
const std::string outputPath = TAUAUTO_PATH + "/output.txt";

// sesanjiegou中用的数据
const std::string paramFile = TAUAUTO_PATH + "/input.txt";  // 和inputPath相同
const std::string workpath = TAUAUTO_PATH;

const std::string basePath = Projectpath + "/interaction_1";
const std::string dispdatapath = basePath + "/tubeData/dispData.json";
const std::string tubeDataPath = basePath + "/tubeData/tubeData.json";
const std::string magDataPath = basePath + "/tubeData/magData.json";
const std::string beamDataPath = basePath + "/tubeData/beamData.json";
const std::string jsonPath = basePath + "/calSetting.json";
const std::string lossDataPath = basePath + "/tubeData/lossData.json";



