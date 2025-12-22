#include"first_param.h"


LXsesan  SeSanMuBiao(double minfre, double maxfre, double voltage, double Pmax,double N)
{
	LXsesan LXtarget;

	LXtarget.fmax = GHZ_turn_HZ(maxfre);
	LXtarget.fmin = GHZ_turn_HZ(minfre);
	LXtarget.f = (LXtarget.fmax + LXtarget.fmin) / 2;
	shuchu::A1(minfre, maxfre, voltage, Pmax);
	LXtarget.vp = XiangSu::way_1(voltage, AA);
	LXtarget.I = DianLiu::way_1(Pmax, voltage, miu);
	LXtarget.kc = ZuKang::way_1(voltage, LXtarget.I, Pmax, MM)+N;

	double xiangduigongzuodaikuang = (LXtarget.fmax - LXtarget.fmin) / LXtarget.f;

	if (std::fmod(std::log2(LXtarget.fmax / LXtarget.fmin), 1.0) == 0.0) {
		std::cout << "此管子为倍频管" << std::endl;
		LXtarget.guanzi_type = 3;
	}
	else if (xiangduigongzuodaikuang < 0.1) {
		std::cout << "此管子为窄带管" << std::endl;
		LXtarget.guanzi_type = 1;
	}
	else if (xiangduigongzuodaikuang > 0.1) {
		std::cout << "此管子为宽带管" << std::endl;
		LXtarget.guanzi_type = 2;
	}

	return LXtarget;
}

LXjiegou SeSanJieGou(LXsesan LXtarget)
{
	LXjiegou guanzi1;

	guanzi1 = YuShe_jiegou::way_1(LXtarget.f, LXtarget.vp);

	renew_input(guanzi1);

	shuchu::A2(LXtarget.vp, LXtarget.I, LXtarget.kc, guanzi1.Ra, guanzi1.Rb, guanzi1.Rc, guanzi1.Rg, guanzi1.L, guanzi1.del);
	return guanzi1;
};