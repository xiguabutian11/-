#include "calculation.h"

//静态成员变量定义
BS::thread_pool calculation::pool;
unsigned int calculation::taskSum = 0;
std::atomic<int> calculation::dealedTaskNum = 0;
std::atomic<int> calculation::failedTaskNum = 0;
set<string> calculation::seqNames;
std::mutex calculation::mtx;
int calculation::soloCalFakeFreqAdd = 1;


void calculation::seqCalculate()
{
    vector<dataToCalculate> datas;
    map<string, double> varis;
    usrData& data = usrData::getInstance();

    if (!preCalculate()) {
        return;
    }
    usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["startCal"]));

    for (auto& par : data.pars.paramList) {
        if (par.second.state == paramState::available) {
            varis.insert(pair<string, double>(par.first, par.second.value));
        }
    }

    for (auto& seq : data.curCalGroup.calS.calSet) {
        if (seq.second.seqEnable) {
            seq.second.seqState = seqState::processing;
            multiTaskGeneration(seq.second, datas, varis);
        }
    }
    taskSum = static_cast<double>(datas.size());
    //Helper::getInstance().emitStartCal(taskSum);
    cout << datas.size() << endl;
    run(datas);

}

void calculation::soloCalculate()
{
    vector<dataToCalculate> datas;
    map<string, double> varis;
    vector<double> freqs;
    usrData& data = usrData::getInstance();
    freqin centerFreq = data.curCalGroup.calS.centerFreq;
    string soloCalName = ctlData::getInstance().dftValue.defaultSoloCalName;
    calSetting::calSequence calSet;
    calSet.soloCal = true;
    signedArray<double, bool> arrTemp;
    if (!preCalculate()) {
        return;
    }
    usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["startSoloCal"]));

    for (auto& par : data.pars.paramList) {
        if (par.second.state == paramState::available) {
            varis.insert(pair<string, double>(par.first, par.second.value));
        }
    }


    arrTemp.array = { centerFreq.freq };
    calSet.seq.insert_or_assign(parScanFixedItem["frequency"], arrTemp);
    arrTemp.array = { centerFreq.pin };
    calSet.seq.insert_or_assign(parScanFixedItem["pin"], arrTemp);
    arrTemp.array = { centerFreq.pin1 };
    calSet.seq.insert_or_assign(parScanFixedItem["pin_1"], arrTemp);
    arrTemp.array = { centerFreq.pin2 };
    calSet.seq.insert_or_assign(parScanFixedItem["pin_2"], arrTemp);

    int index = 1;
    while (1) {
        string name = soloCalName + "_" + to_string(index);
        if (!data.curCalGroup.res.reses.count(name)) {
            calSet.name = name;
            break;
        }
        ++index;
    }
    multiTaskGeneration(calSet, datas, varis);
    taskSum = static_cast<double>(datas.size());
    //Helper::getInstance().emitStartCal(taskSum);
    run(datas);
}

void calculation::waitForAllTasks()
{
    pool.wait();
}

void calculation::calculateQuit()
{
    ctlData::getInstance().setCalculateQuitFlag(true);
    usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["calQuitting"]));
}

bool calculation::threadPoolBusy()
{
    return (pool.get_tasks_total() != 0 || pool.get_tasks_running() != 0);
}

bool calculation::preCalculate()
{
    if (pool.get_tasks_total() != 0 || pool.get_tasks_running() != 0) {
        usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["calOccupied"]));
        return false;
    }
    if (!usrDataCheck::allDataCheck()) {
        usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["dataCheckErr"]));
        return false;
    }

    taskSum = 0;
    dealedTaskNum = 0;
    failedTaskNum = 0;
    parameter::paramStateUpdate();
    dispDtPro::dispFuncsGeneration();

    lossDtPro::posGeneration();
    lossDtPro::dBValuesGeneration();

    ctlData::getInstance().setCalculateQuitFlag(false);
    seqNames.clear();
    return true;
}

