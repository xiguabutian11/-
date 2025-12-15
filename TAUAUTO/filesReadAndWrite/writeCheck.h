#pragma once
#include "filesReadAndWrite\fileReading.h"
//#include "constData\structDefine.h"
//#include "commonFuncs\funcs.h"
#include "commonFuncs\formula.h"

class writeCheck {

public:
	//数据合法性及合理性检查函数
	template<typename T=double, typename ARG=int,	//T表示数据类型，T2表示存储合法性函数和合理性函数所用参数所在的结构体类型
		typename Enable = typename std::enable_if<//将模板参数类型约束为 int、double类型之一
		std::is_same<T, int>::value || 
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, double>::value>::type>
	static T dataCheck(const T& num, vector<string>& errNum, vector<string>& inaptNum, bool(*errCheck)(const T&, vector<string>&, const ARG&) = minusCheck, string(*inaptCheck)(const T&, const ARG& ) =nullptr, const ARG& args = 0) {
		
		if (errCheck(num,errNum,args)){
			if (inaptCheck != nullptr) {
				string textTemp = inaptCheck(num,args);
				if (textTemp != "") {
					inaptNum.push_back(textTemp);
				}
			}
			return num;
		}
		else {
			return 0;
		}
	}


public:
	/*合法性检查函数都有三个形参，const T& num(被检查值)、vector<string>& errNum(错误信息提示)
	和const ARG& args(检查时需要用到的其他数据，可考虑用结构体一次传多个数据)，
	注意，就算不需要args，也要加到函数形参里，这是模板函数所决定的。
	返回值统一为bool类型，true表示合法，false表示不合法*/

	//数值范围合法性检查函数
	static bool rangeReasonCheck(const int& num, vector<string>& errNum, const vector<int>& arg={0,INT_MAX});

	//数据递增判断函数
	static bool sizeRelpReasonCheck(const double& num, vector<string>& errNum,const vector<double>& Rs);


	//数值正负合法性检查函数（检查是否为负，为负则不合法）
	template <typename T=double>
	static bool minusCheck(const T& num, vector<string>& errNum, const int& arg=0) {
		if (num < 0) {
			errNum.push_back(to_string(num) + ": " + writeDataCheckErr["minus"]);
			return false;
		}
		else {
			return true;
		}
	}


	/*合理性检查函数有两个形参，const T& num(被检查值)，
	和const ARG& args(检查时需要用到的其他数据，可考虑用结构体一次传多个数据)，
	注意，就算不需要args，也要加到函数形参里，这是模板函数所决定的。
	返回值统一为string类型，表示不合理信息提示，返回值为""表示合理。*/

	//电压数值合理性检查函数
	static string voltageInaptCheck(const double& v, const int& arg=0);


};

