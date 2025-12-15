#include "readCheck.h"

bool readCheck::histProjFileCheck(Json::Value& data, string path)
{
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	Json::Value replacement;
	string meg = "";

	if (path.empty()) {
		auto curWorkPath = std::filesystem::current_path();
		path = (curWorkPath /globalFilePos["hist"]).string();    //移植到Qt后可能需要修改
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root,path);

	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["histProj"], feedback, { path }));
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, histProjKeys, missingKeys, wrongTypeKeys,"", {1});
	if (firLayer.count(1)==0) {
		for (unsigned int i = 0; i < root[histProjKeys[1].first].size(); ++i) {
			string parent = histProjKeys[1].first + "["+to_string(i)+"]";
			if (jsonMemberCheck(root[histProjKeys[1].first][i], histProjSubKeys, missingKeys, wrongTypeKeys,parent, { 3,4 }).empty()) {
				replacement[histProjKeys[1].first].append(root[histProjKeys[1].first][i]);
			}
		}
	}
	data = replacement;
	
	if (!missingKeys.empty()) {
		usrData::getInstance().warnMesInsert(funcs::mesGen(projFileItems["histProj"], readCheckPrompt["missingKeys"],missingKeys));
	}
	return true;
}

bool readCheck::projectInfoFileCheck(map<string, unsigned int>& maps, vector<string>& text, string& path)
{
	vector<string> missingItems;
	vector<string> missingDataItems;
	bool state = true;

	if (path.empty()) {
		filesystem::path curProj(ctlData::getInstance().tempVaris.currentProject);
		if (curProj.empty()) {
			return false;
		}
		string projName = curProj.filename().string() + fileSuffix["proj"];
		path = (curProj/projName).string();
	}
	replace(path.begin(), path.end(), '\\', '/');
	if (!txtReading(text,path)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["projInfo"], readCheckPrompt["readFail"],{path}));
		return false;
	}
	maps = funcs::indiceSearch(text, projItems, missingItems);

	//若文件缺失数据项，则报错并返回false
	if (!missingItems.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["projInfo"], readCheckPrompt["missingItems"],missingItems));
		return false;
	}
	//检查文件是否缺失数据，即数据项下面没有对应的数据
	for (auto& it : projItems) {
		if (maps[it.second] + 1 == text.size() || maps.count(text[maps[it.second] + 1])) {
			missingDataItems.push_back(it.second);
		}
	}
	if (!missingDataItems.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["projInfo"], readCheckPrompt["missingDataItems"],missingDataItems));
		return false;
	}
	return true;
}

bool readCheck::parameterFileCheck(vector<vector<string>>& parData, string path)
{
	string meg = "";
	set<string> headers;
	vector<string> missingHeaders;
	if (path.empty()) {
		projectInfo proj = usrData::getInstance().proj;
		filesystem::path projPath(proj.directory);
		path = (projPath /proj.paramFlPath).string();

		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["parameter"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	if (_access(path.c_str(), 06) == -1) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["parameter"], readCheckPrompt["readFail"],{path}));
		return false;
	}

	//数据读取程序段
	rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));
	//判断是否缺失必要列
	for (size_t i = 0; i < doc.GetColumnCount(); i++) {
		headers.insert(doc.GetColumnName(i));
	}
	for (auto& it : paramHeaders) {
		if (!headers.count(it.second)) {
			missingHeaders.push_back(it.second);
		}
	}
	if (!missingHeaders.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["parameter"], readCheckPrompt["missingHeaders"],missingHeaders));
		return false;
	}

	parData.push_back(doc.GetColumn<string>(paramHeaders[1]));
	parData.push_back(doc.GetColumn<string>(paramHeaders[2]));
	parData.push_back(doc.GetColumn<string>(paramHeaders[3]));
	return true;
}

