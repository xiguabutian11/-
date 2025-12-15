#pragma once
#include "usrDataInfo\usrData.h"

class dispDtPro {
public:
	static map<double, map<double, dispDataOfFreq, DoubleComparator>, DoubleComparator> getDispData();

	/*生成各位置的色散数据对应插值函数的函数。！！！注意：调用该函数
	之前要求已经对色散数据进行检查和更新*/
	static void dispFuncsGeneration();

	/*生成各频点基础色散数据（基本采样点上的色散数据）的函数。
	！！！注意：调用该函数之前要求已经对dispFuncs进行赋值,即
	已经调用了dispFuncsGeneration*/
	static void dispDatasGeneration(const vector<double>& freqs);

private:
	static void setDisperPsoToUsrData(const vector<double>& samPoints);

	/*补充添加各频点色散数据（指代基本采样点之间的通过插值方式生成的色散数据）的函数
	！！！注意：调用该函数之前要求已经调用过dispDatasGeneration*/
	static void dispDatasSupplement(const vector<double>& freqs, const vector<double>& samPoints);

	static void dispDataModify();

	//插值函数
	inline static void posOrientedInterpo(const double& freq, const vector<double>& samPoints);
	
	//将输入的map<double,double>转为vector<vector<double>>的函数
	inline static vector<vector<double>> mapToVector(const map<double, double, DoubleComparator>& maps);

private:
	static map<double, map<double, dispDataOfFreq, DoubleComparator>, DoubleComparator> dispDatas;//一阶键表示频率，二阶键表示位置

	static vector<double> centerFreqVpInfo;	//中心频点各色散结构相速

	static unordered_map<string, unordered_map<string, boost::math::interpolators::barycentric_rational<double>>> dispFuncs;//一阶键为结构名称，二阶键为kc、vp或al；

	static std::shared_mutex dispDataMtx;

	static set<double> positions;


};