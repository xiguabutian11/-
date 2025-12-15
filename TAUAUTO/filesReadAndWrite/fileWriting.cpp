#include "fileWriting.h"

bool fileWriting::jsonWriting(const Json::Value& root, const string& path)
{
    
    ofstream os;
    os.open(path, std::ios::trunc);
    
    if (!os.is_open()) {
        return false;
    }
    Json::StyledWriter sw;
    os << sw.write(root);
    os.close();
    return true;
}

bool fileWriting::txtWriting(const vector<string>& strs, const string& path, bool modeFlag)
{
    ofstream os;
    if (!modeFlag) {
        os.open(path, std::ios::trunc);
    }
    else {
        os.open(path, std::ios::app);
    }
    
    if (!os.is_open()) {
        return false;
    }
    for (auto& it : strs) {
        os << it << endl;
    }
    os.close();
    return true;
}

bool fileWriting::historyProjWrite(const historyProj& hist, string path)
{
    if (path.empty()) {
        std::filesystem::path currentWorkPath = std::filesystem::current_path();
        path = (currentWorkPath/globalFilePos["hist"]).string();
    }
    Json::Value root;
    for (unsigned int i = 0; i < hist.histList.size(); i++) {
        Json::Value element;
        element[histProjSubKeys[1].first] = Json::Value(hist.histList[i].name);
        element[histProjSubKeys[2].first] = Json::Value(hist.histList[i].directory);
        element[histProjSubKeys[3].first] = Json::Value(hist.histList[i].creationTime);
        element[histProjSubKeys[4].first] = Json::Value(hist.histList[i].lastSaveTime);
    }
    if (jsonWriting(root, path) == false) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["histProj"], fileWritingPrompt["writeFail"],{path}));
        return false;
    }
    return true;
}


