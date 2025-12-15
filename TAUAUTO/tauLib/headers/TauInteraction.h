#pragma once
#include"params.h"
#include"ctlDataUpdate\ctlData.h"
using namespace std;
/*
* 互作用自动化过程相关函数定义
* WriteInterParfile//写hfa.par文件
* WriteInterVesh//写sveh.dat文件
* ReadHfa//从hfa.rez文件中提取输出功率等数据
* ReadHfagr//从hfagr.par文件中提取增益，电子效率等数据
* GetInterResult//从指定路径获取数据，包含hfa.rez和hfagr.par文件
* InterCalculate//对指定互作用预文件进行互作用计算
* Interaction_data//对输入色散及工作参数数据进行互作用计算并提取计算结果
*/
//字符串常量定义
const string strhfapar[] = { "\n",
  "' twt  ',",
  "'CONTROL PARAMETERS',",
  "'nom number of variant-------------------------------------',1",
  "'kv number of changes FREQUENCY OF A SIGNAL no more than6 '," ,
  "'IUST Sites of stability____________________________________',",
  "'IGAR AMOUNT of HARMONICSES-----------------------------------',",
  "'Ikanal---------------- ------------------------------------',",
  "'Ipe Tag of the full output------------------------------ ',",
  "'Igun- DATA INPUT FROM THE GUN ---------------------------------',",
  "'IKOL- DATA OUTPUT FOR THE COLLECTOR--------------------',",
  "'M-  number of electrons no more than 192--------------------------',",
  "'IbN number of changes of SWS-------',",
  "'IdN number of changes of  loss  -const  no more than 50-----------',",
  "'IDR=(n-number of drifts, 0 - no drifts)-----------------------',",
  "'IMP number of changes of magnetic inductance no more than 50-----',",
  "' Point WRITE' ,",
  "' The array  of  wale length  ' Input signal   Pinp ------------',",
  "'ZZZ is coordinate of the beam entrance tj the collectjr',",
  "'xooh Coordinate of the input of the HF power----------------',0",
  "'kanal-----------------------------------------------------',",
  "'Zo coordinate of the nearest to entrance of SWS zero from cathode side',0.0",
  "'IO - beam current-------------------------------------',",
  "'UO-accelerating voltage-----------------------', ",
  "'NC- amount of layers---------------------------', ",
  "'R1 -an upper radius of a boundary of the 1 layer-----------',",
  "'An averaged radial velocity',",
   "'A   radius of the tunnel----------------- ',",
   "'ZB(I) beginning of the regions with DECELERATION-const, I =IBN',",
   " 'ZDR(I) beginning of the regions with FIELD=0, I =IDR'," ,
   "'IMP number of changes of constant of a magnetic inductance50'," ,
   "'LN1(I)  SPECIFIC  DAMPING 1 HARMONIC  I=1+IDN---',",
   "'H(length of the step of the calculation)-------------------',",
   "'Xend(length of the tube)=----------------------------------',",
   "' Sites of stability***********************',",
   "'*********************************************end'",
};

class HfaPar
{
public:
    string GetPinStr(vector<freqin> data_f);//按照hfa.par格式生成频点及输入功率相关信息的字符串
    string GetBeamStr(beam data_beam);//按照hfa.par格式生成电子注相关信息的字符串
    string GetSegStr(vector<double> data_swsbeg);//按照hfa.par格式生成满波结构色散分段相关信息的字符串
    string GetDrftStr(vector<drft> data_drft);//按照hfa.par格式生成截断（漂移区）相关信息的字符串
    string GetLossStr(vector<loss> data_loss,int numOfFreq);//按照hfa.par格式生成集中衰减相关信息的字符串
    string GetMagStr(vector<MagneticField>data_mag);//按照hfa.par格式生成磁场相关信息的字符串
    string writefile(const InteractionParams& interpar); //按照hfa.par格式生成文件总体的字符串集
};
//***************数据提取与导出*************
string WriteInterParfile(const InteractionParams& interpar, string workpath = "", string timeflag = "");

//string WriteInterParfile_old(InteractionParams& interpar, string workpath = "", string timeflag = "");

string WriteInterVesh(const InteractionParams& interpar, const vector<DispersionParams>& dispars, string workpath = "", string timeflag = "");

bool ReadHfa(string filepath);

bool ReadHfagr(InteractionRes& interres, string filepath);

bool GetInterResult(InteractionRes& interres, string filepath);

//**************互作用计算*********************
bool executeWithCreateProcess(const std::string& exePath, const std::string& parameters, const std::string& workDir,DWORD timeOut);

bool InterCalculate(string disfilepath, string intparfilepath, string workpath = "");

bool Interaction_data(const InteractionParams& interpar, const vector<DispersionParams>& dispars, InteractionRes& interres, string workpath = "", size_t threadID = 0);





