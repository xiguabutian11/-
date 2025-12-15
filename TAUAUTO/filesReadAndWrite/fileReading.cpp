#include "fileReading.h"


//bool fileReading::jsonReading(Json::Value& data ,const string& path)
//{
//	Json::Reader reader;
//
//	std::ifstream file(path, ios::binary);
//
//	if (!file.is_open()){
//		throw fileFailExcept(path, "readFail");
//	}
//
//	if (!reader.parse(file, data)){
//		throw fileFailExcept(path, "parseFail");
//	}
//	else {
//		//return root;
//	}
//	return true;
//}


//bool fileReading::txtReading(vector<string>& text, const string& path)
//{
//	std::ifstream file(path);
//	if (!file.is_open()) {
//		return false;
//	}
//
//	std::string line;
//	while (std::getline(file, line)) {
//		line.erase(0, line.find_first_not_of(" "));  //去除前空格
//		line.erase(line.find_last_not_of(" ") + 1);	 //去除后空格
//		if (line != "") {
//			text.push_back(line);
//		}
//	}
//	file.close();
//	return true;
//}


bool fileReading::historyProjRead(historyProj& hist, string path)
{
	Json::Value root;
	if (!readCheck::histProjFileCheck(root, path)) {
		return false;
	}
	hist.histList.clear();
	for (unsigned int i = 0; i < root[histProjKeys[1].first].size(); i++) {
		projectInfo projTemp;
		projTemp.name = root[histProjKeys[1].first][i][histProjSubKeys[1].first].asString();
		projTemp.directory = root[histProjKeys[1].first][i][histProjSubKeys[2].first].asString();
		projTemp.creationTime = root[histProjKeys[1].first][i][histProjSubKeys[3].first].asString();
		projTemp.lastSaveTime = root[histProjKeys[1].first][i][histProjSubKeys[4].first].asString();

		if (projTemp.name != "" && projTemp.directory != "") {
			hist.histList.push_back(projTemp);
		}
	}
	return true;
	
}


bool fileReading::projectInfoRead(projectInfo& proj, string path)
{
	map<string, unsigned int> maps;
	vector<string> text;
	if (!readCheck::projectInfoFileCheck(maps, text,path)) {
		return false;
	}
	proj.name = text[maps[projItems[1]] + 1];
	proj.creationTime = text[maps[projItems[2]] + 1];
	proj.lastSaveTime = text[maps[projItems[3]] + 1];
	proj.directory = path.substr(0, path.find_last_of("/"));
	proj.calGroups.clear();
	for (unsigned int i = maps[projItems[4]]+1; i < text.size() && maps.count(text[i]) == 0; ++i) {
		proj.calGroups.push_back(text[i]);
	}
	proj.paramFlPath = text[maps[projItems[5]] + 1];
	proj.unitFlPath = text[maps[projItems[6]] + 1];

	return true;
}


bool fileReading::parameterRead(parametersList& pars, string path)
{
	vector<vector<string>> parData;
	if (!readCheck::parameterFileCheck(parData, path)) {
		return false;
	}
	pars.paramList.clear();
	pars.orders.clear();
	for (size_t i = 0; i < parData[0].size(); i++) {
		parametersList::parameter par;
		par.name = parData[0][i];
		if (i < parData[1].size()) {			//if判断是为了防止name、expr、desc长度不一样时索引值超出范围而报错
			par.expression = parData[1][i];
		}
		else {
			par.expression = "0";
		}
		if (i < parData[2].size()) {			//同上
			par.description = parData[2][i];
		}
		pars.paramList[par.name] = par;
		pars.orders.push_back(par.name);
	}
	return true;
}

bool fileReading::unitRead(unitIndex& units, string path)
{
	Json::Value root;
	if (!readCheck::unitFileCheck(root, path)) {
		return false;
	}
	units = unitIndex();
	units.voltageIndex = root[unitKeys[1].first].asUInt();
	units.currentIndex = root[unitKeys[2].first].asUInt();
	units.frequencyIndex = root[unitKeys[3].first].asUInt();
	units.lengthIndex = root[unitKeys[4].first].asUInt();
	return true;
}

