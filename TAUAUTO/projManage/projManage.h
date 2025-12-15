#pragma once
#include "usrDataInfo\usrDataSet.h"
#include "usrDataInfo\usrDataCheck.h"
#include "filesReadAndWrite\fileWriting.h"

class projManage {
public:
	static bool newProjInfo(projectInfo& proj,string path = "", string projName = "");

	static bool projFolderGen(const projectInfo& proj);

	static bool newCalGroupStruct(calGroup& cal, string path, string name = "");

	static bool calGroupFolderGen(const calGroup& cal,set<string> resSeqNames = {});

	static bool newProj(string path = "", string projName = "");//新建工程函数

	static bool newProjWithInit(string path = "", string projName = "", string tplProjPath = "",string tplCalGName = "");	//新建工程并以模板工程的数据进行初始化的函数

	static bool newCalgroup(string path,string name="");//计算组新建函数

	//函数只判断必要的文件是否存在，不判断内容是否完整
	static bool projIntegrityVerify(string path);	//工程文件夹完整性验证函数

	static bool calGroupInterVerify(string path);	//计算组文件夹完整性验证函数

	static bool openProj(string path);//打开工程函数

	static bool globalDataFolderCreate();//全局数据文件文件夹创建函数

	static bool newEmptyFile(filesystem::path path);	//空文件创建函数

	static bool newEmptyFolder(filesystem::path path);	//空文件夹创建函数

	static bool saveProj(string path = "");
	
	static bool saveCalGroup(string path = "");

	static bool saveProjectInResSeq(results::BGDataOfRes data,string path);
private:

};	