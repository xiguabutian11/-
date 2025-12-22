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
			mag_A = mag1(V, 0.5 * r, I, 1.8);
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
			mag_A = mag1(V, 0.5 * r, I, 1.8);
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
	voltage_YOUHUA(bestV, test_voltage, length, mag_A, mag_period);
	//---------------回归双段管子----------------
	datachange::lossDataChange(1, 20, 22, 24, 26);
	//---------------管长和输入功率固定----------------
	datachange::changecalsetting("pin",0.01 );
	datachange::tubeDataChange("tubeLength", 300);
	//================对多个频点进行比较================(此处可能需要加个多频点的磁场检测)
	double bestfre = liu.bestfre();

	////--------------管长优化---------------
	//lengthstruct lengths = youhua_length(2,length, Gain, fre, small_pin, bestV);
	////--------------三频点(左右频点与中心频点)磁场优化------------
	//while (
	//	mag_judge(fre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0
	//	)
	//{
	//	mag_A += 0.01;
	//	datachange::mag(mag_A, mag_period);
	//}
	//mag_A = mag1(V, r1, I, 1.8);
	//
	////--------------带宽优化---------------
	//SaturationResult powerresult = best_pin1(fre, bestV, mostpin * 0.5, lengths.final_length);
	//SaturationResult powerresult1 = best_pin1(minfre, bestV, mostpin * 0.5, lengths.final_length);
	//SaturationResult powerresult2 = best_pin1(maxfre, bestV, mostpin * 0.5, lengths.final_length);
	//double T = 0;
	//while (powerresult.maxOutputPower < Pout||
	//	powerresult1.maxOutputPower < Pout||
	//	powerresult2.maxOutputPower < Pout
	//	)
	//{
	//	T = T + 2;

	//	jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, T);
	//	r = 1000 * jiegou.Ra;
	//	datachange::beamDataChange("outerR", r / 2);
	//	datachange::beamDataChange("tunnelR", r);

	//	convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
	//	//--------------管长优化---------------
	//	lengths = youhua_length(2,lengths.final_length, Gain, fre, small_pin, bestV);
	//	//--------------三点磁场优化------------
	//	while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0)
	//	{
	//		mag_A += 0.01;
	//		mag_period = mag2(V, r / 2, I);
	//		datachange::mag(mag_A, mag_period);
	//	}    
	//	mag_A = mag1(V, r1, I, 1.8);
	//	//----------------------------------
	//	powerresult = best_pin1(fre, bestV, mostpin * 0.5, lengths.final_length);         //best_pin初始输入功率是手打的
	//	powerresult1 = best_pin1(minfre, bestV, mostpin * 0.5, lengths.final_length);
	//	powerresult2 = best_pin1(maxfre, bestV, mostpin * 0.5, lengths.final_length);
	//}
	////---------添加一个增益检测----------


	////===================================
	//std::cout << "管子设计结束" << std::endl;

}