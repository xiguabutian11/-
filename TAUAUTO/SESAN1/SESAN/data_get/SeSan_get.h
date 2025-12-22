#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include"parameter/parameter.h"
#include <cmath> 

//**********************从所有相位角的色散数据中提取需要数据**********************

LXsesan centerfre(std::vector<LXsesan>guanzi, double f_phi);               //提取目标相速角的色散数据

LXsesan fre_data(std::vector<LXsesan>guanzi, double fre);             //提取目标频率的色散数据

double Vp_range(std::vector<LXsesan>guanzi, double minfre, double maxfre);//提取频段相速差