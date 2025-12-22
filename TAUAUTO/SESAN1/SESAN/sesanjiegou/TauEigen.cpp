#include "TauEigen.h"
#include "params.h"
#include <cmath>
#include <string> 
#include <fstream> 
#include <Windows.h>
#include <direct.h>
#include <iostream> 
#include <iomanip> 
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <thread>
#include <mutex>

using namespace std;

ifstream& seek_to_line1(ifstream& in, int line)
//将打开的文件in，定位到line行。
{
	int i;
	char buf[1024];
	in.seekg(0, ios::beg);  //定位到文件开始。
	for (i = 0; i < line; i++)
	{
		in.getline(buf, sizeof(buf));//读取行。
	}
	return in;
}

bool GetStructData1(StructParams1& strpar, string filepath)
{
	ifstream mf(filepath, ios::in);
	if (!mf.is_open())
	{
		return false;
	}
	string str, s_tmp;
	double a, b, c, g, fir, cf, L, dl, eps, ratio, pol;
	int m = 0, vrod = 3, vload = 3;
	vector<vector<double>> rod_data;
	while (getline(mf, str))
	{
		if (str.find("a - internal radius of helix") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);  // 此处可能有问题，str.find_last_of()后面应该+1，否则，a会以“，”开头
			a = stod(s_tmp);
		}
		else if (str.find("b-externa radius of helix") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			b = stod(s_tmp);
		}
		else if (str.find("L-helix pitch") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			L = stod(s_tmp);
		}
		else if (str.find("c-wall radius") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			c = stod(s_tmp);
		}
		else if (str.find("g-radius of load") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			g = stod(s_tmp);
		}
		else if (str.find("fir") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			fir = stod(s_tmp);
		}
		else if (str.find("del- helix width") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			dl = stod(s_tmp);
		}
		else if (str.find("eps") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			eps = stod(s_tmp);
		}
		else if (str.find("ratio") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			ratio = stod(s_tmp);
		}
		else if (str.find("correction factor") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			cf = stod(s_tmp);
		}
		else if (str.find("number of rods") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			vrod = stoi(s_tmp);
		}
		else if (str.find("number of loads") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			vload = stoi(s_tmp);
		}
		else if (str.find("m-number of points for description rods") != str.npos) {
			s_tmp = str.substr(str.find_last_of(",") + 1);
			m = stoi(s_tmp);
		}
		else if (str.find("coordinats of points(m*3 value)") != str.npos) {
			double x, y, z;
			for (int i = 0; i < m; i++)
			{
				mf >> x >> y >> z;
				rod_data.push_back({ x,y,z });
				if (i == 1) pol = 2 * x;
			}
		}
		else
		{
			continue;
		}


	}

	strpar.swsr = a; //螺旋线内半径
	strpar.swsro = b; //螺旋线外半径
	strpar.pitch = L; //螺旋线螺距
	strpar.helixwidth = dl; //螺旋线线宽
	strpar.c = c; //管壳内径
	strpar.g = g; //翼片内半径
	strpar.fir = fir; //翼片圆心角
	strpar.eps = eps; //夹持杆相对介电常数
	strpar.cf = cf; //修正系数
	strpar.ratio = ratio; //螺旋线相对于铜的电导率
	strpar.vload = vload; //翼片个数：
	strpar.vrod = vrod; //夹持杆个数
	strpar.rodata = rod_data; //绘制夹持杆的坐标数据，m*3矩阵；以螺旋线圆心为原点，每行为{x,y,flag}，flag表示与相邻点的链接方式，0表示直线，1表示曲线；
	strpar.pol = pol;

	return true;
}

