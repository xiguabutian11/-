#pragma once
#include"parameter/parameter.h"
#include"jisuan/Jisuan.h"
#include"shuchu/shuchu.h"
#include"writeANDread/write.h"

LXsesan  SeSanMuBiao(double minfre, double maxfre, double voltage, double Pmax,double N);

LXjiegou SeSanJieGou(LXsesan LXtarget);