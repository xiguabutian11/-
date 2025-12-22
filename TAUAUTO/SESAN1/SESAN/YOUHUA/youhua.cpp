#include"youhua.h"



std::tuple<LXjiegou,order,double> fre_youhua::way_1(LXsesan target,LXjiegou jiegou,order order1,double time)
{
	double k = 0.05;

	while (order1.n ==-1 && time <= 100&&order1.j!=1)
	{
		std::cout << "进行频率优化" << std::endl;
		LXjiegou old_jiegou = jiegou;

	    jiegou = all_change(jiegou, 1 - k);
	
		renew_input(jiegou);   //更新input
		sesan();              //更新output
		std::vector<LXsesan>guanzi = readTWTData("output.txt");
	
		double old_n = order1.n;
		order1 = compare::way_1(target, guanzi);
		
		//------------------------电流密度以及耦合阻抗权衡判断----------------------
		double Current_density = target.I / (PI * m_cm(jiegou.Ra) * m_cm(jiegou.Ra));
		std::cout << "此时电流密度为:" << Current_density << std::endl;
		if (Current_density > 80 && order1.p < 0)
		{
			order1.j = 1;
			std::cout << "电流密度过大，且此时耦合阻抗过小，无法调节，应改变结构" << std::endl;
		};
		//--------------------------------------------------------------------------
		
		std::cout << "截止频率为：" <<guanzi.back().f<< std::endl;
		time++;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "进行第" << time << "次调整" << std::endl;

		shuchu::A5(order1);
		if (old_n * order1.n == -1)
		{
			k = 0.5 * k;
			jiegou = old_jiegou;     //调整过头，缩小调幅，结构数据返回上一步
			order1.n = old_n;
		};

	};
	return{ jiegou,order1,time };
};

std::tuple<LXjiegou, order, double>Vp_youhua::way_1(LXsesan target, LXjiegou jiegou, order order1, double time)
{
	double k = 0.05;

	while (order1.m != 0 && time <= 100 && order1.j!= 1 && order1.t < 1)//相速优化的前提：相速不满足要求，电流密度满足要求，带宽满足要求
	{
		std::cout << "进行相速优化" << std::endl;
		LXjiegou old_jiegou = jiegou;
		if (order1.m == -1)
		{
			jiegou.L = jiegou.L * (1 + k);
			jiegou.del = 0.3 * jiegou.L;
		}
		else if (order1.m == 1)
		{
			jiegou.L = jiegou.L * (1 - k);
			jiegou.del = 0.3 * jiegou.L;
		};
		renew_input(jiegou);   //更新input
		sesan();              //更新output
		std::vector<LXsesan>guanzi = readTWTData("output.txt");
		
		LXsesan centervalue = fre_data(guanzi, HZ_turn_GHZ(target.f));
		
		double old_m = order1.m;
		order1 = compare::way_1(target, guanzi);

		time++;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "进行第" << time << "次调整" << std::endl;
		shuchu::A4(centervalue);
		shuchu::A5(order1);
		if (old_m * order1.m == -1)
		{
			k = 0.5 * k;
			jiegou = old_jiegou;
			order1.m = old_m;
		};
	};
	return{ jiegou,order1,time };
};

std::tuple<LXjiegou,order, double>Vpcha_youhua::way_1(LXsesan target, LXjiegou jiegou,order order1, double time)
{
	double k = 0.05;
	if (order1.t > 0) { changeinputvalue(inputPath, "number of loads", 3); };
	while (order1.t > 0 && time <= 100 && order1.j!= 1)
	{
		std::cout << "进行频带优化" << std::endl;
		LXjiegou old_jiegou = jiegou;
		time++;
		
		jiegou.Rg = (1-k) * jiegou.Rg;
		if (order1.t == 2&&jiegou.fir<100&&jiegou.Rg<2*jiegou.Rb) {
			jiegou.fir = jiegou.fir + 5;
		};
		
		renew_input(jiegou);   //更新input
		sesan();              //更新output
		std::vector<LXsesan>guanzi = readTWTData("output.txt");
		
		LXsesan centervalue = fre_data(guanzi, HZ_turn_GHZ(target.f));
		double old_t = order1.t;
		order1 = compare::way_1(target, guanzi);
		
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "进行第" << time << "次调整" << std::endl;
		shuchu::A4(centervalue);
		shuchu::A5(order1);

		if (old_t * order1.t == -1)
		{
			k = 0.5 * k;
			jiegou = old_jiegou;
			order1.t = old_t;
		};
	};
	return{jiegou,order1,time };
};

std::tuple<LXjiegou, order, double>Kc_youhua::way_1(LXsesan target, LXjiegou jiegou, order order1, double time)
{
	double k = 0.02;
	LXjiegou old_jiegou = jiegou;
	double cha = jiegou.Rb - jiegou.Ra;
	double cha2 = jiegou.Rg -jiegou.Ra;
	
	while (order1.p < 0 && time <= 100 && order1.j != 1)
	{
		std::cout << "进行耦合阻抗优化" << std::endl;
		
		if (jiegou.Ra > old_jiegou.Ra * 0.5)
		{
			jiegou.Ra = jiegou.Ra * (1 - k);
			jiegou.Rb = jiegou.Ra + cha;
			jiegou.Rg = jiegou.Ra + cha2;
		};

		renew_input(jiegou);   //更新input
		sesan();             //更新output
		std::vector<LXsesan>guanzi = readTWTData("output.txt");

		
		LXsesan centervalue = fre_data(guanzi, HZ_turn_GHZ(target.f));
		double old_p = order1.p;
		
        order1 = compare::way_1(target, guanzi);
		//------------------------电流密度以及耦合阻抗权衡判断----------------------
		double Current_density = target.I / (PI * m_cm(jiegou.Ra) * m_cm(jiegou.Ra));
		std::cout << "此时电流密度为:" << Current_density << std::endl;
		if (Current_density > 80 && order1.p < 0)
		{
			order1.j = 1;
			std::cout << "电流密度过大，且此时耦合阻抗过小，无法调节，应改变结构" << std::endl;
		};
		//--------------------------------------------------------------------------
		time++;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "进行第" << time << "次调整" << std::endl;
		shuchu::A4(centervalue);
		shuchu::A5(order1);

	};
	return{ jiegou,order1,time };
};