bool fileReading::calGroupRead(calGroup& cals, string path)
{
	map<string, unsigned int> maps;
	vector<string> text;
	if (!readCheck::calGroupFileCheck(maps, text, path)) {
		return false;
	}
	cals = calGroup();
	cals.directory = path.substr(0, path.find_last_of("/"));
	cals.name = text[maps[calGroupItems[1]]+1];
	cals.creationTime = text[maps[calGroupItems[2]] + 1];
	cals.lastSaveTime = text[maps[calGroupItems[3]] + 1];
	cals.tubeDPath = text[maps[calGroupItems[4]] + 1];
	cals.magDPath = text[maps[calGroupItems[5]] + 1];
	cals.beamDPath = text[maps[calGroupItems[6]] + 1];
	cals.lossDPath = text[maps[calGroupItems[7]] + 1];
	cals.dispDPath = text[maps[calGroupItems[8]] + 1];
	cals.calSPath = text[maps[calGroupItems[9]] + 1];
	cals.mesPath = text[maps[calGroupItems[10]] + 1];
	cals.paramLPath = text[maps[calGroupItems[11]] + 1];
	if (text.size() > maps[calGroupItems[12]] + 1 && maps.count(text[maps[calGroupItems[12]] + 1]) == 0) {
		cals.resFilePath = text[maps[calGroupItems[12]] + 1];
	}
	return true;
}

bool fileReading::tubeDataRead(InteractionParams& intpar, string path)
{
	Json::Value root;
	if (!readCheck::tubeDataFileCheck(root, path)) {
		return false;
	}
	intpar.V = root[tubeDataKeys[1].first].asDouble();
	intpar.I = root[tubeDataKeys[2].first].asDouble();
	intpar.tubelength = root[tubeDataKeys[3].first].asDouble();
	intpar.interstep = root[tubeDataKeys[4].first].asDouble();
	intpar.harmonicses = root[tubeDataKeys[5].first].asInt();
	intpar.NumofElectrons = root[tubeDataKeys[6].first].asInt();
	intpar.Sites_of_stability = root[tubeDataKeys[7].first].asBool();
	intpar.OutForCollector = root[tubeDataKeys[8].first].asBool();
	intpar.FullOutput = root[tubeDataKeys[9].first].asBool();

	return true;
}

bool fileReading::magDataRead(InteractionParams& intpar, string path)
{
	Json::Value root;
	if (!readCheck::magDataFileCheck(root, path)) {
		return false;
	}
	intpar.data_mag.clear();
	for (unsigned int i = 0; i < root[magDataKeys[1].first].size(); i++) {
		MagneticField magTemp;
		magTemp.begin = root[magDataKeys[1].first][i][magElementKeys[1].first].asDouble();
		magTemp.period = root[magDataKeys[1].first][i][magElementKeys[2].first].asDouble();
		magTemp.m0 = root[magDataKeys[1].first][i][magElementKeys[3].first].asDouble();
		magTemp.m1 = root[magDataKeys[1].first][i][magElementKeys[4].first].asDouble();
		magTemp.m2 = root[magDataKeys[1].first][i][magElementKeys[5].first].asDouble();

		intpar.data_mag.push_back(magTemp);
	}
	return true;
}

bool fileReading::beamDataRead(InteractionParams& intpar, string path)
{
	Json::Value root;
	if (!readCheck::beamDataFileCheck(root, path)) {
		return false;
	}
	intpar.beamdata.Rpna = root[beamDataKeys[1].first].asDouble();
	intpar.beamdata.Rpvn = root[beamDataKeys[2].first].asDouble();
	intpar.beamdata.swsr = root[beamDataKeys[3].first].asDouble();
	intpar.beamdata.beamrate.clear();
	intpar.beamdata.vr.clear();
	for (unsigned int i = 0; i < root[beamDataKeys[4].first].size();++i) {
		intpar.beamdata.beamrate.push_back(root[beamDataKeys[4].first][i].asDouble());
	}
	for (unsigned int i = 0; i < root[beamDataKeys[5].first].size(); ++i) {
		intpar.beamdata.vr.push_back(root[beamDataKeys[5].first][i].asDouble());
	}
	intpar.Igun = root[beamDataKeys[6].first].asUInt();

	return true;
}

