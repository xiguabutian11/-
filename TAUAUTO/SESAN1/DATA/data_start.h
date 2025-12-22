#pragma once
#include <iostream>
#pragma once

static double com_fmin = 1; // 色散结构最小频率
static double hf = 1; // 频率步进
//-------------------------目标初始需求--------------------------
static double minfre = 2;
static double maxfre = 6;
static double V = 12500;
static double Pout = 400;
static double Gain = 30.0;
static double mostpin = Pout / pow(10, Gain / 10); // 达到目标的最大输入功率
//-------------------------最佳电压扫描参数-----------------------
static double Vjiange = 20;    // 电压扫描时的采样间隔数
static double Vcha = 300;      // 目标电压允许的半偏差范围
static double V_change = 500; // 每次调整的电压变化量

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



