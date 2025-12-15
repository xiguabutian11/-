#pragma once
#include"params.h"


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
bool GetStructData(StructParams& strpar, string filepath);

string WriteEigenParfile(StructParams& strpar, string workpath = "", string timeflag = "0");

bool GetEigenResult(DispersionParams& dispar, string path);

void WriteDataBW(DispersionParams& dis, string filepath);

void WriteData(DispersionParams& dis, string filepath);


//************************数据验证相关函数***************************************************************
// //对包含结构数据的strpar结构体数据进行校验，数据合理则返回真
bool EigenDataCheck(StructParams& strpar);

//对地址为strpath的ai.dat文件数据进行校验，数据合理则返回真
bool EigenFileCheck(string filepath);

//************************调用tau进行本征计算相关函数***************************************************************
bool EigenCalculate(string filepath, string workpath = "");

//对地址为strpath的ai.dat文件调用tau进行本征计算
bool Eigen_file(DispersionParams& dispar, string filepath, string workpath = "");

//对包含结构数据的strpar结构体调用tau进行本征计算
bool Eigen_data(DispersionParams& dispar, StructParams& strpar, string workpath = "");


//************************返波处理相关函数***************************************************************
bool GetDataBW(DispersionParams& dis);