bool fileReading::lossDataRead(lossAndCutOffDataInfo& loss, string path)
{
	Json::Value root;
	if (!readCheck::lossDataFileCheck(root, path)) {
		return false;
	}
	loss = lossAndCutOffDataInfo();
	for (unsigned int i = 0; i < root[lossDataKeys[1].first].size(); ++i) {
		drft cutOff;
		cutOff.begin = root[lossDataKeys[1].first][i][cOffElementKeys[1].first].asDouble();
		cutOff.end = root[lossDataKeys[1].first][i][cOffElementKeys[2].first].asDouble();

		loss.cutOffs.push_back(cutOff);
	}
	for (unsigned int i = 0; i < root[lossDataKeys[2].first].size(); ++i) {
		lossAndCutOffDataInfo::lossSegment lossSeg;
		lossSeg.begin = root[lossDataKeys[2].first][i][lossElementKeys[1].first].asDouble();
		lossSeg.end = root[lossDataKeys[2].first][i][lossElementKeys[2].first].asDouble();
		lossSeg.lossType = root[lossDataKeys[2].first][i][lossElementKeys[3].first].asInt();
		lossSeg.defaultDistriType = root[lossDataKeys[2].first][i][lossElementKeys[4].first].asUInt();
		lossSeg.defaultdB = root[lossDataKeys[2].first][i][lossElementKeys[5].first].asDouble();

		loss.lossSegs.push_back(lossSeg);
	}
	for (unsigned int i = 0; i < root[lossDataKeys[3].first].size(); ++i) {
		double freq = root[lossDataKeys[3].first][i][dBElementKeys[1].first].asDouble();
		vector<lossAndCutOffDataInfo::dBofFreqs> dBs;
		for (unsigned int j = 0; j < root[lossDataKeys[3].first][i][dBElementKeys[2].first].size(); ++j) {
			lossAndCutOffDataInfo::dBofFreqs dBTemp;
			Json::Value rootTemp = root[lossDataKeys[3].first][i][dBElementKeys[2].first][j];
			dBTemp.dB = rootTemp[dBsEleKeys[1].first].asDouble();
			dBTemp.distriType = rootTemp[dBsEleKeys[2].first].asUInt();
			dBs.push_back(dBTemp);
		}
		loss.dBInfo.insert_or_assign(freq, dBs);
	}
	return true;
}

