#pragma once
#include "usrDataInfo\usrData.h"
#include "commonFuncs\formula.h"
class errCheckFunc {
public:
    //范围检查
    template<typename T = double>
    static bool rangeCheck(T tar, string name, const vector<T>& range);

    static bool cutOffCheck(vector<drft> cutOffs, string name, const double& tubeLen);

    static bool lossCheck(lossAndCutOffDataInfo loss,string name, const double& tubeLen);

    static bool dispCheck(dispDataInfo disp, string name, const pair<double,vector<drft>>& args);
};


class inaptCheckFunc {
public:
    //电压合理性判断
    static void voltageInaptCheck(double& v, string name, const int& args);

    //static void lossInaptCheck(lossAndCutOffDataInfo& loss, string name, int arg);

    static void dispInaptCheck(dispDataInfo& disp, string name, const vector<drft>& cutOffs);
};



class usrDataCheck: public errCheckFunc,public inaptCheckFunc {
public:
	//接口函数，两个参数，一个是函数指针，一个是该函数指针所指函数的参数，函数指针所指函数的参数类型和参数个数要与args完全对应
	template<typename Func = bool(*)(), typename ... Arg>
    static bool dataCheck(Func checkFunc = nullptr, Arg... args);

    static bool allDataCheck();

	static bool tubeDataCheck();

    static bool magDataCheck();

    static bool beamDataCheck();

    static bool cutOffDataCheck();

    static bool lossDataCheck();

    static bool dispDataCheck();

    static bool paramDataCheck();






    static void setValReplace(const bool& flag);


private:
    template<typename T = double, typename Arg1 = int, typename Arg2 = int>
    static bool checkFunc(T& tar,string name,bool(*errCheck)(T, string, const Arg1&) = defalultErrCheck,
        Arg1 errArg = 0,void(*inaptCheck)(T&, string,const Arg2&) = defaultInaptCheck,Arg2 inaptArg = 0);

    // 默认的errCheck函数,判断是否为负数
    template <typename T = double, typename Arg1 = int>
    static bool defalultErrCheck(T tar, string name, const Arg1& args);

    // 默认的空inaptCheck函数
    template <typename T = double, typename Arg2 = int>
    static void defaultInaptCheck(T& tar, string name, const Arg2& args);

    //范围检查
	//static bool rangeCheck(int tar, string name, vector<int> range);

    friend class errCheckFunc;

    friend class inaptCheckFunc;
private:
	static bool inaptValReplace;
};




template<typename Func, typename ...Arg>
inline bool usrDataCheck::dataCheck(Func checkFunc, Arg ...args)
{
    bool state = true;
    if (checkFunc == nullptr) {
        state = allDataCheck();
    }
    else {
        state = checkFunc(args...);
    }

    inaptValReplace = false;
    return state;
}

template<typename T, typename Arg1, typename Arg2>
inline bool usrDataCheck::checkFunc(T& tar, string name, bool(*errCheck)(T, string, const Arg1&), Arg1 errArg, void(*inaptCheck)(T&, string, const Arg2&), Arg2 inaptArg)
{
    bool state = errCheck(tar, name, errArg);

    inaptCheck(tar, name, inaptArg);
    return state;
}

template<typename T, typename Arg1>
inline bool usrDataCheck::defalultErrCheck(T tar, string name, const Arg1& args)
{
    string obj = name + "( " + to_string(tar) + " )";
    if constexpr (std::is_arithmetic_v<T>) {  // 默认功能，判断 tar 是否为负数
        if (tar < 0) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["minusDetect"]));
            return false;
        }
    }
    else {
        usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["unableErrCheck"]));
        return false;
    }
    return true;
}

template<typename T, typename Arg2>
inline void usrDataCheck::defaultInaptCheck(T& tar, string name, const Arg2& args)
{
    return;
}

template<typename T>
inline bool errCheckFunc::rangeCheck(T tar, string name, const vector<T>& range)
{
    if (range.size() != 2 || range[1] <= range[0]) {
        return true;
    }
    if (tar< range[0] || tar > range[1]) {
        string errMsg = name + "( " + to_string(tar) + " )" + " #" + usrDataCheckPrompt["outOfRange"] + ": " + to_string(range[0]) + "--" + to_string(range[1]);
        usrData::getInstance().errMesInsert(errMsg);
        return false;
    }
    return true;
}
