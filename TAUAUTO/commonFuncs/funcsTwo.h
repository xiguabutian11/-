#pragma once
#include"commonFuncs\funcs.h"
#include <thirdPtyLibs\json\json.h>

class funcsTwo :public funcs {

public:
	//等数量分割线段函数
	static vector<double> fixedNumEvenSeg(double start, double end, int num);

	//等长度分割线段函数
	static vector<double> fixedLenEvenSeg(double start, double end, double length);


	//数值分配的默认函数：线性分配
	static vector<double> linearDistri(double value, int num, bool flag = true, double start = NAN);

	// 包装函数，显式传递所有参数
	static vector<double> defaultDistriWrapper(double value, int num); 

};


