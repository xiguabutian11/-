#pragma once
#include"paramAndUnit\parameter.h"

class paramScan {
public:
	static void megShow();		

	static bool parScanFunction(const calSetting::calSequence& seq);  //扫参接口函数，输入当前运行的sequence，自动调用其他函数生成扫参数据

	static vector<paramScanData> getParScanData();

	static map<string, signedArray<double, bool>> getQualifiedParScanVaris();

private:
	static bool parScanDataGeneration();		//生成扫参数据的函数，运行前需要指定执行的sequence，将其赋值给seqTemp

	static bool parScanVarisCheck();		//判断是否可进行扫参（即扫参序列是否包含频率信息），同时提取出用户自定义的变量

	static void PinDistribution();			//输入功率的扫参函数
		
	static void Pin1Distribution();			//谐波输入功率的扫参函数

	static void Pin2Distribution();			//二次谐波输入功率的扫参函数

	static void paramDistribution(int index);		//用户自定义参数的扫参函数

	static void adaptiveSegmentation(const paramScanData& data);	//适应性切分，因为一次tau计算最多计算6个频点，因此需要调整每组paramScanData中频点的个数

private:
	static calSetting::calSequence seqTemp;		//当前正在运行的sequence的副本

	static paramScanData singleParScanData;			//运行时的中间数据，存储频点和参数信息

	static vector<string> paramScanVariables;	//参与扫参的用户定义变量名称
	
	static map<string, signedArray<double, bool>> qualifiedParScanVaris;	//参与扫参的全部变量名称，包括用户自定义变量和频率、输入功率等固定项

	static vector<paramScanData> parScanData;		//扫参数据
};