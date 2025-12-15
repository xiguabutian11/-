#include "tauLib/headers/TauInteraction.h"


//***************数据提取与导出*************
string WriteInterParfile(const InteractionParams& interpar, string workpath, string timeflag)
{
    char buffer[MAX_PATH];
    char* flagbuffer = _getcwd(buffer, MAX_PATH);  //获取当前项目地址
    int num_of_f = interpar.data_f.size();
    int num_of_mag = interpar.data_mag.size();
    // int num_of_beam = interpar.beamdata.beamrate.size();
    if (num_of_f == 0)return "";
    if (num_of_mag == 0)return "";
    if (timeflag == "")timeflag = currentDateTime();
    string dpath = "V" + dtos(interpar.V) + "I" + dtos(interpar.I);
    string parfiledirpath = "temp/" + timeflag;
    if (workpath == "") workpath = string(buffer);
    parfiledirpath = workpath + "/" + parfiledirpath+ "/" + dpath;
    std::filesystem::create_directories(parfiledirpath);
    //int b = _mkdir(parfiledirpath.c_str());
    //parfiledirpath = parfiledirpath + "/" + dpath;
    //b = _mkdir(parfiledirpath.c_str());
    ofstream hp(parfiledirpath + "/HFA.par", ios::trunc);
    HfaPar hpar;
    string strhfapar = hpar.writefile(interpar);
    hp << strhfapar;
    hp.close();
    return parfiledirpath + "/HFA.par";
}
//string WriteInterParfile_old(InteractionParams& interpar, string workpath, string timeflag)
//{
//    char buffer[MAX_PATH];
//    char* flagbuffer = _getcwd(buffer, MAX_PATH);
//    int num_of_f = interpar.data_f.size();
//    int num_of_sws = interpar.data_swsbeg.size();
//    int num_of_loss = interpar.data_loss.size();
//    int num_of_drft = interpar.data_drft.size();
//    int num_of_mag = interpar.data_mag.size();
//    int num_of_beam = interpar.beamdata.beamrate.size();
//    if (num_of_f == 0)return "";
//    if (num_of_mag == 0)return "";
//    if (timeflag == "")timeflag = currentDateTime();
//    string dpath = "V" + dtos(interpar.V) + "I" + dtos(interpar.I);
//    string parfiledirpath = "temp/" + timeflag;
//    if (workpath == "") workpath = string(buffer);
//    parfiledirpath = workpath + "/" + parfiledirpath + "/" + dpath;
//    std::filesystem::create_directories(parfiledirpath);
//    //int b = _mkdir(parfiledirpath.c_str());
//    //parfiledirpath = parfiledirpath ;
//    //b = _mkdir(parfiledirpath.c_str());
//    ofstream hp(parfiledirpath + "/HFA.par", ios::trunc);
//    HfaPar hpar;
//    string strhfapar = hpar.writefile(interpar);
//    hp << strhfapar;
//    hp.close();
//    return parfiledirpath + "/HFA.par";
//    hp << "' twt  ',\n";
//    hp << "'CONTROL PARAMETERS'," << "\n";
//    hp << "'nom number of variant-------------------------------------',1" << "\n";
//    hp << "'kv number of changes FREQUENCY OF A SIGNAL no more than6 '," << num_of_f << "\n";
//    hp << "'IUST Sites of stability____________________________________',0" << "\n";
//    hp << "'IGAR AMOUNT of HARMONICSES-----------------------------------'," << interpar.harmonicses << "\n";
//    hp << "'Ikanal---------------- ------------------------------------',0" << "\n";
//    hp << "'Ipe Tag of the full output------------------------------ ',0" << "\n";
//    hp << "'Igun- DATA INPUT FROM THE GUN ---------------------------------',0" << "\n";
//    hp << "'IKOL- DATA OUTPUT FOR THE COLLECTOR--------------------',0" << "\n";
//    hp << "'M-  number of electrons no more than 192--------------------------',96" << "\n";
//    hp << "'IbN number of changes of SWS-------'," << num_of_sws << "\n";
//    hp << "'IdN number of changes of  loss  -const  no more than 50-----------'," << num_of_loss << "\n";
//    hp << "'IDR=(n-number of drifts, 0 - no drifts)-----------------------'," << num_of_drft << "\n";
//    hp << "'IMP number of changes of magnetic inductance no more than 50-----'," << num_of_mag << "\n";
//    hp << "' Point WRITE' ," << interpar.tubelength / 1000.0 << "\n";
//    hp << "' The array  of  wale length  ' Input signal   Pinp ------------'," << "\n";
//    for (int inp = 0; inp < num_of_f; inp++)
//    {
//        hp << interpar.data_f[inp].freq << "   " << interpar.data_f[inp].pin << "   " << interpar.data_f[inp].pin1 << "   " << interpar.data_f[inp].pin2 << "\n";
//    }
//    hp << "'ZZZ is coordinate of the beam entrance tj the collectjr'," << interpar.tubelength / 1000.0 << "\n";
//    hp << "'xooh Coordinate of the input of the HF power----------------',0" << "\n";
//    hp << "'kanal-----------------------------------------------------'," << "\n";
//    hp << "'Zo coordinate of the nearest to entrance of SWS zero from cathode side',0.0 " << "\n";
//    hp << "'IO - beam current-------------------------------------'," << interpar.I << "\n";
//    hp << "'UO-accelerating voltage-----------------------', " << interpar.V << "\n";
//    hp << "'NC- amount of layers---------------------------', " << num_of_beam << "\n";
//    hp << "'R1 -an upper radius of a boundary of the 1 layer-----------', " << "\n";
//    for (int i = 0; i < num_of_beam; i++)
//    {
//        double beamr = interpar.beamdata.beamrate[i] * (interpar.beamdata.Rpna * interpar.beamdata.Rpna - interpar.beamdata.Rpvn * interpar.beamdata.Rpvn) + interpar.beamdata.Rpvn * interpar.beamdata.Rpvn;
//        hp << dtos(pow(beamr, 0.5) / 1000.0) << "\n";
//    }
//    hp << "'An averaged radial velocity', " << "\n";
//    hp << "0.0, 0.0, 0.0" << "\n";
//    hp << "'A   radius of the tunnel----------------- '," << dtos(interpar.beamdata.swsr / 1000.0) << "\n";
//    hp << "'ZB(I) beginning of the regions with DECELERATION-const, I =IBN'," << "\n";
//    if (num_of_sws > 1)
//    {
//        for (int i = 0; i < interpar.data_swsbeg.size(); i++)
//        {
//            hp << interpar.data_swsbeg[i] / 1000.0 << endl;
//        }
//    }
//    else
//    {
//        hp << "0.0" << "\n";
//    }
//    hp << " 'ZDR(I) beginning of the regions with FIELD=0, I =IDR'," << "\n";
//    if (num_of_drft)
//    {
//        for (int i = 0; i < num_of_drft; i++)
//        {
//            hp << interpar.data_drft[i].begin / 1000.0 << " " << interpar.data_drft[i].end / 1000.0 << endl;
//        }
//    }
//    hp << "'IMP number of changes of constant of a magnetic inductance50'," << "\n";
//    if (num_of_mag)
//    {
//        for (int i = 0; i < num_of_mag; i++)
//        {
//
//            hp << i + 1 << "     " << interpar.data_mag[i].begin / 1000.0 << "     " << interpar.data_mag[i].period / 1000.0 << "     ";
//            hp << interpar.data_mag[i].m0 << "     " << interpar.data_mag[i].m1 << "     " << interpar.data_mag[i].m2 << "     " << endl;
//        }
//    }
//    hp << "'LN1(I)  SPECIFIC  DAMPING 1 HARMONIC  I=1+IDN---'," << "\n";
//    if (num_of_loss)
//    {
//        for (int i = 0; i < num_of_loss; i++)
//        {
//
//            hp << interpar.data_loss[i].type << "     " << interpar.data_loss[i].begin / 1000.0 << "     " << interpar.data_loss[i].end / 1000.0 << "     ";
//            for (int j = 0; j < interpar.data_f.size(); j++)
//            {
//                if (j < interpar.data_loss[i].dataloss.size()) {
//                    hp << interpar.data_loss[i].dataloss[j] << "     ";
//                }
//                else {
//                    if (interpar.data_loss[i].dataloss.size() != 0) {
//                        hp << interpar.data_loss[i].dataloss.back() << "     ";
//                    }
//                    else {
//                        hp << 500 << "     ";
//                    }
//                } 
//            }
//            hp << endl;
//        }
//    }
//    hp << "'H(length of the step of the calculation)-------------------'," << dtos(interpar.interstep / 1000.0) << "\n";
//    hp << "'Xend(length of the tube)=----------------------------------'," << dtos(interpar.tubelength / 1000.0) << "\n";
//    hp << "' Sites of stability***********************'," << "\n";
//    hp << "'*********************************************end'" << "\n";
//    hp.close();
//    return parfiledirpath + "/HFA.par";
//}
string WriteInterVesh(const InteractionParams& interpar, const vector<DispersionParams>& dispars, string workpath, string timeflag)
{
    int num_of_f = interpar.data_f.size();
    if (num_of_f == 0)return"";
    int num_of_sws = interpar.data_swsbeg.size();
    if (num_of_sws == 0 || num_of_sws != dispars.size())return"";
    vector<vector<vector<double>>>DataVp(num_of_sws, vector<vector<double>>(num_of_f, vector<double>(3, 1)));
    vector<vector<vector<double>>>DataKc(num_of_sws, vector<vector<double>>(num_of_f, vector<double>(3, 0)));
    vector<vector<vector<double>>>DataAlfa(num_of_sws, vector<vector<double>>(num_of_f, vector<double>(3, 0)));
    for (int k = 0; k < num_of_sws; k++)
    {
        int l = dispars[k].data_f.size();
        if (l < 1)return"";
        double df = dispars[k].data_f[l - 1] - dispars[k].data_f[0];
        df = df / (l - 1);
        for (int h = 0; h < 3; h++)
        {
            for (int j = 0; j < num_of_f; j++)
            {
                int step = int(((h + 1) * interpar.data_f[j].freq - dispars[k].data_f[0]) / df);
                if (step == ((h + 1) * interpar.data_f[j].freq - dispars[k].data_f[0]) / df && step < l && step >= 0) {
                    DataVp[k][j][h] = dispars[k].data_vp[step];
                    DataKc[k][j][h] = dispars[k].data_kc[step];
                    DataAlfa[k][j][h] = dispars[k].data_alfa[step];
                }
                else if (step != ((h + 1) * interpar.data_f[j].freq - dispars[k].data_f[0]) / df && step >= 0 && step < l - 1)
                {
                    double x1 = ((h + 1) * interpar.data_f[j].freq - dispars[k].data_f[0]) / df - step;
                    double x2 = step + 1 - ((h + 1) * interpar.data_f[j].freq - dispars[k].data_f[0]) / df;
                    double vp = x1 * dispars[k].data_vp[step + 1] + x2 * dispars[k].data_vp[step];
                    double kc = x1 * dispars[k].data_kc[step + 1] + x2 * dispars[k].data_kc[step];
                    double alfa = x1 * dispars[k].data_alfa[step + 1] + x2 * dispars[k].data_alfa[step];
                    DataVp[k][j][h] = vp;
                    DataKc[k][j][h] = kc;
                    DataAlfa[k][j][h] = alfa;
                }
                else
                {
                    continue;
                }

            }
        }
    }
    if (timeflag == "")timeflag = currentDateTime();
    string parfiledirpath = "temp/" + timeflag;
    char buffer[MAX_PATH];
    char* flagbuffer = _getcwd(buffer, MAX_PATH);
    if (workpath == "")workpath = string(buffer);
    parfiledirpath = workpath + "/" + parfiledirpath;
    std::filesystem::create_directories(parfiledirpath);
    ofstream VESH(parfiledirpath + "/SVEH.dat", ios::trunc);
    VESH << "twt" << "\n";
    VESH << "'amount of kinds='," << num_of_sws << "\n";
    VESH << "'    a       b        L        c        g        fir       del      eps'" << "\n";
    for (int k = 0; k < num_of_sws; k++)
    {
        VESH << " #\n";
    }
    VESH << num_of_f << "\n";
    for (int i = 0; i < 3; i++)
    {
        VESH << i + 1 << " harmonic" << "\n";
        for (int j = 0; j < num_of_f; j++)
        {
            VESH << i * interpar.data_f[j].freq + interpar.data_f[j].freq << "   ";
        }
        VESH << "\n";
        for (int k = 0; k < num_of_sws; k++)
        {
            for (int j = 0; j < num_of_f; j++)
            {
                VESH << 1 / DataVp[k][j][i] << "   ";
            }
            VESH << "\n";
        }
        VESH << "          -----------------------------\n";
        for (int k = 0; k < num_of_sws; k++)
        {
            for (int j = 0; j < num_of_f; j++)
            {
                VESH << DataKc[k][j][i] << "   ";
            }
            VESH << "\n";
        }
        VESH << "          -----------------------------\n";
        for (int k = 0; k < num_of_sws; k++)
        {
            for (int j = 0; j < num_of_f; j++)
            {
                VESH << DataAlfa[k][j][i] << "   ";
            }
            VESH << "\n";
        }
    }
    VESH << "--------------------------------------------------\n";
    VESH.close();
    return parfiledirpath + "/SVEH.dat";
}
bool ReadHfa(string filepath)
{
    ifstream mf(filepath);
    if (!mf.is_open())
    {
        cout << "未成功打开文件" << filepath << endl;
        return false;
    }
    string str;
    while (getline(mf, str)) {
        if (str.find("time of the calculation end") != str.npos)
        {
            mf.close();
            return true;
        }

    }
    mf.close();
    return false;
}
bool ReadHfagr(InteractionRes& interres, string filepath)
{
    int begin, points, f, N, harm, num_m, num_sws, num_loss, num_drft;
    double radius_e;
    int line = 5;
    ifstream in(filepath, ios::in);
    if (!in)
    {
        cout << "found no hfagr.par file" << endl;
        return false;
    }
    string str;
    double x1 = 0, x2 = 0;
    double tubelength = 0;

    //按格式获取基本参数
    getline(in, str);
    in >> x1 >> f >> x1 >> harm >> x1 >> x1;
    double* freq = arry1(f, 0);
    double* pin = arry1(f, 0);
    double* pso = arry1(f, 0);
    double* pso_len = arry1(f, 0);;
    in >> x1 >> x1 >> N >> num_m;
    in >> num_sws >> num_loss >> num_drft;
    in >> tubelength;
    for (int i = 0; i < f; i++)
    {
        in >> x1 >> x2;
        freq[i] = 0.3 / x1;
        pin[i] = x2;
        for (int j = 1; j < harm; j++)
        {
            in >> x1;
        }
    }
    line = 5 + f * ceil((1 + harm) / 3.0) + 3;
    seek_to_line(in, line);
    in >> radius_e;
    radius_e = radius_e * 1000;
    line = line + 2 + ceil(num_sws / 3.0) + 2 * num_m + ceil(num_loss / 3.0);
    if (num_loss == 0)line += 1;
    seek_to_line(in, line);
    in >> points;
    double*** tra = arry3(f, points, N, 0);
    double*** phase = arry3(f, points, N, 0);
    double*** e0 = arry3(2 * harm, points, f, 0);
    double*** e1 = arry3(2 * harm, points, f, 0);;
    vector<double>length(points);
    vector<double>mag(points);
    double** gain_end = arry2(harm, f, 0);
    double** eff_end = arry2(harm, f, 0);
    vector<vector<vector<double>>> gain(f, vector<vector<double>>(harm, vector<double>(points)));
    vector<vector<vector<double>>> eff(f, vector<vector<double>>(harm, vector<double>(points)));
    vector<vector<vector<double>>> pout(f, vector<vector<double>>(harm, vector<double>(points)));
    vector<vector<double>>current_settle(f, vector<double>(points));
    vector<vector<double>>speed(f, vector<double>(points));
    //读取管长节点值
    for (int i = 0; i < points; i++)
    {
        in >> length[i];
    }
    //读取输出位置各频点（各谐波下）增益
    for (int i = 0; i < harm; i++)
    {
        for (int j = 0; j < f; j++)
        {
            in >> gain_end[i][j];
        }
    }
    //读取输出位置各频点（各谐波下）电子效率
    for (int i = 0; i < harm; i++)
    {
        for (int j = 0; j < f; j++)
        {
            in >> eff_end[i][j];
        }
    }

    //读取增益随管长变化数据
    for (int i = 0; i < harm; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < f; k++)
            {
                in >> gain[k][i][j];
            }
        }
    }

    //读取电子效率随管长变化数据
    for (int i = 0; i < harm; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < f; k++)
            {
                in >> eff[k][i][j];
            }
        }
    }

    //读取各频点流通率随管长数据
    for (int i = 0; i < points; i++)
    {
        for (int j = 0; j < f; j++)

        {
            in >> current_settle[j][i];
        }
    }

    //读取各频点平均电子速度随管长数据
    for (int i = 0; i < points; i++)
    {
        for (int j = 0; j < f; j++)

        {
            in >> speed[j][i];
        }
    }
    //读取磁场随管长数据
    for (int i = 0; i < points; i++)
    {
        in >> mag[i];
    }
    //读取1号未知数据组
    for (int i = 0; i < 2 * harm; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < f; k++)
            {
                in >> e0[i][j][k];
            }
        }
    }
    //读取2号未知数据组
    for (int i = 0; i < 2 * harm; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < f; k++)
            {
                in >> e1[i][j][k];
            }
        }
    }
    //读取各频点下电子轨迹的径向坐标数据（相对最外围电子半径的相对值）
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < N; k++)
            {
                in >> tra[i][j][k];
            }
        }
    }
    //读取各频点下电子相位数据
    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < points; j++)
        {
            for (int k = 0; k < N; k++)
            {
                in >> phase[i][j][k];
            }
        }
    }
    in.close();

    for (int i = 0; i < f; i++)
    {
        for (int j = 0; j < points; j++)
        {
            double poutdata = pin[i] * pow(10, 0.1 * gain[i][0][j]);
            if (gain[i][0][j] == 0 && j > 0)
            {
                pout[i][0][j] = pout[i][0][j - 1];
                for (int k = 1; k < harm; k++)
                {
                    pout[i][k][j] = pout[i][k][j - 1];
                }
            }
            else
            {
                pout[i][0][j] = poutdata;
                for (int k = 1; k < harm; k++)
                {
                    pout[i][k][j] = poutdata * pow(10, 0.1 * gain[i][k][j]);
                }
            }

            if (poutdata > pso[i])
            {
                pso[i] = poutdata;
                pso_len[i] = length[j];
            }

        }
    }
    FreqRes fres;
    for (int j = 0; j < f; j++)
    {
        fres.freq = freq[j];
        fres.Pso = pso[j];
        fres.len_pso = pso_len[j];
        fres.FlowRate = current_settle[j];
        fres.speed = speed[j];
        fres.Gain = gain[j];
        fres.eff = eff[j];
        fres.Pout = pout[j];

        fres.tra.resize(points, vector<double>(N));
        for (int p_idx = 0; p_idx < points; ++p_idx)
        {
            for (int n_idx = 0; n_idx < N; ++n_idx)
            {
                fres.tra[p_idx][n_idx] = tra[j][p_idx][n_idx];
            }
        }

        interres.res.push_back(fres);
    }
    interres.tube = length;
    interres.mag = mag;
    return true;
}
bool GetInterResult(InteractionRes& interres, string filepath)
{
    string graphpath = filepath.substr(0, filepath.find_last_of("/"));
    graphpath = graphpath + "/hfagr.par";
    bool blpar = ReadHfagr(interres, graphpath);
    return  blpar;
}
//**************互作用计算*********************

