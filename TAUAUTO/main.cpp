#include"SeSanJieGou.h"
#include"NEW/tube_length.h"
#include"NEW/gongshi.h"
#include"cal_change/find_voltage.h"



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
	double r1 = 0.5*r;                          //单位mm  
	datachange::beamDataChange("outerR", r1);
	datachange::beamDataChange("tunnelR", r);
	
	convertTxtToJson(outputPath, dispdatapath, minfre-1, maxfre+1);
	double I = DianLiu::way_1(Pout, V, miu);
	double fre = (minfre + maxfre) / 2;
	//--------------管长和磁场的初始化---------------
	double length = jiegou.L*1000*20+30;
	datachange::tubeDataChange("tubeLength", length);
	double mag_A = mag1(V,r1,I,1.8);
	double mag_period = mag2(V, r/2, I);
	datachange::mag(mag_A, mag_period);
	//--------------磁场优化------------
	while (mag_judge(fre, 0.001*mostpin, V, mag_A, mag_period) == 0)
	{
		mag_A += 0.01;
		datachange::mag(mag_A, mag_period);
	}
	mag_A = mag1(V, r1, I, 1.8);
    //========简单优化电压==========
	double startV=liu.bestvoltage3(V, fre, I, Vjiange);
	double start_voltage = V;
	double F = 0;
	//--------------优化电压---------------
	while (startV < V - Vcha || startV > V + Vcha)
	{
		if (startV < V - Vcha)
		{
			std::cout << "最佳电压低于目标范围" << std::endl;
			if (F == 1) { V_change = V_change / 2; }
			start_voltage = start_voltage + V_change;

			jiegou = YOUHUA_sesan(minfre, maxfre, start_voltage, Pout, 0);//调整色散结构
			r = 1000 * jiegou.Ra;
			datachange::beamDataChange("outerR", r / 2);
			datachange::beamDataChange("tunnelR", r);
			convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);//传入色散数据

			//--------------磁场优化------------
			while (mag_judge(fre, 0.001 * mostpin, V, mag_A, mag_period) == 0)
			{
				mag_A += 0.01;
				mag_period = mag2(V, r / 2, I);
				datachange::mag(mag_A, mag_period);
			}
			mag_A = mag1(V, r1, I, 1.8);
			//----------------------------------
			startV = liu.bestvoltage3(V, fre, I, Vjiange);//寻找最佳电压
			F = -1;
		}
		else if (startV > V + Vcha)
		{
			std::cout << "最佳电压高于目标范围" << std::endl;
			if (F == -1) { V_change = V_change / 2; }
			start_voltage = start_voltage - V_change;

			jiegou = YOUHUA_sesan(minfre, maxfre, start_voltage, Pout, 0);
			r = 1000 * jiegou.Ra;
			datachange::beamDataChange("outerR", r / 2);
			datachange::beamDataChange("tunnelR", r);

			convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);

			//--------------磁场优化------------
			while (mag_judge(fre, 0.001 * mostpin, V, mag_A, mag_period) == 0)
			{
				mag_A += 0.01;
				mag_period = mag2(V, r / 2, I);
				datachange::mag(mag_A, mag_period);
			}
			mag_A = mag1(V, r1, I, 1.8);
			//----------------------------------
			startV = liu.bestvoltage3(V, fre, I, Vjiange);
			F = 1;
		}
	}

	//--------------根据管长找小信号功率---------------
	double small_pin = smallpin(1,length);             
	
	//--------------寻找最佳电压---------------
	double bestV = liu.bestvoltage3(V, fre, I, Vjiange);              //实际最佳电压变量
	double test_voltage = start_voltage;                              //设计目标时涉及的电压
	F = 0;
	V_change = V_change / 2;
	//--------------优化电压---------------
	while (bestV < V - Vcha ||  bestV > V + Vcha)
	{
		if (bestV < V - Vcha)
		{
			std::cout << "最佳电压低于目标范围" << std::endl;
			if (F == 1) { V_change = V_change / 2; }
			test_voltage = test_voltage + V_change;

			jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);//调整色散结构
			r = 1000 * jiegou.Ra;
			datachange::beamDataChange("outerR", r / 2);
			datachange::beamDataChange("tunnelR", r);
			convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);//传入色散数据

			small_pin = smallpin(1,length);//小信号
			//--------------磁场优化------------
			while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0)
			{
				mag_A += 0.01;
				mag_period = mag2(V, r / 2, I);
				datachange::mag(mag_A, mag_period);
			}
			mag_A = mag1(V, r1, I, 1.8);
			//----------------------------------
			bestV = liu.bestvoltage3(V,fre, I, Vjiange);//寻找最佳电压
			F = -1;
		}
		else if (bestV > V + Vcha)
		{
			std::cout << "最佳电压高于目标范围" << std::endl;
			if (F == -1) { V_change = V_change / 2; }
			test_voltage = test_voltage - V_change;

			jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, 0);
			r = 1000 * jiegou.Ra;
			datachange::beamDataChange("outerR", r / 2);
			datachange::beamDataChange("tunnelR", r);

			convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
			small_pin = smallpin(1,length);
			//--------------磁场优化------------
			while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0)
			{
				mag_A += 0.01;
				mag_period = mag2(V, r / 2, I);
				datachange::mag(mag_A, mag_period);
			}
			mag_A = mag1(V, r1, I, 1.8);
			//----------------------------------
			bestV = liu.bestvoltage3(V, fre, I, Vjiange);
			F = 1;
		}
	}
	std::cout << "最佳电压处于范围内" << std::endl;
	std::cout << "开始管长调制" << std::endl;
	//---------------回归双段管子----------------
	datachange::lossDataChange(1, 20, 22, 24, 26);

	//--------------管长优化---------------
	lengthstruct lengths = youhua_length(2,length, Gain, fre, small_pin, bestV);
	//--------------三频点(左右频点与中心频点)磁场优化------------
	while (
		mag_judge(fre, small_pin, V, mag_A, mag_period) == 0||
		mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0||
		mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0
		)
	{
		mag_A += 0.01;
		datachange::mag(mag_A, mag_period);
	}
	mag_A = mag1(V, r1, I, 1.8);
	
	//--------------带宽优化---------------
	SaturationResult powerresult = best_pin1(fre, bestV, mostpin * 0.5, lengths.final_length);
	double T = 0;
	while (powerresult.maxOutputPower < Pout)
	{
		T = T + 2;

		jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, T);
		r = 1000 * jiegou.Ra;
		datachange::beamDataChange("outerR", r / 2);
		datachange::beamDataChange("tunnelR", r);

		convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
		//--------------管长优化---------------
		lengths = youhua_length(2,lengths.final_length, Gain, fre, small_pin, bestV);
		//--------------磁场优化------------
		while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0)
		{
			mag_A += 0.01;
			mag_period = mag2(V, r / 2, I);
			datachange::mag(mag_A, mag_period);
		}
		mag_A = mag1(V, r1, I, 1.8);
		//----------------------------------
		powerresult = best_pin1(fre, bestV,0.15, lengths.final_length);         //best_pin初始输入功率是手打的
	}
	std::cout << "右端点输出" << std::endl;
	HuZuoYong(maxfre, powerresult.optimalPin, bestV);
	std::cout << "左端点输出" << std::endl;
	HuZuoYong(minfre, powerresult.optimalPin, bestV);
	std::cout << "管子设计结束" << std::endl;

}