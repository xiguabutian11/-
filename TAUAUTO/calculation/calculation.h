#pragma once
#include "paramScan\paramScan.h"
//#include "filesReadAndWrite\fileWriting.h"
#include "thirdPtyLibs\BSThreadPool\BS_thread_pool.hpp"
#include "disperDataProcess\dispDtProcess.h"
#include"lossDataProcess\lossDataProcess.h"
#include "tauLib\headers\TauInteraction.h"
#include "usrDataInfo\usrDataCheck.h"

class calculation : public paramScan, public dispDtPro {
public:
	static void seqCalculate();

	static void soloCalculate();

	static void waitForAllTasks();

	static void calculateQuit();

	static bool threadPoolBusy();

private:
	static bool preCalculate();

	static void multiTaskGeneration(calSetting::calSequence seq, vector<dataToCalculate>& datas, const map<string, double>& varis);

	static void backgroundDataOfResGen(const calSetting::calSequence& seq, results::BGDataOfRes& BGData,bool singlePoint = false);

	static InteractionParams parScanDataToIntpar(paramScanData& data, map<string, double> varis);

	//调用该函数之前，需要调用dispDtPro类的dispDataFunction，对所有频点求色散数据
	static vector<DispersionParams> parScanDataToDispar(paramScanData& data);

	static void taskProcessFunc(const dataToCalculate& data, const string& exePath);

	static void run(const vector<dataToCalculate>& datas);

	static void resProcess(const InteractionRes& intres, const dataToCalculate& data, bool calSucc, bool calQuit = false);

	static void resNameSet();

private:
	static BS::thread_pool pool;	//线程池
	static unsigned int taskSum;	//当前执行的总任务数量
	static std::atomic<int> dealedTaskNum;		//已经执行完的任务数量
	static std::atomic<int> failedTaskNum;      //错误的任务数量
	static set<string> seqNames;
	static std::mutex mtx;
	static int soloCalFakeFreqAdd;
};