void calculation::multiTaskGeneration(calSetting::calSequence seq, vector<dataToCalculate>& datas, const map<string, double>& varis)
{
    vector<double> freqs;
    bool singlePoint = false;
    if (seq.seqEnable == false) {
        return;
    }
    freqs.assign(seq.seq[parScanFixedItem["frequency"]].array.begin(), seq.seq[parScanFixedItem["frequency"]].array.end());
    if (freqs.size() == 0) {
        return;
    }
    if (freqs.size() == 1) {
        seq.seq[parScanFixedItem["frequency"]].array.push_back(freqs[0] + soloCalFakeFreqAdd);
        freqs.push_back(freqs[0] + soloCalFakeFreqAdd);
        singlePoint = true;
    }
    seq.seqState = seqState::unporcessed;

    if (!paramScan::parScanFunction(seq)) {
        return;		//考虑添加提示信息
    };
    dispDtPro::dispDatasGeneration(freqs);

    for (auto& data : paramScan::getParScanData()) {
        dataToCalculate dataTemp;
        dataTemp.intpar = parScanDataToIntpar(data, varis);
        dataTemp.dispars = parScanDataToDispar(data);
        dataTemp.seqName = seq.name;
        dataTemp.parScanDt = data;
        dataTemp.singlePoint = singlePoint;

        datas.push_back(dataTemp);
    }
    results res = usrData::getInstance().curCalGroup.res;
    res.reses.erase(seq.name);
    res.BGDatas.erase(seq.name);
    res.extraction.erase(seq.name);
    usrData::getInstance().setDataRes(res);

    results::BGDataOfRes BGData;
    backgroundDataOfResGen(seq, BGData, singlePoint);

    usrData::getInstance().insertDataResBGData(BGData, seq.name);
    seqNames.insert(seq.name);
}

void calculation::backgroundDataOfResGen(const calSetting::calSequence& seq, results::BGDataOfRes& BGData, bool singlePoint)
{
    auto& data = usrData::getInstance();
    BGData.pars = data.pars;
    BGData.calS.seq = paramScan::getQualifiedParScanVaris();
    BGData.calS.soloCal = seq.soloCal;
    if (singlePoint) {
        vector<double> arrTemp = BGData.calS.seq.at(parScanFixedItem["frequency"]).array;
        BGData.calS.seq.at(parScanFixedItem["frequency"]).array = { *min_element(arrTemp.begin(),arrTemp.end()) };
    }
    BGData.parLink = data.curCalGroup.parLink;
    BGData.intpar = data.curCalGroup.intpar;
    BGData.disp = data.curCalGroup.disp;
    BGData.loss = data.curCalGroup.loss;
}

InteractionParams calculation::parScanDataToIntpar(paramScanData& data, map<string, double> varis)
{
    vector<double> freqsTemp;
    InteractionParams intpar = usrData::getInstance().curCalGroup.intpar;
    for (auto& it : data.variants) {
        varis[it.first] = it.second;
    }

    intpar.data_f.assign(data.freqs.begin(), data.freqs.end());

    parameter::paramLinkFunction(varis, intpar);
    for (auto& it : data.freqs) {
        freqsTemp.push_back(it.freq);
    }
    intpar.data_loss.clear();
    intpar.data_loss = lossDtPro::lossDataGeneration(freqsTemp);
    intpar.data_drft = usrData::getInstance().curCalGroup.loss.cutOffs;
    return intpar;
}

vector<DispersionParams> calculation::parScanDataToDispar(paramScanData& data)
{
    vector<DispersionParams> dispars;
    auto dispDatas = dispDtPro::getDispData();
    for (auto& pos : usrData::getInstance().curCalGroup.intpar.data_swsbeg) {
        DispersionParams dispar;
        for (auto& f : data.freqs) {
            dispar.data_f.push_back(f.freq);
            dispar.data_kc.push_back(dispDatas[f.freq][pos].kc);
            dispar.data_vp.push_back(dispDatas[f.freq][pos].vp);
            dispar.data_alfa.push_back(dispDatas[f.freq][pos].al);
        }
        dispars.push_back(dispar);
    }
    return dispars;
}

