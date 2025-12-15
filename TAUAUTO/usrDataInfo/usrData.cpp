#include "usrDataInfo\usrData.h"

void usrData::errMesInsert(string meg, bool flag)
{
	signedString str;
	str.flag = messageState::error;
	str.str = meg;
	str.time = currentDateTime(1);
	this->insertDataMessage(str);
}

void usrData::warnMesInsert(string meg, bool flag)
{
	signedString str;
	str.flag = messageState::warning;
	str.str = meg;
	str.time = currentDateTime(1);
	this->insertDataMessage(str);
}

void usrData::promptMesInsert(string meg, bool flag)
{
	signedString str;
	str.flag = messageState::prompt;
	str.str = meg;
	str.time = currentDateTime(1);
	this->insertDataMessage(str);
}

bool usrData::dataCheckedOrNot()
{
	bool state = true;
	for (auto& bit : usrDataCheckFlags) {
		state &= (dataCheckFlag >> bit.second & 1);
	}
	return state;
}


void usrData::setDataProj(const projectInfo& pro)
{
	std::lock_guard<std::mutex> lock(proInfoMtx);
	proj = pro;
}

void usrData::setDataPars(const parametersList& params)
{
	std::lock_guard<std::mutex> lock(parameterMtx);
	pars = params;
}

void usrData::setDataUnit(const unitIndex& units)
{
	std::lock_guard<std::mutex> lock(unitMtx);
	unit = units;
}

void usrData::setDataCalGroup(const calGroup& calGrp)
{
	std::lock_guard<std::mutex> lock(calGroupMtx);
	curCalGroup.calG = calGrp;
}

void usrData::setDataCalSetting(const calSetting& calSet)
{
	std::lock_guard<std::mutex> lock(calSettingMtx);
	curCalGroup.calS = calSet;
}

void usrData::setDataParLink(const paramLinks& paramLink)
{
	std::lock_guard<std::mutex> lock(parLinksMtx);
	curCalGroup.parLink = paramLink;
}

void usrData::setDataMessage(const vector<signedString>& meg)
{
	std::lock_guard<std::mutex> lock(mesgMtx);
	curCalGroup.message = meg;
}

void usrData::setDataIntpar(const InteractionParams& intp)
{
	std::lock_guard<std::mutex> lock(intparMtx);
	curCalGroup.intpar = intp;
}

void usrData::setDataDispar(const dispDataInfo& dispar)
{
	std::lock_guard<std::mutex> lock(dispDtMtx);
	curCalGroup.disp = dispar;
}

void usrData::setDataRes(const results& result)
{
	std::lock_guard<std::mutex> lock(resMtx);
	curCalGroup.res = result;
}

void usrData::setDataLoss(const lossAndCutOffDataInfo& loss)
{
	std::lock_guard<std::mutex> lock(lossMtx);
	curCalGroup.loss = loss;
}

void usrData::setDataCheckFlag(unsigned int bit, bool flag)
{
	std::lock_guard<std::mutex> lock(dataCheckMtx);
	if (bit > 63) {
		return;
	}
	if (flag) {
		// 设置第 bit 位为 1
		dataCheckFlag |= (static_cast<unsigned long long>(1) << bit);
	}
	else {
		// 设置第 bit 位为 0
		dataCheckFlag &= ~(static_cast<unsigned long long>(1) << bit);
	}
	//if (bit != usrDataCheckFlags["initial"]) {
	//	dataCheckFlag &= ~(static_cast<unsigned long long>(1) << usrDataCheckFlags["initial"]);
	//}
}

void usrData::insertDataParLink(paramLinks::parLink parL)
{
	std::lock_guard<std::mutex> lock(parLinksInsertMtx);
	string name = to_string(parL.index1) + "-" + to_string(parL.index2) + "-" + to_string(parL.index3);
	if (curCalGroup.parLink.parLinks.count(parL.dataType) == 0) {
		curCalGroup.parLink.parLinks[parL.dataType] = {};
	}
	curCalGroup.parLink.parLinks[parL.dataType][name] = parL;
}

void usrData::insertDataRes(const results::res& result, const string& seqName)
{
	std::lock_guard<std::mutex> lock(resInsertMtx);
	curCalGroup.res.reses[seqName].push_back(result);
}

void usrData::insertDataResExtraction(const results::resExtractedResult& extr, const string& seqName, const string& extrName)
{
	std::lock_guard<std::mutex> lock(resExtInsertMtx);
	if (curCalGroup.res.extraction.count(seqName) == 0) {
		curCalGroup.res.extraction[seqName] = {};
	}
	curCalGroup.res.extraction[seqName][extrName] =  extr;
}

void usrData::insertDataResBGData(const results::BGDataOfRes& data, const string& seqName)
{
	std::lock_guard<std::mutex> lock(resBGDataInsertMtx);
	curCalGroup.res.BGDatas[seqName] = data;
}
	
void usrData::insertDataMessage(const signedString& str)
{
	std::lock_guard<std::mutex> lock(megInsertMtx);
	curCalGroup.message.push_back(str);
}

void usrData::setDataCurCalGroup(const calGroupUnite& calGU)
{
	setDataCalGroup(calGU.calG);
	setDataCalSetting(calGU.calS);
	setDataParLink(calGU.parLink);
	setDataMessage(calGU.message);
	setDataIntpar(calGU.intpar);
	setDataDispar(calGU.disp);
	setDataRes(calGU.res);
	setDataLoss(calGU.loss);
}

void usrData::resetDataCheckFlag()
{
	for (auto& bit : usrDataCheckFlags) {
		setDataCheckFlag(bit.second, false);
	}
}
