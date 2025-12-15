#pragma once
#include "usrDataInfo\usrData.h"

class resPro {
public:
	//特定结果删除函数
	static void removeRes(string seqName, string unitResName, unsigned int flag = 15);

	static bool resExtraction(string seqName, map<string, parStateInResExt> varis,string extractedResName);

	static void removeExtedRes(string seqName, string extractedResName);

	static void unitResSetImport(string seqName);

private:
	static bool beforeResExtCheck(string seqName, map<string, parStateInResExt>& varis);

	static double qualifiedCheck(const results::res& res, const freqin& freq, const map<string, double>& params, unsigned int flag, string arg);
};