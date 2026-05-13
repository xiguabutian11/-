#include"NEW/TIAOBIAN_youhua.h"
#include <filesystem>
namespace fs = std::filesystem;

void clearTempFolder() {
	string temp_path = TAUAUTO_PATH+"/temp";  // 就是你代码里的 temp 根目录
	if (fs::exists(temp_path)) {
		fs::remove_all(temp_path);  // 一键清空所有旧临时文件
	}
	fs::create_directory(temp_path);
}

double guanzi_type;
double bestV;

int main(){
	guanzi_type = 1;
	if (Gain > 20 && Gain < 45) { guanzi_type = 2; }
	else if (Gain>=45&&Gain < 70) { 
		guanzi_type = 3; 
	}
	else if (Gain>=70)
	{
		std::cerr << "错误：增益过大（>=70dB），程序终止！" << std::endl;
		exit(EXIT_FAILURE);  
	}
	
	clearTempFolder();
	if (V < 2500) {
		std::cout << "电压过低，电压设为2500" << std::endl;
		V = 2500;
	};
	//---------------单段管子初始化----------------
	datachange::lossDataChange(0, 0, 0, 0, 0);
	//=============================================
    BEST liu;
	jieduan L_L = { 0,0,0,0 };
	jieduan L_L_2 = { 0,0,0,0 };
	LXjiegou jiegou = YOUHUA_sesan(minfre, maxfre, V, Pout, 0);
	
	double step = two_point(jiegou.L * 1000 * 0.3);
	if (step >= 1) { step = 1; }
	datachange::tubeDataChange("interstep", step);
	
	double r = 1000 * jiegou.Ra;                //单位mm 
	datachange::beamDataChange("outerR", 0.5*r);
	datachange::beamDataChange("tunnelR", r);
	convertTxtToJson(outputPath, dispdatapath, minfre-1, maxfre+1, L_L,L_L_2);
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
	double startV=liu.bestvoltage2(V, fre, I, Vjiange);
	double start_voltage = V;
	//-------------优化电压---------------
	LXjiegou old_jiegou = jiegou;
	jiegou=voltage_YOUHUA_Brief(startV, start_voltage, mag_A, mag_period);
	if (jiegou.Ra == 0 && jiegou.Rb == 0 && jiegou.L == 0 &&
		jiegou.Rc == 0 && jiegou.Rg == 0 && jiegou.del == 0 &&
		jiegou.fir == 0) {
		jiegou = old_jiegou;
	}
	//--------------根据管长找小信号功率---------------
	double small_pin = smallpin(length);             
	//--------------寻找最佳电压---------------
	bestV = liu.bestvoltage2(V, fre, I, Vjiange);              //实际最佳电压变量
	double test_voltage = start_voltage;                              //设计目标时涉及的电压
	V_change = V_change / 2;
	//--------------优化电压---------------
	old_jiegou = jiegou;
	jiegou=voltage_YOUHUA(test_voltage, length, mag_A, mag_period);
	if (jiegou.Ra == 0 && jiegou.Rb == 0 && jiegou.L == 0 &&
		jiegou.Rc == 0 && jiegou.Rg == 0 && jiegou.del == 0 &&
		jiegou.fir == 0) {
		jiegou = old_jiegou;
	}
	//--------------寻找截断对应的管长位置---------------
	double L_cut=0;
	double L_cut_2=0;
	if (guanzi_type ==2) {
		L_cut = two_point(L_from_smallGain(Gain*0.45, jiegou.L));
		std::cout << "双段管子截断位置：" << L_cut << "mm" << std::endl;
		if (L_cut < 20)
		{
			L_L.A = two_point(L_cut * 0.6);
			L_L.B = two_point(L_cut * 0.9);
			L_L.C = two_point(L_cut * 1.1);
			L_L.D = two_point(L_cut * 1.4);
		}
		else {
			L_L.A = L_cut - 8;
			L_L.B = L_cut - 2;
			L_L.C = L_cut + 2;
			L_L.D = L_cut + 8;
		}
		//---------------回归双段管子----------------
		datachange::lossDataChange(1, L_L.A, L_L.B, L_L.C, L_L.D);
		convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L_2);
	}
	
	if (guanzi_type == 3) {
		L_cut = two_point(L_from_smallGain(Gain * 0.3, jiegou.L));
		std::cout << "三段管子第一处截断位置：" << L_cut << "mm" << std::endl;
		if (L_cut < 20)
		{
			L_L.A = two_point(L_cut * 0.6);
			L_L.B = two_point(L_cut * 0.9);
			L_L.C = two_point(L_cut * 1.1);
			L_L.D = two_point(L_cut * 1.4);
		}
		else {
			L_L.A = L_cut - 8;
			L_L.B = L_cut - 2;
			L_L.C = L_cut + 2;
			L_L.D = L_cut + 8;
		}
		//---------------回归双段管子----------------
		datachange::lossDataChange(1, L_L.A, L_L.B, L_L.C, L_L.D);
		convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L_2);
	
		L_cut_2 = two_point(L_from_smallGain(Gain * 0.65, jiegou.L));
		std::cout << "三段管子第二处截断位置：" << L_cut_2 << "mm" << std::endl;
		L_L_2.A = L_cut_2 - 8;
		L_L_2.B = L_cut_2 - 2;
		L_L_2.C = L_cut_2 + 2;
		L_L_2.D = L_cut_2 + 8;

		datachange::lossDataChange_2(L_L.A, L_L.B, L_L.C, L_L.D, L_L_2.A, L_L_2.B, L_L_2.C, L_L_2.D);
		convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1, L_L, L_L_2);
	}

	//---------------管长和输入功率固定----------------
	datachange::changecalsetting("pin",0.01 );
	double L1 = two_point(pre_length(jiegou.L));
	datachange::tubeDataChange("tubeLength", L1);
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
	std::cout << "优化后磁场，幅度：" << mag_A << std::endl;
	mag_A = mag1(V, 0.5 * r, I, 1.8);
	
	//================对多个频点进行比较================
	double bestfre = liu.bestfre();
	datachange::changecalsetting("frequency", bestfre);
	//==============获得大磁场下的一个偏长管长=================
	L_YOUHUA LL;
	LL = L_from_Gain(Gain, 0.5, L1);
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
	std::cout << "优化后磁场，幅度：" << mag_A << std::endl;
	mag_A = mag1(V, 0.5 * r, I, 1.8);
	//===================二轮寻找管长==================
	datachange::changecalsetting("frequency", bestfre);
	if (LL.tubeLength <= 250) {
		LL = L_from_Gain(Gain, 0.5, LL.tubeLength);
		datachange::tubeDataChange("tubeLength", LL.tubeLength);
	}
	
	vector<SweepData> junyun_data = fre_sweep(sweep_point, bestV, LL);   //均匀管子的扫频数据作为参考组
	plotWithPython(junyun_data, 0);
	//-----------------获取电子速度为跳变做准备-----------------
	SaturationResult NN_1 = best_pin1(fre, bestV, LL.optimalPin, LL.tubeLength);
	vector<double>guanzi_speed = Get_speed(fre,NN_1.optimalPin,bestV); //中心频点的归一化速度曲线
	double point_numble = guanzi_speed.size();
	double old_L = jiegou.L;
	double cut_percent =0;
	if(L_cut !=0)
	{
		cut_percent = L_cut / LL.tubeLength;
		if (L_cut_2 != 0) 
		{
			cut_percent = L_cut_2 / LL.tubeLength;
		}
	}
	vector<SpeedPoint>need_speed = Speed_need(guanzi_speed, cut_percent, 8);   //提取归一化速度曲线中下降突变的部分，确认需要的点数为10
	vector<double> score_array;
	//-----------------根据轴向电子速度决定跳变位置，对每个跳变位置处评分-----------------
	for (int i = 0; i < need_speed.size(); i++)
	{
		//==========更新跳变结构==========
		jiegou.L = old_L * (need_speed[i].value-0.005);
		renew_input(jiegou);
		std::cout << "螺距更改为：" << jiegou.L*1000 << " mm" << std::endl;
		sesan();

		double tiaobian = LL.tubeLength*need_speed[i].index / point_numble;
		updateDispJson(tiaobian);

		vector<SweepData> tiaobian_data= fre_sweep(sweep_point, bestV, LL);
		double score = tiaobian_score(junyun_data, tiaobian_data);  //评分系统
		score_array.push_back(score);

		plotWithPython(tiaobian_data, i+1);
		revertLastDispJson();
	}
	//-----------------根据得分结果确定最终跳变位置-----------------
	cout << "\n=============================================" << endl;
	cout << "所有跳变点得分结果：" << endl;
	for (int i = 0; i < score_array.size(); i++)
	{
		cout << "第 " << i << " 个点：\t" << score_array[i] << endl;
	}
	double max_score = score_array[0];
	int max_index = 0;

	for (int i = 1; i < score_array.size(); i++)
	{
		if (score_array[i] > max_score)
		{
			max_score = score_array[i];
			max_index = i;
		}
	}
	cout << "=============================================" << endl;
	cout << "最高得分：\t" << max_score << endl;
	cout << "对应位置：\t第 " << max_index << " 个点" << endl;
	cout << "=============================================" << endl;
	//-----------------在最终跳变位置处微调螺距-----------------
	double old_score = max_score;
	double tiaobian = LL.tubeLength * need_speed[max_index].index / point_numble;
	double cut_step = 0.01 / 1000;
	int dir = 0;

	cout << "\n=============================================" << endl;
	cout << "开始螺距微调优化（步长 = " << cut_step << "）" << endl;
	cout << "=============================================" << endl;

	// 先正向试探
	jiegou.L = old_L * (need_speed[max_index].value - 0.005);
	jiegou.L += cut_step;
	renew_input(jiegou);
	std::cout << "螺距更改为：" << jiegou.L * 1000 << " mm" << std::endl;
	sesan();
	updateDispJson(tiaobian);
	vector<SweepData> data_pos = fre_sweep(sweep_point, bestV, LL);
	plotWithPython(data_pos, need_speed.size() + 1);
	double score_pos = tiaobian_score(junyun_data, data_pos);
	revertLastDispJson();
	cout << "→ 正向试探得分：" << score_pos << "  |  基准得分：" << old_score << endl;

	if (score_pos > old_score)
	{
		cout << "正向更优，确定方向：增大螺距" << endl;
		dir = 1;
	}
	else
	{
		cout << "正向无提升，开始反向试探" << endl;

		jiegou.L -= cut_step;
		jiegou.L -= cut_step;
		renew_input(jiegou);
		std::cout << "螺距更改为：" << jiegou.L * 1000 << " mm" << std::endl;
		sesan();
		updateDispJson(tiaobian);
		vector<SweepData> data_neg = fre_sweep(sweep_point, bestV, LL);
		plotWithPython(data_neg, need_speed.size() + 2);
		double score_neg = tiaobian_score(junyun_data, data_neg);
		revertLastDispJson();
		cout << "→ 反向试探得分：" << score_neg << "  |  基准得分：" << old_score << endl;

		if (score_neg > old_score)
		{
			cout << "反向更优，确定方向：减小螺距" << endl;
			dir = -1;
		}
		else
		{
			cout << "正反方向均无提升，不进行优化，恢复原始长度" << endl;
			jiegou.L += cut_step;
		}
	}

	// 迭代优化
	while (dir != 0)
	{
		cout << "\n→ 沿方向 " << dir << " 继续优化..." << endl;

		jiegou.L += dir * cut_step;
		renew_input(jiegou);
		std::cout << "螺距更改为：" << jiegou.L * 1000 << " mm" << std::endl;
		sesan();
		updateDispJson(tiaobian);
		vector<SweepData> cur_data = fre_sweep(sweep_point, bestV, LL);
		plotWithPython(cur_data, need_speed.size() + 3);
		double cur_score = tiaobian_score(junyun_data, cur_data);
		revertLastDispJson();
		cout << "→ 当前得分：" << cur_score << "  |  历史最优：" << old_score << endl;

		if (cur_score <= old_score)
		{
			cout << "得分不再提升，回退到最优位置并停止优化" << endl;
			jiegou.L -= dir * cut_step;
			break;
		}

		old_score = cur_score;
		cout << "得分提升，继续迭代" << endl;
	}

	cout << "\n=============================================" << endl;
	cout << "螺距优化全部完成，最终长度：" << jiegou.L << endl;
	cout << "开始绘制最终优化结果" << endl;
	cout << "=============================================\n" << endl;

	renew_input(jiegou);
	sesan();
	updateDispJson(tiaobian);
	vector<SweepData> cur_data = fre_sweep(sweep_point, bestV, LL);
	plotWithPython(cur_data, need_speed.size() + 4);
	revertLastDispJson();
	exit(EXIT_FAILURE);




	SaturationResult NN_2 = best_pin1(minfre, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_3 = best_pin1(maxfre, bestV, LL.optimalPin, LL.tubeLength);

	double T = 0;        
	//while (NN_1.maxOutputPower < Pout||
	//	NN_2.maxOutputPower < Pout ||
	//	NN_3.maxOutputPower < Pout)            //这个环节可能不完善
	//{
	//	T = T + 2;

	//	jiegou = YOUHUA_sesan(minfre, maxfre, test_voltage, Pout, T);
	//	r = 1000 * jiegou.Ra;
	//	datachange::beamDataChange("outerR", r / 2);
	//	datachange::beamDataChange("tunnelR", r);

	//	convertTxtToJson(outputPath, dispdatapath, minfre - 1, maxfre + 1);
	//	//================对多个频点进行比较================
	//	double bestfre = liu.bestfre();
	//	datachange::changecalsetting("frequency", bestfre);
	//	//--------------管长优化---------------
	//	LL = L_from_Gain(Gain, 0.5);
	//	datachange::tubeDataChange("tubeLength", LL.tubeLength);
	//	//--------------三点磁场优化------------
	//	while (mag_judge(fre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(minfre, small_pin, V, mag_A, mag_period) == 0||
	//	mag_judge(maxfre, small_pin, V, mag_A, mag_period) == 0)
	//	{
	//		mag_A += 0.01;
	//		mag_period = mag2(V, r / 2, I);
	//		datachange::mag(mag_A, mag_period);
	//	}    
	//	mag_A = mag1(V, 0.5*r, I, 1.8);
	//	//----------------------------------
	//	NN_1 = best_pin1(fre, bestV, LL.optimalPin, LL.tubeLength);
	//	NN_2 = best_pin1(minfre, bestV, LL.optimalPin, LL.tubeLength);
	//	NN_3 = best_pin1(maxfre, bestV, LL.optimalPin, LL.tubeLength);
	//}
	
	
	double gezi = (maxfre - minfre) / 8;
	SaturationResult NN_21 = best_pin1(minfre+gezi*1, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_22 = best_pin1(minfre + gezi * 2, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_23 = best_pin1(minfre + gezi * 3, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_11 = best_pin1(fre + gezi * 1, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_12 = best_pin1(fre + gezi * 2, bestV, LL.optimalPin, LL.tubeLength);
	SaturationResult NN_13 = best_pin1(fre + gezi * 3, bestV, LL.optimalPin, LL.tubeLength);

	//---------添加一个增益检测----------
	writeDataToFile("gain_data.txt",jiegou,LL.tubeLength,L_L, L_L_2,
		NN_1,NN_2,NN_3,
		NN_21, NN_22,NN_23
	, NN_11, NN_12, NN_13);

	//===================================
	std::cout << "管子设计结束" << std::endl;

	system(("python " + TAUAUTO_PATH + "/plot.py").c_str());
  }

 