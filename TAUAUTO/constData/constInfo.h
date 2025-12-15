#pragma once
#include "tauLib/headers/params.h"
#include "thirdPtyLibs\json\json.h"

using namespace std;

//文件读取检查readCheck提示信息
static map<string, string> readCheckPrompt = {
	{"emptyPath","File path is empty"},
	{"emptyFile","File is empty"},
	{"readFail", "Unable to read file"},
	{"parseFail","Unable to parse json file"},
	{"defltPathFail","Unable to secure default path"},
	{"missingKeys","Json file lacks of essential keys"},
	{"missingItems","TXT file lacks of essential items"},
	{"missingHeaders","Excel-type file lacks of essential headers"},
	{"missingDataItems","TXT file lacks of essential data beneath items"},
	{"jsonObjWrongType","Follwing json key-value pairs' values are in wrong type"},

	{ "arrElement","certain element in the array" }
	
};
//文件读取检查readCheck中各部分的名称
static map<string, string> projFileItems = {
	{"histProj", "Data history of projects"},
	{"projInfo","Information of project"},
	{"parameter","Parameter.csv"},
	{"unit","Unit data"},
	{"calGroup","Information of calculation group"},
	{"tubeData","tubeData.json"},
	{"magData","magData.json"},
	{"beamData","beamData.json"},
	{"lossData","lossData.json"},
	{"dispData","dispData.json"},
	{"calSet","calSetting.json"},
	{"paramLink","paramLink.json"},
	{"unitRes","Unit result"},
	{"res","Results"},
	{"meg","Message"}

};

//文件写入fileWriting提示信息
static map<string, string> fileWritingPrompt = {
	{"writeFail","Unable to write file"}
};

//usrDataCheck err提示信息
static map<string, string> usrDataCheckPrompt = {
	{"minusDetect","Argument is minus"},
	{"unableErrCheck","ErrCheck function not specified for non-arithmetic argument"},
	{"outOfRange","Argument is out of range"},
	{"lackOfData","There are no Data"},
	{"wrongData","Data has encountered an exception"},
	{"overlapData","Data's ranges overlap"},
	{"lossOverlapCutOff","Localized  attenuation range overlaps with cutoff range"},
	{"dispOverlapCutOff","Dispersion sampoint conflicts with cutoff range"},
	{"lackOfDispData","Dispersion data are not complete"},
	{"errorDispData","Dispersion data don't exist or are unavailable"},
	{"minusDispData","Dispersion data contain minus values"}

};
//usrDataCheck inapt提示信息
static map<string, string> usrDtInaptPrompt = {
	{"inaptVoltage","Voltage is inappropriate"}
};

//projManage err提示信息
static	map<string, string> projManageErrPrompt = {
	{"emptyPath","File path is empty"},
	{"emptyDir","Directory is empty"},
	{"unavailPath","File path does't exist or is unavailable"},
	{"unavailDir","Directory does't exist or is unavailable"},
	{"notDir","Provided path is not a directory"},
	{"notFilePath","Provided path is not a file path"},
	{"projDfltPathErr","Default project directory is unavailable"},
	{"calGCreateFail","Calculation group creation failed due to unavailable path(es)"},
	{"projCreateFail","Project creation failed due to unavailable path(es)"},
	{"globalFolderCrFail","Global folders and files'creation failed due to unavailable path(es)"},
	{"projNotInte","Project folder is not integral"},
	{"calGNotInte","Calculation group folder is not integral"},
	{"newProjFail","Unable to create a new project"},
	{"newCalGFail","Unable to create a new calgroup"}

};

//projManage 提示信息
static map<string, string> projManagePrompt = {
	{"tplInitFail","Initialization with template project failed"},
	{"openSuccess","Project open succeeded"},
	{"openFail","Project open failed"},
	{"newSuccess","New project creation succeeded"},
	{"newFail","New project creation  failed"},
	{"saveSuccess","Project save succeeded"},
	{"saveFail","Project save failed"}
};

//projManage 对象信息
static	map<string, string> proManageObj = {
	{"newCalGroup","New calculation group creation"},
	{"newProj"," New project creation"},
	{"proj","Project creation"},
	{"calGroup","Calculation group creation"},
	{"projCheck","Project check"},
	{"calGCheck","Calculation  group check"},
	{"globalFolder","Global folders and files creation"}
};

