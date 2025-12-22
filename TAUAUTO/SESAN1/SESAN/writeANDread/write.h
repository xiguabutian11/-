#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "jisuan/Jisuan.h"
#include"judgeANDchange/change_units.h"
#include"DATA/data_start.h"

#include <sstream>
#include <iomanip>
using namespace std;

void changeinputvalue(const string& filename, const string& paramName, double newValue);

void modifyCoordinates(const string& filename, const vector<vector<double>>& newCoordinates);//改变夹持杆坐标

void renew_input(LXjiegou jiegou);