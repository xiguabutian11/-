
#include "projManage.h"

bool projManage::newProjInfo(projectInfo& proj, string path, string projName)
{
	// 获取当前工作路径
	std::filesystem::path currentWorkPath = std::filesystem::current_path();
	std::filesystem::path projPath(path);
	std::filesystem::path folderPath(ProjPAUFolders[1]);

	if (projPath.empty()) {
		if (!globalDataFolderCreate()) {
			usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newProj"], projManageErrPrompt["projDfltPathErr"]));
			return false;
		}
		projPath = currentWorkPath / globalFilePos["dfltProj"];
	}
	if (!filesystem::exists(projPath)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newProj"], projManageErrPrompt["unavailDir"], { projPath.string() }));
		return false;
	}
	if (!filesystem::is_directory(projPath)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newProj"], projManageErrPrompt["notDir"], { projPath.string() }));
		return false;
	}

	if (projName.empty()) {
		unsigned int i = 1;
		string defaultName = ctlData::getInstance().dftValue.defaultProjName;
		while (1) {
			string name = defaultName + "_" + to_string(i);
			if (!filesystem::exists(projPath / name)) {
				projName = name;
				break;
			}
			++i;
		}
	}
	proj.name = projName;
	proj.creationTime = currentDateTime(1);
	proj.directory = (projPath / projName).string();
	proj.paramFlPath = (folderPath / PAUFolderFiles[1]).string();
	proj.unitFlPath = (folderPath / PAUFolderFiles[2]).string();

	return true;
}

bool projManage::projFolderGen(const projectInfo& proj)
{
	vector<string> projErrPath = {};
	filesystem::path projPath(proj.directory);
	filesystem::path filePath = "";
	if (!std::filesystem::exists(projPath) && !newEmptyFolder(projPath)) {
		string obj = proManageObj["proj"] + " -- " + proj.name;
		usrData::getInstance().errMesInsert(funcs::mesGen(obj, projManageErrPrompt["projCreateFail"], { projPath.string() }));
		return false;
	}
	filePath = projPath / (proj.name + fileSuffix["proj"]);
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		projErrPath.push_back(filePath.string());
	}
	filePath = projPath / proj.paramFlPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		projErrPath.push_back(filePath.string());
	}
	filePath = projPath / proj.unitFlPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		projErrPath.push_back(filePath.string());
	}

	if (!projErrPath.empty()) {
		string obj = proManageObj["proj"] + " -- " + proj.name;
		usrData::getInstance().errMesInsert(funcs::mesGen(obj, projManageErrPrompt["projCreateFail"], projErrPath));
		return false;
	}
	return true;
}

bool projManage::newCalGroupStruct(calGroup& cal, string path, string name)
{
	std::filesystem::path targetPath(path);
	filesystem::path folderPath(CalGFolders[1]);
	if (targetPath.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newCalGroup"], projManageErrPrompt["emptyDir"]));
		return false;
	}
	if (!filesystem::exists(targetPath)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newCalGroup"], projManageErrPrompt["unavailDir"], { targetPath.string() }));
		return false;
	}
	if (!filesystem::is_directory(targetPath)) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["newCalGroup"], projManageErrPrompt["notDir"], { targetPath.string() }));
		return false;
	}
	if (name.empty()) {
		unsigned int i = 1;
		string defaultName = ctlData::getInstance().dftValue.defaultCalGroupName;
		while (1) {
			name = defaultName + "_" + to_string(i);
			if (!filesystem::exists(targetPath / name)) {
				break;
			}
			++i;
		}
	}
	cal.name = name;
	cal.creationTime = currentDateTime(1);
	cal.directory = (targetPath / name).string();
	cal.tubeDPath = (folderPath / tubeDFolderFiles[1]).string();
	cal.magDPath = (folderPath / tubeDFolderFiles[2]).string();
	cal.beamDPath = (folderPath / tubeDFolderFiles[3]).string();
	cal.lossDPath = (folderPath / tubeDFolderFiles[4]).string();
	cal.dispDPath = (folderPath / tubeDFolderFiles[5]).string();
	cal.calSPath = calGFiles[1];
	cal.mesPath = calGFiles[2];
	cal.paramLPath = calGFiles[3];
	cal.resFilePath = CalGFolders[2];

	return true;
}

