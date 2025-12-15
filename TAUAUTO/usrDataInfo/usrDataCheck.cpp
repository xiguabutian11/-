#include "usrDataCheck.h"

//静态变量定义
bool usrDataCheck::inaptValReplace;


void inaptCheckFunc::voltageInaptCheck(double& v, string name, const int& args)
{
    /*double maxV = 0;
    double minV = 0;
    double recommendV = 0;

    dispDataInfo disp = usrData::getInstance().curCalGroup.disp;
    if (disp.dispInfo.size() != 0) {
        vector<double> vpTemp;
        for (auto& it : disp.dispInfo.begin()->second.vp) {
            vpTemp.push_back(it.second);
        }
        double maxVp = *max_element(vpTemp.begin(), vpTemp.end());
        double minVp = *min_element(vpTemp.begin(), vpTemp.end());
        double avgVp = accumulate(vpTemp.begin(), vpTemp.end(), 0.0) / disp.dispInfo.begin()->second.vp.size();

        maxV = formula::voltageFromVp(maxVp);
        minV = formula::voltageFromVp(minVp);
        recommendV = formula::voltageFromVp(avgVp);
    }
    if (minV > v || maxV < v) {
        string meg = usrDtInaptPrompt["inaptVoltage"];
        meg += ", recommed: " + to_string(recommendV);
        usrDataCheck::inaptMessage.push_back(meg);
    }*/
    v = 5616.54;
}

void inaptCheckFunc::dispInaptCheck(dispDataInfo& disp, string name, const vector<drft>& cutOffs)
{
    bool cutOffState = usrData::getInstance().dataCheckFlag >> usrDataCheckFlags["cutOffData"];
    vector<pair<double, double>> y;
    for (auto& it : cutOffs) {
        y.push_back(pair<double, double>(it.begin, it.end));
    }

    for (auto& it : disp.samplePoints) {
        if (disp.samplePoints.upper_bound(it.first) == disp.samplePoints.end()) {
            break;
        }
        double nextPos = disp.samplePoints.upper_bound(it.first)->first;
        if (cutOffState && !funcs::isOverlapping({ it.first,nextPos }, y)) {
            it.second.interpoType = dispInterpoType::uniform;
        }
    }
}


bool usrDataCheck::allDataCheck()
{
    bool state = true;
    state &= tubeDataCheck();
    state &= magDataCheck();
    state &= beamDataCheck();
    state &= cutOffDataCheck();
    state &= lossDataCheck();
    state &= dispDataCheck();
    return state;
}

bool usrDataCheck::tubeDataCheck()
{
    InteractionParams intpar = usrData::getInstance().curCalGroup.intpar;
    bool state = true;
    state &= checkFunc(intpar.V, tubeDataKeys[1].first,defalultErrCheck,0,voltageInaptCheck);
    state &= checkFunc(intpar.I, tubeDataKeys[2].first);
    state &= checkFunc(intpar.tubelength, tubeDataKeys[3].first);
    state &= checkFunc(intpar.interstep, tubeDataKeys[4].first);
    state &= checkFunc(intpar.harmonicses, tubeDataKeys[5].first, rangeCheck<int>, { 1,3 });
    state &= checkFunc(intpar.NumofElectrons, tubeDataKeys[6].first, rangeCheck<int>, { 1,192 });
    state &= checkFunc(intpar.Sites_of_stability, tubeDataKeys[7].first);
    state &= checkFunc(intpar.OutForCollector, tubeDataKeys[8].first);
    state &= checkFunc(intpar.FullOutput, tubeDataKeys[9].first);

    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["tubeData"], state);
    
    if (!state) {
        return false;
    }
    if (inaptValReplace) {
        usrData::getInstance().setDataIntpar(intpar);
    }
    return true;
}

bool usrDataCheck::magDataCheck()
{
    InteractionParams intpar = usrData::getInstance().curCalGroup.intpar;
    bool state = true;
    double tubeLen = intpar.tubelength;

    if (intpar.data_mag.size() == 0) {
        usrData::getInstance().errMesInsert(funcs::mesGen(magDataKeys[1].first, usrDataCheckPrompt["lackOfData"]));
        state = false;
    }
    for (int i = 0; i < intpar.data_mag.size();++i) {
        state &= checkFunc(intpar.data_mag[i].begin, magDataKeys[1].first + " [" + to_string(i) + "] " + magElementKeys[1].first, rangeCheck<double>, { 0,tubeLen });
        state &= checkFunc(intpar.data_mag[i].period, magDataKeys[1].first + " [" + to_string(i) + "] " + magElementKeys[2].first);
        state &= checkFunc(intpar.data_mag[i].m0, magDataKeys[1].first + " [" + to_string(i) + "] " + magElementKeys[3].first);
        state &= checkFunc(intpar.data_mag[i].m1, magDataKeys[1].first + " [" + to_string(i) + "] " + magElementKeys[4].first);
        state &= checkFunc(intpar.data_mag[i].m2, magDataKeys[1].first + " [" + to_string(i) + "] " + magElementKeys[5].first);
    }

    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["magData"], state);
    return state;
}