bool readCheck::unitFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		projectInfo proj = usrData::getInstance().proj;
		filesystem::path projPath(proj.directory);
		path = (projPath/proj.paramFlPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["unit"],readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["unit"], feedback, { path }));
		return false;
	}

	jsonMemberCheck(root, unitKeys, missingKeys, wrongTypeKeys);

	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["unit"], readCheckPrompt["missingKeys"],missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["unit"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::calGroupFileCheck(map<string, unsigned int>& maps, vector<string>& text, string& path)
{
	vector<string> missingItems;
	vector<string> missingDataItems;
	bool state = true;
	if (path.empty()) {
		ctlData& data = ctlData::getInstance();
		filesystem::path curProj(data.tempVaris.currentProject);
		curProj = curProj/data.tempVaris.currentCalGroup;
		string projName = data.tempVaris.currentCalGroup + fileSuffix["calGroup"];
		path = (curProj/ projName).string();
	}
	replace(path.begin(), path.end(), '\\', '/');
	if (!txtReading(text, path)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calGroup"], readCheckPrompt["readFail"], { path }));
		return false;
	}

	maps = funcs::indiceSearch(text, calGroupItems, missingItems);

	if (!missingItems.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calGroup"], readCheckPrompt["missingItems"], missingItems));
		return false;
	}
	for (auto& it : calGroupItems) {
		if (it.first != 12) {		//跳过检查的项，可以自定义
			if (maps[it.second] + 1 == text.size() || maps.count(text[maps[it.second] + 1])) {
				missingDataItems.push_back(it.second);
			}
		}
	}
	if (!missingDataItems.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calGroup"], readCheckPrompt["missingDataItems"], missingDataItems));
		return false;
	}
	return true;
	
	
}

bool readCheck::tubeDataFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;

	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/data.calG.tubeDPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["tubeData"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["tubeData"], feedback, { path }));
		return false;
	}
	jsonMemberCheck(root, tubeDataKeys, missingKeys, wrongTypeKeys);

	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["tubeData"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["tubeData"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;

}

bool readCheck::magDataFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/data.calG.magDPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["magData"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["magData"], feedback, { path }));
		return false;
	}
	
	set<int> firLayer = jsonMemberCheck(root, magDataKeys, missingKeys, wrongTypeKeys);
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[magDataKeys[1].first].size(); ++i) {
			string parent = magDataKeys[1].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[magDataKeys[1].first][i], magElementKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["magData"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["magData"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::beamDataFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	bool beamRateEleType = true;
	bool vrEleType = true;
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/data.calG.beamDPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["beamData"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["beamData"], feedback, { path }));
		return false;
	}
	jsonMemberCheck(root, beamDataKeys, missingKeys, wrongTypeKeys);
	for (auto& it : root[beamDataKeys[4].first]) {
		if (!it.isDouble()) {
			beamRateEleType = false;
		}
	}
	for (auto& it : root[beamDataKeys[5].first]) {
		if (!it.isDouble()) {
			vrEleType = false;
		}
	}
	if (!beamRateEleType) {
		string meg = beamDataKeys[4].first + "--" + readCheckPrompt["arrElement"] + "(double)";
		wrongTypeKeys.push_back(meg);
	}
	if (!vrEleType) {
		string meg = beamDataKeys[5].first + "--" + readCheckPrompt["arrElement"] + "(double)";
		wrongTypeKeys.push_back(meg);
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["beamData"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["beamData"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::lossDataFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/ data.calG.lossDPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["lossData"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["lossData"], feedback, { path }));
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root,lossDataKeys, missingKeys, wrongTypeKeys,"",{1,2,3});
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[lossDataKeys[1].first].size(); ++i) {
			string parent = lossDataKeys[1].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[lossDataKeys[1].first][i], cOffElementKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!firLayer.count(2)) {
		for (unsigned int i = 0; i < root[lossDataKeys[2].first].size(); ++i) {
			string parent = lossDataKeys[2].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[lossDataKeys[2].first][i], lossElementKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!firLayer.count(3)) {
		for (unsigned int i = 0; i < root[lossDataKeys[3].first].size(); ++i) {
			string parent = lossDataKeys[3].first + "[" + to_string(i) + "]";
			set<int> secLayer = jsonMemberCheck(root[lossDataKeys[3].first][i], dBElementKeys, missingKeys, wrongTypeKeys, parent);
			if (!secLayer.count(2)) {
				Json::Value rootTemp = root[lossDataKeys[3].first][i][dBElementKeys[2].first];
				for (unsigned int j = 0; j < rootTemp.size(); ++j) {
					string furtherParent = parent+" -- "+ dBElementKeys[2].first + "[" + to_string(j) + "]";
					jsonMemberCheck(rootTemp[j], dBsEleKeys, missingKeys, wrongTypeKeys, furtherParent);
				}
			}
		}
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["lossData"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["lossData"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::dispDataFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;

	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath / data.calG.dispDPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["dispData"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["dispData"], feedback, { path }));
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, dispDataKeys, missingKeys, wrongTypeKeys);
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[dispDataKeys[1].first].size(); ++i) {
			string parent = dispDataKeys[1].first + "[" + to_string(i) + "]";
			set<int> secLayer = jsonMemberCheck(root[dispDataKeys[1].first][i], dispElementKeys, missingKeys, wrongTypeKeys, parent, {5,6,7});
			Json::Value rootTemp = root[dispDataKeys[1].first][i];
			auto detailCheck = [&](int index) {
				if (index < 5 || index > 7) {	//此lambda函数只是用于检查kc、vp、alpha的数据，因此index只能为5，6，7，即dispElementKeys中三者的键值
					return;
				}
				if (!secLayer.count(index)) {
					string FurtherParent = parent + " -- " + dispElementKeys[index].first;
					bool freqType = true;
					bool valType = true;
					jsonMemberCheck(rootTemp[dispElementKeys[index].first], kvaEleKeys, missingKeys, wrongTypeKeys, FurtherParent);
					for (auto& it : rootTemp[dispElementKeys[index].first][kvaEleKeys[1].first]) {
						if (!it.isDouble()) {
							freqType = false;
						}
					}
					for (auto& it : rootTemp[dispElementKeys[index].first][kvaEleKeys[2].first]) {
						if (!it.isDouble()) {
							valType = false;
						}
					}
					if (!freqType) {
						string meg = FurtherParent + " -- " + kvaEleKeys[1].first + "--" + readCheckPrompt["arrElement"] + "(double)";
						wrongTypeKeys.push_back(meg);
					}
					if (!valType) {
						string meg = FurtherParent + " -- " + kvaEleKeys[2].first + "--" + readCheckPrompt["arrElement"] + "(double)";
						wrongTypeKeys.push_back(meg);
					}
				}
			};
			detailCheck(5);	//kc
			detailCheck(6);	//vp
			detailCheck(7);	//alpha

		}
	}

	if (!firLayer.count(2)) {
		for (unsigned int i = 0; i < root[dispDataKeys[2].first].size(); ++i) {
			string parent = dispDataKeys[2].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[dispDataKeys[2].first][i], samPointEleKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["dispData"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["dispData"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::calSetFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/ data.calG.calSPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calSet"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calSet"], feedback, { path }));
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, calSetKeys, missingKeys, wrongTypeKeys,"",{2});
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[calSetKeys[1].first].size(); ++i) {
			string parent = calSetKeys[1].first+ "[" + to_string(i) + "]";
			set<int> secLayer = jsonMemberCheck(root[calSetKeys[1].first][i], calElementKeys, missingKeys, wrongTypeKeys, parent,{3});
			if (!secLayer.count(2)) {
				Json::Value rootTemp = root[calSetKeys[1].first][i][calElementKeys[2].first];
				for (unsigned int j = 0; j < rootTemp.size(); ++j) {
					string furtherParent = parent+" -- "+ calElementKeys[2].first+ "[" + to_string(j) + "]";
					set<int> thdLayer = jsonMemberCheck(rootTemp[j], seqKeys, missingKeys, wrongTypeKeys, furtherParent);
					if (!thdLayer.count(2)&& rootTemp[j][seqKeys[2].first].isArray()) {
						bool arrType = true;
						for (auto& val : rootTemp[j][seqKeys[2].first]) {
							if (!val.isDouble()) {
								arrType = false;
							}
						}
						if (!arrType) {
							string meg = furtherParent+" -- "+seqKeys[2].first + "--" + readCheckPrompt["arrElement"] + "(double)";
							wrongTypeKeys.push_back(meg);
						}
					}
				}
			}
		}
	}
	if (!firLayer.count(2)) {
		string parent = calSetKeys[2].first;
		set<int> secLayer = jsonMemberCheck(root[calSetKeys[2].first], centerFreqKeys, missingKeys, wrongTypeKeys, parent,{5});
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calSet"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calSet"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::paramLinkFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/ data.calG.paramLPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["paramLink"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["paramLink"], feedback, { path }));
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, paramLinkKeys, missingKeys, wrongTypeKeys);
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[paramLinkKeys[1].first].size(); ++i) {
			string parent = paramLinkKeys[1].first+ "[" + to_string(i) + "]";
			jsonMemberCheck(root[paramLinkKeys[1].first][i], parInfoKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!missingKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["paramLink"], readCheckPrompt["missingKeys"], missingKeys));
		state = false;
	}
	if (!wrongTypeKeys.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["paramLink"], readCheckPrompt["jsonObjWrongType"], wrongTypeKeys));
		state = false;
	}
	data = root;
	return state;
}

