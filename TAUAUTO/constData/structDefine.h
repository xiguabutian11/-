#pragma once
//#include "tauLib/headers/params.h"
#include "constData\constInfo.h"
#include "constData\constDataInfo.h"
#include "thirdPtyLibs\rapidcsv\rapidcsv.h"
#include "ctlDataUpdate\ctlData.h"

//带有标识的数组
template <typename T,typename T2= T,typename container = std::vector<T>>
struct signedArray {
	string sign = "";		//特征字符串
	T2 signNum = 0;			//特征值
	container array = {};	//数据数组
};
//带标识的字符串，主要用于消息的管理
struct signedString {
	unsigned int flag = messageState::prompt;
	string str = "";
	string time = "";
};

//以double为键的std::map的比较器，避免了精度导致的匹配不成功
struct DoubleComparator {
	bool operator()(double a, double b) const {
		const double epsilon = ctlData::getInstance().dftValue.doubleEpsilon; // 设置精度
		return (std::fabs(a - b) > epsilon) && (a < b); // 按小于关系比较
	}
};

/*******************工程数据*********************/

//项目参数数据结构体
struct parametersList {

	//单个参数数据结构体
	struct parameter {
		string name = "";		//参数名称
		string expression = "0";		//参数表达式
		double value = 0;			//参数值
		string description = "";		//参数描述
		bool independent = false;	//是否独立于其他参数,true表示独立，false表示不独立
		set<string> dependentVariables = {}; //参数表达式所依赖的变量，即其他参数的名称
		set<string> dependee = {};  //依赖此变量的变量
		paramState state = paramState::unchecked;		//参数状态值，4种类型：0-未检查（默认）、1-检查中、2-不可用、3-可用
		string errMessage = "";
	};

	map<string, parametersList::parameter> paramList;	//参数列表,键即为参数名称
	vector<string>	orders;		//参数顺序
};



//色散文件信息结构体
struct dispDataInfo {
	//单个结构的色散文件路径信息结构体
	struct dispData {
		string fileType = dispFilesSuffix[0];		//色散文件类型，目前只支持指定格式的“txt”和“csv”
		string path = "";				//文件路径：“csv”文件的path为包含文件名的完整路径，而“txt”文件的path为文件上一级的文件夹名
		unsigned int skipHeader = 0;	//txt色散文件一般应跳过前三行，csv色散文件则不用跳
		map<double, double, DoubleComparator> vp = {};		//相速(键为频率，值为相速)
		map<double, double, DoubleComparator> kc = {};	//耦合阻抗(键为频率，值为耦合阻抗)
		map<double, double, DoubleComparator> al = {};	//alpha(键为频率，值为alpha)
		bool state = true;				//状态值，为true表示可用，否则不可用
	};
	struct samplePoint {
		string dispKey = "";			//采样点所使用的dispData的名称，
		int interpoType = dispInterpoType::linear;		//采样点与下一个采样点之间插值的类型
		bool state = true;				//状态值，为true表示可用，否则不可用
	};

	map<string, dispDataInfo::dispData> dispInfo;		//存放各结构色散文件路径信息的键值对，键为string型，表示结构名称

	std::vector<string>	orders;		//色散信息命名顺序

	map<double, dispDataInfo::samplePoint, DoubleComparator> samplePoints;	//采样点信息，键为位置,值为该位置处的采样点信息

};

struct lossAndCutOffDataInfo {
	struct lossSegment {
		double begin = 0;		//分段起始点
		double end = 0;			//分段结束点
		int lossType = 0;		//衰减类型
		unsigned int defaultDistriType = dBDistriType::increLinear;			//段上频点的默认衰减分配类型
		double defaultdB = 0;	//段上默认的dB值
	};
	struct dBofFreqs {
		dBofFreqs() {
			ctlData& data = ctlData::getInstance();
			dB = data.dftValue.defltfreqbB;
			distriType = dBDistriType::increLinear;
		}
		double dB;		//衰减值
		unsigned int distriType;	//衰减分配类型
	};


	vector<lossSegment> lossSegs;	//衰减分段信息
	map<double, vector<dBofFreqs>, DoubleComparator> dBInfo;	//各频点的衰减值
	vector<drft> cutOffs;		//截断数据
};


//单个计算组的计算设置结构体
struct calSetting {

	//单次计算序列结构体
	struct calSequence {
		string name = "";				//扫参名称
		map<string,signedArray<double,bool>> seq;	//扫参数据,键为参数名称，值为参数数据

		unsigned int seqState = seqState::unporcessed;	//序列状态
		bool seqEnable = true;		//序列使能标志位
		bool soloCal = false;		//单独计算标志位

	};
	map<string,calSetting::calSequence> calSet;   //扫参组信息,键为序列名称

	freqin centerFreq;		//中心频点
};

