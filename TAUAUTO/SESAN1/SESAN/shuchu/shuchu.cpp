#include"shuchu.h"


void shuchu::A1(double minfre, double maxfre, double voltage, double Pmax)
{
	std::cout << "设计参数为" << std::endl;
	std::cout << "工作频段:" << minfre << "--" << maxfre << std::endl;
	std::cout << "工作电压:" << voltage << std::endl;
	std::cout << "饱和输出功率:" << Pmax << std::endl;
}


void shuchu::A2(double vp,double I,double kc,double Ra,double Rb,double Rc,double Rg,double L,double del)
{
	std::cout << "初步预测" << std::endl;
	std::cout << "相速:" << vp/ Vc<<"个光速" << std::endl;
	std::cout << "电流:" << I<< "A" << std::endl;
	std::cout << "耦合阻抗:" <<kc<< "om" << std::endl;
	std::cout << "螺旋线内半径:" << m_mm(Ra)  <<"mm" << std::endl;
	std::cout << "螺旋线外半径:" << m_mm(Rb)  << "mm" << std::endl;
	std::cout << "屏蔽壳内半径:" << m_mm(Rc) << "mm" << std::endl;
	std::cout << "翼片内半径:" << m_mm(Rg) << "mm" << std::endl;
	std::cout << "螺距:" << m_mm(L) << "mm" << std::endl;
	std::cout << "线宽:" << m_mm(del) << "mm" << std::endl;
}

void shuchu::A3(LXsesan guanzi)
{
	std::cout << "f:" << HZ_turn_GHZ(guanzi.f) << std::endl;
	std::cout << "vp:" << guanzi.vp / Vc << std::endl;
	std::cout << "kc:" << guanzi.kc << std::endl;
};

void shuchu::A4(LXsesan guanzi)
{
	std::cout << "f:" << guanzi.f_end << std::endl;
	std::cout << "vp:" << guanzi.vp << std::endl;
	std::cout << "kc:" << guanzi.kc << std::endl;
};

void shuchu::A5(order order)
{
	std::cout << "判断参数" << order.n << "  " << order.t << "  " << order.m << "  " <<order.p << "  " << std::endl;
};

void shuchu::A6(LXjiegou jiegou,LXsesan centervalue,LXjiegou youhua_jiegou,LXsesan youhua_centervalue)
{
	std::cout << "初始值：" << std::endl;
	std::cout << " Ra:" << jiegou.Ra * 1e3 << " Rb:" << jiegou.Rb * 1e3 << " Rc:" << jiegou.Rc * 1e3 << " Rg:" << jiegou.Rg * 1e3 << " L:" << jiegou.L * 1e3 << " del:" << jiegou.del * 1e3 << std::endl;
	std::cout << " f:" << centervalue.f_end << " vp:" << centervalue.vp << " kc:" << centervalue.kc << std::endl;
	std::cout << "调整后：" << std::endl;
	std::cout << " Ra:" << youhua_jiegou.Ra * 1e3 << " Rb:" << youhua_jiegou.Rb * 1e3 << " Rc:" << youhua_jiegou.Rc * 1e3 << " Rg:" << youhua_jiegou.Rg * 1e3 << " L:" << youhua_jiegou.L * 1e3 << " del:" << youhua_jiegou.del * 1e3 << std::endl;
	std::cout << " f:" << youhua_centervalue.f_end << " vp:" << youhua_centervalue.vp << " kc:" << youhua_centervalue.kc << std::endl;
};