bool projManage::calGroupFolderGen(const calGroup& cal, set<string> resSeqNames)
{
	vector<string> calGErrPath = {};
	filesystem::path calGPath(cal.directory);
	filesystem::path filePath = "";
	filesystem::path folderPath = "";

	if (!std::filesystem::exists(calGPath) && !newEmptyFolder(calGPath)) {
		string obj = proManageObj["calGroup"] + " -- " + cal.name;
		usrData::getInstance().errMesInsert(funcs::mesGen(obj, projManageErrPrompt["calGCreateFail"], { calGPath.string() }));
		return false;
	}
	filePath = calGPath / (cal.name + fileSuffix["calGroup"]);
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.calSPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.mesPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.paramLPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.tubeDPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.magDPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.beamDPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.lossDPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.dispDPath;
	if (!std::filesystem::exists(filePath) && !newEmptyFile(filePath)) {
		calGErrPath.push_back(filePath.string());
	}
	folderPath = calGPath / CalGFolders[2];
	if (!std::filesystem::exists(folderPath) && !newEmptyFolder(folderPath)) {
		calGErrPath.push_back(filePath.string());
	}
	else {
		for (auto& seq : resSeqNames) {
			filesystem::path tempPath = folderPath / seq;
			if (!std::filesystem::exists(tempPath) && !newEmptyFolder(tempPath)) {
				calGErrPath.push_back(tempPath.string());
				continue;
			}
			projectInfo proj;
			calGroup cal;
			string defaultProjName = ctlData::getInstance().dftValue.defaultProjName;
			newProjInfo(proj, folderPath.string(),defaultProjName+"_1") && projFolderGen(proj) && newCalGroupStruct(cal, proj.directory) && calGroupFolderGen(cal);
		}
	}

	if (!calGErrPath.empty()) {
		string obj = proManageObj["newCalGroup"] + " -- " + cal.name;
		usrData::getInstance().errMesInsert(funcs::mesGen(obj, projManageErrPrompt["calGCreateFail"], calGErrPath));
		return false;
	}
	return true;
}

bool projManage::newProj(string path, string projName)
{
	projectInfo proj;
	calGroup cal;
	if (newProjInfo(proj, path, projName) && projFolderGen(proj) && newCalGroupStruct(cal, proj.directory) && calGroupFolderGen(cal)) {
		proj.calGroups.push_back(cal.name);
		ctlData::getInstance().setCurrentProj(proj.directory);
		ctlData::getInstance().setCurrentCalGroup(cal.name);
		usrData::getInstance().setDataProj(proj);
		usrData::getInstance().setDataPars(parametersList());
		usrData::getInstance().setDataUnit(unitIndex());
		usrData::getInstance().setDataCurCalGroup(calGroupUnite());
		usrData::getInstance().setDataCalGroup(cal);
		usrData::getInstance().promptMesInsert(funcs::mesGen("", projManagePrompt["newSuccess"]));
		return true;
	}
	usrData::getInstance().errMesInsert(funcs::mesGen("", projManagePrompt["newFail"]));
	return false;
}

bool projManage::newProjWithInit(string path, string projName, string tplProjPath, string tplCalGName)
{
	return true;
}


bool projManage::newCalgroup(string path, string name)
{
	calGroup cal;
	if (newCalGroupStruct(cal, path, name) && calGroupFolderGen(cal)) {
		projectInfo proj = usrData::getInstance().proj;
		proj.calGroups.push_back(cal.name);
		usrData::getInstance().setDataProj(proj);
		return true;
	}
	return false;
}

bool projManage::projIntegrityVerify(string path)
{
	if (path.empty()) {
		return false;
	}
	std::filesystem::path projPath(path);
	string projName = projPath.filename().string();
	filesystem::path filePath;
	vector<string> lostPath;
	projectInfo proj;
	bool state = true;

	if (!fileReading::projectInfoRead(proj, (projPath / (projName + fileSuffix["proj"])).string())) {
		return false;
	}
	filePath = projPath / proj.paramFlPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = projPath / proj.unitFlPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	for (auto& it : proj.calGroups) {
		state &= calGroupInterVerify((projPath / it).string());
	}
	if (!lostPath.empty()) {
		string obj = proManageObj["projCheck"] + " -- " + projName;
		usrData::getInstance().warnMesInsert(funcs::mesGen(obj, projManageErrPrompt["projNotInte"], lostPath));
		return false;
	}
	return state;
}

