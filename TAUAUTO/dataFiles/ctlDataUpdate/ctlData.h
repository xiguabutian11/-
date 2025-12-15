#pragma once
#include <mutex>
#include <set>
//#include <string>

class ctlData {				//存储用户数据的类，单例模式，全局有效
public:
	static ctlData& getInstance()
	{
		static ctlData instance;
		return instance;
	}

	//用于全局变量的默认值
	struct defaultValue {
		double defaultPin = 0.3;	//输入频率的默认pin
		double defaultPin_1 = 0;	//输入频率的默认pin1
		double defaultPin_2 = 0;	//输入频率的默认pin2

		int maxDispSegs = 45;		//色散数据分段的最大个数，不建议超过50
		int maxLossSegs = 45;		//集中衰减分段的最大个数，不可超过50


		double defaultKc = 10;		//插值色散数据中kc为非正数时的默认值
		double defaultVp = 0.2;		//插值色散数据中vp为非正数时的默认值
		double defaultAl = 10;		//插值色散数据中al为非正数时的默认值

		std::string exeFolderPath = "";	//默认exe文件夹路径
		std::string defaultProjName = "project";			//默认工程命名
		std::string defaultCalGroupName = "interaction";	//默认计算组命名
		std::string defaultUnitResName = "res";				//默认单元结果命名
		std::string defaultSoloCalName = "soloCal";			//默认单独计算结果命名

		unsigned int maxThread = 0;	//最大可同时执行的线程数量
		unsigned int timeOut = 120000;	//多线程计算时，单个线程的超时时间,单位为ms，理论上不应该低于70秒即70000；
		unsigned int restartTime = 3;	//多线程计算时，单个线程计算失败后，重启计算的次数

		std::set<std::string> parScanType = { "tubeData","magData","beamData" };    //可进行链接的文件限制
		unsigned int maxArgsOfRes = 2;	//结果查询过程中可设置为自变量的参数的最大个数
			
		double defltfreqbB = 31;		//在设置集中衰减时，频点的默认衰减值

		double doubleEpsilon = 1e-9;	//控制double类型数据精度的项

		unsigned int skipHeaderOfTXT = 3;   //读取txt类型色散文件时，默认跳过的行数

		unsigned int skipHeaderOfCSV = 0;   //读取csv类型色散文件时，默认跳过的行数

	} dftValue;

	struct ctlFlags {
		int msgShowState = 0;		//该变量的值会决定msgShow函数的实现方式（暂时还未实现），全局有效

		bool calculPause = false;	//运算暂停标志位,true表示暂停

		bool calculQuit = false;	//运算终止标志位，true表示运算终止，false表示跳过当前sequence

	}flags;

	struct temporaryVaris {
		std::string currentProject = "";	//当前工作工程路径

		std::string currentCalGroup = "";	//当前工作计算组名称

	} tempVaris;

private:
	ctlData() {};
	~ctlData() {};
	ctlData(const ctlData&);
	ctlData& operator=(const ctlData&);

	void setDataDftVal(const ctlData::defaultValue& data);	//dftValue数据设置函数

	void setDataFlags(const ctlData::ctlFlags& data);		//flags数据设置函数

	void setDataTempVaris(const ctlData::temporaryVaris& data);	//tempVaris数据设置函数

	void setCalculateQuitFlag(bool state);

	void setCurrentProj(std::string path);

	void setCurrentCalGroup(std::string name);

private:

	//写入defaultValue的互斥锁
	mutable std::mutex dftValMtx;

	//写入ctlFlags的互斥锁
	mutable std::mutex	flagMtx;

	//写入tempVaris的互斥锁
	mutable std::mutex tempVarisMtx;

	//设置计算强制退出位的互斥锁
	mutable std::mutex calQuitMtx;

	//设置当前工作工程路径的互斥锁
	mutable std::mutex curProjMtx;

	//设置当前工作工程下当前计算组名称的互斥锁
	mutable std::mutex curCalGroupMtx;

	friend class calculation;


	// 声明外部数据管理类为友元类，允许其访问私有方法
	friend class projManage;
};