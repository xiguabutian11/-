#pragma once

#include "usrDataInfo\usrData.h"

class lossDtPro {
public:
	static void posGeneration();
	
	//必须调用posGeneration()
	static void dBValuesGeneration();

	//必须先执行posGeneration()和dBValuesGeneration()才可调用该函数
	static vector<loss> lossDataGeneration(const vector<double>& freqs);


private:
	//调用该函数之前，必须先调用posGeneration()生成positions和defltDistriType数据
	static vector<vector<double>> dBValOfFreqGen(const vector<lossAndCutOffDataInfo::dBofFreqs>& data);

private:
	static vector<signedArray<double,int>> positions;

	static vector<unsigned int> defltDistriType;

	static vector<double> defltdBOfEachSeg;

	static map<double, vector<vector<double>>, DoubleComparator> dBValues;

	static vector<vector<double>> defaultdBValues;
};