#pragma once
#include "params.h"
#include"DATA/data_start.h"
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
#include <cmath>

using namespace std;
/*
* GetStructData();从ai.dat文件中提取结构数据
* EigenDataCheck():检查结构体数据是否合理
* EigenFileCheck();检ai.dat文件数据是否合理
* WriteEigenParfile();依据结构体数据，写ai.dat文件
* EigenCalculate();调用子程序进行本征计算
* Eigen_file();对ai.dat文件进行本征计算
* Eigen_data();对包含结构数据的结构体进行本征计算
* GetEigenResult();从本征计算结果文件中提取色散数据
* GetDataBW();从tau提取的色散数据中解析返波色散数据
* WriteDataBW();向指定地址写返波色散数据文件
* WriteData();向指定地址写色散数据文件
*/

//************************数据提取与导出相关函数***************************************************************
bool GetStructData1(StructParams1& strpar, string filepath);

string WriteEigenParfile1(StructParams1& strpar, string workpath = "", string timeflag = "0");

bool GetEigenResult1(DispersionParams1& dispar, string path);

void WriteDataBW1(DispersionParams1& dis, string filepath);

void WriteData1(DispersionParams1& dis, string filepath);


//************************数据验证相关函数***************************************************************
// //对包含结构数据的strpar结构体数据进行校验，数据合理则返回真
bool EigenDataCheck1(StructParams1& strpar);

//对地址为strpath的ai.dat文件数据进行校验，数据合理则返回真
bool EigenFileCheck1(string filepath);

//************************调用tau进行本征计算相关函数***************************************************************
bool EigenCalculate1(string filepath, string workpath = "");

//对地址为strpath的ai.dat文件调用tau进行本征计算
bool Eigen_file1(DispersionParams1& dispar, string filepath, string workpath = "");

//对包含结构数据的strpar结构体调用tau进行本征计算
bool Eigen_data1(DispersionParams1& dispar, StructParams1& strpar, string workpath = "");

//************************返波处理相关函数***************************************************************
bool GetDataBW1(DispersionParams1& dis);

ifstream& seek_to_line1(ifstream& in, int line);

bool directoryExists(const string& path);

void createDirectories(const string& path);

#pragma once
