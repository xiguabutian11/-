#pragma once
#define exprtk_disable_caseinsensitivity

#include "constData\structDefine.h"
#include "thirdPtyLibs\ExprTk\exprtk.hpp"
#include "thirdPtyLibs\boost\math\interpolators\barycentric_rational.hpp"


//exprTk相关数据
typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double> expression_t;
typedef exprtk::parser<double> parser_t;

class funcs {
public:
	//报错信息提示函数，当与GUI结合时，需要重新编写，以适应GUI
	static void msgShow(const char* msg, const char* title); 

	//判断字符串是否为数值，是返回true，否则返回false
	static bool isNum(const string& s);

	//去除字符串前后空格
	static string trim(const std::string& str);

	//表达式求解函数，expr为字符型表达式,variants为变量集合，其中键为变量名，值为变量值
	static double expressionSolve(const string& expr, map<string, double> variants = {});

	//字符串解析函数，str为数值则返回，否则调用expressionSolve求解，求解正常则返回结果，否则返回NaN
	static double stringParse(const string& str, map<string, double> variants ={});

	//表达式依赖项获取函数
	static bool dependentVaris(const string& expr, set<string>& dependencies, map<string,double>variants= {});

	//字符串标识合法性检查（是否仅有字母、数字或者下划线组成，且不已数字开头）
	static bool isValidIdentifier(const std::string& str);

	//数值分配函数，将数值val分配为一个或者多个数值之和，这些数值不超过max且相差最多为1
	static vector<int> splitNum(int val, int max = 6);

	//路径存在检查函数
	static bool ifPathExist(vector<string>& paths);

	//int布尔化
	static int boolConvert(const int& num);

	//数组递增性判断函数
	static bool dataIncreasing(const vector<double>& arr);

	//范围重叠检测函数，true表示不重叠
	static bool isOverlapping(pair<double, double> x, const vector<pair<double, double>>& y);

	//范围重叠检测函数的重载，true表示不重叠
	static bool isOverlapping(double x, const vector<pair<double, double>>& y);

	//错误提示信息生成函数
	static string mesGen(string obj, string prompt, vector<string> items = {});

	//在vec中查找第一个obj的下标，没有则返回-1,同时将未查到的值放到err中，主要用于txt文件的关键字索引值查找
	template <typename T>
	static int indexSearch(vector<T>& vec, T& obj, vector<T>& err);

	//在vec中查找maps每一个键对的值的下标，没有则为-1，同时将未查到的值放到err中，主要用于txt文件的关键字索引值查找
	template <typename T>
	static map<T,unsigned int> indiceSearch(vector<T>& vec, map<int, T>& maps, vector<T>& err); 
};



template<typename T>
inline int funcs::indexSearch(vector<T>& vec, T& obj,vector<T>& err)
{
	typename vector<T>::iterator ite = std::find(vec.begin(), vec.end(), obj);//返回的是一个迭代器指针
	if (ite == vec.end())
	{
		err.push_back(obj);
		return -1;
	}
	else {
		return std::distance(vec.begin(), ite);
	}
}


template<typename T>
inline map<T, unsigned int> funcs::indiceSearch(vector<T>& vec, map<int, T>& maps, vector<T>& err)
{
	map<T, unsigned int> indice;
	for (auto& it : maps) {
		indice.insert_or_assign(it.second,indexSearch<T>(vec, it.second, err));
	}
	return indice;
}