//parameter 提示信息
static map<string, string> parameterPrompt = {
	{"parNameWrong","Invalid name, Parameter names can only consist of letters, numbers, or underscores, and cannot start with a digit."},
	{"parNoExist","Parameter does't exist"},
	{"depVarisNoExist","Certain variable the expression relies on is undefined"},
	{"unresolveExpr","Expression are unresolved."},
	{"parDeleted","Parameter is successfully deleted"}
};

//parameter 对象信息
static map<string, string> parameterObj = {
	{"par","Parameter"}
};

//calculate 提示信息
static map<string, string> calculatePrompt = {
	{"calOccupied","Calculation program is running，wait for the calculation to complete or try again after a force quit"},
	{"dataCheckErr","Data check error, check message list for details"},
	{"startCal","Sequence Calculation Start"},
	{"startSoloCal","Solo Calculation Start"},
	{"calComplete","Calculation completed!"},
	{"calQuitting","Calculation quitting......, waiting for tasks in pool to complete"},
	{"calQuit","Calculation quit"},
	
};

//******************结构信息******************//

//***************全局文件***************//
static map<char, string> JsonValType = {
	{'a',"Array"},
	{'i',"Int"},
	{'u',"Unsigned int"},
	{'d',"Double"},
	{'b',"Bool"},
	{'s',"String"},
	{'o',"Json object"},
	{'n',"Numeric"}
};

//文件historyProj.json中的键名
static map<int, pair<string,char>> histProjKeys = {
	{1,{"historyList",'a'}}
};
//文件historyProj.json中键"hist"对应的数组中元素的键名
static map<int, pair<string, char>> histProjSubKeys = {
	{1,{"name",'s'}},
	{2,{"projPath",'s'}},
	{3,{"creationTime",'s'}},
	{4,{"lastSavaTime",'s'}}
};

//文件currentProj.txt中的项目名
//static map<int, string>currentProjItems = {
//	{1,"[current project path]"},
//	{2,"[current calculation group]"}
//};

//全局文件夹和文件相对于程序自动获得的当前路径的位置(需要在该路径上添加的后缀)
static map<string, string> globalFilePos = {
	{"df","dataFiles"},
	{"hist","dataFiles\\historyProj.json"},
	{"tplProj","dataFiles\\templateProject"},
	{"dfltProj","dataFiles\\projects"},
	{"disp","dataFiles\\templateDispersionFiles"}
};

//模板色散数据文件夹下的色散文件分类文件夹，1为csv，2为txt
static map<int, string> tplDispFiles = {
	{1,"dispersionData_1"},
	{2,"dispersionData_2"}
};

//***************工程文件***************//
// //计算组名称的默认值（创建新工程时默认存在的计算组），同时也是源代码文件夹下模板工程中的默认计算组名称，尽量别修改
static map<int, string> calGroupDefault = {
	{1,"interaction_1"}
};

//新建工程时，工程文件夹内的参数和单位信息文件夹
static map<int, string> ProjPAUFolders = {
	{1,"paramsAndUnits"}
};

//工程中paramsAndUnits文件夹下的各文件名
static map<int, string> PAUFolderFiles = {
	{1,"parameter.csv"},
	{2,"unit.json"}
};

//工程计算组interaction_1内的文件夹
static map<int, string> CalGFolders = {
	{1,"tubeData"},
	{2,"resFiles"}
};

//计算组中tubeData文件夹下的各文件名
static map<int, string> tubeDFolderFiles = {
	{1,"tubeData.json"},
	{2,"magData.json"},
	{3,"beamData.json"},
	{4,"lossData.json"},
	{5,"dispData.json"}
};

//计算组文件夹下的各文件名
static map<int, string> calGFiles = {
	{1,"calSetting.json"},
	{2,"message.txt"},
	{3,"paramLink.json"}
};

//特定文件后缀信息
static map<string, string> fileSuffix = {
	{"proj",".txt"},
	{"calGroup",".txt"},
	{"unitRes",".json"},
	{"resParScanVaris",".json"}
};

//新建工程时，信息文档中的初始信息
static map<int, string> mesInitInfo = {
	{1,"A new project has been successfully created"}
};

//工程文件中的项目名
static map<int, string> projItems = {
	{1,"[project name]"},
	{2,"[creation time]"},
	{3,"[last save time]"},
	{4,"[calculation groups]"},
	{5,"[parameter file path]"},
	{6,"[unit file path]"}
};