bool fileReading::dispDataRead(dispDataInfo& disp, string path)
{
	Json::Value root;
	if (!readCheck::dispDataFileCheck(root, path)) {
		return false;
	}
	disp = dispDataInfo();
	auto dispTempFunc = [&](int index,int i)->map<double, double, DoubleComparator> {
		map<double, double, DoubleComparator> maps;
		if (index < 5 || index >7) {
			return maps;
		}
		if (root[dispDataKeys[1].first][i].isMember(dispElementKeys[index].first)) {
			vector<double> freq;
			vector<double> val;
			for (auto& it : root[dispDataKeys[1].first][i][dispElementKeys[index].first][kvaEleKeys[1].first]) {
				freq.push_back(it.asDouble());
			}
			for (auto& it : root[dispDataKeys[1].first][i][dispElementKeys[index].first][kvaEleKeys[2].first]) {
				val.push_back(it.asDouble());
			}
			for (unsigned int j = 0; j < min(freq.size(), val.size()); ++j) {
				maps.insert_or_assign(freq[j], val[j]);
			}
		}
		return maps;
	};

	for (unsigned int i = 0; i < root[dispDataKeys[1].first].size(); ++i) {
		string name = "";
		dispDataInfo::dispData dispTemp;
		name = root[dispDataKeys[1].first][i][dispElementKeys[1].first].asString();
		dispTemp.fileType = root[dispDataKeys[1].first][i][dispElementKeys[2].first].asString();
		dispTemp.path = root[dispDataKeys[1].first][i][dispElementKeys[3].first].asString();
		dispTemp.skipHeader = root[dispDataKeys[1].first][i][dispElementKeys[4].first].asUInt();
		
		dispTemp.kc = dispTempFunc(5,i);
		dispTemp.vp = dispTempFunc(6,i);
		dispTemp.al = dispTempFunc(7,i);
		if (dispTemp.kc.empty() || dispTemp.vp.empty() || dispTemp.al.empty()) {
			if (dispTemp.fileType == dispFilesSuffix[0]) {
				dispDataInTXT(dispTemp);
			}
			else {
				dispDataInCSV(dispTemp);
			}
		}
		disp.dispInfo.insert_or_assign(name, dispTemp);
		disp.orders.push_back(name);
	}
	
	for (unsigned int i = 0; i < root[dispDataKeys[2].first].size(); ++i) {
		double point = 0;
		dispDataInfo::samplePoint samPoint;
		point = root[dispDataKeys[2].first][i][samPointEleKeys[1].first].asDouble();
		samPoint.dispKey = root[dispDataKeys[2].first][i][samPointEleKeys[2].first].asString();
		samPoint.interpoType = root[dispDataKeys[2].first][i][samPointEleKeys[3].first].asUInt();

		disp.samplePoints.insert_or_assign(point, samPoint);
	}
	return true;
}

bool fileReading::calSetRead(calSetting& calSet, string path)
{
	Json::Value root;
	if (!readCheck::calSetFileCheck(root, path)) {
		return false;
	}
	calSet.calSet.clear();
	for (unsigned int i = 0; i < root[calSetKeys[1].first].size(); ++i) {
		calSetting::calSequence cal;
		cal.name = root[calSetKeys[1].first][i][calElementKeys[1].first].asString();
		for (unsigned int j = 0; j < root[calSetKeys[1].first][i][calElementKeys[2].first].size(); ++j) {
			signedArray<double, bool> arr;
			arr.sign = root[calSetKeys[1].first][i][calElementKeys[2].first][j][seqKeys[1].first].asString();
			for (auto& val:root[calSetKeys[1].first][i][calElementKeys[2].first][j][seqKeys[2].first]) {
				arr.array.push_back(val.asDouble());
			}
			arr.signNum = root[calSetKeys[1].first][i][calElementKeys[2].first][j][seqKeys[3].first].asUInt();
			cal.seq.insert_or_assign(arr.sign, arr);
		}
		if (root[calSetKeys[1].first][i].isMember(calElementKeys[3].first)) {
			cal.soloCal = static_cast<bool>(root[calSetKeys[1].first][i][calElementKeys[3].first].asUInt());
		}
		calSet.calSet.insert_or_assign(cal.name, cal);
	}
	if (root.isMember(calSetKeys[2].first)) {
		calSet.centerFreq.freq = root[calSetKeys[2].first][centerFreqKeys[1].first].asDouble();
		calSet.centerFreq.pin = root[calSetKeys[2].first][centerFreqKeys[2].first].asDouble();
		calSet.centerFreq.pin1 = root[calSetKeys[2].first][centerFreqKeys[3].first].asDouble();
		calSet.centerFreq.pin2 = root[calSetKeys[2].first][centerFreqKeys[4].first].asDouble();
		if (root[calSetKeys[2].first].isMember(centerFreqKeys[5].first)) {
			calSet.centerFreq.desc = root[calSetKeys[2].first][centerFreqKeys[5].first].asString();
		}
	}
	return true;
}

