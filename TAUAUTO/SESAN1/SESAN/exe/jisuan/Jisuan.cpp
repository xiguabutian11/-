#include"Jisuan.h"



using namespace std;

double XiangSu::way_1(double voltage,double A)
{
    double M1 = Me * Vc * Vc / (Me * Vc * Vc + voltage * e);
    double Ve = Vc * std::pow((1 - M1 * M1), 0.5)+A;
    return Ve;
}
double DianLiu::way_1(double Pmax, double voltage,double miu)
{

    double current = Pmax /( miu * voltage);
    return  current;
}
double ZuKang::way_1(double voltage, double current, double Pmax,double M)
{
    double P = std::pow(Pmax, 3);
    double A = 1 + 0.01 * std::pow(current, 4) + 1E-15 * std::pow(voltage, 2.5);
    double B = std::pow(PI, 2) * std::pow(voltage, 2) * std::pow(current, 4) - P * 0.01;
    double Kc = P * A / B+M;
    return Kc;
}



LXjiegou YuShe_jiegou::way_1(double fre, double vp)
{
    LXjiegou guodu_jiegou;
    double numda = Vc / fre; double ra = 0.05 * numda;             //自由空间波长求出螺旋线内半径
    guodu_jiegou.Ra = ra;                  //螺旋线内半径                       
    //*********************按照螺旋线内半径的一定比例预设其它尺寸************************
    guodu_jiegou.Rb = 1.3 * ra;            //螺旋线外半径
    guodu_jiegou.Rc = 3.5 * ra;            //屏蔽壳内半径
    guodu_jiegou.Rg = 3.5 * ra;            //无翼片
	guodu_jiegou.fir = first_fir;                 //线距
    double l1 = 2 * PI * ra * vp / Vc;
    double l2 = std::pow(1 - std::pow(vp / Vc, 2), 0.5);
    guodu_jiegou.L = l1 / l2;              //螺距
    guodu_jiegou.del = 0.3 * guodu_jiegou.L;           //线宽

    return guodu_jiegou;
};

