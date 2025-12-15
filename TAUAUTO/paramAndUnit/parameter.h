#pragma once
#include"usrDataInfo\usrData.h"

class parameter {
public:
	static bool addParam(parametersList::parameter par);//参数添加函数

	static bool removeParam(string par);//参数删除函数

	static void paramLinkFunction(map<string, double>& variants, InteractionParams& intpar);//参数链接函数

	static bool parScanableCheck(string name, set<string> linkedParam = set<string>());//判断参数是否用于扫参链接的函数

	static set<string> scanableParameters();	//获取可用于扫参链接的参数

	static void paramStateUpdate();//参数状态更新函数

	static void paramLinkStateUpdate();//参数链接状态更新函数

	static double exprSoluable(string expr);//表达式是否可解析的判断表达式


private:
	static bool tubeDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string,double>& variants, InteractionParams& intpar);
	
	static bool magDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar);
	
	static bool beamDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar);

	static bool lossDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar);
	
	//调用该函数前，必须保证输入的pars中各参数的state均为unchecked
	static double dfsDepVarsCheck(string par, parametersList& pars);
};