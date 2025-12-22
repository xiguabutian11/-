#include"change_units.h"

double GHZ_turn_HZ(double fre)
{
	return fre * 1e9;
};

double HZ_turn_GHZ(double fre)
{
	return fre / 1e9;
};

double m_mm(double R)
{
	return R * 1000;
};

double m_cm(double R)
{
	return R * 100;
};