//参数文件中的标头名（.csv文件）
static map<int, string> paramHeaders = {
	{1,"name"},
	{2,"expression"},
	{3,"description"}
};

//单位文件中的键名（.json文件）
static map<int, pair<string, char>> unitKeys = {
	{1,{"VIndex",'u'}},
	{2,{"IIndex",'u'}},
	{3,{"FIndex",'u'}},
	{4,{"LIndex",'u'}}
};


//***************计算组文件***************//
//计算组文件中的项目名
static map<int, string> calGroupItems = {
	{1,"[name]"},
	{2,"[creationTime]"},
	{3,"[lastSaveTime]"},
	{4,"[tubeData]"},
	{5,"[magData]"},
	{6,"[beamData]"},
	{7,"[lossData]"},
	{8,"[dispData]"},
	{9,"[calSetting]"},
	{10,"[message]"},
	{11,"[paramLink]"},
	{12,"[resFiles]"}
};

//tubeData文件中的键名（.json文件）
static map<int, pair<string,char>> tubeDataKeys = {
	{1,{"voltage",'n'}},
	{2,{"current",'n'}},
	{3,{"tubeLength",'n'}},
	{4,{"interstep",'n'}},
	{5,{"harmonic",'u'}},
	{6,{"numsOfE",'u'}},
	{7,{"stability",'u'}},
	{8,{"outForCollector",'u'}},
	{9,{"fullOutput",'u'}}
};

//magData文件中的键名（.json文件）
static map<int, pair<string,char>> magDataKeys = {
	{1,{"magnetic",'a'}}
};
//magData文件中键"magnetic"对应的数组中的元素的键名
static map<int, pair<string,char>> magElementKeys = {
	{1,{"begin",'d'}},
	{2,{"period",'d'}},
	{3,{"DcAMP",'d'} },
	{4,{"AMP",'d' }},
	{5,{"QuadAMP",'d' }}
};

//beamData文件中的键名（.json文件）
static map<int, pair<string, char>> beamDataKeys = {
	{1,{"outerR",'d'}},
	{2,{"innerR",'d'}},
	{3,{"tunnelR",'d'} },
	{4,{"beamrate",'a'}},
	{5,{"vr",'a'}},
	{6,{"Igun",'u'}},
	{7,{"INDIN.tau",'s'}}
};

//lossData文件中的键名（.json文件）
static map<int, pair<string, char>> lossDataKeys = {
	{1,{"cutOffs",'a'}},
	{2,{"lossSegs",'a'}},
	{3,{"dBInfo",'a'}}
};
//lossData文件中键"cutOffs"对应的数组中的元素的键名
static map<int, pair<string, char>> cOffElementKeys = {
	{1,{"begin",'d'}},
	{2,{"end",'d'}}
};
//lossData文件中键"lossSegs"对应的数组中的元素的键名
static map<int, pair<string, char>> lossElementKeys = {
	{1,{"begin",'d'}},
	{2,{"end",'d'}},
	{3,{"lossType",'i'}},
	{4,{"defaultDistriType",'u'}},
	{5,{"defaultdB",'d'}}
};

//lossData文件中键"dBInfo"对应的数组中的元素的键名
static map<int, pair<string, char>> dBElementKeys = {
	{1,{"freq",'d'}},
	{2,{"dBs",'a'}}
};
//lossData文件中键"dBInfo"对应的数组中键"dBs"对应的数组的元素的键名
static map<int, pair<string, char>> dBsEleKeys = {
	{1,{"dB",'d'}},
	{2,{"distriType",'u'}}
};

//dispData文件中的键名（.json文件）
static map<int, pair<string, char>> dispDataKeys = {
	{1,{"dispGroup",'a'}},
	{2,{"samplePoints",'a'}}
};
//dispData文件中键"dispGroup"对应的数组中元素的键名
static map<int, pair<string, char>> dispElementKeys = {
	{1,{"name",'s'}},
	{2,{"fileType",'s'}},
	{3,{"path",'s'}},
	{4,{"skipHeader",'u'}},
	{5,{"kc",'o'}},
	{6,{"vp",'o'}},
	{7,{"alpha",'o'}}
};
//dispData文件中键"dispGroup"下kc、vp、alpha对应的键名
static map<int, pair<string, char>> kvaEleKeys = {
	{1,{"freq",'a'}},
	{2,{"val",'a'}}
};