bool fileReading::paramLinkRead(paramLinks& pars, string path)
{
	Json::Value root;
	if (!readCheck::paramLinkFileCheck(root, path)) {
		return false;
	}
	pars.parLinks.clear();
	for (auto& par : root[paramLinkKeys[1].first]) {
		paramLinks::parLink parTemp;
		parTemp.index1 = par[parInfoKeys[1].first].asUInt();
		parTemp.index2 = par[parInfoKeys[2].first].asUInt();
		parTemp.index3 = par[parInfoKeys[3].first].asUInt();
		parTemp.expression = par[parInfoKeys[4].first].asString();
		parTemp.decription = par[parInfoKeys[5].first].asString();
		parTemp.dataType = par[parInfoKeys[6].first].asString();

		string key = to_string(parTemp.index1) + "-" + to_string(parTemp.index2) + "-" + to_string(parTemp.index3);
		if (pars.parLinks.count(parTemp.dataType) == 0) {
			map<string, paramLinks::parLink> parLinkTemp;
			parLinkTemp.insert_or_assign(key, parTemp);
			pars.parLinks.insert_or_assign(parTemp.dataType, parLinkTemp);
		}
		else {
			pars.parLinks.at(parTemp.dataType).insert_or_assign(key, parTemp);
		}
	}
	return true;
}

bool fileReading::resultsRead(results& reses, string path)
{
	if (path.empty()) {
		calGroupUnite data = usrData::getInstance().curCalGroup;
		filesystem::path calGPath(data.calG.directory);
		path = (calGPath / data.calG.resFilePath).string();
		if (path.empty()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["res"], readCheckPrompt["defltPathFail"]));
			return false;
		}
	}
	filesystem::path resPath(path);
	if (!filesystem::exists(resPath) || !filesystem::is_directory(resPath)) {
		return false;
	}
	reses = results();

	for (const auto& entry : filesystem::directory_iterator(resPath)) {
		if (entry.is_directory()) {
			string seqName = entry.path().filename().string();
			reses.reses[seqName] = vector<results::res>();
			reses.BGDatas[seqName] = results::BGDataOfRes();
			string parScanVarisFileName = resParScanVarisKeys[1].first + fileSuffix["resParScanVaris"];
			for (const auto& file : filesystem::directory_iterator(entry)) {
				if (!file.is_regular_file()) {
					continue;
				}
				if (file.path().filename() != parScanVarisFileName) {
					results::res tempRes;
					tempRes.name = file.path().filename().string();
					reses.reses[seqName].push_back(tempRes);
				}
				else {
					resParScanVarisRead(reses.BGDatas[seqName].calS, file.path().string());
				}
			}
			sort(reses.reses[seqName].begin(), reses.reses[seqName].end(), [](results::res& a, results::res& b) {
				if (a.name.size() != b.name.size()) {
					return a.name.size() < b.name.size();
				}
				return a.name < b.name;
			});
		}
	}
	return true;

}

