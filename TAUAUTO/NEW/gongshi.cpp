#include"gongshi.h"



double mag1(double V,double r,double I,double N)
{
	double A = N * round((8.3 * sqrt((I / pow(V, 1.5) * 1e6) * V) / r) / 10000 * 1000) / 1000.0;

	return A;
};

double mag2(double V, double r, double I)
{
	double B = round((35.92 * r * 0.5 / sqrt(I / pow(V, 1.5) * 1e6)) / 3 * 1000) / 1000.0;

	return B;
};