//paramLink结构体
struct paramLinks {
	//单个对象的参数链接结构体
	struct parLink {
		string dataType = "";		//参数指向目标的所属数据类型
		unsigned int index1 = 0;	//第一阶对象索引值
		unsigned int index2 = 0;	//第二阶对象索引值
		unsigned int index3 = 0;	//第三阶对象索引值
		//int index4 = 0;				//第四阶对象索引值
		string expression = "";		//表达式
		bool linkable = true;		//链接对象是否可进行链接，可进行链接需要满足：索引值正确和表达式可解析
		string decription = "";		//描述信息
	};
	map<string, map<string,paramLinks::parLink>>parLinks;	//存放参数信息的map，第一层键表示文件名，值表示该文件中的参数链接信息；
	//第二层键表示参数三阶索引值的组合，表示每个参数的“身份证”，不允许重复，例如"1-8-3"，即index1为1，index为8，index3为3；

	std::set<string> fileToUpdate;	//待更新文件的数组，文件数据更新完成后应清空
};

//计算组文档结构体
struct calGroup {

	bool integrity = true;		//文档完整性

	string name = "";			//计算组名称
	string creationTime = "";	//计算组创建时间
	string lastSaveTime = "";	//计算组最后保存时间
	string directory = "";		//计算组所在路径（绝对路径）
	string resFilePath = "";	//计算组结果文件路径
	string tubeDPath = "";		//tubeData文件路径（相对路径）
	string magDPath = "";		//magData文件路径（相对路径）
	string beamDPath = "";		//beamData文件路径（相对路径）
	string lossDPath = "";		//lossData文件路径（相对路径）
	string dispDPath = "";		//dispData文件路径（相对路径）

	string calSPath = "";		//calSetting文件路径（相对路径）
	string mesPath = "";		//message文件路径（相对路径）
	string paramLPath = "";     //paramLink文件路径（相对路径）
};

//计算结果结构体
struct results {
	struct res {
		string name = "";       //命名
		freqin freqy;
		map<string, double> params = {};	//存放其他参数名称和对应数值的数值

		FreqRes result;			//tau计算结果结构体（胡师兄编写，直接调用）
		vector<double> tube;	//tube信息
		vector<double> mag;		//mag信息
	};
	struct resExtractedResult {
		unordered_set<string> arguments;
		unordered_map<string, double> fixedValPars;
		map<double,results::res> result;	//单元结果的命名集合

		string seqName;
	};

	struct BGDataOfRes {	//结果背景数据
		parametersList pars;	//参数数据
		calSetting::calSequence calS;
		paramLinks parLink;
		InteractionParams intpar;
		dispDataInfo disp;
		lossAndCutOffDataInfo loss;

		bool resWaitToImport = true;
	};

	//计算结果，键为sequence名称，值为该sequence下的单元结果的集合
	map<string, vector<results::res>> reses = {};

	//计算结果提取数据，一阶键为sequence名称，二阶键为提取集合的名称，值为提取条件及单元结果的集合
	map<string, map<string, resExtractedResult>> extraction = {};

	//与计算结果配套的背景数据信息。一阶键为sequence名称
	map<string, BGDataOfRes> BGDatas = {};
};

//计算组数据的整合结构体
struct calGroupUnite {
	calGroup calG;
	calSetting calS;
	paramLinks parLink;
	vector<signedString> message;

	InteractionParams intpar;
	dispDataInfo disp;
	lossAndCutOffDataInfo loss;

	results res; 
};




//项目单位索引值结构体
struct unitIndex {
	int voltageIndex = 0;		//电压单位索引值
	int currentIndex = 0;		//电流单位索引值
	int frequencyIndex = 0;		//频率单位索引值
	int lengthIndex = 0;		//长度单位索引值
	//int timeIndex = 0;			//时间单位索引值
	//int resistanceIndex = 0;	//耦合阻抗单位索引值
};

//工程文档数据结构体
struct projectInfo {

	bool integrity = true;		//工程完整性
	string name = "";			//工程名称
	string creationTime = "";	//工程创建时间
	string lastSaveTime = "";	//工程最后保存时间
	string directory="";			//工程路径（绝对路径）
	vector<string> calGroups = {};	//计算组名称列表
	string paramFlPath = "";	//参数文件路径（相对路径）
	string unitFlPath = "";		//单位文件路径（相对路径）
};


/****************全局数据******************/
//历史工程结构体
struct historyProj {
	vector<projectInfo> histList;	//历史工程信息
};

/****************运行中产生和使用的临时数据******************/

//扫参操作过程中用于存储扫参信息的中间数据
struct paramScanData {
	vector<freqin> freqs;
	map<string, double> variants;
};

struct dispDataOfFreq {
	double vp = 0;
	double kc = 0;
	double al = 0;
};

//直接用于参与计算的数据结构体
struct dataToCalculate {
	InteractionParams intpar;		
	vector<DispersionParams> dispars;
	string seqName;
	paramScanData parScanDt;
	bool singlePoint = false;
};

struct parStateInResExt {
	unsigned int parState = resParState::fixedVal;
	double value = 0;
};