bool fileReading::unitResRead(results::res& res, string path)
{
	Json::Value root;
	if (!readCheck::unitResFileCheck(root, path)) {
		return false;
	}
	res = results::res();
	res.name = root[unitResKeys[1].first].asString();
	res.freqy.freq = root[unitResKeys[2].first][resFreqyKeys[1].first].asDouble();
	res.freqy.pin = root[unitResKeys[2].first][resFreqyKeys[2].first].asDouble();
	if (root[unitResKeys[2].first].isMember(resFreqyKeys[3].first)) {
		res.freqy.pin1 = root[unitResKeys[2].first][resFreqyKeys[3].first].asDouble();
	}
	if (root[unitResKeys[2].first].isMember(resFreqyKeys[4].first)) {
		res.freqy.pin2 = root[unitResKeys[2].first][resFreqyKeys[4].first].asDouble();
	}
	for (unsigned int i = 0; i < root[unitResKeys[3].first].size(); ++i) {
		string par = root[unitResKeys[3].first][i][resParamsKeys[1].first].asString();
		double val = root[unitResKeys[3].first][i][resParamsKeys[2].first].asDouble();
		res.params.insert_or_assign(par, val);
	}
	for (unsigned int i = 0; i < root[unitResKeys[4].first].size(); ++i) {
		res.tube.push_back(root[unitResKeys[4].first][i].asDouble());
	}
	for (unsigned int i = 0; i < root[unitResKeys[5].first].size(); ++i) {
		res.mag.push_back(root[unitResKeys[5].first][i].asDouble());
	}
	res.result.freq = root[unitResKeys[6].first][resResultKeys[1].first].asDouble();
	res.result.Pso = root[unitResKeys[6].first][resResultKeys[2].first].asDouble();
	res.result.len_pso = root[unitResKeys[6].first][resResultKeys[3].first].asDouble();
	for (auto& it: root[unitResKeys[6].first][resResultKeys[4].first]) {
		res.result.FlowRate.push_back(it.asDouble());
	}
	for (auto& it : root[unitResKeys[6].first][resResultKeys[5].first]) {
		res.result.speed.push_back(it.asDouble());
	}
	auto func = [&](Json::Value rootTemp, vector<vector<double>>& arrs) {
		for (unsigned int i = 1; i <= 3; ++i) {
			if (rootTemp.isMember(resultArraysEleKeys[i].first)) {
				vector<double> arr;
				for (auto& it : rootTemp[resultArraysEleKeys[i].first]) {
					arr.push_back(it.asDouble());
				}
				arrs.push_back(arr);
			}
			else {
				return;
			}
		}
	};
	func(root[unitResKeys[6].first][resResultKeys[6].first], res.result.Pout);
	func(root[unitResKeys[6].first][resResultKeys[7].first], res.result.Gain);
	func(root[unitResKeys[6].first][resResultKeys[8].first], res.result.eff);
	return true;
}

bool fileReading::unitResSetRead(vector<results::res>& reses, string path)
{
	reses.clear();
	filesystem::path seqPath(path);
	if (!filesystem::exists(seqPath) || !filesystem::is_directory(seqPath)) {
		return false;
	}
	string seqName = seqPath.filename().string();
	string parScanVarisFileName = resParScanVarisKeys[1].first + fileSuffix["resParScanVaris"];
	for (const auto& file : filesystem::directory_iterator(seqPath)) {
		if (!file.is_regular_file()) {
			continue;
		}
		if (file.path().filename() != parScanVarisFileName) {
			results::res resTemp;
			if (unitResRead(resTemp, file.path().string())) {
				reses.push_back(resTemp);
			}
		}
	}
	sort(reses.begin(), reses.end(), [](results::res& a, results::res& b) {
		if (a.name.size() != b.name.size()) {
			return a.name.size() < b.name.size();
		}
		return a.name < b.name;
	});
	return true;
}

bool fileReading::resParScanVarisRead(calSetting::calSequence& data, string path)
{
	Json::Value root;
	if (!readCheck::resParScanVarisCheck(root, path)) {
		return false;
	}
	data.seq.clear();
	for (auto& par : root[resParScanVarisKeys[1].first]) {
		signedArray<double, bool> arr;
		arr.sign = par[resVarisKeys[1].first].asString();
		arr.signNum = par[resVarisKeys[2].first].asUInt();
		for (auto& val : par[resVarisKeys[3].first]) {
			arr.array.push_back(val.asDouble());
		}
		data.seq.insert_or_assign(arr.sign, arr);
	}
	if (root.isMember(resParScanVarisKeys[2].first)) {
		data.soloCal = static_cast<bool>(root[resParScanVarisKeys[2].first].asUInt());
	}
	return true;
}

bool fileReading::messageRead(vector<signedString>& meg, string path)
{
	vector<string> text;
	string type, message, time;
	if (!readCheck::messageFileCheck(text, path)) {
		return false;
	}
	auto isUnsignedInt = [=](string str)->bool {
		std::regex pattern("^[0-9]+$"); // 正则表达式匹配纯数字
		return std::regex_match(str, pattern);
	};
	for (auto& it : text) {
		std::istringstream iss(it);
		if (!(iss >> type >> message>> time)) {
			continue;
		}
		if (isUnsignedInt(type)) {
			signedString strTemp;
			strTemp.flag = stoul(type);
			strTemp.str = message;
			strTemp.time = time;
			meg.push_back(strTemp);
		}
	}
	return true;
}