bool usrDataCheck::beamDataCheck()
{
    InteractionParams intpar = usrData::getInstance().curCalGroup.intpar;
    bool state = true;
    state &= checkFunc(intpar.beamdata.Rpna, beamDataKeys[1].first);
    state &= checkFunc(intpar.beamdata.Rpvn, beamDataKeys[2].first);
    state &= checkFunc(intpar.beamdata.swsr, beamDataKeys[3].first);

    if (intpar.beamdata.Rpna <= intpar.beamdata.Rpvn || intpar.beamdata.swsr <= intpar.beamdata.Rpna) {

        string obj = beamDataKeys[1].first +","+ beamDataKeys[2].first + ","+ beamDataKeys[3].first;
        usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["wrongData"]));
        state = false;
    }
    if (intpar.beamdata.beamrate.size() == 0) {
        usrData::getInstance().errMesInsert(funcs::mesGen(beamDataKeys[4].first, usrDataCheckPrompt["lackOfData"]));
        state = false;
    }
    for (int i = 0; i < intpar.beamdata.beamrate.size();++i) {
        state &= checkFunc(intpar.beamdata.beamrate[i], beamDataKeys[4].first + "[" + to_string(i) + "] ");
    }
    if (intpar.beamdata.vr.size() == 0) {
        usrData::getInstance().errMesInsert(funcs::mesGen(beamDataKeys[5].first, usrDataCheckPrompt["lackOfData"]));
        state = false;
    }
    for (int i = 0; i < intpar.beamdata.vr.size(); ++i) {
        state &= checkFunc(intpar.beamdata.vr[i], beamDataKeys[5].first + "[" + to_string(i) + "] ");
    }

    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["beamData"], state);
    return state;
}

bool usrDataCheck::cutOffDataCheck()
{
    calGroupUnite data = usrData::getInstance().curCalGroup;
    double tubeLen = data.intpar.tubelength;
    lossAndCutOffDataInfo lossDt = data.loss;
    bool state = true;

    state = checkFunc(data.loss.cutOffs, lossDataKeys[1].first, cutOffCheck, tubeLen);

    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["cutOffData"], state);
    return state;
}

bool usrDataCheck::lossDataCheck()
{
    calGroupUnite data = usrData::getInstance().curCalGroup;
    double tubeLen = data.intpar.tubelength;
    lossAndCutOffDataInfo lossDt = data.loss;
    bool state = true;

    state = checkFunc(lossDt, "", lossCheck, tubeLen);

    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["lossData"], state);
    return state;
   

}

bool usrDataCheck::dispDataCheck()
{
    calGroupUnite data = usrData::getInstance().curCalGroup;
    double tubeLen = data.intpar.tubelength;
    dispDataInfo disp = data.disp;
    bool state = true;

    state = checkFunc(disp, "", dispCheck, {tubeLen,data.loss.cutOffs},dispInaptCheck,data.loss.cutOffs);
    if (state) {
        usrData::getInstance().setDataDispar(disp);
    }
    usrData::getInstance().setDataCheckFlag(usrDataCheckFlags["dispData"], state);
    return state;

}

bool usrDataCheck::paramDataCheck()
{
    return false;
}

void usrDataCheck::setValReplace(const bool& flag)
{
    inaptValReplace = flag;
}

bool errCheckFunc::cutOffCheck(vector<drft> cutOffs, string name, const double& tubeLen)
{
    bool state = true;
    vector<pair<double, double>> y;
    int i = 0;
    for (auto& it : cutOffs) {
        string obj = name + "[" + to_string(i) + "]";
        if (it.begin< 0 || it.end <= it.begin || it.end> tubeLen) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["wrongData"]));
            state = false;
        }
        else {
            if (funcs::isOverlapping(pair<double, double>(it.begin, it.end), y)) {
                y.push_back(pair<double, double>(it.begin, it.end));
            }
            else {
                usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["overlapData"]));
                state = false;
            }
        }
        i++;
    }
    return state;
}

