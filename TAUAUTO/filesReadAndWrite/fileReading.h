#pragma once
#include "filesReadAndWrite\readCheck.h"

class fileReading:public readCheck {
public:
	static bool historyProjRead(historyProj& hist, string path = "");//历史工程文档读取函数，未完善，考虑添加默认路径

	static bool projectInfoRead(projectInfo& proj, string path="");//项目文档读取函数

	static bool parameterRead(parametersList& pars, string path = "");//参数文档读取函数

	static bool unitRead(unitIndex& units, string path = "");//单位文件读取函数

	static bool calGroupRead(calGroup& cals, string path = "");//计算组文档读取函数

	static bool tubeDataRead(InteractionParams& intpar, string path = "");//tubeData文件读取函数

	static bool magDataRead(InteractionParams& intpar, string path = "");//magData文件读取函数

	static bool beamDataRead(InteractionParams& intpar, string path = "");//beamData文件读取函数

	static bool lossDataRead(lossAndCutOffDataInfo& loss, string path = "");//lossData文件读取函数

	static bool dispDataRead(dispDataInfo& disp, string path = "");//dispData文件读取函数

	static bool calSetRead(calSetting& calSet, string path = "");//calSetting文件读取函数

	static bool paramLinkRead(paramLinks& pars, string path = "");//paramLinks文件读取函数

	static bool resultsRead(results& reses, string path = "");//结果文件读取函数  

	static bool unitResRead(results::res& res, string path);

	static bool unitResSetRead(vector<results::res>& reses, string path);
	
	static bool resParScanVarisRead(calSetting::calSequence& data, string path);

	static bool messageRead(vector<signedString>& meg, string path = "");	//消息文件读取函数

	static bool calGroupUniteRead(calGroupUnite& calGu, string path = "");	//calGroupUnite读取函数

	//返回值的二进制下第2、3、4位（从右数）分别用以表示kc\vp\alpha的状态，为1表示文件读取异常，为0表示文件读取正常
	static unsigned int dispDataInTXT(dispDataInfo::dispData& disp);//txt类型色散文件读取函数
	
	//返回值为true，表示文件读取正常，否则表示文件读取异常
	static bool dispDataInCSV(dispDataInfo::dispData& disp);//csv类型色散文件读取函数
};