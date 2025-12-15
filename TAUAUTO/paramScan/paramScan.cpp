#include "paramScan.h"

// 静态成员变量的定义
calSetting::calSequence paramScan::seqTemp;
paramScanData paramScan::singleParScanData;
std::vector<std::string> paramScan::paramScanVariables;
std::vector<paramScanData> paramScan::parScanData;
map<string, signedArray<double, bool>> paramScan::qualifiedParScanVaris;
//std::shared_mutex paramScan::mtx;



void paramScan::megShow()
{
	for (auto& parScan : parScanData) {
		for (auto& freq : parScan.freqs) {
			cout << freq.freq << "  " << freq.pin << "  " << freq.pin1 << "  " << freq.pin2 << endl;
		}
		for (auto& vari : parScan.variants) {
			cout << vari.first << "  " << vari.second << endl;
		}
	}
}

bool paramScan::parScanFunction(const calSetting::calSequence& seq)
{
	seqTemp = seq;
	return parScanDataGeneration();
}

vector<paramScanData> paramScan::getParScanData()
{
	//std::shared_lock<std::shared_mutex> lock(mtx);  // 共享锁（多线程可同时读取）
	return parScanData;  
}

map<string, signedArray<double, bool>> paramScan::getQualifiedParScanVaris()
{
	return qualifiedParScanVaris;
}

bool paramScan::parScanDataGeneration()
{
	singleParScanData.freqs.clear();
	singleParScanData.variants.clear();
	paramScanVariables.clear();
	parScanData.clear();
	qualifiedParScanVaris.clear();

	if (!parScanVarisCheck()) {
		return false;		//此处返回false表示未指定计算的频点，此处可用改为调用相应程序自动生成频点
	}
	set<double> freqArray(seqTemp.seq[parScanFixedItem["frequency"]].array.begin(), seqTemp.seq[parScanFixedItem["frequency"]].array.end());
	for (auto& it : freqArray) {
		freqin freqTemp;
		freqTemp.freq = it;
		singleParScanData.freqs.push_back(freqTemp);
	}
	PinDistribution();
	return true;
}

bool paramScan::parScanVarisCheck()
{
	//paramScanVariables.clear();
	if (seqTemp.seq.count(parScanFixedItem["frequency"]) == 0) {
		return false;
	}
	set<string> linkedParam = parameter::scanableParameters();
	for (auto& it : seqTemp.seq) {
		if (!parScanFixedItem.count(it.first)) {
			if (parameter::parScanableCheck(it.first,linkedParam)) {
				paramScanVariables.push_back(it.first);
				qualifiedParScanVaris.insert_or_assign(it.first,it.second);
			}
			
		}
		else {
			qualifiedParScanVaris.insert_or_assign(it.first, it.second);
		}
	}
	return true;
}

void paramScan::PinDistribution()
{
	if (!seqTemp.seq.count(parScanFixedItem["pin"])) {
		Pin1Distribution();
		return;
	}
	if (seqTemp.seq[parScanFixedItem["pin"]].signNum == 0) {		//默认特征值为0，表示扫参
		set<double> setTemp(seqTemp.seq[parScanFixedItem["pin"]].array.begin(), seqTemp.seq[parScanFixedItem["pin"]].array.end()); //去掉重复值
		for (auto& it : setTemp) {
			for (auto& freq : singleParScanData.freqs) {
				freq.pin = it;
			}
			Pin1Distribution();
		}
	}
	else {															//特征值为1，表示一对一赋值，未被cover的则采用默认值
		int length = min(seqTemp.seq[parScanFixedItem["pin"]].array.size(), singleParScanData.freqs.size());
		for (int i = 0; i < length; ++i) {
			singleParScanData.freqs[i].pin = seqTemp.seq[parScanFixedItem["pin"]].array[i];
		}
		Pin1Distribution();
	}
}

void paramScan::Pin1Distribution()
{
	if (!seqTemp.seq.count(parScanFixedItem["pin_1"])) {
		Pin2Distribution();
		return;
	}
	if (seqTemp.seq[parScanFixedItem["pin_1"]].signNum == 0) {		//默认特征值为0，表示扫参
		set<double> setTemp(seqTemp.seq[parScanFixedItem["pin_1"]].array.begin(), seqTemp.seq[parScanFixedItem["pin_1"]].array.end()); //去掉重复值
		for (auto& it : setTemp) {
			for (auto& freq : singleParScanData.freqs) {
				freq.pin1 = it;
			}
			Pin2Distribution();
		}
	}
	else {			//特征值为1，表示一对一赋值，未被cover的则采用默认值
		int length = min(seqTemp.seq[parScanFixedItem["pin_1"]].array.size(), singleParScanData.freqs.size());
		for (int i = 0; i < length; ++i) {
			singleParScanData.freqs[i].pin1 = seqTemp.seq[parScanFixedItem["pin_1"]].array[i];
		}
		Pin2Distribution();
	}
}

void paramScan::Pin2Distribution()
{
	if (!seqTemp.seq.count(parScanFixedItem["pin_2"])) {
		paramDistribution(0);
		return;
	}
	if (seqTemp.seq[parScanFixedItem["pin_2"]].signNum == 0) {		//默认特征值为0，表示扫参
		set<double> setTemp(seqTemp.seq[parScanFixedItem["pin_2"]].array.begin(), seqTemp.seq[parScanFixedItem["pin_2"]].array.end()); //去掉重复值
		for (auto& it: setTemp) {
			for (auto& freq : singleParScanData.freqs) {
				freq.pin2 = it;
			}
			paramDistribution(0);
		}
	}
	else {			//特征值为1，表示一对一赋值，未被cover的则采用默认值
		int length = min(seqTemp.seq[parScanFixedItem["pin_2"]].array.size(), singleParScanData.freqs.size());
		for (int i = 0; i < length; ++i) {
			singleParScanData.freqs[i].pin2 = seqTemp.seq[parScanFixedItem["pin_2"]].array[i];
		}
		paramDistribution(0);
	}
}

void paramScan::paramDistribution(int index)
{
	if (index >= paramScanVariables.size()) {
		adaptiveSegmentation(singleParScanData);
		return;
	}
	set<double> setTemp(seqTemp.seq[paramScanVariables[index]].array.begin(), seqTemp.seq[paramScanVariables[index]].array.end());
	for (auto& it : setTemp) {
		singleParScanData.variants[paramScanVariables[index]] = it;
		paramDistribution(index + 1);
	}
}

void paramScan::adaptiveSegmentation(const paramScanData& data)
{
	int n = static_cast<int>(data.freqs.size());
	vector<int> res = funcs::splitNum(n);

	int fir = 0;
	for (auto& it : res) {
		paramScanData dataTemp;
		dataTemp.variants = data.variants;
		dataTemp.freqs.assign(data.freqs.begin() + fir, data.freqs.begin() + fir + it);

		parScanData.push_back(dataTemp);
		fir += it;
	}
}
