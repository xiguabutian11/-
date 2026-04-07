#pragma once
#include <cmath>
#include "SESAN1/DATA/data_start.h"


double two_point(double data);//保留两位小数

double pre_length(double jiegou_length); //获取一个偏大的管长，jiegou.L螺距单位会在该函数里进行转换，输出为mm