// 检查目录是否存在
bool directoryExists(const string& path) {
	struct stat info;
	return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

// 递归创建目录
void createDirectories(const string& path) {
	size_t pos = 0;
	do {
		pos = path.find_first_of("\\/", pos + 1);
		string subdir = path.substr(0, pos);
		if (!directoryExists(subdir)) {
			if (_mkdir(subdir.c_str()) != 0) {
				/*perror(("无法创建目录: " + subdir).c_str());*/
			}
		}
	} while (pos != string::npos);
}

string WriteEigenParfile1(StructParams1& strpar, string workpath, string timeflag)
{
	double swsr_tmp, swsro_tmp, pitch_tmp, helixwidth_tmp, g_tmp, fir_tmp, c_tmp, area_tmp, ratio_tmp;
	int mpoints = 0;
	string filename = "";
	mpoints = strpar.rodata.size();
	swsr_tmp = strpar.swsr;
	swsro_tmp = strpar.swsro;
	pitch_tmp = strpar.pitch;
	helixwidth_tmp = strpar.helixwidth;
	g_tmp = strpar.g;
	fir_tmp = strpar.fir;
	c_tmp = strpar.c;
	area_tmp = strpar.area;
	ratio_tmp = strpar.ratio;
	string thread_index = timeflag;
	string dpath = "temp\\" + timeflag;
	if (workpath != "")
		dpath = workpath + "\\" + dpath;
	filename = dpath + "\\ai.dat";
	createDirectories(dpath);

	if (directoryExists(dpath)) {
		/*cout << "文件夹创建成功: " << dpath << endl;*/
	}
	else {
		/*cout << "文件夹创建失败" << endl;*/
	}

	double fmax, phimax;
	phimax = constpar_pi1;
	fmax = constpar_clight1 * phimax / (2 * constpar_pi1 * sqrt(pow(2 * constpar_pi1 * swsr_tmp, 2.0) + pow(pitch_tmp, 2.0))) / 1e6;
	//fmax = constpar_clight / (2 * pitch_tmp) / 1e6;
	fmax = ceil(fmax);
	ofstream ai(filename, ios::trunc);

	ai << "'name of variant=_______________________________',twt" << "\n";
	ai << "'ideli=_________________________________________'," << ("%d", 1) << "\n";
	ai << "'a-internal radius of helix=____________________'," << to_string(swsr_tmp) << "\n";
	ai << "'b-externa radius of helix=_____________________'," << to_string(swsro_tmp) << "\n";
	ai << "'L-helix pitch=_________________________________'," << to_string(pitch_tmp) << "\n";
	ai << "'c-wall radius =________________________________'," << to_string(c_tmp) << "\n";
	ai << "'g-radius of load=______________________________'," << to_string(g_tmp) << "\n";
	ai << "'fir=___________________________________________'," << to_string(fir_tmp) << "\n";
	ai << "'del- helix width=______________________________'," << to_string(helixwidth_tmp) << "\n";
	ai << "'eps=___________________________________________'," << to_string(strpar.eps) << "\n";
	ai << "'Rpvn/a=________________________________________'," << to_string(0) << "\n";
	ai << "'Rpna/a=________________________________________'," << to_string(0.5) << "\n";
	ai << "'ratio=_________________________________________'," << to_string(ratio_tmp) << "\n";
	ai << "'correction factor______________________________'," << to_string(strpar.cf) << "\n";
	ai << "'fmin=__________________________________________'," << to_string(com_fmin) << "\n";
	ai << "'fmax=__________________________________________'," << to_string(fmax) << "\n";
	ai << "'hf=____________________________________________'," << to_string(hf) << "\n";
	if (mpoints != 0)
	{
		ai << "'v(number of rods)=_____________________________'," << to_string(strpar.vrod) << "\n";
		ai << "'if v=0,dielectric of tube=_____________________'," << to_string(0) << "\n";
		ai << "'v1(number of loads)=___________________________'," << to_string(strpar.vload) << "\n";
		ai << "'if v>0:'" << "\n";
		ai << "'m-number of points for description rods=_______'," << to_string(mpoints) << "\n";
		ai << "'coordinats of points(m*3 value)_________________'," << "\n";
		for (int i = 0; i < mpoints; i++)
		{
			ai << "     " << to_string(strpar.rodata[i][0])
				<< "     " << to_string(strpar.rodata[i][1])
				<< "     " << strpar.rodata[i][2]
				<< "\n";
		}
	}
	else
	{
		double s1 = sin(area_tmp / 12);
		double c1 = cos(area_tmp / 12);
		double s2 = sin(area_tmp / 6);
		double c2 = cos(area_tmp / 6);
		double r = 0.5 * swsro_tmp + 0.5 * c_tmp;
		ai << "'v(number of rods)=_____________________________'," << to_string(3) << "\n";
		ai << "'if v=0,dielectric of tube=_____________________'," << to_string(0) << "\n";
		ai << "'v1(number of loads)=___________________________'," << to_string(strpar.vload) << "\n";
		ai << "'if v>0:'" << "\n";
		ai << "'m-number of points for description rods=_______'," << to_string(7) << "\n";
		ai << "'coordinats of points(m*3 value)_________________'," << "\n";
		ai << "     " << to_string(0) << "     " << to_string(swsro_tmp) << "   " << (0) << "\n";
		ai << "     " << to_string(swsro_tmp * s1) << "     " << to_string(swsro_tmp * c1) << "   " << (1) << "\n";
		ai << "     " << to_string(swsro_tmp * s2) << "     " << to_string(swsro_tmp * c2) << "   " << (0) << "\n";
		ai << "     " << to_string(r * s2) << "     " << to_string(r * c2) << "   " << (0) << "\n";
		ai << "     " << to_string(c_tmp * s2) << "     " << to_string(c_tmp * c2) << "   " << (0) << "\n";
		ai << "     " << to_string(c_tmp * s1) << "     " << to_string(c_tmp * c1) << "   " << (1) << "\n";
		ai << "     " << to_string(0) << "     " << to_string(c_tmp) << "   " << (0) << "\n";
	}

	ai.close();
	return filename;
}

bool GetEigenResult1(DispersionParams1& dispar, string path)
{
	ifstream mf(path, ios::in);
	string time, s_tmp;
	bool flag = false;
	if (!mf.is_open())
	{
		MessageBoxA(NULL, "未找到文件！", 0, 0);
		return false;
	}
	getline(mf, time);
	time = time.substr(0, time.find_first_of("/"));
	time = time.substr(time.find_first_not_of(" "));
	while (getline(mf, s_tmp))
	{
		if (s_tmp.find(time) != s_tmp.npos)
		{
			flag = true;
			break;
		}
	}
	mf.close();
	if (flag == false)
	{
		MessageBoxA(NULL, "文件格式错误！", 0, 0);
		return false;
	}
	mf = ifstream(path, ios::in);
	double f, vp, kc, alfa, phi, pitch;
	int points = 0, line = 0;
	vector<double>vec_f;
	vector<double>vec_phi;
	vector<double>vec_vp;
	vector<double>vec_kc;
	vector<double>vec_alfa;
	getline(mf, time);
	line++;
	while (getline(mf, s_tmp))
	{
		line++;
		if (s_tmp.find("pitch=") != s_tmp.npos)
		{

			string strpitch = s_tmp.substr(s_tmp.find("=") + 1);
			pitch = stod(strpitch);
		}
		else if (s_tmp.find("leng.wave") != s_tmp.npos)
		{
			getline(mf, s_tmp);
			line++;
			while (s_tmp != "")
			{
				getline(mf, s_tmp);
				points++;
			}
			seek_to_line1(mf, line - 1);
			for (int i = 0; i < points; i++)
			{
				double lw, ka, g0a, v_1, v1, epseq;
				mf >> f >> lw >> ka >> g0a >> v_1 >> vp >> v1 >> epseq;
				phi = f * pitch * vp * 1.2008307760;
				vec_f.push_back(f);
				vec_phi.push_back(phi);
				vec_vp.push_back(1 / vp);
			}
			getline(mf, s_tmp);
		}
		else if (s_tmp.find("Interaction Impedance") != s_tmp.npos)
		{
			double kc_1, kc1;
			getline(mf, s_tmp);
			for (int i = 0; i < points; i++)
			{
				mf >> f >> kc_1 >> kc >> kc1;
				vec_kc.push_back(kc);
			}
			getline(mf, s_tmp);
		}
		else if (s_tmp.find("Damping per meter") != s_tmp.npos)
		{
			getline(mf, s_tmp);
			for (int i = 0; i < points; i++)
			{
				getline(mf, s_tmp);
				string strf, stralfa;
				while (s_tmp.find("D") != s_tmp.npos)
				{
					s_tmp.replace(s_tmp.find("D"), 1, "E");
				}
				s_tmp = s_tmp.substr(s_tmp.find_first_not_of(" "));
				strf = s_tmp.substr(0, s_tmp.find_first_of(" "));
				stralfa = s_tmp.substr(s_tmp.find_first_of(" ") + 1);
				f = stod(strf);
				alfa = stod(stralfa);
				vec_alfa.push_back(alfa);
			}
			getline(mf, s_tmp);
		}
		else
		{
			continue;
		}
	}
	mf.close();
	dispar.data_f = vec_f;
	dispar.data_kc = vec_kc;
	dispar.data_vp = vec_vp;
	dispar.data_phi = vec_phi;
	dispar.data_alfa = vec_alfa;
	return true;
}

void WriteDataBW1(DispersionParams1& dis, string filepath)
{

	ofstream bw(filepath, ios::trunc);
	bw << "f  vp  phi  kc  alfa" << endl;
	for (int i = 0; i < dis.backwave_phi.size(); i++)
	{
		bw << dis.backwave_f[i] << "  " << dis.backwave_vp[i] << "  " << dis.backwave_phi[i] / 180 << "  " << dis.backwave_kc[i] << "  " << dis.backwave_alfa[i] << endl;
	}
	bw.close();
}

void WriteData1(DispersionParams1& dis, string filepath)
{
	/*cout << "进入 WriteData 函数..." << endl;*/
	ofstream dat(filepath, ios::trunc);
	if (!dat.is_open()) {
		cerr << "无法打开文件进行写入: " << filepath << endl;
		return; // 或返回错误状态
	}
	dat << "f  vp  phi  kc  alfa" << endl;
	for (int i = 0; i < dis.data_f.size(); i++)
	{
		double current_f = dis.data_f[i];
		int integer_part = static_cast<int>(current_f);

		// 允许1e-6的精度误差
		const double epsilon = 1e-6;
		/*if (current_f - integer_part < epsilon)*/ {
			dat << dis.data_f[i] << "  " << dis.data_vp[i] << "  " << dis.data_phi[i] / 180 << "  " << dis.data_kc[i] << "  " << dis.data_alfa[i] << endl;
		}
	}
	dat.close();
}

bool EigenDataCheck1(StructParams1& strpar)
{
	if (strpar.swsr < 0.1 || strpar.swsr> 60)
	{
		MessageBoxA(NULL, "螺旋线内径不合理，小于0.1mm或大于10mm，注意输入数据单位！", 0, 0);
		return 0;
	}
	if (strpar.swsro - strpar.swsr < 0.1)
	{
		MessageBoxA(NULL, "螺旋线厚度过小，小于0.1mm！", 0, 0);
		return 0;
	}
	if (strpar.swsro / strpar.swsr > 10)
	{
		MessageBoxA(NULL, "螺旋线内径与外径可能不在一个数量级！", 0, 0);
		return 0;
	}
	if (strpar.pitch < 0.1 || strpar.pitch> 60)
	{
		MessageBoxA(NULL, "螺旋线螺距不合理，小于0.1mm或大于10mm，注意输入数据单位！", 0, 0);
		return 0;
	}
	if (strpar.helixwidth < 0)
	{
		MessageBoxA(NULL, "螺旋线线宽小于0！", 0, 0);
		return 0;
	}
	if (strpar.pitch / strpar.swsr >= 6.28 || strpar.pitch / strpar.swsr < 0.314)
	{
		MessageBoxA(NULL, "螺旋线内半径与螺距数值关系异常，可能使用不同单位！", 0, 0);
		return 0;
	}
	if (strpar.c < strpar.swsro || strpar.c >20 * strpar.swsro)
	{
		MessageBoxA(NULL, "管壳内径数值异常，可能使用不同单位！", 0, 0);
		return 0;
	}
	if (strpar.g < strpar.swsro || strpar.g >20 * strpar.swsro)
	{
		MessageBoxA(NULL, "翼片内径数值异常，可能使用不同单位！", 0, 0);
		return 0;
	}
	if (strpar.c < strpar.g)
	{
		MessageBoxA(NULL, "管壳内径小于翼片内径！", 0, 0);
		return 0;
	}
	if (strpar.eps < 1)
	{
		MessageBoxA(NULL, "夹持杆相对介电常数小于1！", 0, 0);
		return 0;
	}
	if (strpar.fir < 0 || strpar.fir> 120)
	{
		MessageBoxA(NULL, "翼片夹角为负值或总和大于360°！", 0, 0);
		return 0;
	}
	if (strpar.area < 0.15 || strpar.area >= 1)
	{
		if (strpar.rodata.size() == 0)
		{
			MessageBoxA(NULL, "夹持杆空间占比不合理！", 0, 0);
			return 0;
		}
	}
	return 1;
}

bool EigenFileCheck1(string filepath)
{
	StructParams1 strpar;
	bool flag = GetStructData1(strpar, filepath);
	if (flag)
	{
		return EigenDataCheck1(strpar);
	}
	return false;
}

bool EigenCalculate1(string filepath, string workpath)
{
	if (workpath == "")
	{
		char buffer[MAX_PATH];
		char* flagbuffer = _getcwd(buffer, MAX_PATH);
		workpath = string(buffer);
		string strtmppath = workpath + "\\temp";
		int a = _mkdir(strtmppath.c_str());
	}
	string spath = filepath.substr(0, filepath.find_last_of("\\"));
	string aipath = workpath + "\\exe\\ai.exe";
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = aipath.c_str();
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = spath.c_str();
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteExA(&ShExecInfo);
	if (ShExecInfo.hProcess)
	{
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}
	else
	{
		MessageBoxExA(0, "ai.exe调用失败，请检查程序是否存在", "", 0, 0);
		return false;
	}
	string respath = spath + "\\ai.rez";;
	ifstream mf(respath, ios::in);
	if (!mf.is_open())
	{
		return false;
	}
	string time, s_tmp;
	getline(mf, time);
	time = time.substr(0, time.find_first_of("/"));
	time = time.substr(time.find_first_not_of(" "));
	bool flag = false;
	while (getline(mf, s_tmp))
	{
		if (s_tmp.find(time) != s_tmp.npos)
		{
			flag = true;
			break;
		}
	}
	mf.close();
	if (flag == false)
	{
		MessageBoxA(NULL, "计算异常终止，检查输入文件是否正确！", 0, 0);
		return false;
	}
	int flagpath = _chdir(workpath.c_str());
	return true;
}

bool Eigen_file1(DispersionParams1& dispar, string filepath, string workpath)
{
	bool flag = EigenCalculate1(filepath, workpath);
	//修改地址为结果文件地址
	string respath = filepath.replace(filepath.find(".dat"), 4, ".rez");
	if (flag)
	{
		//读取结果文件的色散数据
		return GetEigenResult1(dispar, respath);
	}
	return false;
}

bool Eigen_data1(DispersionParams1& dispar, StructParams1& strpar, string workpath)
{
	string strpath = WriteEigenParfile1(strpar);
	//对地址为strpath的ai.dat文件调用tau进行本征计算
	bool flag = EigenCalculate1(strpath, workpath);
	//修改地址为结果文件地址
	string respath = strpath.replace(strpath.find(".dat"), 4, ".rez");
	if (flag)
	{
		//读取结果文件的色散数据
		return GetEigenResult1(dispar, respath);
	}
	return false;
}

bool GetDataBW1(DispersionParams1& dis)
{
	//检测是否存在相位数据，无数据则返回布尔值：假
	if (dis.data_phi.size() == 0) {
		cout << "no data" << endl;
		return false;
	}
	int i = 0;
	//判断是否存在大于180°的相位，如有则定位该数据在数组中的位置
	while (i < dis.data_phi.size())
	{
		if (dis.data_phi[i] >= 180)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	//判断数据定位点是否有效，无效则返回布尔值：假
	if (i >= dis.data_phi.size()) {
		cout << "no data more than pi" << endl;
		return false;
	}
	//定义临时变量用以存储转化数据
	vector<double>wb_f;
	vector<double>wb_phi;
	vector<double>wb_vp;
	vector<double>wb_kc;
	vector<double>wb_alfa;
	double phi, phi1, kc, kc1, vp, vp1;
	//返波数据提取与转化；
	for (int j = i; j < dis.data_phi.size(); j++)
	{
		phi = dis.data_phi[j];//从结构体获取前向波计算数据
		kc = dis.data_kc[j];//从结构体获取前向波计算数据
		vp = dis.data_vp[j];//从结构体获取前向波计算数据
		phi1 = 360 - phi;//返波相位，为前向波相对pi点的对称值
		kc1 = kc * pow(phi / phi1, 2.0);//返波耦合阻抗计算
		vp1 = vp * phi / phi1;//返波相速计算
		wb_f.push_back(dis.data_f[j]);//返波频带与前向波频点一致，将数据填入临时变量
		wb_alfa.push_back(dis.data_alfa[j]);//返波衰减与前向波一致，将数据填入临时变量
		wb_phi.push_back(phi1); //将数据填入临时变量
		wb_kc.push_back(kc1);//将数据填入临时变量
		wb_vp.push_back(vp1);//将数据填入临时变量
	}
	//将获取的返波数据回传给结构体对应变量，并返回布尔值：真
	dis.backwave_f = wb_f;
	dis.backwave_alfa = wb_alfa;
	dis.backwave_phi = wb_phi;
	dis.backwave_kc = wb_kc;
	dis.backwave_vp = wb_vp;
	return true;
}

