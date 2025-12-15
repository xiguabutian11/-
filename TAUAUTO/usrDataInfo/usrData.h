#pragma once
#include"commonFuncs\funcsTwo.h"
#include"constData\structDefine.h"

class usrData {				//存储用户数据的类，单例模式，全局有效
public:
	static usrData& getInstance()
	{
		static usrData instance;
		return instance;
	}

	//用户数据
	projectInfo proj;		//工程命名和各文件路径数据
	parametersList pars;	//工程参数数据
	unitIndex unit;			//工程单位索引值数据
	calGroupUnite curCalGroup;	//当前计算组数据集合

	//数据待检查标志位，二进制下每一位代表一项的检查情况，为1表示已经检查过且无错误，为0表示待检测或者有错误，默认为0
	unsigned long long int dataCheckFlag = 0;

public:
	//错误信息，flag用于在有界面时，是否弹出警示框
	void errMesInsert(string meg, bool flag = false);
	//警告信息
	void warnMesInsert(string meg, bool flag = false);
	//提示信息
	void promptMesInsert(string meg, bool flag = false);

	//数据待检查标志查看函数
	bool dataCheckedOrNot();

private:
	usrData() {};
	~usrData() {};
	usrData(const usrData&);
	usrData& operator=(const usrData&);

	// 私有线程安全写接口
	void setDataProj(const projectInfo& pro);	//proj数据设置函数

	void setDataPars(const parametersList& params);	//pars数据设置函数

	void setDataUnit(const unitIndex& units);	//unit数据设置函数

	void setDataCalGroup(const calGroup& calGrp);	//calG数据设置函数

	void setDataCalSetting(const calSetting& calSet);	//calS数据设置函数

	void setDataParLink(const paramLinks& paramLink);	//parLink数据设置函数

	void setDataMessage(const vector<signedString>& meg);	//message数据设置函数

	void setDataIntpar(const InteractionParams& intp);	//intpar数据设置函数

	void setDataDispar(const dispDataInfo& dispar);		//disp数据设置函数

	void setDataRes(const results& result);		//res数据设置函数

	void setDataLoss(const lossAndCutOffDataInfo& loss);	//loss数据设置函数

	void setDataCheckFlag(unsigned int bit,bool flag);	//dataCheckFlag数据设置函数

	void insertDataParLink(paramLinks::parLink parL);	//parLink数据插入函数

	void insertDataRes(const results::res& result, const string& seqName);	//res中reses数据插入函数

	void insertDataResExtraction(const results::resExtractedResult& extr, const string& seqName, const string& extrName); //res中reses数据插入函数

	void insertDataResBGData(const results::BGDataOfRes& data, const string& seqName);//res中BGDatas数据插入函数

	void insertDataMessage(const signedString& str);	//message数据插入函数

	void setDataCurCalGroup(const calGroupUnite& calGU);	//curCalGroup数据设置函数

	void resetDataCheckFlag();		//dataCheckFlag数据重置函数（全置为0，即全部都要进行检查）

private:
	//读写锁
	mutable std::shared_mutex rwLock;

	//写入projectInfo的互斥锁
	mutable std::mutex proInfoMtx;

	//写入parametersList的互斥锁
	mutable std::mutex parameterMtx;

	//写入unitIndex的互斥锁
	mutable std::mutex unitMtx;

	//写入calGroup的互斥锁
	mutable std::mutex calGroupMtx;

	//写入calSetting的互斥锁
	mutable std::mutex calSettingMtx;

	//写入paramLinks的互斥锁
	mutable std::mutex parLinksMtx;

	//写入message的互斥锁
	mutable std::mutex mesgMtx;

	//写入InteractionParams的互斥锁
	mutable std::mutex intparMtx;

	//写入dispDataInfo的互斥锁
	mutable std::mutex dispDtMtx;

	//写入results的互斥锁
	mutable std::mutex resMtx;

	//写入loss的互斥锁
	mutable std::mutex lossMtx;

	//对parLink的parlinks进行insert的互斥锁

	mutable std::mutex parLinksInsertMtx;

	//对results的reses进行insert的互斥锁
	mutable std::mutex resInsertMtx;

	//对results的extraction进行insert的互斥锁
	mutable std::mutex resExtInsertMtx;

	//对results的BGDatas进行insert的互斥锁
	mutable std::mutex resBGDataInsertMtx;

	//对message进行insert的互斥锁
	mutable std::mutex megInsertMtx;

	//写入dataCheckFlag的互斥锁
	mutable std::mutex dataCheckMtx;



	// 声明外部数据管理类为友元类，允许其访问私有方法
	friend class usrDataSet;

	friend class usrDataCheck;

	friend class parameter;

	friend class dispDtPro;

	friend class calculation;

	friend class resPro;

	friend class lossDtPro;

	friend class projManage;
};