#include "lossDataProcess.h"

//静态变量定义
vector<signedArray<double, int>> lossDtPro::positions = {};
vector<unsigned int> lossDtPro::defltDistriType = {};
vector<double> lossDtPro::defltdBOfEachSeg = {};
map<double, vector<vector<double>>, DoubleComparator> lossDtPro::dBValues = {};
vector<vector<double>> lossDtPro::defaultdBValues = {};

void lossDtPro::posGeneration()
{
	positions.clear();
	defltDistriType.clear();
	defltdBOfEachSeg.clear();
	dBValues.clear();
	defaultdBValues.clear();
	vector<lossAndCutOffDataInfo::lossSegment> lossSegs = usrData::getInstance().curCalGroup.loss.lossSegs;
	int maxSegs = ctlData::getInstance().dftValue.maxLossSegs;
	double lossSegLength = 0;
	double fixedLength = 0;
	for (auto& it : lossSegs) {
		lossSegLength += (it.end - it.begin);
	}
	fixedLength = std::ceil(lossSegLength / maxSegs);

	for (auto& losSeg : lossSegs) {
		signedArray<double, int> arr;
		arr.array = funcsTwo::fixedLenEvenSeg(losSeg.begin, losSeg.end, fixedLength);

		if (losSeg.lossType == -1 || losSeg.lossType == 0 || losSeg.lossType == 1) {
			arr.signNum = losSeg.lossType;
		}
		else {
			arr.signNum = 0;
		}
		if (!arr.array.empty()) {
			positions.push_back(arr);
			defltDistriType.push_back(losSeg.defaultDistriType);
			defltdBOfEachSeg.push_back(losSeg.defaultdB);
		}
	}
}

void lossDtPro::dBValuesGeneration()
{
	lossAndCutOffDataInfo loss = usrData::getInstance().curCalGroup.loss;

	defaultdBValues = dBValOfFreqGen({});
	for (auto& freq : loss.dBInfo) {
		double freqVal = freq.first;
		vector<vector<double>> res = dBValOfFreqGen(freq.second);
		dBValues.insert_or_assign(freqVal, res);
	}
}

vector<loss> lossDtPro::lossDataGeneration(const vector<double>& freqs)
{
	vector<loss> res;
	for (unsigned int i = 0; i < positions.size();++i) {
		signedArray<double, int> seg = positions[i];
		for (unsigned int j = 0; j < seg.array.size() - 1; ++j) {
			loss lossTemp;
			lossTemp.begin = seg.array[j];
			lossTemp.end = seg.array[static_cast<std::vector<double, std::allocator<double>>::size_type>(j) + 1];
			lossTemp.type = seg.signNum;
			res.push_back(lossTemp);
		}
	}
	for (auto& freq : freqs) {
		vector<vector<double>> dbs = defaultdBValues;
		unsigned int length = 0;

		if (dBValues.count(freq)) {
			dbs = dBValues.at(freq);
		}
		for (auto& it : dbs) {
			length += it.size();
		}
		if (length != res.size()) {		//数据对不上，说明出现错误，以后考虑添加错误信息提示
			return vector<loss>();
		}
		int i = 0;
		for (auto& it : dbs) {
			for (auto& obj : it) {
				double val = obj*1000 / (res[i].end - res[i].begin);
				res[i].dataloss.push_back(val);
				++i;
			}
		}
	}
	return res;
}


vector<vector<double>> lossDtPro::dBValOfFreqGen(const vector<lossAndCutOffDataInfo::dBofFreqs>& data)
{
	double defaultdB = ctlData::getInstance().dftValue.defltfreqbB;
	int maxNum = ctlData::getInstance().dftValue.maxLossSegs;
	unsigned int num = positions.size();
	unsigned int dBNum = data.size();
	vector<vector<double>> res;

	for (unsigned int i = 0; i < num; ++i) {
		double val = defltdBOfEachSeg[i]>0 ? defltdBOfEachSeg[i]: defaultdB;
		unsigned int type = defltDistriType[i];
		unsigned int distriNum = positions[i].array.size() - 1;
		vector<double> arrTemp;
		if (maxNum <= 0) {
			break;
		}
		
		if (maxNum <= distriNum) {
			distriNum = maxNum;
			maxNum = 0;
		}
		else {
			maxNum -= distriNum;
		}
		
		if (i < dBNum) {
			if (data[i].dB > 0) {
				val = data[i].dB;
			}
			type = data[i].distriType;
		}
		switch (type)
		{
		case dBDistriType::increLinear:
			arrTemp = funcsTwo::linearDistri(val, distriNum);
			break;
		case dBDistriType::decreLinear:
			arrTemp = funcsTwo::linearDistri(val, distriNum, false);
			break;
		default:
			arrTemp = funcsTwo::linearDistri(val, distriNum);
			break;
		}
		res.push_back(arrTemp);
	}
	return res;
}