bool projManage::calGroupInterVerify(string path)
{
	if (path.empty()) {
		return false;
	}
	std::filesystem::path calGPath(path);
	string calGName = calGPath.filename().string();
	vector<string> lostPath;
	filesystem::path filePath;
	calGroup cal;
	
	if (!fileReading::calGroupRead(cal, (calGPath / (calGName + fileSuffix["calGroup"])).string())) {
		return false;
	}
	filePath = calGPath / cal.tubeDPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.magDPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.beamDPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.lossDPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.dispDPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.calSPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.mesPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	filePath = calGPath / cal.paramLPath;
	if (!filesystem::exists(filePath)) {
		lostPath.push_back(filePath.string());
	}
	if (!cal.resFilePath.empty()) {
		filePath = calGPath /cal.resFilePath;
		if (!filesystem::exists(filePath)) {
			lostPath.push_back(filePath.string());
		}
	}

	if (!lostPath.empty()) {
		string obj = proManageObj["calGCheck"] + " -- " + calGName;
		usrData::getInstance().warnMesInsert(funcs::mesGen(obj, projManageErrPrompt["calGNotInte"], lostPath));
		return false;
	}
	return true;
}

bool projManage::openProj(string path)
{
	projectInfo proj;
	parametersList pars;
	unitIndex unit;
	calGroupUnite calGu;
	filesystem::path projPath(path);
	filesystem::path filePath;
	bool state = true;

	if (!projIntegrityVerify(projPath.string())) {
		return false;
	}
	filePath = projPath / (projPath.filename().string()+ fileSuffix["proj"]);
	if (!fileReading::projectInfoRead(proj, filePath.string())) {
		return false;
	}
	filePath = projPath / proj.paramFlPath;
	if (!fileReading::parameterRead(pars, filePath.string())) {
		state = false;
	}
	filePath = projPath / proj.unitFlPath;
	if (!fileReading::unitRead(unit, filePath.string())) {
		state = false;
	}

	bool tempFlag = false;
	for (auto& it : proj.calGroups) {
		filesystem::path calGPath = projPath / it;
		if (fileReading::calGroupUniteRead(calGu, calGPath.string())) {
			tempFlag = true;
			break;
		}
	}
	state &= tempFlag;

	if (state) {
		ctlData::getInstance().setCurrentProj(proj.directory);
		ctlData::getInstance().setCurrentCalGroup(calGu.calG.name);
		usrData::getInstance().setDataProj(proj);
		usrData::getInstance().setDataPars(pars);
		usrData::getInstance().setDataUnit(unit);
		usrData::getInstance().setDataCurCalGroup(calGu);

		usrData::getInstance().resetDataCheckFlag();
		usrData::getInstance().promptMesInsert(funcs::mesGen("", projManagePrompt["openSuccess"]));
		return true;
	}
	usrData::getInstance().warnMesInsert(funcs::mesGen("", projManagePrompt["openFail"]));
	return false;
}


bool projManage::globalDataFolderCreate()
{
	// 获取当前工作路径
	std::filesystem::path currentWorkPath = std::filesystem::current_path();
	vector<string> errPath = {};

	// 尝试创建所需的目录
	for (const auto& dir : { globalFilePos["df"], globalFilePos["dfltProj"],globalFilePos["tplProj"],globalFilePos["disp"] }) {
		std::filesystem::path dirPath = currentWorkPath / dir;
		if (!std::filesystem::exists(dirPath) && !newEmptyFolder(dirPath)) {
			errPath.push_back(dirPath.string());
		}
	}
	//创建历史工程文件夹
	std::filesystem::path histProjPath = currentWorkPath / globalFilePos["hist"];
	if (!std::filesystem::exists(histProjPath) && !newEmptyFile(histProjPath.string())) {
		errPath.push_back(histProjPath.string());
	}
	if (!errPath.empty()) {
		usrData::getInstance().errMesInsert(funcs::mesGen(proManageObj["globalFolder"], projManageErrPrompt["globalFolderCrFail"], errPath));
		return false;
	}
	return true;
}

bool projManage::newEmptyFile(filesystem::path path)
{
	if (!filesystem::exists(path.parent_path()) && !newEmptyFolder(path.parent_path())) {
		return false;
	}
	ofstream os;
	os.open(path);

	if (!os.is_open()) {
		return false;
	}
	else {
		os.close();
		return true;
	}
}

