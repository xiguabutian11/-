#pragma once
#include <iostream>
#include"jisuan/Jisuan.h"
#include"judgeANDchange/change_units.h"

class shuchu
{
public:
	static void A1(double minfre, double maxfre, double voltage, double Pmax);
	static void A2(double vp, double I, double kc, double Ra, double Rb, double Rc, double Rg, double L, double del);
	static void A3(LXsesan guanzi);
	static void A4(LXsesan guanzi);
	static void A5(order order);
	static void A6(LXjiegou jiegou, LXsesan centervalue, LXjiegou youhua_jiegou, LXsesan youhua_centervalue);
};