bool fileReading::calGroupUniteRead(calGroupUnite& calGu, string path)
{
	bool state = true;
	filesystem::path calGPath(path);
	filesystem::path filePath;
	filePath = calGPath / (calGPath.filename().string()+ fileSuffix["calGroup"]);
	if (!calGroupRead(calGu.calG, filePath.string())) {
		return false;
	}
	filePath = calGPath / calGu.calG.calSPath;
	if (!calSetRead(calGu.calS,filePath.string())) {
		state =  false;
	}
	filePath = calGPath / calGu.calG.paramLPath;
	if (!paramLinkRead(calGu.parLink, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.mesPath;
	if (!messageRead(calGu.message, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.tubeDPath;
	if (!tubeDataRead(calGu.intpar, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.magDPath;
	if (!magDataRead(calGu.intpar, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.beamDPath;
	if (!beamDataRead(calGu.intpar, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.lossDPath;
	if (!lossDataRead(calGu.loss, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.dispDPath;
	if (!dispDataRead(calGu.disp, filePath.string())) {
		state = false;
	}
	filePath = calGPath / calGu.calG.resFilePath;
	if (!resultsRead(calGu.res, filePath.string())) {
		state = false;
	}
	return state;
}



unsigned int fileReading::dispDataInTXT(dispDataInfo::dispData& disp)
{
	unsigned int flag = 14;
	//将色散数据往结构体中赋值的操作
	for (unsigned int i = 1; i < 4; ++i) {
		vector<string> text;
		map<double, double, DoubleComparator> objTemp;
		string element1,element2;
		if (!readCheck::txtReading(text, disp.path + "/" + dispTxtFileInfo[i])) {
			continue;
		}
		flag = flag & ~(1 << i);
		for (unsigned int j = disp.skipHeader; j < text.size(); ++j) {
			std::istringstream iss(text[j]);
			if (!(iss >> element1 >> element2)) {
				continue;
			}
			if (funcs::isNum(element1) && funcs::isNum(element2)) {
				objTemp.insert_or_assign(stod(element1), stod(element2));
			}
		}
		if (objTemp.empty()) {
			continue;
		}
		switch (i)
		{
		case 1:
			disp.kc = objTemp;
			break;
		case 2:
			disp.vp = objTemp;
			break;
		case 3:
			disp.al = objTemp;
			break;
		}
	}
	return flag;
}

bool fileReading::dispDataInCSV(dispDataInfo::dispData& disp)
{
	try
	{
		//数据读取程序段
		rapidcsv::Document doc(disp.path, rapidcsv::LabelParams(-1, -1));
		//判断是否缺失列（由于未定义标头，所以无法准确确定缺少谁，只能判断列数）
		if (doc.GetColumnCount() < dispCsvFileInfo["columnColumn"]) {
			return false;
		}
		vector<string> noNumeric;
		string strTemp;
		for (unsigned int i = 0; i < doc.GetRowCount(); i++) {
			double key = 0;
			strTemp = doc.GetCell<string>(dispCsvFileInfo["freq"], i);
			if (!funcs::isNum(strTemp)) {
				continue;
			}
			key = stod(strTemp);
			strTemp = doc.GetCell<string>(dispCsvFileInfo["vp"], i);
			if (funcs::isNum(strTemp)) {
				disp.vp.insert_or_assign(key, stod(strTemp));
			}
			strTemp = doc.GetCell<string>(dispCsvFileInfo["kc"], i);
			if (funcs::isNum(strTemp)) {
				disp.kc.insert_or_assign(key, stod(strTemp));
			}
			strTemp = doc.GetCell<string>(dispCsvFileInfo["al"], i);
			if (funcs::isNum(strTemp)) {
				disp.al.insert_or_assign(key, stod(strTemp));
			}
		}
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}