bool projManage::newEmptyFolder(filesystem::path path)
{
	try
	{
		filesystem::create_directories(path);
		return true;
	}
	catch (const std::filesystem::filesystem_error&)
	{
		return false;
	}
}

bool projManage::saveProj(string path)
{
	usrData& data = usrData::getInstance();
	projectInfo project = data.proj;
	bool state = true;
	filesystem::path projPath(path);
	if (path.empty()) {
		projPath = ctlData::getInstance().tempVaris.currentProject;
	}
	if (!filesystem::exists(projPath)) {
		return false;
	}
	project.directory = projPath.string();

	if (!projFolderGen(project)) {
		return false;
	}
	project.lastSaveTime = currentDateTime(1);
	filesystem::path pathTemp = projPath / (project.name + fileSuffix["proj"]);
	if (!fileWriting::projectInfoWrite(project, pathTemp.string())){
		return false;
	}
	pathTemp = projPath / project.paramFlPath;
	if (!fileWriting::parameterWrite(data.pars,pathTemp.string())) {
		state = false;
	}
	pathTemp = projPath / project.unitFlPath;
	if (!fileWriting::unitWrite(data.unit, pathTemp.string())) {
		state = false;
	}
	state &= saveCalGroup();
	if (state) {
		usrData::getInstance().promptMesInsert(funcs::mesGen("", projManagePrompt["saveSuccess"]));
	}
	else {
		usrData::getInstance().warnMesInsert(funcs::mesGen("", projManagePrompt["saveFail"]));
	}
	return state;
}

bool projManage::saveCalGroup(string path)
{
	usrData& data = usrData::getInstance();
	calGroup calGr = data.curCalGroup.calG;
	set<string> resSeqNames;
	bool state = true;
	filesystem::path mainPath(path);
	if (path.empty()) {
		auto& tempData = ctlData::getInstance().tempVaris;
		mainPath = tempData.currentProject;
		mainPath = mainPath / calGr.name;
	}
	if (!filesystem::exists(mainPath)) {
		return false;
	}
	calGr.directory = (mainPath).string();

	for (auto& it : data.curCalGroup.res.BGDatas) {
		resSeqNames.insert(it.first);
	}

	if (!calGroupFolderGen(calGr,resSeqNames)) {
		return false;
	}
	state&= fileWriting::calGroupUniteWrite(data.curCalGroup, mainPath.string());
	filesystem::path resPath(mainPath/data.curCalGroup.calG.resFilePath);
	
	for (auto& it : data.curCalGroup.res.BGDatas) {
		string projPath = (resPath / it.first).string();
		saveProjectInResSeq(it.second, projPath);
	}
}

bool projManage::saveProjectInResSeq(results::BGDataOfRes data, string path)
{
	filesystem::path projPath(path);
	if (!filesystem::exists(projPath) || !filesystem::is_directory(projPath)) {
		return false;
	}
	auto& dataTemp = ctlData::getInstance().dftValue;
	string projName = dataTemp.defaultProjName + "_" + to_string(1);
	string calGroupName = dataTemp.defaultCalGroupName + "_" + to_string(1);
	projectInfo proj;
	calGroup cal;
	bool state = true;
	if (newProjInfo(proj, projPath.string(), projName) && projFolderGen(proj) && newCalGroupStruct(cal, proj.directory, calGroupName) && calGroupFolderGen(cal)) {
		projPath = projPath / projName;
		proj.lastSaveTime = currentDateTime(1);
		filesystem::path pathTemp = projPath / (proj.name + fileSuffix["proj"]);
		if (!fileWriting::projectInfoWrite(proj, pathTemp.string())) {
			return false;
		}
		pathTemp = projPath / proj.paramFlPath;
		if (!fileWriting::parameterWrite(data.pars, pathTemp.string())) {
			state = false;
		}
		calGroupUnite calGu;
		calGu.calG = cal;
		calGu.calS.calSet.insert_or_assign(data.calS.name, data.calS);
		calGu.disp = data.disp;
		calGu.intpar = data.intpar;
		calGu.loss = data.loss;
		calGu.parLink = data.parLink;
		state &= fileWriting::calGroupUniteWrite(calGu, cal.directory);
		return state;
	}
	return false;
}
