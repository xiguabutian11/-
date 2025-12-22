#pragma once
#include"constpar.h"
/*
* 数据所使用单位明细
* 一般长度参数，例如慢波结构尺寸等采用单位：mm
* 相速，电子注速度等高速度参数采用相对光速的归一化值
* 电磁波频率采用单位：GHz
* 相位，角度等参数采用角度制单位：°（度，degrees）
*/
struct StructParams1
{
	StructParams1()
	{
		this->swsr = 0;
		this->swsro = 0;
		this->pitch = 0;
		this->helixwidth = 0;
		this->g = 0;
		this->fir = 60;
		this->vload = 3;
		this->eps = 6.5;
		this->area = 0;
		this->vrod = 3;
		this->ratio = 1;
		this->c = 0;
		this->cf = 1;
		this->rodata = {};
		this->points = 6;
		this->pol = 0.2;
	}
	double swsr;//螺旋线内半径
	double swsro;//螺旋线外半径

	double pitch;//螺旋线螺距
	double helixwidth;//螺旋线线宽

	double g;//翼片内半径
	double fir;//翼片圆心角
	int vload;//翼片个数：

	double eps;//夹持杆相对介电常数
	double area;//当不设置实际夹持杆时，用空间占比等效
	int vrod;//夹持杆个数
	double pol;//夹持杆厚度

	double c;//管壳内径
	double ratio;//螺旋线相对于铜的电导率
	double cf;//修正系数
	vector<vector<double>> rodata;//绘制夹持杆的坐标数据，m*3矩阵；以螺旋线圆心为原点，每行为{x,y,flag}，flag表示与相邻点的链接方式，0表示直线，1表示曲线；

	int points;//按相位划分计算点数
};
struct DispersionParams1
{
	DispersionParams1()
	{
		this->data_f = {};
		this->data_kc = {};
		this->data_vp = {};
		this->data_phi = {};
		this->data_alfa = {};
		this->backwave_f = {};
		this->backwave_kc = {};
		this->backwave_vp = {};
		this->backwave_phi = {};
		this->backwave_alfa = {};
	}

	vector<double> data_f;//频点
	vector<double> data_kc;//耦合阻抗
	vector<double>data_vp;//相速
	vector<double>data_phi;//相位
	vector<double>data_alfa;//衰减
	vector<double> backwave_f;//频点，返波
	vector<double> backwave_phi;//相位，返波
	vector<double> backwave_kc;//耦合阻抗，返波
	vector<double>backwave_vp;//相速，返波
	vector<double>backwave_alfa;//衰减，返波
};

struct MagneticField1 {
	MagneticField1()
	{
		this->begin = 0;
		this->period = 6;
		this->m0 = 0;
		this->m1 = 0.3;
		this->m2 = 0;
	}
	double begin;//起始位置
	double period;//周期
	double m0;//直流偏移
	double m1;//幅值
	double m2;//二次项幅值
};
struct drft1 {
	drft1()
	{
		this->begin = 0;
		this->end = 0;
	}
	double begin;//起始位置
	double end;//终止位置
};
struct loss1 {
	loss1() {
		this->type = 0;
		this->begin = 0;
		this->end = 0;
		this->dataloss = { 1200,1200,1200,1200,1200,1200 };
	}
	int type;//衰减类型，取值-1，0，1
	double begin;//衰减起始位置
	double end;//衰减终止位置
	vector<double>dataloss;//每频点衰减系数
};
struct freqin1 {
	freqin1() {
		this->freq = 0;
		this->pin = 0.1;
		this->pin1 = 0;
		this->pin2 = 0;
	}
	double freq;//频点频率
	double pin;//输入功率
	double pin1;//谐波输入功率
	double pin2;//2次谐波输入
};
struct beam1 {
	beam1() {
		this->Rpna = 0;
		this->Rpvn = 0;
		this->swsr = 0;
		this->beamrate = { 0.33,0.66,1 };
		this->vr = { 0,0,0 };
	}
	double Rpna;//电子注外半径
	double Rpvn;//电子注内半径
	double swsr;//通道半径
	vector<double>beamrate;//电子注分层数据，beamrate[i]为前i+1层的占比
	vector<double>vr;//电子注初始径向速度
};
struct InteractionParams1 {
	InteractionParams1()
	{
		this->Sites_of_stability = 0;
		this->Igun = 0;
		this->OutForCollector = 0;
		this->FullOutput = 0;
		this->harmonicses = 3;//计算谐波次数
		this->NumofElectrons = 96;//宏粒子数目
		this->V = 0;//工作电压
		this->I = 0;//工作电流
		this->tubelength = 0;//互作用管长
		this->interstep = 0;//互作用步长	
		this->beamdata = {};
		this->data_mag = {};
		this->data_f = {};//频点
		this->data_drft = {};//截断数据
		this->data_loss = {};//衰减数据
		this->data_swsbeg = {};//色散分段数据
	}
	int Sites_of_stability;//稳定性计算，默认0不计算
	int Igun;//是否采用电子枪输入
	int OutForCollector;//是否为收集级输出数据
	int FullOutput;
	int harmonicses;//计算谐波次数
	int NumofElectrons;//宏粒子数目
	double V;//工作电压
	double I;//工作电流
	double tubelength;//互作用管长
	double interstep;//互作用步长
	beam1 beamdata;//电子注数据
	vector<MagneticField1>data_mag;//磁场参数
	vector<freqin1> data_f;//频点及其输入功率
	vector<drft1> data_drft;//截断数据
	vector<loss1> data_loss;//衰减数据
	vector<double> data_swsbeg;//色散分段数据，每段起始位置
};

struct FreqRes1 {
	FreqRes1() {
		this->freq = 0;
		this->Pso = 0;
		this->len_pso = 0;
		this->FlowRate = {};
		this->eff = {};
		this->Gain = {};
		this->Pout = {};
		this->speed = {};
	}
	double freq;//频率值
	double Pso;//饱和输出功率
	double len_pso;//饱和输出位置
	vector<double>FlowRate;// 随管长的电子流通率
	vector<double>speed;//各频点随管长的归一化平均电子速度，以初始速度为基准归一化
	vector<vector<double>>Pout;//各谐波，随管长的输出功率
	vector<vector<double>>Gain;//各谐波，随管长的增益
	vector<vector<double>> eff;//各谐波，随管长的电子效率
};
struct InteractionRes1 {
	InteractionRes1() {
		this->fpoints = 0;
		this->tube = {};
		this->mag = {};
		this->res = {};
	}
	int fpoints;//频点数
	vector<double> tube;//管长采样点
	vector<double>mag;//磁场随管长的变化值
	vector<FreqRes1> res;
};