bool readCheck::messageFileCheck(vector<string>& text, string path)
{
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath/data.calG.mesPath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["meg"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	replace(path.begin(), path.end(), '\\', '/');
	if (!txtReading(text, path)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["meg"], readCheckPrompt["readFail"], { path }));
		return false;
	}
	return true;
}

bool readCheck::unitResFileCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		return false;
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, unitResKeys, missingKeys, wrongTypeKeys, { 3 });
	if (!firLayer.count(2)) {
		string parent = unitResKeys[2].first;
		jsonMemberCheck(root[unitResKeys[2].first], resFreqyKeys, missingKeys, wrongTypeKeys, parent,{3,4});
	}
	if (!firLayer.count(3)) {
		for (unsigned int i = 0; i < root[unitResKeys[3].first].size(); ++i) {
			string parent = unitResKeys[3].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[unitResKeys[3].first][i], resParamsKeys, missingKeys, wrongTypeKeys, parent);
		}
	}
	if (!firLayer.count(6)) {
		string parent = unitResKeys[6].first;
		set<int> secLayler = jsonMemberCheck(root[unitResKeys[6].first], resResultKeys, missingKeys, wrongTypeKeys, parent);
		for (unsigned int j = 6; j <= 8; ++j) {
			if (!secLayler.count(j)) {
				string parent = unitResKeys[6].first + " -- " + resResultKeys[j].first;
				jsonMemberCheck(root[unitResKeys[6].first][resResultKeys[j].first], resultArraysEleKeys, missingKeys, wrongTypeKeys, parent,{2,3});
			}
		}
	}
	if (!missingKeys.empty() || !wrongTypeKeys.empty()) {
		return false;
	}
	data = root;
	return state;
}

