#pragma once
#include "usrDataInfo/usrData.h"
#include "ctlDataUpdate/ctlData.h"

class fileWriting {
public:
	static bool jsonWriting(const Json::Value& root, const string& path);//通用json文件写入函数

	static bool txtWriting(const vector<string>& strs, const string& path, bool modeFlag = false);//通用txt文件写入函数

	static bool historyProjWrite(const historyProj& hist, string path = "");//历史工程文档写入函数

	static bool projectInfoWrite(const projectInfo& proj, string path="");//工程文档写入函数

	static bool parameterWrite(parametersList& parData, string path = "");//参数文档写入函数

	static bool unitWrite(unitIndex& units, string path = "");//单位文件写入函数

	static bool calGroupWrite(calGroup& cals, string path = "");//计算组文档写入函数

	static bool tubeDataWrite(InteractionParams& intpar, string path = "");//tubeData文件写入函数

	static bool magDataWrite(InteractionParams& intpar, string path = "");//magData文件写入函数

	static bool beamDataWrite(InteractionParams& intpar, string path = "");//beamData文件写入函数

	static bool lossDataWrite(lossAndCutOffDataInfo& loss, string path = "");//lossData文件写入函数

	static bool dispDataWrite(dispDataInfo& disps, string path = "");//dispData文件写入函数

	static bool calSetWrite(calSetting& calSet, string path = "");//calSetting文件写入函数

	static bool paramLinkWrite(paramLinks& pars, string path = "");//paramLinks文件写入函数

	static bool resultsWrite(results& res, string path = "");//results文件写入函数

	static bool unitResWrite(results::res& res, string path,string seqName);

	static bool resParScanVarisWrite(calSetting::calSequence data, string path);

	static bool calGroupUniteWrite(calGroupUnite calGu, string path = "");	//calGroupUnite写入函数


};