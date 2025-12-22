#pragma once
#include"jisuan/Jisuan.h"
#include"change_units.h"
#include"data_get/SeSan_get.h"


double centerfreAngle();

double judge_I_density(LXsesan sesan,LXjiegou jiegou,double type_guanzi);

double judge_type_guanzi();

class compare{
public:
	static order way_1(LXsesan target, std::vector<LXsesan> guanzi); //n为频率，m为相速，p为耦合阻抗
};