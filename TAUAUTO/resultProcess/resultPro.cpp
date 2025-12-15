#include "resultPro.h"
#include "filesReadAndWrite\fileReading.h"

void resPro::removeRes(string seqName, string unitResName, unsigned int flag)
{
    results result = usrData::getInstance().curCalGroup.res;
    if (result.reses.find(seqName) == result.reses.end()) {
        return;
    }
    for (auto it = result.reses[seqName].begin(); it != result.reses[seqName].end();) {
        if (it->name == unitResName) {
            it = result.reses[seqName].erase(it);
        }
        else {
            ++it;
        }

    }
    usrData::getInstance().setDataRes(result);
}

bool resPro::resExtraction(string seqName, map<string, parStateInResExt> varis, string extractedResName)
{
    freqin freq;
    map<string, double> params;
    unsigned int flag = 0;
    results::resExtractedResult res;
    results result = usrData::getInstance().curCalGroup.res;

    if (!beforeResExtCheck(seqName, varis)) {
        return false;
    }
    res.seqName = seqName;
    for (auto& it : varis) {
        if (it.second.parState == resParState::argument) {
            res.arguments.insert(it.first);
            continue;
        }
        res.fixedValPars.insert_or_assign(it.first, it.second.value);
        if (parScanFixedItem.count(it.first) == 0) {
            params.insert_or_assign(it.first, it.second.value);
        }
        else {
            if (it.first == parScanFixedItem["frequency"]) {
                flag = 1 << 3 | flag;
                freq.freq = it.second.value;
                continue;
            }
            if (it.first == parScanFixedItem["pin"]) {
                flag = 1 << 2 | flag;
                freq.pin = it.second.value;
                continue;
            }
            if (it.first == parScanFixedItem["pin_1"]) {
                flag = 1 << 1 | flag;
                freq.pin1 = it.second.value;
                continue;
            }
            if (it.first == parScanFixedItem["pin_2"]) {
                flag = 1 << 0 | flag;
                freq.pin2 = it.second.value;
            }
        }
    }
    for (auto& it : result.reses[seqName]) {
        if (res.arguments.size() == 1) {
            double feedback = qualifiedCheck(it, freq, params, flag, *res.arguments.begin());
            if (feedback != -1) {
                res.result.insert_or_assign(feedback, it);
            }

        }
    }
    if (res.result.size() != 0) {
        usrData::getInstance().insertDataResExtraction(res, seqName, extractedResName);
        return true;
    }

    return false;
}


void resPro::removeExtedRes(string seqName, string extractedResName)
{
    results res = usrData::getInstance().curCalGroup.res;
    res.extraction.at(seqName).erase(extractedResName);
    usrData::getInstance().setDataRes(res);
}

void resPro::unitResSetImport(string seqName)
{
    auto& data = usrData::getInstance().curCalGroup;
    results reses = data.res;
    if (reses.BGDatas[seqName].resWaitToImport) {
        filesystem::path seqPath(data.calG.directory);
        seqPath = seqPath / data.calG.resFilePath / seqName;
        fileReading::unitResSetRead(reses.reses[seqName], seqPath.string());

        reses.BGDatas[seqName].resWaitToImport = false;
        usrData::getInstance().setDataRes(reses);
    }
}


bool resPro::beforeResExtCheck(string seqName, map<string, parStateInResExt>& varis)
{
    results result = usrData::getInstance().curCalGroup.res;
    unsigned int argNums = 0;
    int bandNum = 0;
    set<string> missingPar = {};
    calSetting::calSequence calSet;

    if (result.reses.count(seqName) == 0 || result.BGDatas.count(seqName) == 0) {
        return false;
    }
    calSet = result.BGDatas[seqName].calS;
    if (calSet.seq.count(parScanFixedItem["pin"]) && calSet.seq.at(parScanFixedItem["pin"]).signNum) {
        varis.erase(parScanFixedItem["pin"]);
        ++bandNum;
    }
    if (calSet.seq.count(parScanFixedItem["pin_1"]) && calSet.seq.at(parScanFixedItem["pin_1"]).signNum) {
        varis.erase(parScanFixedItem["pin_1"]);
        ++bandNum;
    }
    if (calSet.seq.count(parScanFixedItem["pin_2"]) && calSet.seq.at(parScanFixedItem["pin_2"]).signNum) {
        varis.erase(parScanFixedItem["pin_2"]);
        ++bandNum;
    }


    if (calSet.seq.size() - bandNum != varis.size()) {
        return false;
    }

    for (auto& it : varis) {
        if (calSet.seq.count(it.first) == 0) {
            return false;
        }
        if (it.second.parState == resParState::argument) {
            if (argNums >= ctlData::getInstance().dftValue.maxArgsOfRes) {
                it.second.parState = resParState::fixedVal;
                it.second.value = calSet.seq.at(it.first).array.size() > 0 ? calSet.seq.at(it.first).array[0] : 0;
            }
            else {
                ++argNums;
            }
        }
    }
    if (argNums == 0) {
        return false;
    }
    return true;

}

double resPro::qualifiedCheck(const results::res& res, const freqin& freq, const map<string, double>& params, unsigned int flag, string arg)
{
    bool checkFlag = true;

    for (auto& par : res.params) {
        if (par.first != arg && (params.count(par.first) == 0 || params.at(par.first) != par.second)) {
            return -1;
        }
    }
    checkFlag = ((flag >> 3 & 1) ? res.freqy.freq == freq.freq : true) &&
        ((flag >> 2 & 1) ? res.freqy.pin == freq.pin : true) &&
        ((flag >> 1 & 1) ? res.freqy.pin1 == freq.pin1 : true) &&
        ((flag >> 0 & 1) ? res.freqy.pin2 == freq.pin2 : true);

    if (checkFlag) {
        if (parScanFixedItem.count(arg)) {
            if (arg == parScanFixedItem["frequency"]) {
                return res.freqy.freq;
            }
            if (arg == parScanFixedItem["pin"]) {
                return res.freqy.pin;
            }
            if (arg == parScanFixedItem["pin_1"]) {
                return res.freqy.pin1;
            }
            if (arg == parScanFixedItem["pin_2"]) {
                return res.freqy.pin2;
            }
        }
        else {
            return res.params.at(arg);
        }
    }

    return -1;
}



