#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include"writeANDread/read.h"
#include"judgeANDchange/judge.h"
#include"judgeANDchange/change_units.h"
#include"YuChuLi/first_param.h"
#include"YOUHUA/youhua.h"
#include"data_get/SeSan_get.h"
#pragma once

void convertTxtToJson(const std::string& inputFile, const std::string& outputFile,
    double minfre, double maxfre);

std::tuple <LXjiegou, LXsesan>  youhua_way1(LXjiegou jiegou, LXsesan target, std::vector<LXsesan> n_guanzi);

LXjiegou YOUHUA_sesan(double minfre, double maxfre, double V, double Pout,double N);
