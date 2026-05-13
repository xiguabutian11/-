#include "XIGUA.h"


double two_point(double data)
{
	return std::round(data * 100) / 100;
}

double four_point(double data)
{
	return std::round(data * 10000) / 10000;
}

double pre_length(double jiegou_length)
{
	double L = jiegou_length;
	if(guanzi_type==1)
	{
		L = L * 1000 * 100; 
	}
	if (guanzi_type == 2)
	{
		L = L * 1000 * 200;
	}
	if (guanzi_type == 3)
	{
		L = L * 1000 * 300 ; 
	}
	return L;
}

