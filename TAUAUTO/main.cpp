#include"TAU.h"




int main(){
	if (V < 2500) {
		std::cout << "电压过低，电压设为2500" << std::endl;
		V = 2500;
	};
	//---------------单段管子初始化----------------
	datachange::lossDataChange(0, 0, 0, 0, 0);
	//=============================================
    BEST liu;
	LXjiegou jiegou = YOUHUA_sesan(minfre, maxfre, V, Pout, 0);
	double r = 1000 * jiegou.Ra;                //单位mm 
	datachange::beamDataChange("outerR", 0.5*r);
	datachange::beamDataChange("tunnelR", r);
	convertTxtToJson(outputPath, dispdatapath, minfre-1, maxfre+1);
	double I = DianLiu::way_1(Pout, V, miu);
	double fre = (minfre + maxfre) / 2;
	//--------------管长和磁场的初始化---------------
	double length = jiegou.L*1000*20+30;
	datachange::tubeDataChange("tubeLength", length);
	double mag_A = mag1(V, 0.5 * r,I,1.8);
	double mag_period = mag2(V, r/2, I);
	datachange::mag(mag_A, mag_period);
	//--------------磁场优化------------
	while (mag_judge(fre, 0.001*mostpin, V, mag_A, mag_period) == 0)
	{
		mag_A += 0.01;
		datachange::mag(mag_A, mag_period);
	}
	mag_A = mag1(V, 0.5 * r, I, 1.8);
    //========简单优化电压==========
	double startV=liu.bestvoltage3(V, fre, I, Vjiange);
	double start_voltage = V;
	//-------------优化电压---------------
	voltage_YOUHUA_Brief(startV, start_voltage, mag_A, mag_period);
	//--------------根据管长找小信号功率---------------
	double small_pin = smallpin(1,length);             
	//--------------寻找最佳电压---------------
	double bestV = liu.bestvoltage3(V, fre, I, Vjiange);              //实际最佳电压变量
	double test_voltage = start_voltage;                              //设计目标时涉及的电压
	V_change = V_change / 2;
	//--------------优化电压---------------
	voltage_YOUHUA(bestV, test_voltage, length, mag_A, mag_period);
	//--------------寻找截断对应的管长位置---------------
	double jieduan = L_from_smallGain(15);
	//---------------回归双段管子----------------
	datachange::lossDataChange(1, jieduan-8, jieduan-2, jieduan+2, jieduan+8);
	//---------------管长和输入功率固定----------------
	datachange::changecalsetting("pin",0.01 );
	datachange::tubeDataChange("tubeLength", 500);
	//================对之后要比较的频点都进行磁场优化================
	while (
		mag_judge(fre, 0.01, V, mag_A, mag_period) == 0||
		mag_judge(minfre, 0.01, V, mag_A, mag_period) == 0||
		mag_judge(maxfre, 0.01, V, mag_A, mag_period) == 0
		)
	{
		mag_A += 0.01;
		datachange::mag(mag_A, mag_period);
	}
	mag_A = mag1(V, 0.5 * r, I, 1.8);
	
	//================对多个频点进行比较================
	double bestfre = liu.bestfre();
	datachange::changecalsetting("frequency", bestfre);
	//==============找到最低限度的管长=================
	L_YOUHUA LL = L_from_Gain(Gain,0.5);
	datachange::tubeDataChange("tubeLength", LL.tubeLength);
	//--------------三点磁场优化------------
	while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0 ||
		mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0 ||
		mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0)
	{
		mag_A += 0.01;
		mag_period = mag2(V, r / 2, I);
		datachange::mag(mag_A, mag_period);
	}
	mag_A = mag1(V, 0.5 * r, I, 1.8);
	//--------------带宽优化---------------
	SaturationResult NN_1 = best_pin1(fre, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_2 = best_pin1(minfre, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_3 = best_pin1(maxfre, bestV, LL.optimalPin, LL.tubeLength);

	double T = 0;
	while (NN_1.maxOutputPower < 400||
		NN_2.maxOutputPower < 400 ||
		NN_3.maxOutputPower < 400)
	{
		T = T + 2;

		jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, T);
		r = 1000 * jiegou.Ra;
		datachange::beamDataChange("outerR", r / 2);
		datachange::beamDataChange("tunnelR", r);

		convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
		//--------------管长优化---------------
		LL = L_from_Gain(Gain, 0.5);
		datachange::tubeDataChange("tubeLength", LL.tubeLength);
		//--------------三点磁场优化------------
		while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0||
		mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0||
		mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0)
		{
			mag_A += 0.01;
			mag_period = mag2(V, r / 2, I);
			datachange::mag(mag_A, mag_period);
		}    
		mag_A = mag1(V, 0.5*r, I, 1.8);
		//----------------------------------
		NN_1 = best_pin1(fre, bestV, LL.optimalPin, LL.tubeLength);
		NN_2 = best_pin1(minfre, bestV, LL.optimalPin, LL.tubeLength);
		NN_3 = best_pin1(maxfre, bestV, LL.optimalPin, LL.tubeLength);
	}
	//---------添加一个增益检测----------


	//===================================
	std::cout << "管子设计结束" << std::endl;

  }