//dispData文件中键"samplePoints"对应的数组中元素的键名
static map<int, pair<string, char>> samPointEleKeys = {
	{1,{"point",'d'}},
	{2,{"dispKey",'s'}},
	{3,{"interpoType",'u'}}
};

//calSetting文件中的键（.json文件）
static map<int, pair<string, char>> calSetKeys = {
	{1,{"calSetting",'a'}},
	{2,{"centerFreq",'o'}},
};
//calSetting文件中键"calSetting"对应的数组中元素的键名
static map<int, pair<string, char>> calElementKeys = {
	{1,{"name",'s'}},
	{2,{"par",'a'}},
	{3,{"soloCal",'u'}}
};
//"calSetting"键下键"par"对应的数组中元素的键名
static map<int, pair<string, char>> seqKeys = {
	{1,{"sign",'s'}},
	{2,{"array",'a'}},
	{3,{"stateSign",'u'}}
};
//"centerFreq"键下各键名
static map<int, pair<string, char>> centerFreqKeys = {
	{1,{"freq",'d'}},
	{2,{"pin",'d'}},
	{3,{"pin_1",'d'}},
	{4,{"pin_2",'d'}},
	{5,{"desc",'s'}}
};


//paramLink文件中的键名（.json文件）
static map<int, pair<string, char>> paramLinkKeys = {
	{1,{"parLinks",'a'}}
};
//paramLink文件中键"parLinks"对应值的元素的键名
//static map<int, pair<string, char>> parLinkEleKeys = {
//	{1,{"pars",'a'}}
//};

//paramLink文件中每个参数结构体的键名
static map<int, pair<string, char>> parInfoKeys = {
	{1,{"index1",'u'}},
	{2,{"index2",'u'}},
	{3,{"index3",'u'}},
	{4,{"expression",'s'}},
	{5,{"description",'s'}},
	{6,{"dataType",'s'}}
};

//results的各unitRes文件中的键名
static map<int, pair<string, char>> unitResKeys = {
	{1,{"name",'s'}},
	{2,{"freqy",'o'}},
	{3,{"params",'a'}},
	{4,{"tube",'a'}},
	{5,{"mag",'a'}},
	{6,{"result",'o'}}
};
//results的unitRes文件中键freqy下的子键名
static map<int, pair<string, char>> resFreqyKeys = {
	{1,{"freq",'d'}},
	{2,{"pin",'d'}},
	{3,{"pin_1",'d'}},
	{4,{"pin_2",'d'}}
};
//results的unitRes文件中键params下数组各元素的键名
static map<int, pair<string, char>> resParamsKeys = {
	{1,{"parName",'s'}},
	{2,{"value",'d'}}
};
//results的unitRes文件中键result下的子键名
static map<int, pair<string, char>> resResultKeys = {
	{1,{"freq",'d'}},
	{2,{"satuPout",'d'}},
	{3,{"satuPoutPos",'d'}},
	{4,{"flowRate",'a'}},
	{5,{"speed",'a'}},
	{6,{"pout",'o'}},
	{7,{"gain",'o'}},
	{8,{"eff",'o'}}
};
//results的unitRes文件中键result下的二位数组的元素的键名
static map<int, pair<string, char>> resultArraysEleKeys = {
	{1,{"fundam",'a'}},
	{2,{"firHarm",'a'}},
	{3,{"secHarm",'a'}},
};

//results的parScanVaris文件中各键名
static map<int, pair<string, char>> resParScanVarisKeys = {
	{1,{"parScanVaris",'a'}},
	{2,{"soloCal",'u'}}
};
//results的parScanVaris文件中键parScanVaris下各数组元素的键名
static map<int, pair<string, char>> resVarisKeys = {
	{1,{"parName",'s'}},
	{2,{"value",'u'}},
	{3,{"array",'a'}}
};

//色散数据文件存放的格式
static map<int, string> dispFilesSuffix = {
	{0,"TXT"},
	{1,"CSV"}
};

//以txt类型存储色散数据的文件的文件名和其他信息
static map<int, string> dispTxtFileInfo = {
	{1,"kc0.txt"},
	{2,"vc0.txt"},
	{3,"aa.txt"}
};