bool errCheckFunc::lossCheck(lossAndCutOffDataInfo loss, string name, const double& tubeLen)
{
    bool cutOffState = usrData::getInstance().dataCheckFlag >> usrDataCheckFlags["cutOffData"];
    bool state = true;
    vector<pair<double, double>> y; //截断覆盖范围
    vector<pair<double, double>> z; //集中衰减覆盖范围
    int i = 0;
    for (auto& it : loss.cutOffs) {
        y.push_back(pair<double, double>(it.begin, it.end));
    }
    for (auto& it : loss.lossSegs) {
        string obj = lossDataKeys[2].first + "[" + to_string(i) + "]";
        if (it.begin< 0 || it.end <= it.begin || it.end> tubeLen) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["wrongData"]));
            state = false;
        }
        else {
            if (cutOffState && !funcs::isOverlapping(pair<double, double>(it.begin, it.end), y)) {     //检查集中衰减段和截断是否重叠
                usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["lossOverlapCutOff"]));
                state = false;
            }
            if (funcs::isOverlapping(pair<double, double>(it.begin, it.end), z)) {
                z.push_back(pair<double, double>(it.begin, it.end));
            }
            else {
                usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["overlapData"]));
                state = false;
            }
        }
        i++;
    }
    i = 0;
    for (auto& it : loss.dBInfo) {
        state &= usrDataCheck::checkFunc(it.first, lossDataKeys[3].first + "[" + to_string(i) + "]" + " " + dBElementKeys[1].first);
        for (int j = 0; j < it.second.size(); ++j) {
            state &= usrDataCheck::checkFunc(it.second[i].dB, lossDataKeys[3].first + "[" + to_string(i) + "]" + " " + dBElementKeys[2].first + "[" + to_string(j) + "]" + " " + dBsEleKeys[1].first);
        }
    }
    return state;
}

bool errCheckFunc::dispCheck(dispDataInfo disp, string name, const pair<double, vector<drft>>& args)
{
    bool cutOffState = usrData::getInstance().dataCheckFlag >> usrDataCheckFlags["cutOffData"];
    bool state = true;
    set<string> errDisp;
    int i = 0;
    double tubeLen = args.first;
    vector<pair<double, double>> y; //截断覆盖范围

    auto mapsMinusValCheck = [=](const map<double, double, DoubleComparator>& maps)->bool {       //lambda函数：用于判断maps中键或者值是否存在负数
        for (const auto& [key, value] : maps) {
            if (key < 0 || value < 0) {
                return false; // 如果发现负数，直接返回false
            }
        }
        return true; // 没有发现负数，返回 true
    };
    for (auto& it : args.second) {
        y.push_back(pair<double, double>(it.begin,it.end));
    }
    for (auto& it : disp.dispInfo) {
        string obj = dispDataKeys[1].first + " -- " + it.first;
        string meg = "";
        if (it.second.kc.empty() || it.second.vp.empty() || it.second.al.empty()) {
            meg = funcs::mesGen(obj, usrDataCheckPrompt["lackOfDispData"]);
        }
        else {
            vector<string> minusItems;
            if (!mapsMinusValCheck(it.second.kc)) {
                minusItems.push_back(dispFuncType["kc"]);
            }
            if (!mapsMinusValCheck(it.second.vp)) {
                minusItems.push_back(dispFuncType["vp"]);
            }
            if (!mapsMinusValCheck(it.second.al)) {
                minusItems.push_back(dispFuncType["al"]);
            }
            if (!minusItems.empty()) {
                meg = funcs::mesGen(obj, usrDataCheckPrompt["minusDispData"], minusItems);
            }
        }
        if (!meg.empty()) {
            usrData::getInstance().errMesInsert(meg);
            errDisp.insert(it.first);
            state = false;
        }
    }
    for (auto& it : disp.samplePoints) {
        string obj = dispDataKeys[2].first + " -- " + to_string(it.first);
        if (it.first<0 || it.first> tubeLen) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["wrongData"]));
            state = false;
        }
        if (disp.dispInfo.count(it.second.dispKey) == 0 || errDisp.count(it.second.dispKey)) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["errorDispData"]));
            state = false;
        }
        if (cutOffState && !funcs::isOverlapping(it.first, y)) {
            usrData::getInstance().errMesInsert(funcs::mesGen(obj, usrDataCheckPrompt["dispOverlapCutOff"]));
            state = false;
        }
    }
    return state;
}
