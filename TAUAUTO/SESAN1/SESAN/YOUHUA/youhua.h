#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include"writeANDread/read.h"
#include"sesanjiegou/sesan.h"
#include"judgeANDchange/judge.h"
#include"YOUHUA/tiaozheng.cpp"
#include"judgeANDchange/change_units.h"
#include"YuChuLi/first_param.h"
#include"data_get/SeSan_get.h"


class fre_youhua
{public:
	static std::tuple<LXjiegou, order,double>way_1(LXsesan target, LXjiegou jiegou, order order1, double time);
};

class Vp_youhua
{public:
	static std::tuple<LXjiegou, order, double>way_1(LXsesan target, LXjiegou jiegou, order order1, double time);
};

class Vpcha_youhua
{public:
	static std::tuple<LXjiegou,order, double>way_1(LXsesan target, LXjiegou jiegou,order order1, double time);
};

class Kc_youhua
{
public:
	static std::tuple<LXjiegou, order, double>way_1(LXsesan target, LXjiegou jiegou, order order1, double time);
};