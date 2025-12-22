#pragma once
#include <iostream>
#include <fstream>  
#include <vector>
#include <string>
#include <sstream> 
#include "jisuan/Jisuan.h"

std::vector<LXsesan> readTWTData(const std::string& filename);  //读取output文件中的sesan数据