#include "SeSanJieGou.h"

void convertTxtToJson(const std::string& inputFile, const std::string& outputFile,
    double minfre, double maxfre) {
    std::ifstream infile(inputFile);
    std::string line;
    std::vector<double> freq;
    std::vector<double> vp, kc, alfa;

    // 跳过表头
    std::getline(infile, line);

    // 读取数据并过滤频率范围
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        double f, v, phi, k, a;
        if (!(iss >> f >> v >> phi >> k >> a)) continue;

        // 只添加在指定频率范围内的数据
        if (f >= minfre && f <= maxfre) {
            freq.push_back(f);
            vp.push_back(v);
            kc.push_back(k);
            alfa.push_back(a);
        }
    }

    // 输出JSON格式
    std::ofstream outfile(outputFile);
    outfile << std::fixed << std::setprecision(5);
    outfile << "{\n";
    outfile << "  \"dispGroup\": [\n";
    outfile << "    {\n";
    outfile << "      \"alpha\": {\n";
    outfile << "        \"freq\": [";
    for (size_t i = 0; i < freq.size(); ++i) {
        outfile << freq[i];
        if (i != freq.size() - 1) outfile << ", ";
    }
    outfile << "],\n        \"val\": [";
    for (size_t i = 0; i < alfa.size(); ++i) {
        outfile << alfa[i];
        if (i != alfa.size() - 1) outfile << ", ";
    }
    outfile << "]\n      },\n";
    outfile << "      \"fileType\": \"TXT\",\n";
    outfile << "      \"kc\": {\n";
    outfile << "        \"freq\": [";
    for (size_t i = 0; i < freq.size(); ++i) {
        outfile << freq[i];
        if (i != freq.size() - 1) outfile << ", ";
    }
    outfile << "],\n        \"val\": [";
    for (size_t i = 0; i < kc.size(); ++i) {
        outfile << kc[i];
        if (i != kc.size() - 1) outfile << ", ";
    }
    outfile << "]\n      },\n";
    outfile << "      \"name\": \"position_1\",\n";
    outfile << "      \"path\": \"C:/Users/HYGK/Desktop/TauCalculation/pro2_17_1\\\\position_1\",\n";
    outfile << "      \"skipHeader\": 1,\n";
    outfile << "      \"vp\": {\n";
    outfile << "        \"freq\": [";
    for (size_t i = 0; i < freq.size(); ++i) {
        outfile << freq[i];
        if (i != freq.size() - 1) outfile << ", ";
    }
    outfile << "],\n        \"val\": [";
    for (size_t i = 0; i < vp.size(); ++i) {
        outfile << vp[i];
        if (i != vp.size() - 1) outfile << ", ";
    }
    outfile << "]\n      }\n";
    outfile << "    }\n";
    outfile << "  ],\n";
    outfile << "  \"samplePoints\": [\n";
    outfile << "    {\"dispKey\": \"position_1\", \"interpoType\": 0, \"point\": 0.0},\n";
    outfile << "    {\"dispKey\": \"position_1\", \"interpoType\": 1, \"point\": 27.90},\n";
    outfile << "    {\"dispKey\": \"position_1\", \"interpoType\": 0, \"point\": 28.83}\n";
    outfile << "  ]\n";
    outfile << "}\n";
    outfile.close();
}



std::tuple <LXjiegou, LXsesan>  youhua_way1(LXjiegou jiegou, LXsesan target, std::vector<LXsesan> n_guanzi) {
    
	LXsesan centervalue = fre_data(n_guanzi, HZ_turn_GHZ(target.f));

    order order1 = compare::way_1(target, n_guanzi);

	double bigtime = 0; 
	double  time = 0;
	LXjiegou old_jiegou;

	std::cout << "目标" << std::endl;
	shuchu::A3(target);
	std::cout << "初始色散" << std::endl;
	shuchu::A4(centervalue);
	shuchu::A5(order1);

	while (!(order1.n == 0 && order1.m == 0 && order1.p == 0&&order1.t==0) && time <= 100&&order1.j!=1)
	{
		
		std::tuple<LXjiegou, order,double>fre_youhua = fre_youhua::way_1(target, jiegou, order1, time);
		jiegou= std::get<0>(fre_youhua);
		order1= std::get<1>(fre_youhua);
		time= std::get<2>(fre_youhua);
		
        std::tuple<LXjiegou, order, double>Vp_youhua = Vp_youhua::way_1(target, jiegou, order1, time);
        jiegou = std::get<0>(Vp_youhua);
        order1 = std::get<1>(Vp_youhua);
        time = std::get<2>(Vp_youhua);

		std::tuple<LXjiegou,order, double>Vpcha_youhua = Vpcha_youhua::way_1(target,jiegou,order1, time);
		jiegou = std::get<0>(Vpcha_youhua);
		order1 = std::get<1>(Vpcha_youhua);
		time = std::get<2>(Vpcha_youhua);

		std::tuple<LXjiegou, order, double>Kc_youhua = Kc_youhua::way_1(target, jiegou, order1, time);
		jiegou = std::get<0>(Kc_youhua);
		order1 = std::get<1>(Kc_youhua);
		time = std::get<2>(Kc_youhua);

		bigtime++;
	};
	std::cout << "进行第" << bigtime << "次大调整" << std::endl;
	std::vector<LXsesan>guanzi = readTWTData("output.txt");
	LXsesan N_centervalue = fre_data(guanzi, HZ_turn_GHZ(target.f));
	return{ jiegou,N_centervalue };
}

LXjiegou YOUHUA_sesan(double minfre, double maxfre, double V, double Pout,double N)
{
	LXsesan target = SeSanMuBiao(minfre, maxfre, V, Pout,N);
	//根据用户给出的频带，电压，饱和输出功率，计算出色散结构所需要达到的目标值
	LXjiegou jiegou = SeSanJieGou(target);
	// 根据目标获得色散几何结构的可能值，将几何几何结构数值存入input.txt	
	sesan();
	//通过input.txt的数据，计算出色散数据存放在output.txt中
	std::vector<LXsesan>guanzi = readTWTData("output.txt");
	//将output内的数据转移到动态结构体  guanzi  中
   
	LXsesan centervalue = fre_data(guanzi, HZ_turn_GHZ(target.f));
	//从全频带色散数据中提取对应相位角频带的色散数据，存入centervalue中
 
	std::tuple<LXjiegou, LXsesan> result = youhua_way1(jiegou, target, guanzi);
	//由几个结构，目标参数，目前实际参数3个参量以优化方式1进行优化，获得优化后的几何机构和色散参数
    
	LXjiegou youhua_jiegou = std::get<0>(result);
	//优化后的结构参数
	LXsesan youhua_centervalue = std::get<1>(result);
	//优化后对应相位角的色散参数

	shuchu::A6(jiegou, centervalue, youhua_jiegou, youhua_centervalue);

    return youhua_jiegou;
}

