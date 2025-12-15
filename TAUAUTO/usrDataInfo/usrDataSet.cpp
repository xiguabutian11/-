#include "usrDataSet.h"

//void usrDataSet::errMesInsert(string meg, bool flag)
//{
//	signedString str;
//	str.flag = messageState::error;
//	str.str = meg;
//	str.time = currentDateTime();
//	usrData::getInstance().insertDataMessage(str);
//}
//
//void usrDataSet::warnMesInsert(string meg, bool flag)
//{
//	signedString str;
//	str.flag = messageState::warning;
//	str.str = meg;
//	str.time = currentDateTime();
//	usrData::getInstance().insertDataMessage(str);
//}
//
//void usrDataSet::promptMesInsert(string meg, bool flag)
//{
//	signedString str;
//	str.flag = messageState::prompt;
//	str.str = meg;
//	str.time = currentDateTime();
//	usrData::getInstance().insertDataMessage(str);
//}

bool usrDataSet::dataSecureFromFiles(string projPath, string calGroupName)
{
	string calGroupPath = "";
	if (!projPath.empty()) {
		if (calGroupName.empty()) {
			calGroupName = ctlData::getInstance().dftValue.defaultCalGroupName + "_" + to_string(1);
		}
		calGroupPath = projPath + "/" + calGroupName;
	}
	 bool state = projDataSecure(projPath) && calGroupDataSecure(calGroupPath);
	return state;
}

bool usrDataSet::projDataSecure(string path)
{
	projectInfo proj;
	parametersList pars;
	unitIndex unit;
	filesystem::path curProj(path);
	bool state = true;

	if (path.empty()) {
		curProj = ctlData::getInstance().tempVaris.currentProject;
	}
	string projName = curProj.filename().string() + fileSuffix["proj"];
	path = (curProj / projName).string();

	

	if (!fileReading::projectInfoRead(proj, path)) {
		return false;
	}
	if (!fileReading::parameterRead(pars, proj.directory + proj.paramFlPath)) {
		state = false;
	}
	if (!fileReading::unitRead(unit, proj.directory + proj.unitFlPath)) {
		state = false;
	}
	if (!state) {
		return false;
	}
	usrData::getInstance().setDataProj(proj);
	usrData::getInstance().setDataPars(pars);
	usrData::getInstance().setDataUnit(unit);

	return true;
}

bool usrDataSet::calGroupDataSecure(string path)
{
	calGroupUnite calGu;
	filesystem::path curProj(path);
	bool state = true;

	if (path.empty()) {
		ctlData& data = ctlData::getInstance();
		curProj=data.tempVaris.currentProject;
		curProj = curProj / data.tempVaris.currentCalGroup;
	}
	string calGName = curProj.filename().string() + fileSuffix["calGroup"];
	path = (curProj/calGName).string();

	
	if (!fileReading::calGroupRead(calGu.calG, path)) {
		return false;
	}
	if (!fileReading::tubeDataRead(calGu.intpar, calGu.calG.directory + calGu.calG.tubeDPath)) {
		state =  false;
	}
	if (!fileReading::magDataRead(calGu.intpar, calGu.calG.directory + calGu.calG.magDPath)) {
		state = false;
	}
	if (!fileReading::beamDataRead(calGu.intpar, calGu.calG.directory + calGu.calG.beamDPath)) {
		state = false;
	}
	if (!fileReading::lossDataRead(calGu.loss, calGu.calG.directory + calGu.calG.lossDPath)) {
		state = false;
	}
	if (!fileReading::dispDataRead(calGu.disp, calGu.calG.directory + calGu.calG.dispDPath)) {
		state = false;
	}
	if (!fileReading::calSetRead(calGu.calS, calGu.calG.directory + calGu.calG.calSPath)) {
		state = false;
	}
	if (!fileReading::paramLinkRead(calGu.parLink, calGu.calG.directory + calGu.calG.paramLPath)) {
		state = false;
	}
	if (!fileReading::resultsRead(calGu.res, calGu.calG.directory + calGu.calG.resFilePath)) {
		state = false;
	}
	//if (!fileReading::messageRead(calGu.message, calGu.calG.directory + calGu.calG.mesPath)) {
	//	state = false;
	//}
	if (!state) {
		return false;
	}
	usrData::getInstance().setDataCurCalGroup(calGu);
	usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["initial"], 1);
	return true;
}

bool usrDataSet::setIntpar(const InteractionParams& intpar)
{
	usrData::getInstance().setDataIntpar(intpar);
	return true;
}