bool fileWriting::projectInfoWrite(const projectInfo& proj, string path)
{
    if (path.empty()) {
        filesystem::path curProjPath(ctlData::getInstance().tempVaris.currentProject);
        string projName = curProjPath.filename().string();
        path = (curProjPath / (projName + fileSuffix["proj"])).string();
    }
    vector<string> text;
    text.push_back(projItems[1]);
    text.push_back(proj.name);
    text.push_back("");
    text.push_back(projItems[2]);
    text.push_back(proj.creationTime);
    text.push_back("");
    text.push_back(projItems[3]);
    text.push_back(proj.lastSaveTime);
    text.push_back("");
    text.push_back(projItems[4]);
    for (unsigned int i = 0; i < proj.calGroups.size(); i++) {
        text.push_back(proj.calGroups[i]);
    }
    text.push_back("");
    text.push_back(projItems[5]);
    text.push_back(proj.paramFlPath);
    text.push_back("");
    text.push_back(projItems[6]);
    text.push_back(proj.unitFlPath);

    
    if (!txtWriting(text, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["projInfo"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::parameterWrite(parametersList& parData, string path)
{
    if (path.empty()) {
        projectInfo proj = usrData::getInstance().proj;
        filesystem::path projPath(proj.directory);
        path = (projPath / proj.paramFlPath).string();
    }
    ofstream os(path, ios::trunc);
    if (!os.is_open()) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["parameter"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }

    os << paramHeaders[1] << "," << paramHeaders[2] << "," << paramHeaders[3] << endl;
    for (auto& it : parData.orders) {
        if (parData.paramList.count(it) == 0) {
            continue;
        }
        os << it << "," << parData.paramList[it].expression << "," << parData.paramList[it].description << endl;
    }
    os.close();
    return true;
}

bool fileWriting::unitWrite(unitIndex& units, string path)
{
    if (path.empty()) {
        projectInfo proj = usrData::getInstance().proj;
        filesystem::path projPath(proj.directory);
        path = (projPath / proj.unitFlPath).string();
    }
    Json::Value root;
    auto jsonAssign = [&root](map<int, pair<string, double>> Map, int unitIndex, int index) {
        if (Map.count(unitIndex) != 0) {
            root[unitKeys[index].first] = unitIndex;
        }
        else {
            root[unitKeys[index].first] = defaultUnitIndex[index];
        }
        };
    jsonAssign(VUnit, units.voltageIndex, 1);
    jsonAssign(IUnit, units.currentIndex, 2);
    jsonAssign(freqUnit, units.frequencyIndex, 3);
    jsonAssign(lengthUnit, units.lengthIndex, 4);

    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["unit"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::calGroupWrite(calGroup& cals, string path)
{
    if (path.empty()) {
        auto& data = ctlData::getInstance().tempVaris;
        filesystem::path tempPath(data.currentProject);
        tempPath = tempPath / data.currentCalGroup;
        string projName = tempPath.filename().string();
        path = (tempPath / (projName + fileSuffix["calGroup"])).string();
    }
    vector<string> text;
    text.push_back(calGroupItems[1]);
    text.push_back(cals.name);
    text.push_back("");
    text.push_back(calGroupItems[2]);
    text.push_back(cals.creationTime);
    text.push_back("");
    text.push_back(calGroupItems[3]);
    text.push_back(cals.lastSaveTime);
    text.push_back("");
    text.push_back(calGroupItems[4]);
    text.push_back(cals.tubeDPath);
    text.push_back("");
    text.push_back(calGroupItems[5]);
    text.push_back(cals.magDPath);
    text.push_back("");
    text.push_back(calGroupItems[6]);
    text.push_back(cals.beamDPath);
    text.push_back("");
    text.push_back(calGroupItems[7]);
    text.push_back(cals.lossDPath);
    text.push_back("");
    text.push_back(calGroupItems[8]);
    text.push_back(cals.dispDPath);
    text.push_back("");
    text.push_back(calGroupItems[9]);
    text.push_back(cals.calSPath);
    text.push_back("");
    text.push_back(calGroupItems[10]);
    text.push_back(cals.mesPath);
    text.push_back("");
    text.push_back(calGroupItems[11]);
    text.push_back(cals.paramLPath);
    text.push_back("");
    text.push_back(calGroupItems[12]);
    text.push_back(cals.resFilePath);

    if (!txtWriting(text, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calGroup"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::tubeDataWrite(InteractionParams& intpar, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.tubeDPath).string();
    }
    Json::Value root;
    root[tubeDataKeys[1].first] = intpar.V;
    root[tubeDataKeys[2].first] = intpar.I;
    root[tubeDataKeys[3].first] = intpar.tubelength;
    root[tubeDataKeys[4].first] = intpar.interstep;
    root[tubeDataKeys[5].first] = intpar.harmonicses;
    root[tubeDataKeys[6].first] = intpar.NumofElectrons;
    root[tubeDataKeys[7].first] = static_cast<unsigned int>(intpar.Sites_of_stability);
    root[tubeDataKeys[8].first] = static_cast<unsigned int>(intpar.OutForCollector);
    root[tubeDataKeys[9].first] = static_cast<unsigned int>(intpar.FullOutput);

    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["tubeData"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
    
}

bool fileWriting::magDataWrite(InteractionParams& intpar, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.magDPath).string();
    }
    Json::Value root;

    //将data_mag按照begin大小排序
    sort(intpar.data_mag.begin(), intpar.data_mag.end(), [](const MagneticField& a, const MagneticField& b) { return a.begin < b.begin; });

    for (unsigned int i = 0; i < intpar.data_mag.size(); i++) {
        Json::Value mag;
        mag[magElementKeys[1].first] = intpar.data_mag[i].begin;
        mag[magElementKeys[2].first] = intpar.data_mag[i].period;
        mag[magElementKeys[3].first] = intpar.data_mag[i].m0;
        mag[magElementKeys[4].first] = intpar.data_mag[i].m1;
        mag[magElementKeys[5].first] = intpar.data_mag[i].m2;

        root[magDataKeys[1].first].append(mag);
    }

    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["magData"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::beamDataWrite(InteractionParams& intpar, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.beamDPath).string();
    }
    Json::Value root;
    root[beamDataKeys[1].first] = intpar.beamdata.Rpna;
    root[beamDataKeys[2].first] = intpar.beamdata.Rpvn;
    root[beamDataKeys[3].first] = intpar.beamdata.swsr;

    for (double& it : intpar.beamdata.beamrate) {
        root[beamDataKeys[4].first].append(it);
    }
    for (double& it : intpar.beamdata.vr) {
        root[beamDataKeys[5].first].append(it);
    }
    root[beamDataKeys[6].first] = funcs::boolConvert(intpar.Igun);
    root[beamDataKeys[7].first] = intpar.INDINTauPath;
    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["beamData"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::lossDataWrite(lossAndCutOffDataInfo& loss, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.lossDPath).string();
    }
    Json::Value root;
    for (unsigned int i = 0; i < loss.cutOffs.size(); ++i) {
        Json::Value rootTemp;
        rootTemp[cOffElementKeys[1].first] = loss.cutOffs[i].begin;
        rootTemp[cOffElementKeys[2].first] = loss.cutOffs[i].end;
        root[lossDataKeys[1].first].append(rootTemp);
    }
    for (unsigned int i = 0; i < loss.lossSegs.size(); ++i) {
        Json::Value rootTemp;
        rootTemp[lossElementKeys[1].first] = loss.lossSegs[i].begin;
        rootTemp[lossElementKeys[2].first] = loss.lossSegs[i].end;
        rootTemp[lossElementKeys[3].first] = loss.lossSegs[i].lossType;
        rootTemp[lossElementKeys[4].first] = loss.lossSegs[i].defaultDistriType;
        rootTemp[lossElementKeys[5].first] = loss.lossSegs[i].defaultdB;

        root[lossDataKeys[2].first].append(rootTemp);
    }
    for (auto& freq:loss.dBInfo) {
        Json::Value rootTemp;
        rootTemp[dBElementKeys[1].first] = freq.first;
        for (unsigned int i = 0; i < freq.second.size(); ++i) {
            Json::Value subRootTemp;
            subRootTemp[dBsEleKeys[1].first] = freq.second[i].dB;
            subRootTemp[dBsEleKeys[2].first] = freq.second[i].distriType;
            rootTemp[dBElementKeys[2].first].append(subRootTemp);
        }
        root[lossDataKeys[3].first].append(rootTemp);
    }
    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["lossData"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::dispDataWrite(dispDataInfo& disps, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.dispDPath).string();
    }
    auto dispTempFunc = [&](Json::Value& rootIn, const map<double, double, DoubleComparator>& maps) {
        for (auto& it : maps) {
            rootIn[kvaEleKeys[1].first].append(it.first);
            rootIn[kvaEleKeys[2].first].append(it.second);
        }
    };

    Json::Value root;
    for (auto& dispName : disps.orders) {
        Json::Value rootTemp;
        dispDataInfo::dispData disp = disps.dispInfo.at(dispName);
        rootTemp[dispElementKeys[1].first] = dispName;
        rootTemp[dispElementKeys[2].first] = disp.fileType;
        rootTemp[dispElementKeys[3].first] = disp.path;
        rootTemp[dispElementKeys[4].first] = disp.skipHeader;

        dispTempFunc(rootTemp[dispElementKeys[5].first], disp.kc);
        dispTempFunc(rootTemp[dispElementKeys[6].first], disp.vp);
        dispTempFunc(rootTemp[dispElementKeys[7].first], disp.al);
        
        root[dispDataKeys[1].first].append(rootTemp);
    }
    for (auto& samPoint : disps.samplePoints) {
        Json::Value rootTemp;
        rootTemp[samPointEleKeys[1].first] = samPoint.first;
        rootTemp[samPointEleKeys[2].first] = samPoint.second.dispKey;
        rootTemp[samPointEleKeys[3].first] = samPoint.second.interpoType;

        root[dispDataKeys[2].first].append(rootTemp);
    }
    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["dispData"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::calSetWrite(calSetting& calSet, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.calSPath).string();
    }
    Json::Value root;
    for (auto& seq : calSet.calSet) {
        Json::Value rootTemp;
        rootTemp[calElementKeys[1].first] = seq.first;
        for (auto& par : seq.second.seq) {
            Json::Value subRootTemp;
            subRootTemp[seqKeys[1].first] = par.first;
            for (auto& val : par.second.array) {
                subRootTemp[seqKeys[2].first].append(val);
            }
            subRootTemp[seqKeys[3].first] = static_cast<unsigned int>(par.second.signNum);

            rootTemp[calElementKeys[2].first].append(subRootTemp);
        }
        rootTemp[calElementKeys[3].first] = static_cast<unsigned int>(seq.second.soloCal);
        root[calSetKeys[1].first].append(rootTemp); 
    }
    if (calSet.centerFreq.freq > 0) {
        Json::Value rootTemp;
        rootTemp[centerFreqKeys[1].first] = calSet.centerFreq.freq;
        rootTemp[centerFreqKeys[2].first] = calSet.centerFreq.pin;
        rootTemp[centerFreqKeys[3].first] = calSet.centerFreq.pin1;
        rootTemp[centerFreqKeys[4].first] = calSet.centerFreq.pin2;
        if (!calSet.centerFreq.desc.empty()) {
            rootTemp[centerFreqKeys[5].first] = calSet.centerFreq.desc;
        }

        root[calSetKeys[2].first] = rootTemp;
    }

    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["calSet"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::paramLinkWrite(paramLinks& pars, string path)
{
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        path = (projPath / calG.paramLPath).string();
    }
    Json::Value root;
    for (auto& file : pars.parLinks) {
        for (auto& par : file.second) {
            Json::Value rootTemp;
            rootTemp[parInfoKeys[1].first] = par.second.index1;
            rootTemp[parInfoKeys[2].first] = par.second.index2;
            rootTemp[parInfoKeys[3].first] = par.second.index3;
            rootTemp[parInfoKeys[4].first] = par.second.expression;
            rootTemp[parInfoKeys[5].first] = par.second.decription;
            rootTemp[parInfoKeys[6].first] = par.second.dataType;
            root[paramLinkKeys[1].first].append(rootTemp);
        }
    }
    if (!jsonWriting(root, path)) {
        usrData::getInstance().errMesInsert(funcs::mesGen(projFileItems["paramLink"], fileWritingPrompt["writeFail"], { path }));
        return false;
    }
    return true;
}

bool fileWriting::resultsWrite(results& res, string path)
{
    filesystem::path resPath(path);
    if (path.empty()) {
        calGroup calG = usrData::getInstance().curCalGroup.calG;
        filesystem::path projPath(calG.directory);
        resPath = (projPath / calG.resFilePath).string();
    }
    
    for (auto& seq : res.reses) {

        vector<string> errUnitRes;
        if (!filesystem::exists(resPath / seq.first)) {
            bool state = filesystem::create_directories(resPath / seq.first);
            if (!state) {
                continue;
            }
        }
        if (res.BGDatas[seq.first].resWaitToImport) {
            continue;
        }

        if (filesystem::is_directory(resPath / seq.first)) {
            for (const auto& entry : filesystem::directory_iterator(resPath / seq.first)) {
                // 如果是文件，删除它
                if (filesystem::is_regular_file(entry)) {
                    filesystem::remove(entry);
                }
            }
        }
        for (auto& unitRes : seq.second) {
            filesystem::path unitResPath = resPath / seq.first / (unitRes.name + fileSuffix["unitRes"]);
            if (!unitResWrite(unitRes, unitResPath.string(), seq.first)) {
                errUnitRes.push_back(unitRes.name);
            }

        }
        filesystem::path parScanVarisPath = resPath / seq.first / (resParScanVarisKeys[1].first + fileSuffix["resParScanVaris"]);
        if (!resParScanVarisWrite(res.BGDatas.at(seq.first).calS, parScanVarisPath.string())) {
            errUnitRes.push_back(resParScanVarisKeys[1].first);
        }
    }
    return true;
}

bool fileWriting::unitResWrite(results::res& res, string path, string seqName)
{
    Json::Value root;
    Json::Value resultRoot;

    auto func = [&](Json::Value& rootIn, const vector<vector<double>>& vect) {
        for (int i = 0; i < 3; ++i) {
            if (vect.size() <= i) {
                return;
            }
            for (auto& val : vect[i]) {
                rootIn[resultArraysEleKeys[i+1].first].append(val);
            }
        }
    };

    root[unitResKeys[1].first] = res.name;
    root[unitResKeys[2].first][resFreqyKeys[1].first] = res.freqy.freq;
    root[unitResKeys[2].first][resFreqyKeys[2].first] = res.freqy.pin;
    if (res.freqy.pin1 != 0) {
        root[unitResKeys[2].first][resFreqyKeys[3].first] = res.freqy.pin1;
    }
    if (res.freqy.pin2 != 0) {
        root[unitResKeys[2].first][resFreqyKeys[4].first] = res.freqy.pin2;
    }
    for (auto& par : res.params) {
        Json::Value rootTemp;
        rootTemp[resParamsKeys[1].first] = par.first;
        rootTemp[resParamsKeys[2].first] = par.second;
        root[unitResKeys[3].first].append(rootTemp);
    }
    for (auto& val : res.tube) {
        root[unitResKeys[4].first].append(val);
    }
    for (auto& val : res.mag) {
        root[unitResKeys[5].first].append(val);
    }
    
    resultRoot[resResultKeys[1].first] = res.result.freq;
    resultRoot[resResultKeys[2].first] = res.result.Pso;
    resultRoot[resResultKeys[3].first] = res.result.len_pso;
    for (auto& val : res.result.FlowRate) {
        resultRoot[resResultKeys[4].first].append(val);
    }
    for (auto& val : res.result.speed) {
        resultRoot[resResultKeys[5].first].append(val);
    }

    func(resultRoot[resResultKeys[6].first], res.result.Pout);
    func(resultRoot[resResultKeys[7].first], res.result.Gain);
    func(resultRoot[resResultKeys[8].first], res.result.eff);

    root[unitResKeys[6].first] = resultRoot;
    if (!jsonWriting(root, path)) {
        return false;
    }
    return true;
}

bool fileWriting::resParScanVarisWrite(calSetting::calSequence data, string path)
{
    Json::Value root;
    for (auto& it : data.seq) {
        Json::Value rootTemp;
        rootTemp[resVarisKeys[1].first] = it.first;
        rootTemp[resVarisKeys[2].first] = static_cast<unsigned int>(it.second.signNum);
        for (auto& val : it.second.array) {
            rootTemp[resVarisKeys[3].first].append(val);
        }
        root[resParScanVarisKeys[1].first].append(rootTemp);
    }
    root[resParScanVarisKeys[2].first] = static_cast<unsigned int>(data.soloCal);
    if (!jsonWriting(root, path)) {
        return false;
    }
    return true;
}

bool fileWriting::calGroupUniteWrite(calGroupUnite calGu, string path)
{
    filesystem::path mainPath(path);
    if (!filesystem::exists(mainPath) || !filesystem::is_directory(mainPath)) {
        return false;
    }
    bool state = true;
    calGu.calG.lastSaveTime = currentDateTime(1);
    filesystem::path pathTemp = mainPath / (calGu.calG.name + fileSuffix["calGroup"]);
    if (!fileWriting::calGroupWrite(calGu.calG, pathTemp.string())) {
        return false;
    }

    pathTemp = mainPath / calGu.calG.tubeDPath;
    if (!fileWriting::tubeDataWrite(calGu.intpar, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.magDPath;
    if (!fileWriting::magDataWrite(calGu.intpar, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.beamDPath;
    if (!fileWriting::beamDataWrite(calGu.intpar, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.lossDPath;
    if (!fileWriting::lossDataWrite(calGu.loss, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.dispDPath;
    if (!fileWriting::dispDataWrite(calGu.disp, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.calSPath;
    if (!fileWriting::calSetWrite(calGu.calS, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.paramLPath;
    if (!fileWriting::paramLinkWrite(calGu.parLink, pathTemp.string())) {
        state = false;
    }
    pathTemp = mainPath / calGu.calG.resFilePath;
    if (!fileWriting::resultsWrite(calGu.res, pathTemp.string())) {
        state = false;
    }
    return state;
}

//bool fileWriting::resParScanVarisWrite(map<string, signedArray<double, bool>> data, string path, string seqName)
//{
//    Json::Value root;
//    for (auto& it : data) {
//        Json::Value rootTemp;
//        rootTemp[resVarisKeys[1].first] = it.first;
//        rootTemp[resVarisKeys[2].first] = static_cast<unsigned int>(it.second.signNum);
//        for (auto& val : it.second.array) {
//            rootTemp[resVarisKeys[3].first].append(val);
//        }
//        root[resParScanVarisKeys[1].first].append(rootTemp);
//    }
//    if (!jsonWriting(root, path)) {
//        //string objName = projFileItems["unitRes"] + "_" + seqName +"_"+ resParScanVarisKeys[1].first;
//        //usrData::getInstance().errMesInsert(funcs::mesGen(objName, fileWritingPrompt["writeFail"], { path }));
//        return false;
//    }
//    return true;
//}