//以csv类型存储色散数据的文件的列索引信息和总列数
static map<string, int> dispCsvFileInfo = {
	{"freq",4},
	{"vp",5},
	{"kc",6},
	{"al",7},
	{"columnCount",8}
};



//******************功能信息******************//

//writeCheck, 数据写入时数据检查的报错信息
static map<string, string> writeDataCheckErr = {
	{"minus","The item this data will be assigned to does NOT accpet negative"},
	{"tooBig","The number is out of range whose upper limit is "},
	{"outRange","is not in corrent range"},
	{"noIncrease","The following data are not increasing"}

};

//参数链接结构体中单个参数对象的dataType的集合
static map<string,string> parLinkDataTypes = {
	{"tube","tubeData"},
	{"mag","magData"},
	{"beam","beamData"},
	{"loss","lossData"},
	{"disp","dispData"}
};

//参数链接时参数出错的提示信息
static map<int, string> parLinkCheckPrompt = {
	{1,"Repetition of parameter link indice"},
	{2,"Unsolvable expression of parameter"}
};

//tubeData各数据项对应的参数链接数据的一阶索引值
static map<string, int> TDparLinkIndexOne = {
	{"voltage",1},
	{"current",2},
	{"tubeLen",3},
	{"interstep",4},
	{"harmonic",5},
	{"numOfE",6}
};

//用户自定义参数的状态信息
enum paramState {
	unchecked = 0,	//未检查时的状态，默认值
	checking = 1,	//检察中的状态，用于判断是否出现循环引用
	unavailable = 2,	//参数不可用
	available = 3		//参数可用
};


//扫参序列中的固定项
static map<string, string> parScanFixedItem{
	{"frequency","frequency"},		//频率
	{"pin","pin"},					//输入功率
	{"pin_1","pin_1"},				//谐波输入功率
	{"pin_2","pin_2"}				//二次谐波输入功率
};

//色散数据各段的插值类型
enum dispInterpoType {
	linear = 0,		//线性插值
	uniform = 1		//均匀分布
};

//色散数据各段的插值点生成方式
enum dispPointInsertType {
	fixedNumber = 0,	//固定个数
	fixedLength = 1		//固定长度
};

//色散数据类型
static map<string, string> dispFuncType{
	{"kc","kc"},
	{"vp","vp"},
	{"al","al"}
};

//calSetting中各计算序列（sequence）的状态信息
enum seqState {
	unporcessed = 0,
	processing = 1,
	processed = 2
};

//结果查询时各变量的状态
enum resParState {
	argument = 0,
	fixedVal = 1
};

//集中衰减段的各小段的生成方式
enum lossSementType {
	lossSegFixedNumber = 0,		//固定个数
	lossSegFixedLength = 1		//固定长度
};

//单个频点的dB值的分配方式
enum dBDistriType {
	increLinear = 0,	//上升式线性分配
	decreLinear = 1,	//下降式线性分配
	uniformDistri = 2,		//均匀分配
	gaussDistri = 3		//高斯分配

};

//程序运行时产生的消息的状态
enum messageState {
	prompt = 0,
	warning = 1,
	error = 2
};

/*注意，键值对中的值代表dataCheckFlag右移的位数，即dataCheckFlag二进制下
每一位代表一个对象的检查情况，因此最多有64项，若要扩展，则需要额外考虑其
他方法，目前64位够用*/
static map<string, unsigned int> usrDataCheckFlags{
	{"tubeData",0},
	{"magData",1},
	{"beamData",2},
	{"cutOffData",3},
	{"lossData",4},
	{"dispData",5}
};

static map<unsigned int,string> usrDataCheckFlagsRev{
	{0,"tubeData"},
	{1,"magData"},
	{2,"beamData"},
	{3,"cutOffData"},
	{4,"lossData"},
	{5,"dispData"}
};

/*****************参数链接时索引值信息******************/

//tubeData参数链接信息
enum tubeDataParLink {
	voltage = 1,
	current = 2,
	tubeLen = 3,
	interstep = 4
};

//magData参数链接信息
enum magDataParLink {
	mag = 1
};
//magData数组各元素参数链接信息
enum magEleParLink {
	magBegin = 1,
	period= 2,
	DCAMP = 3,
	AMP = 4,
	QuadAMP = 5
};

//beamData参数链接信息
enum beamDataParLink {
	innerR = 1,
	outerR = 2,
	tunnelR = 3
};