void calculation::taskProcessFunc(const dataToCalculate& data, const string& exePath)
{
    bool state = ctlData::getInstance().flags.calculQuit;
    InteractionRes intres;
    std::thread::id threadID = std::this_thread::get_id();
    std::ostringstream oss;
    oss << threadID;
    if (state || Interaction_data(data.intpar, data.dispars, intres, exePath, stoi(oss.str()))) {
        resProcess(intres, data, true, state);
    }
    else {
        resProcess(intres, data, false);
    }

}

void calculation::run(const vector<dataToCalculate>& datas)
{
    ctlData::defaultValue dftVal = ctlData::getInstance().dftValue;
    if (dftVal.maxThread == 0 || dftVal.maxThread > thread::hardware_concurrency()) {
        dftVal.maxThread = thread::hardware_concurrency() / 2;
        ctlData::getInstance().setDataDftVal(dftVal);
    }
    pool.reset(dftVal.maxThread);	//设置最大同时运行的线程数

    string workPath = "";
    string exeFolderPath = ctlData::getInstance().dftValue.exeFolderPath;
    if (exeFolderPath != "" && true) {			// && 后应为exeFolderPath的检查程序，检查该文件路径是否合法，且文件夹下是否存在所需的.exe文件
        workPath = exeFolderPath;
    }
    for (auto& data : datas) {
        pool.submit_task([data, workPath]() {
            taskProcessFunc(data, workPath);
            });
    }
}

void calculation::resProcess(const InteractionRes& intres, const dataToCalculate& data, bool calSucc, bool calQuit)
{
    std::lock_guard<std::mutex> lock(mtx);
    unordered_map<string, freqin> mapsss;

    if (calSucc) {
        dealedTaskNum.fetch_add(1, std::memory_order_relaxed);
    }
    else {
        failedTaskNum.fetch_add(1, std::memory_order_relaxed);
    }

    if (calQuit == false) {
        for (auto& freq : data.parScanDt.freqs) {
            mapsss.insert_or_assign(to_string(freq.freq), freq);
        }
        string fakeFreq = "";
        if (data.singlePoint) {
            vector<double> temp;
            for (auto& freq : intres.res) {
                temp.push_back(freq.freq);
            }
            if (temp.size() == 2) {
                fakeFreq = to_string(*max_element(temp.begin(), temp.end()));
            }
        }
        std::vector<results::res> arrs;
        for (auto& res : intres.res) {
            results::res resTemp;
            string freqStr = to_string(res.freq);
            if (freqStr == fakeFreq) {
                continue;
            }
            resTemp.freqy = mapsss[freqStr];
            resTemp.params = data.parScanDt.variants;
            resTemp.result = res;
            resTemp.tube = intres.tube;
            resTemp.mag = intres.mag;
            arrs.push_back(resTemp);
        }
        results result = usrData::getInstance().curCalGroup.res;
        for (auto& it : arrs) {
            usrData::getInstance().insertDataRes(it, data.seqName);
        }
        //Helper::getInstance().emitCalProgress(dealedTaskNum, failedTaskNum, taskSum);
    }

    if (dealedTaskNum + failedTaskNum == taskSum) {
        resNameSet();
        if (calQuit == false) {
            usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["calComplete"]));
            //Helper::getInstance().emitCalComplete();
        }
        else {
            usrData::getInstance().promptMesInsert(funcs::mesGen("", calculatePrompt["calQuit"]));
            //Helper::getInstance().emitCalQuit();
        }

    }
}

void calculation::resNameSet()
{
    string prefix = ctlData::getInstance().dftValue.defaultUnitResName;
    results result = usrData::getInstance().curCalGroup.res;
    for (auto& seq : result.reses) {
        if (!seqNames.count(seq.first)) {
            continue;
        }
        for (int i = 0; i < seq.second.size(); ++i) {
            seq.second[i].name = prefix + "_" + to_string(i + 1);
        }
        result.BGDatas[seq.first].resWaitToImport = false;
    }
    usrData::getInstance().setDataRes(result);
}
