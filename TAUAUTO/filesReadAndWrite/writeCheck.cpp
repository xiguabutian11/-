#include "writeCheck.h"

bool writeCheck::rangeReasonCheck(const int& num,vector<string>& errNum, const vector<int>& arg)
{
	if (arg[0] > num || arg[1] < num) {
		errNum.push_back("harmonic number " + to_string(num) + writeDataCheckErr["outRange"] + "("+ to_string(arg[0])+" to " + to_string(arg[1])+ ")");
		return false;
	}
	return true;
}


bool writeCheck::sizeRelpReasonCheck(const double& num, vector<string>& errNum, const vector<double>& Rs)
{
	if (!minusCheck(num, errNum)) {
		return false;
	}
	if (Rs.size() == 0) {
		return true;
	}
	bool flag;

	flag = funcs::dataIncreasing(Rs);
	string errInfo ="";
	if (!flag) {
		errInfo = to_string(Rs[0]) + "..." + to_string(Rs.back());
	}
	if (num < Rs.back()) {
		flag = false;
		errInfo += "、" + to_string(num);
	}
	if (flag) {
		return true;
	}
	else {
		errNum.push_back(writeDataCheckErr["noIncrease"] + ":"+ errInfo);
		return false;
	} 
}


string writeCheck::voltageInaptCheck(const double& v, const int& arg) // 优化方案，求每一段的平均vp，然后求各段的vp的加权平均值作为推荐电压的vp
{
	dispDataInfo disps;
	if (fileReading::dispDataRead(disps, "")) {
		double maxV = 0;
		double minV = 0;
		double recommendV = 0;
		if (disps.dispInfo.size()!= 0) {
			vector<double> vpTemp;
			for (auto& it : disps.dispInfo.begin()->second.vp) {
				vpTemp.push_back(it.second);
			}
			double maxVp = *max_element(vpTemp.begin(), vpTemp.end());
			double minVp = *min_element(vpTemp.begin(), vpTemp.end());
			double avgVp = accumulate(vpTemp.begin(), vpTemp.end(),0.0)/disps.dispInfo.begin()->second.vp.size();
			
			maxV = formula::voltageFromVp(maxVp);
			minV = formula::voltageFromVp(minVp);
			recommendV = formula::voltageFromVp(avgVp);
		}
		string text = "";
		if (minV > v || maxV < v) {
			text = "voltage " + to_string(v) + " may not be in the optimal range" + "(" + to_string(minV) + " to " + to_string(maxV) + ")";
			text += ",recommend voltage is " + to_string(recommendV);
		}
		return text;
	}
	return "";
}

