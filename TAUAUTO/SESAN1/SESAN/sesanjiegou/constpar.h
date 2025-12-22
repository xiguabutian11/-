#pragma once
#include <cmath>
#include <string> 
#include <fstream> 
#include <Windows.h>
#include <direct.h>
#include <iostream> 
#include <iomanip> 
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <thread>
#include <mutex>
#include <sys/stat.h>  // stat()
#include <errno.h>  // perror()

using namespace std;
const double constpar_e_m1 = 1.7588e11;//电子的电荷质量比
const double constpar_pi1 = 3.1415926535;//圆周率pi
const double constpar_clight1 = 299792448;//光速

std::string dtos1(double d);
void string2tchar1(std::string& src, TCHAR* buf);
const std::string currentDateTime1();
bool isnum1(std::string s);
static ifstream& seek_to_line1(ifstream& in, int line);
double*** arry31(int i, int j, int k, double data = 0);
int arrydelete1(double*** ary, int x, int y);
double** arry21(int i, int j, double data = 0);
int arrydelete1(double** ary, int x);
double* arry11(int i, double data = 0);
int arrydelete1(double* ary);