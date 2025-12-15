#include "formula.h"

double formula::voltageFromVp(const double& vp)
{
	if (vp >= 1) {
		return 0;
	}
	else {
		double resTemp = sqrt(1.0 / (1.0 - pow(vp, 2))) - 1;
		return pow(constpar_clight, 2) * resTemp / constpar_e_m;
	}
	
}
