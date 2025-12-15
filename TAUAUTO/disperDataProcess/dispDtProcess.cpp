#include "dispDtProcess.h"

map<double, map<double, dispDataOfFreq, DoubleComparator>, DoubleComparator> dispDtPro::dispDatas;
vector<double> dispDtPro::centerFreqVpInfo;
unordered_map<string, unordered_map<string, boost::math::interpolators::barycentric_rational<double>>> dispDtPro::dispFuncs;
std::shared_mutex dispDtPro::dispDataMtx;
set<double> dispDtPro::positions;


map<double, map<double, dispDataOfFreq, DoubleComparator>, DoubleComparator> dispDtPro::getDispData()
{
	std::shared_lock<std::shared_mutex> lock(dispDataMtx);
	return dispDatas;
}

void dispDtPro::dispFuncsGeneration()
{
	const usrData& data = usrData::getInstance();
	dispFuncs.clear();
	for (auto& disp : data.curCalGroup.disp.dispInfo) {
		string name = disp.first;
		unordered_map<string, boost::math::interpolators::barycentric_rational<double>> funcsTemp;

		vector<vector<double>> kcArray = mapToVector(disp.second.kc);
		boost::math::interpolators::barycentric_rational<double> kcFunc(kcArray[0].begin(),kcArray[0].end(),kcArray[1].begin(),kcArray[0].size()-1);
		funcsTemp.insert_or_assign(dispFuncType["kc"], kcFunc);

		vector<vector<double>> vpArray = mapToVector(disp.second.vp);
		boost::math::interpolators::barycentric_rational<double> vpFunc(vpArray[0].begin(), vpArray[0].end(), vpArray[1].begin(),vpArray[0].size()-1);
		funcsTemp.insert_or_assign(dispFuncType["vp"], vpFunc);

		vector<vector<double>> alArray = mapToVector(disp.second.al);
		boost::math::interpolators::barycentric_rational<double> alFunc(alArray[0].begin(),alArray[0].end(),alArray[1].begin(),alArray[0].size()-1);
		funcsTemp.insert_or_assign(dispFuncType["al"], alFunc);

		dispFuncs[name]=funcsTemp;
	}
}

void dispDtPro::dispDatasGeneration(const vector<double>& freqs)
{
	const usrData& data = usrData::getInstance();
	set<double> freqsArray(freqs.begin(), freqs.end());
	double centerFreq = data.curCalGroup.calS.centerFreq.freq;
	if (centerFreq == 0) {
		double sum = 0;
		for (auto& freq : freqsArray) {
			sum += freq;
		}
		centerFreq = sum / static_cast<int>(freqsArray.size());
	}

	dispDatas.clear();
	centerFreqVpInfo.clear();
	positions.clear();

	for (auto& freq : freqsArray) {
		map<double, dispDataOfFreq, DoubleComparator> mapTemp;
		for (auto& pos : data.curCalGroup.disp.samplePoints) {
			dispDataOfFreq dispTemp;

			double position = pos.first;
			string name = pos.second.dispKey;
			const auto& vpFunc = dispFuncs[name].find(dispFuncType["vp"]);
			dispTemp.vp = vpFunc->second(freq);
			const auto& kcFunc = dispFuncs[name].find(dispFuncType["kc"]);
			dispTemp.kc = kcFunc->second(freq);
			const auto& alFunc = dispFuncs[name].find(dispFuncType["al"]);
			dispTemp.al = alFunc->second(freq);

			mapTemp.insert_or_assign(position, dispTemp);
		}
		dispDatas.insert_or_assign(freq, mapTemp);
	}
	vector<double> samPoints;
	for (auto& pos : data.curCalGroup.disp.samplePoints) {
		double vp;
		double position = pos.first;
		string name = pos.second.dispKey;
		const auto& vpFunc = dispFuncs[name].find(dispFuncType["vp"]);
		vp = vpFunc->second(centerFreq);
		vp = vp <= 0 ? 0.1 : vp;

		centerFreqVpInfo.push_back(vp);
		samPoints.push_back(position);
	}
	setDisperPsoToUsrData(samPoints);
	dispDatasSupplement(freqs,samPoints);
	dispDataModify();
}

void dispDtPro::setDisperPsoToUsrData(const vector<double>& samPoints)
{
	int	maxNum = ctlData::getInstance().dftValue.maxDispSegs;
	InteractionParams intpar = usrData::getInstance().curCalGroup.intpar;
	intpar.data_swsbeg.clear();

	vector<int> segNums;
	vector<double> diffes;
	double sumDifference = 0;
	for (int i = 1; i < samPoints.size(); ++i) {
		double diffe = std::abs(centerFreqVpInfo[i] - centerFreqVpInfo[(i - 1)]);
		diffes.push_back(diffe);
		sumDifference += diffe;
	}
	for (auto& it : diffes) {
		segNums.push_back(std::round(it * maxNum / sumDifference));
	}
	for (int i = 1; i < samPoints.size(); ++i) {
		vector<double> poses = funcsTwo::fixedNumEvenSeg(samPoints[i-1], samPoints[i], segNums[i-1]);
		for (auto& it : poses) {
			positions.insert(it);
		}
	}
	for (auto& it : positions) {
		intpar.data_swsbeg.push_back(it);
	}
	usrData::getInstance().setDataIntpar(intpar);
}


void dispDtPro::dispDatasSupplement(const vector<double>& freqs, const vector<double>& samPoints)
{
	
	set<double> freqsArray(freqs.begin(), freqs.end());
	for (auto& freq : freqs) {
		posOrientedInterpo(freq, samPoints);
	}
}

void dispDtPro::dispDataModify()
{
	auto& data = ctlData::getInstance().dftValue;
	for (auto& freq : dispDatas) {
		for (auto& disp : freq.second) {
			disp.second.kc = disp.second.kc > 0 ? disp.second.kc : data.defaultKc;
			disp.second.vp = disp.second.vp > 0 ? disp.second.vp : data.defaultVp;
			disp.second.al = disp.second.al > 0 ? disp.second.al : data.defaultAl;
		}
	}
}


inline void dispDtPro::posOrientedInterpo(const double& freq, const vector<double>& samPoints)
{
	const dispDataInfo& data = usrData::getInstance().curCalGroup.disp;
	vector<double> kc;
	vector<double> vp;
	vector<double> al;
	for (auto& it : dispDatas[freq]) {
		kc.push_back(it.second.kc);
		vp.push_back(it.second.vp);
		al.push_back(it.second.al);
	}

	boost::math::interpolators::barycentric_rational<double> kcInterpo(samPoints.begin(), samPoints.end(), kc.begin(),1);
	boost::math::interpolators::barycentric_rational<double> vpInterpo(samPoints.begin(), samPoints.end(), vp.begin(), 1);
	boost::math::interpolators::barycentric_rational<double> alInterpo(samPoints.begin(), samPoints.end(), al.begin(), 1);
	for (auto& point : positions) {
		if (data.samplePoints.count(point)) {
			continue;
		}
		dispDataOfFreq dispTemp;
		dispTemp.kc = kcInterpo(point);
		dispTemp.vp = vpInterpo(point);
		dispTemp.al = alInterpo(point);

		dispDatas[freq].insert_or_assign(point, dispTemp);
	}
}

inline vector<vector<double>> dispDtPro::mapToVector(const map<double, double, DoubleComparator>& maps)
{
	vector<double> freq;
	vector<double> obj;
	for (auto& it : maps) {
		freq.push_back(it.first);
		obj.push_back(it.second);
	}
	return { freq,obj };
}
