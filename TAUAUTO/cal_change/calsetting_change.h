#pragma once
#include "filesReadAndWrite/fileWriting.h"
#include "filesReadAndWrite/fileReading.h"
#include "tauLib/headers/TauInteraction.h"
#include "tauLib/headers/constpar.h"
#include "usrDataInfo/usrDataSet.h"
#include "usrDataInfo/usrDataCheck.h"
#include "paramScan/paramScan.h"
#include "calculation/calculation.h"
#include "disperDataProcess/dispDtProcess.h"
#include "lossDataProcess/lossDataProcess.h"
#include "resultProcess/resultPro.h"
#include "projManage/projManage.h"
#include "cal_change/calsetting_change.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include "SESAN1/DATA/data_start.h"

class datachange {
public:
	static bool changecalsetting(const std::string& targetParam, double numble);

	static bool tubeDataChange(const std::string& fieldName, double newValue);

	static bool magDataChange(const std::string& fieldName, double newValue);

	static bool mag(double A, double P);//更改磁场，A为幅值，P为周期

	static bool beamDataChange(const std::string& fieldName, const Json::Value& newValue);

	static bool r(double r1, double r2);//r1为电子束外径，r2为通道半径

	static bool lossDataChange(int mode, double firstBegin, double firstEnd, double secondBegin, double secondEnd);
};