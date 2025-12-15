#include "funcs.h"

void funcs::msgShow(const char* msg, const char* title)
{
	ctlData& ctl = ctlData::getInstance();
	if (ctl.flags.msgShowState == 0) {
		return;
	}
	cout << title << endl;
	cout << msg << endl;
}

bool funcs::isNum(const string& s)
{
	char* end = 0;
	double val = strtod(s.c_str(), &end);
	return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}

string funcs::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t");
	if (start == std::string::npos) return ""; // 全是空白字符
	size_t end = str.find_last_not_of(" \t");
	return str.substr(start, end - start + 1);
}

double funcs::expressionSolve(const string& expr, map<string, double> variants)
{
	expression_t expression;
	symbol_table_t symbol_table;
	parser_t parser;

    for (auto& it : variants) {
        symbol_table.add_variable(it.first, it.second);
    }
    symbol_table.add_constants();
    expression.register_symbol_table(symbol_table);

    parser.compile(expr, expression);

    return expression.value();
}

double funcs::stringParse(const string& str, map<string, double> variants)
{
	expression_t expression;
	symbol_table_t symbol_table;
	parser_t parser;

	for (auto& it : variants) {
		symbol_table.add_variable(it.first, it.second);
	}
	symbol_table.add_constants();
	expression.register_symbol_table(symbol_table);
	if (parser.compile(str, expression)) {
		vector<string> list;
		exprtk::collect_variables(str, list);
	}
	return expression.value();

}

bool funcs::dependentVaris(const string& expr, set<string>& dependencies, map<string, double> variants)
{
	expression_t expression;
	symbol_table_t symbol_table;
	parser_t parser;

	for (auto& it : variants) {
		symbol_table.add_variable(it.first, it.second);
	}
	symbol_table.add_constants();
	expression.register_symbol_table(symbol_table);
	parser.compile(expr, expression);
	vector<string> list;
	exprtk::collect_variables(expr, list);
	for (auto& it : list) {
		dependencies.insert(it);
	}
	if (parser.compile(expr, expression)) {
		return true;
	}
	else {
		return false;
	}
}



bool funcs::isValidIdentifier(const std::string& str)
{
	if (str.empty()) {
		return false;
	}

	// 检查第一个字符是否是字母或下划线
	if (!(std::isalpha(str[0]) || str[0] == '_')) {
		return false;
	}

	// 检查剩余的字符是否是字母、数字或下划线
	for (size_t i = 1; i < str.length(); ++i) {
		if (!(std::isalnum(str[i]) || str[i] == '_')) {
			return false;
		}
	}

	return true;
}

vector<int> funcs::splitNum(int val, int max)
{
	if (val <= 0 || max <= 0) {
		return {};
	}
	if (val <= max) {
		return { val };
	}

	int n = val / max;
	int remainder = val % max;

	vector<int> res(n, max);
	if (remainder != 0) {
		res.push_back(max);
		int len = static_cast<int>(res.size());
		remainder = max - remainder;

		int cutNum = remainder / len;
		int rest = remainder % len;
		for (auto& it : res) {
			it -= cutNum;
		}
		for (int i = 0; i < rest; ++i) {
			res[i] -= 1;
		}
	}
	return res;

}

bool funcs::ifPathExist(vector<string>& paths)
{
	vector<string> text = {};
	for (unsigned int i = 0; i < paths.size(); i++) {
		if (_access(paths[i].c_str(), 06) == -1) {
			text.push_back(paths[i] + "\n");
		}
	}
	paths = text;
	return paths.size()==0;
}

int funcs::boolConvert(const int& num)
{
	if (num != 0) {
		return 1;
	}
	else {
		return 0;
	}
}

bool funcs::dataIncreasing(const vector<double>& arr)
{
	if (arr.size() == 0) {
		return true;
	}
	for (int i = 0; i < arr.size()-1; ++i) {
		if (arr[i] > arr[i + 1]) {
			return false;
		}
	}
	return true;
}

bool funcs::isOverlapping(pair<double, double> x, const vector<pair<double, double>>& y)
{
	auto isOverlapping = [](pair<double, double> seg1, pair<double, double> seg2) {
		// 确保线段的起点小于等于终点
		if (seg1.first > seg1.second) {
			swap(seg1.first, seg1.second);
		}
		if (seg2.first > seg2.second) {
			swap(seg2.first, seg2.second);
		}

		// 检查是否重叠
		return !(seg1.second <= seg2.first || seg2.second <= seg1.first);
		};

	// 遍历 y 中的所有线段，检查是否与 x 重叠
	for (const auto& segment : y) {
		if (isOverlapping(x, segment)) {
			return false; // 存在重叠
		}
	}
	return true; // 没有重叠
}

bool funcs::isOverlapping(double x, const vector<pair<double, double>>& y)
{
	bool state = true;
	for (auto& it : y) {
		if (it.first > it.second) {
			continue;
		}
		if (x > it.first && x < it.second) {
			state = false;
		}
	}
	return state;
}

string funcs::mesGen(string obj, string prompt, vector<string> items)
{
	string meg;
	meg = obj + "  #" + prompt;
	if (!items.empty()) {
		meg += "  @";
		for (unsigned int x = 0; x < items.size(); ++x) {
			meg += items[x];
			if (x != items.size() - 1) {
				meg += ",";
			}
		}
	}
	return meg;
}
