#pragma once
#include"filesReadAndWrite\fileReading.h"
#include"usrDataInfo\usrData.h"


class usrDataSet{		//用户数据更新填充的类，通过统一接口函数进行数据更新和填充
public:
	//接口函数，两个参数，一个是函数指针，一个是该函数指针所指函数的参数，函数指针所指函数的参数类型和参数个数要与args完全对应
	template<typename Func=bool(*)(), typename ... Arg>
	static bool dataSet(Func pdFunc=nullptr, Arg... args) {
		
		//return true;
		if (pdFunc == nullptr) {
			return dataSecureFromFiles("","");
		}
		else {
			return pdFunc(args...);
		}
	}
public:
	
	static bool dataSecureFromFiles(string projPath = "",string calGroupName = "");

	static bool projDataSecure(string path = "");		//意义需要再进一步讨论，与工程管理类--openProj功能重复

	static bool calGroupDataSecure(string path = "");	//意义需要再进一步讨论，与工程管理类--openProj功能重复

	static bool setIntpar(const InteractionParams& intpar);
};
