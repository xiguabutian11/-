#include "XIGUA.h"


double two_point(double data)
{
	return std::round(data * 100) / 100;
}

double pre_length(double jiegou_length)
{
	double L = jiegou_length;
	if(guanzi_type==1)
	{
		L = L * 1000 * 100; //单段管子100个波导波长
	}
	if(guanzi_type==2)
	{
	L = L * 1000 * 200 ; //双段管子200个波导波长
}
	if (guanzi_type == 3)
	{
		L = L * 1000 * 300 ; //三段管子300个波导波长
	}
	return L;
}