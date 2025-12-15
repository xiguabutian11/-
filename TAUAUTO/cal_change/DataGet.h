#pragma once
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
#include "filesReadAndWrite/fileReading.h"
#include "filesReadAndWrite/fileWriting.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>

class dataget {
public:
	static double dispDataGet(int line);	//1--maxVpVal  2--minKcVal 3-- maxKcVal

	static double tubeDataGet(const std::string& fieldName);
};