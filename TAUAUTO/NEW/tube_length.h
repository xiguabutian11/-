#pragma once
#include"TAU.h"

struct lengthstruct
{ 
	double first_lentgh=0;
	double fisrt_gain=0;
	double final_length=0;
	double final_gain=0;
};

lengthstruct youhua_length(double guanzi_type, double L, double Gain, double fre, double small_pin, double V);