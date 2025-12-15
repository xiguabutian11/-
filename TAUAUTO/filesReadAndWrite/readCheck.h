#pragma once
#include "constData\structDefine.h"
#include "commonFuncs\funcsTwo.h"
#include "usrDataInfo\usrData.h"

class readCheck {
protected:
	static bool histProjFileCheck(Json::Value& data, string path);

	static bool projectInfoFileCheck(map<string,unsigned int>& maps,vector<string>& text,string& path);

	static bool parameterFileCheck(vector<vector<string>>& parData,string path);

	static bool unitFileCheck(Json::Value& data, string path);

	static bool calGroupFileCheck(map<string, unsigned int>& maps, vector<string>& text, string& path);

	static bool tubeDataFileCheck(Json::Value& data, string path);

	static bool magDataFileCheck(Json::Value& data, string path);

	static bool beamDataFileCheck(Json::Value& data, string path);

	static bool lossDataFileCheck(Json::Value& data, string path);

	static bool dispDataFileCheck(Json::Value& data, string path);

	static bool calSetFileCheck(Json::Value& data, string path);

	static bool paramLinkFileCheck(Json::Value& data, string path);

	static bool messageFileCheck(vector<string>& text, string path);

	static bool unitResFileCheck(Json::Value& data, string path);

	static bool resParScanVarisCheck(Json::Value& data, string path);



	static string jsonReading(Json::Value& data, const string& path);//通用json文件读取函数

	static bool txtReading(vector<string>& text, const string& path);//通用txt文件读取函数

private:
	static set<int>	jsonMemberCheck(const Json::Value& root, map<int, pair<string,char>> keys, vector<string>& missingKeys,vector<string>&wrongTypeKeys, const string& parent = "", set<int>dispensableKey = {});
};