bool executeWithCreateProcess(const std::string& exePath, const std::string& parameters, const std::string& workDir,DWORD timeOut)
{
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = { 0 };

    std::string cmdLine = exePath + " " + parameters;
    char cmdBuffer[MAX_PATH];
    strncpy_s(cmdBuffer, cmdLine.c_str(), MAX_PATH);

    if (!CreateProcessA(
            NULL, cmdBuffer, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, workDir.c_str(),
            &si, &pi)) {
        return false;
    }
    DWORD timeConsume = WaitForSingleObject(pi.hProcess, timeOut);
    if (timeConsume == WAIT_TIMEOUT) {
        // 如果超时，强制终止进程
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

bool InterCalculate(string disfilepath, string intparfilepath, string workpath)
{
    if (workpath.empty()) {
        workpath = std::filesystem::current_path().string();
    }
    string hfapath = workpath + "/exe/hfa_ac.exe";
    string interworkpath = intparfilepath.substr(0, intparfilepath.find_last_of("/"));
    string cmdline = workpath + "/exe/hfa.exe" + " " + interworkpath;
    cout << interworkpath << endl;
    CopyFileA(disfilepath.c_str(), (interworkpath + "/SVEH.dat").c_str(), FALSE);
    //SHELLEXECUTEINFOA ShExecInfo = { 0 };
    //ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    //ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    //ShExecInfo.hwnd = NULL;
    //ShExecInfo.lpVerb = NULL;
    //ShExecInfo.lpFile = hfapath.c_str();
    //ShExecInfo.lpParameters = cmdline.c_str();
    //ShExecInfo.lpDirectory = interworkpath.c_str();
    //ShExecInfo.nShow = SW_HIDE;
    //ShExecInfo.hInstApp = NULL;
    //ShellExecuteExA(&ShExecInfo);
    /*if (ShExecInfo.hProcess)
    {
        WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
    }
    else
    {
        MessageBoxExA(0, "hfa(AutoClose).exe调用失败，请检查程序是否存在", "", 0, 0);
        return false;
    }*/

    auto& it = ctlData::getInstance().dftValue;
    unsigned int tryTimes =it.restartTime;
    unsigned int timeOut = it.timeOut;
    for (int attempt = 1; attempt <= tryTimes; ++attempt) {
        if (executeWithCreateProcess(hfapath, cmdline, interworkpath, timeOut* attempt)) {
            cout << std::this_thread::get_id() << " " << attempt << endl;
            string interrespath = interworkpath + "/hfa.rez";
            return ReadHfa(interrespath);
        }
    }
    return false;
    //if (!executeWithCreateProcess(hfapath, cmdline, interworkpath)) {
    //    MessageBoxExA(0, "hfa(AutoClose).exe调用失败，请检查程序是否存在", "", 0, 0);
    //    return false;
    //}
    //int flagpath = _chdir(buffer);
    
}
bool Interaction_data(const InteractionParams& interpar, const vector<DispersionParams>& dispars, InteractionRes& interres, string workpath,size_t threadID)
{
    char buffer[MAX_PATH];
    char* flagbuffer = _getcwd(buffer, MAX_PATH);
    if (workpath == "")workpath = string(buffer);
    //cout << "workPath: " << workpath << endl;
    string tf = currentDateTime()+"_" + to_string(threadID);
    string strvesh = WriteInterVesh(interpar, dispars, workpath, tf);
    string strhfa = WriteInterParfile(interpar, workpath, tf);
    //cout << "strvesh: " << strvesh << endl;
    //cout <<"strhfa: "<< strhfa << endl;
    string s;
    if (strvesh == "" || strhfa == "")return false;
    bool blinter = InterCalculate(strvesh, strhfa, workpath);
    cout << "state: " << blinter << endl;
    if (blinter)
    {
        string strres = strhfa.substr(0, strhfa.find_last_of("/"));
        strres = strres + "/hfa.rez";
        return GetInterResult(interres, strres);
    }
    return false;
}


inline string HfaPar::GetPinStr(vector<freqin> data_f)
{
    if (data_f.size() == 0)return "";
    string str0 = "", str1 = "";
    str0 = strhfapar[4] + to_string(data_f.size()) + strhfapar[0];
    str1 = strhfapar[17] + strhfapar[0];
    for (int inp = 0; inp < data_f.size(); inp++)
    {
        str1 = str1 + to_string(data_f[inp].freq) + "   " + to_string(data_f[inp].pin) + "   ";
        str1 = str1 + to_string(data_f[inp].pin1) + "   " + to_string(data_f[inp].pin2) + strhfapar[0];
    }
    return str0 + "&" + str1;
};
inline string HfaPar::GetBeamStr(beam data_beam)
{
    double preVal = 0;
    double beamRateSum = accumulate(data_beam.beamrate.begin(), data_beam.beamrate.end(), 0.0);

    if (data_beam.beamrate.size() == 0)return "";
    string str = strhfapar[24] + dtos(data_beam.beamrate.size()) + strhfapar[0];
    str = str + strhfapar[25] + strhfapar[0];
    
    for (int i = 0; i < data_beam.beamrate.size(); i++)
    {
        data_beam.beamrate[i] /= beamRateSum;
        double beamVal = data_beam.beamrate[i] + preVal;
        double beamr = beamVal * (data_beam.Rpna * data_beam.Rpna - data_beam.Rpvn * data_beam.Rpvn) + data_beam.Rpvn * data_beam.Rpvn;
        str = str + dtos(pow(beamr, 0.5) / 1000.0) + strhfapar[0];
        preVal = beamVal;
    }
    str = str + strhfapar[26] + strhfapar[0];
    for (int i = 0; i < data_beam.vr.size(); i++)
    {
        str = str + dtos(data_beam.vr[i]) + ", ";
    }
    str = str + strhfapar[0];
    str = str + strhfapar[27] + dtos(data_beam.swsr / 1000.0) + strhfapar[0];
    return str;
}
inline string HfaPar::GetSegStr(vector<double> data_swsbeg)
{
    // if (data_swsbeg.size() == 0)return nullptr;
    string str0 = "", str1 = "";
    str0 = strhfapar[12] + dtos(data_swsbeg.size()) + strhfapar[0];
    str1 = strhfapar[28] + strhfapar[0];
    for (int i = 0; i < data_swsbeg.size(); i++)
    {
        str1 = str1 + dtos(data_swsbeg[i] / 1000.0) + strhfapar[0];
    }
    return str0 + "&" + str1;
}
inline string HfaPar::GetDrftStr(vector<drft> data_drft)
{
    // if (data_drft.size() == 0)return nullptr;
    string str0 = "", str1 = "";
    str0 = strhfapar[14] + dtos(data_drft.size()) + strhfapar[0];
    str1 = strhfapar[29] + strhfapar[0];
    for (int i = 0; i < data_drft.size(); i++)
    {
        str1 = str1 + dtos(data_drft[i].begin / 1000.0) + " " + dtos(data_drft[i].end / 1000.0) + strhfapar[0];
    }
    return str0 + "&" + str1;
}
inline string HfaPar::GetLossStr(vector<loss> data_loss, int numOfFreq)
{
    // if (data_loss.size() == 0)return nullptr;
    string str0 = "", str1 = "";
    str0 = strhfapar[13] + dtos(data_loss.size()) + strhfapar[0];
    str1 = strhfapar[31] + strhfapar[0];
    for (int i = 0; i < data_loss.size(); i++)
    {
        str1 = str1 + dtos(data_loss[i].type) + "     " + dtos(data_loss[i].begin / 1000.0) + "     " + dtos(data_loss[i].end / 1000.0);
        for (int j = 0; j < numOfFreq; j++)
        {
            if (j < data_loss[i].dataloss.size()) {
                str1 = str1 + "     " + dtos(data_loss[i].dataloss[j]);
            }
            else {
                if (data_loss[i].dataloss.size() != 0) {
                    str1 = str1 + "     " + dtos(data_loss[i].dataloss.back());
                }
                else {
                    str1 = str1 + "     " + to_string(500);
                }
            }
            
        }
        str1 = str1 + strhfapar[0];
    }
    return str0 + "&" + str1;
}
inline string HfaPar::GetMagStr(vector<MagneticField> data_mag)
{
    //if (data_mag.size() == 0)return "";
    string str0 = "", str1 = "";
    str0 = strhfapar[15] + dtos(data_mag.size()) + strhfapar[0];
    str1 = strhfapar[30] + strhfapar[0];
    for (int i = 0; i < data_mag.size(); i++)
    {
        str1 = str1 + to_string(i + 1) + "     " + dtos(data_mag[i].begin / 1000.0) + "     " + dtos(data_mag[i].period / 1000.0);
        str1 = str1 + "     " + dtos(data_mag[i].m0) + "     " + dtos(data_mag[i].m1) + "     " + dtos(data_mag[i].m2);
        str1 = str1 + strhfapar[0];
    }
    return str0 + "&" + str1;
}
inline string HfaPar::writefile(const InteractionParams& interpar)
{
    string PinStr = GetPinStr(interpar.data_f);
    string PinStr0 = PinStr.substr(0, PinStr.find_first_of("&"));
    string PinStr1 = PinStr.substr(PinStr.find_first_of("&") + 1);
    string BeamStr = GetBeamStr(interpar.beamdata);
    string SegStr = GetSegStr(interpar.data_swsbeg);
    string SegStr0 = SegStr.substr(0, SegStr.find_first_of("&"));
    string SegStr1 = SegStr.substr(SegStr.find_first_of("&") + 1);
    string DrftStr = GetDrftStr(interpar.data_drft);
    string DrftStr0 = DrftStr.substr(0, DrftStr.find_first_of("&"));
    string DrftStr1 = DrftStr.substr(DrftStr.find_first_of("&") + 1);
    string LossStr = GetLossStr(interpar.data_loss,interpar.data_f.size());
    string LossStr0 = LossStr.substr(0, LossStr.find_first_of("&"));
    string LossStr1 = LossStr.substr(LossStr.find_first_of("&") + 1);
    string MagStr = GetMagStr(interpar.data_mag);
    string MagStr0 = MagStr.substr(0, MagStr.find_first_of("&"));
    string MagStr1 = MagStr.substr(MagStr.find_first_of("&") + 1);
    string str = strhfapar[1] + strhfapar[0] + strhfapar[2] + strhfapar[0] + strhfapar[3] + strhfapar[0];
    str = str + PinStr0;
    str = str + strhfapar[5] + dtos(interpar.Sites_of_stability) + strhfapar[0];
    str = str + strhfapar[6] + dtos(interpar.harmonicses) + strhfapar[0];
    str = str + strhfapar[7] + "0" + strhfapar[0];
    str = str + strhfapar[8] + dtos(interpar.FullOutput) + strhfapar[0];
    str = str + strhfapar[9] + dtos(interpar.Igun) + strhfapar[0];
    str = str + strhfapar[10] + dtos(interpar.OutForCollector) + strhfapar[0];
    str = str + strhfapar[11] + dtos(interpar.NumofElectrons) + strhfapar[0];
    str = str + SegStr0;
    str = str + LossStr0;
    str = str + DrftStr0;
    str = str + MagStr0;
    str = str + strhfapar[16] + dtos(interpar.tubelength / 1000.0) + strhfapar[0];
    str = str + PinStr1;
    str = str + strhfapar[18] + dtos(interpar.tubelength / 1000.0) + strhfapar[0];
    str = str + strhfapar[19] + strhfapar[0];
    str = str + strhfapar[20] + strhfapar[0];
    str = str + strhfapar[21] + strhfapar[0];
    str = str + strhfapar[22] + dtos(interpar.I) + strhfapar[0];
    str = str + strhfapar[23] + dtos(interpar.V) + strhfapar[0];
    str = str + BeamStr;
    str = str + SegStr1;
    str = str + DrftStr1;
    str = str + MagStr1;
    str = str + LossStr1;
    str = str + strhfapar[32] + dtos(interpar.interstep / 1000.0) + strhfapar[0];
    str = str + strhfapar[33] + dtos(interpar.tubelength / 1000.0) + strhfapar[0];
    str = str + strhfapar[34] + strhfapar[0];
    str = str + strhfapar[35] + strhfapar[0];
    return str;
}