bool readCheck::resParScanVarisCheck(Json::Value& data, string path)
{
	bool state = true;
	vector<string> missingKeys;
	vector<string> wrongTypeKeys;
	Json::Value root;
	if (path.empty()) {
		return false;
	}
	replace(path.begin(), path.end(), '\\', '/');
	string feedback = jsonReading(root, path);
	if (!feedback.empty()) {
		return false;
	}
	set<int> firLayer = jsonMemberCheck(root, resParScanVarisKeys, missingKeys, wrongTypeKeys,{2});
	if (!firLayer.count(1)) {
		for (unsigned int i = 0; i < root[resParScanVarisKeys[1].first].size(); ++i) {
			string parent = resParScanVarisKeys[1].first + "[" + to_string(i) + "]";
			jsonMemberCheck(root[resParScanVarisKeys[1].first][i], resVarisKeys, missingKeys, wrongTypeKeys,parent);
		}
	}
	if (!missingKeys.empty() || !wrongTypeKeys.empty()) {
		state = false;
	}
	data = root;
	return state;
}



string readCheck::jsonReading(Json::Value& data, const string& path)
{
	Json::Reader reader;

	std::ifstream file(path, ios::binary);

	if (!file.is_open()) {
		return readCheckPrompt["readFail"];
	}

	if (!reader.parse(file, data)) {
		return readCheckPrompt["parseFail"];
	}
	return "";
}

bool readCheck::txtReading(vector<string>& text, const string& path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		return false;  // 无法打开文件
	}

	std::string line;
	while (std::getline(file, line)) {
		line = funcs::trim(line);  // 去除前后空格
		if (!line.empty()) {
			text.push_back(std::move(line));  // 使用 std::move 避免拷贝
		}
	}

	return true;
}


set<int> readCheck::jsonMemberCheck(const Json::Value& root, map<int, pair<string,char>> keys, vector<string>& missingKeys, vector<string>& wrongTypeKeys ,const string& parent, set<int> dispensableKey)
{
	set<int> res;
	string strTemp = "";
	for (auto& it : keys) {
		bool state = true;
		if (parent.empty()) {
			strTemp = it.second.first;
		}
		else {
			strTemp = parent + " -- " + it.second.first;
		}
		if (!root.isMember(it.second.first)) {
			if (dispensableKey.count(it.first) == 0) {
				missingKeys.push_back(strTemp);
			}
			res.insert(it.first);
		}
		else {
			switch (it.second.second)
			{
			case 'a':
				state = root[it.second.first].isArray();
				break;
			case 'i':
				state = root[it.second.first].isInt();
				break;
			case 'u':
				state = root[it.second.first].isUInt();
				break;
			case 'd':
				state = root[it.second.first].isDouble();
				break;
			case 'b':
				state = root[it.second.first].isBool();
				break;
			case 's':
				state = root[it.second.first].isString();
				break;
			case 'o':
				state = root[it.second.first].isObject();
				break;
			case 'n':
				state = root[it.second.first].isNumeric();
				break;
			default:
				break;
			}
			if (!state) {
				strTemp += "(" + JsonValType[it.second.second] + ")";
				wrongTypeKeys.push_back(strTemp);
			}
		}
	}
	return res;
}

