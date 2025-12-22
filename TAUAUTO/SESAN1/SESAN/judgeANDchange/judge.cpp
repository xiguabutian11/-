#include"judge.h"

double centerfreAngle()
{
	return 120;
}

double judge_I_density(LXsesan sesan, LXjiegou jiegou, double type_guanzi)
{
	double Density = sesan.I / jiegou.Ra* jiegou.Ra*PI;
	double target_density;
	if (type_guanzi == 0) { target_density = I_density_MaiChong;}
	else if (type_guanzi == 1) {  target_density = I_density_LianXu; };   //0为脉冲管子，1为连续波管子

	if (Density > target_density) { return 1; }
	else if (Density <= target_density) { return 0; };
}

double judge_type_guanzi()
{
	return 1;
}


order compare::way_1(LXsesan target, std::vector<LXsesan> guanzi)
{

	LXsesan valuemin = fre_data(guanzi, HZ_turn_GHZ(target.fmin));   //获得最小频率点的色散数据
	LXsesan valuemax = fre_data(guanzi, HZ_turn_GHZ(target.fmax));   //获得最大频率点的色散数据

	std::cout << "------------------------------------------" << std::endl;
	std::cout << "左端频率色散值:" << std::endl;
	std::cout << "频率:" << valuemin.f<<"相速："<<valuemin.vp<<"耦合阻抗："<<valuemin.kc << std::endl;
	std::cout << "右端频率色散值:" << std::endl;
	std::cout << "频率:" << valuemax.f<< "相速：" << valuemax.vp << "耦合阻抗：" << valuemax.kc << std::endl;
	
	double Vpcha = Vp_range(guanzi, HZ_turn_GHZ(target.fmin), HZ_turn_GHZ(target.fmax));
	order xunhuan;
	
	
	if (guanzi.back().f < HZ_turn_GHZ(target.fmax*1.5)) { xunhuan.n = -1; }
	

	if (valuemin.vp>target.vp / Vc+0.0025) { xunhuan.m = 1; }
	if (valuemax.vp<target.vp / Vc-0.0025) { xunhuan.m = -1; }
	
	if (valuemax.kc < target.kc) { xunhuan.p = -1; } 

	if (Vpcha > 0.008) { xunhuan.t = 2; }
	else if (Vpcha > 0.0038){ xunhuan.t = 1;}
	
	std::cout << "相速差为：" << Vpcha << std::endl;
	
	return xunhuan;
}