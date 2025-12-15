#include "ctlData.h"

void ctlData::setDataDftVal(const ctlData::defaultValue& data)
{
	std::lock_guard<std::mutex> lock(dftValMtx);
	dftValue = data;
}

void ctlData::setDataFlags(const ctlData::ctlFlags& data)
{
	std::lock_guard<std::mutex> lock(flagMtx);
	flags = data;
}

void ctlData::setDataTempVaris(const ctlData::temporaryVaris& data)
{
	std::lock_guard<std::mutex> lock(tempVarisMtx);
	tempVaris = data;
}

void ctlData::setCalculateQuitFlag(bool state)
{
	std::lock_guard<std::mutex> lock(calQuitMtx);
	flags.calculQuit = state;
}

void ctlData::setCurrentProj(std::string path)
{
	std::lock_guard<std::mutex> lock(curProjMtx);
	tempVaris.currentProject = path;
}

void ctlData::setCurrentCalGroup(std::string name)
{
	std::lock_guard<std::mutex> lock(curCalGroupMtx);
	tempVaris.currentCalGroup = name;
}
