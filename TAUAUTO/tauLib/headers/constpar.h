#pragma once
#include <string> 
#include <fstream> 
#include <Windows.h>
#include<direct.h>
#include <iostream> 
#include <iomanip> 
#include <sstream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include <time.h>
#include<thread>
#include<mutex>
#include<shared_mutex>
#include<algorithm>
#include<io.h>
#include<math.h>
#include<numeric>
#include<map>
#include<set>
#include<regex>
#include<unordered_set>
#include<filesystem>

using namespace std;
const double constpar_e_m = 1.758819e11;//电子的电荷质量比
const double constpar_pi = 3.1415926535;//圆周率pi
const double constpar_clight = 299792448;//光速

std::string dtos(double d);
void string2tchar(std::string& src, TCHAR* buf);
const std::string currentDateTime(int style = 0);
bool isnum(std::string s);
//bool isNum(std::string s);	// new
ifstream& seek_to_line(ifstream& in, int line);
double*** arry3(int i, int j, int k,double data=0);
int arrydelete(double*** ary,int x, int y );
double** arry2(int i, int j,  double data = 0);
int arrydelete(double** ary, int x);
double* arry1(int i, double data = 0);
int arrydelete(double* ary);