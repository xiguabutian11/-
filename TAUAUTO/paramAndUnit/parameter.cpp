#include "parameter.h"

bool parameter::addParam(parametersList::parameter par)
{
    parametersList pars = usrData::getInstance().pars;
    if (!funcs::isValidIdentifier(par.name)) {
        string obj = parameterObj["par"] + " -- " + par.name;
        usrData::getInstance().warnMesInsert(funcs::mesGen(obj, parameterPrompt["parNameWrong"]));
        return false;
    }
    if (!pars.paramList.count(par.name)) {
        pars.orders.push_back(par.name);
    }
    pars.paramList.insert_or_assign(par.name, par);

    usrData::getInstance().setDataPars(pars);
    paramStateUpdate();
    return true;
}

bool parameter::removeParam(string par)
{
    parametersList pars = usrData::getInstance().pars;
    if (pars.paramList.count(par)) {
        string obj = parameterObj["par"] + " -- " + par+" ( "+ pars.paramList[par].expression + " )";
        pars.paramList.erase(par);
        for (auto it = pars.orders.begin(); it != pars.orders.end();) {
            if (*it == par) {
                it = pars.orders.erase(it);
                break;
            }
            ++it;
        }
        usrData::getInstance().setDataPars(pars);
        paramStateUpdate();
        usrData::getInstance().promptMesInsert(funcs::mesGen(obj, parameterPrompt["parDeleted"]));
    }
    return true;
}

void parameter::paramLinkFunction(map<string, double>& variants, InteractionParams& intpar)
{
    paramLinks linkData = usrData::getInstance().curCalGroup.parLink;
    if (linkData.fileToUpdate.empty()) {
        for (auto& file : parLinkDataTypes) {
            linkData.fileToUpdate.insert(file.second);
        }
    }
    if (linkData.fileToUpdate.count(parLinkDataTypes["tube"])) {
        tubeDataParLinkAssign(linkData.parLinks[parLinkDataTypes["tube"]], variants, intpar);
    }
    if (linkData.fileToUpdate.count(parLinkDataTypes["mag"])) {
        magDataParLinkAssign(linkData.parLinks[parLinkDataTypes["mag"]], variants, intpar);
    }
    if (linkData.fileToUpdate.count(parLinkDataTypes["beam"])) {
        beamDataParLinkAssign(linkData.parLinks[parLinkDataTypes["beam"]], variants, intpar);
    }
    if (linkData.fileToUpdate.count(parLinkDataTypes["loss"])) {
        lossDataParLinkAssign(linkData.parLinks[parLinkDataTypes["loss"]], variants, intpar);
    }
    linkData.fileToUpdate.clear();
    usrData::getInstance().setDataParLink(linkData);
}

bool parameter::parScanableCheck(string name, set<string> linkedParam)
{
    if (linkedParam.empty()) {
        linkedParam = scanableParameters();
    }
    return linkedParam.count(name);
}

set<string> parameter::scanableParameters()
{
    //paramStateUpdate();
    set<string> parScanable = ctlData::getInstance().dftValue.parScanType;
    set<string> linkedParam;
    const usrData& data = usrData::getInstance();

    for (auto& it : data.curCalGroup.parLink.parLinks) {
        if (parScanable.count(it.first) == 0) {
            continue;
        }
        for (auto& par : it.second) {
            funcs::dependentVaris(par.second.expression, linkedParam);
        }
    }
    for (auto& name : linkedParam) {
        if (data.pars.paramList.count(name) == 0 || data.pars.paramList.at(name).state != paramState::available) {
            linkedParam.erase(name);
        }
    }
    return linkedParam;
}

void parameter::paramStateUpdate()
{
    parametersList pars = usrData::getInstance().pars;
    set<string> parNames(pars.orders.begin(), pars.orders.end());
    for (auto& it : pars.paramList) {
        it.second.state = paramState::unchecked;

        if (!parNames.count(it.first)) {
            pars.orders.push_back(it.first);
        }
    }
    set<string> repeteCheck;
    for (auto it = pars.orders.begin(); it != pars.orders.end(); ) {
        if (!pars.paramList.count(*it) || repeteCheck.count(*it)) {
            it = pars.orders.erase(it);  // 删除当前元素并更新迭代器
        }
        else {
            repeteCheck.insert(*it);
            ++it;  // 迭代到下一个元素
        }
    }
    for (auto& it : pars.paramList) {
        if (it.second.state == paramState::unchecked) {
            dfsDepVarsCheck(it.first, pars);
        }
    }
    for (auto& it : pars.paramList) {
        if (it.second.state == paramState::available) {
            for (auto& dep : it.second.dependentVariables) {
                pars.paramList[dep].dependee.insert(it.first);
            }
        }
    }
    usrData::getInstance().setDataPars(pars);
}

void parameter::paramLinkStateUpdate()
{
    map<string, double> variants;
    paramLinks data = usrData::getInstance().curCalGroup.parLink;
    const parametersList& pars = usrData::getInstance().pars;
    InteractionParams intpar;

    paramStateUpdate();
    for (auto& it : pars.paramList) {
        if (it.second.state == paramState::available) {
            variants.insert(pair<string, double>(it.first, it.second.value));
        }
    }
    
    for (auto& it : parLinkDataTypes) {
        data.fileToUpdate.insert(it.second);
    }
    usrData::getInstance().setDataParLink(data);

    paramLinkFunction(variants, intpar);

}

double parameter::exprSoluable(string expr)
{
    //paramStateUpdate();
    parametersList pars = usrData::getInstance().pars;

    set<string> variants;
    map<string, double> varis;
    funcs::dependentVaris(expr,variants);
    for (auto& it : variants) {
        if (pars.paramList.count(it) == 0 || pars.paramList[it].state != paramState::available) {
            return NAN;
        }
        varis[it] = pars.paramList[it].value;
    }

    return funcs::expressionSolve(expr,varis);
}



bool parameter::tubeDataParLinkAssign(map<string,paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar)
{
    set<string> dependencies;
    for (auto& it : parLinks) {
        dependencies.clear();
        funcs::dependentVaris(it.second.expression, dependencies);
        if (dependencies.empty()) {
            it.second.linkable = false;
            continue;
        }
        double value = funcs::expressionSolve(it.second.expression, variants);
        if (isnan(value)) {
            it.second.linkable = false;
            continue;
        }
        switch (it.second.index1)
        {
        case 1:
            intpar.V = value;
            break;
        case 2:
            intpar.I = value;
            break;
        case 3:
            intpar.tubelength = value;
            break;
        case 4:
            intpar.interstep = value;
            break;
        //case 5:
        //    intpar.harmonicses = static_cast<int>(value);
        //    break;
        //case 6:
        //    intpar.NumofElectrons = static_cast<int>(value);
        //    break;
        default:
            it.second.linkable = false;
            break;
        }
    }
    return true;
}

bool parameter::magDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar)
{
    set<string> dependencies;
    for (auto& it : parLinks) {
        dependencies.clear();
        funcs::dependentVaris(it.second.expression, dependencies);
        if (dependencies.empty()) {
            it.second.linkable = false;
            continue;
        }
        double value = funcs::expressionSolve(it.second.expression, variants);
        if (isnan(value)) {
            it.second.linkable = false;
            continue;
        }
        switch (it.second.index1)
        {
        case 1:
        {
            if (it.second.index2 >= intpar.data_mag.size() || it.second.index2 < 0) {
                it.second.linkable = false;
            }
            else {
                switch (it.second.index3)
                {
                case 1:
                    intpar.data_mag[it.second.index2].begin = value;
                    break;
                case 2:
                    intpar.data_mag[it.second.index2].period = value;
                    break;
                case 3:
                    intpar.data_mag[it.second.index2].m0 = value;
                    break;
                case 4:
                    intpar.data_mag[it.second.index2].m1 = value;
                    break;
                case 5:
                    intpar.data_mag[it.second.index2].m2 = value;
                    break;
                default:
                    it.second.linkable = false;
                    break;
                }
            }
            break;
        }
        default:
            it.second.linkable = false;
            break;
        }
    }
    return true;
}

bool parameter::beamDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar)
{
    set<string> dependencies;
    for (auto& it : parLinks) {
        dependencies.clear();
        funcs::dependentVaris(it.second.expression, dependencies);
        if (dependencies.empty()) {
            it.second.linkable = false;
            continue;
        }
        double value = funcs::expressionSolve(it.second.expression, variants);
        if (isnan(value)) {
            it.second.linkable = false;
            continue;
        }
        switch (it.second.index1)
        {
        case 1:
            intpar.beamdata.Rpvn = value;
            break;
        case 2:
            intpar.beamdata.Rpna = value;
            break;
        case 3:
            intpar.beamdata.swsr = value;
            break;
        /*case 4:
            if (it.second.index2 >= intpar.beamdata.beamrate.size() || it.second.index2 < 0) {
                it.second.linkable = false;
            }
            else {
                intpar.beamdata.beamrate[it.second.index2] = value;
            }
            break;
        case 5:
            if (it.second.index2 >= intpar.beamdata.vr.size() || it.second.index2 < 0) {
                it.second.linkable = false;
            }
            else {
                intpar.beamdata.vr[it.second.index2] = value;
            }
            break;*/
        default:
            it.second.linkable = false;
            break;
        }
    }
    return true;
}

bool parameter::lossDataParLinkAssign(map<string, paramLinks::parLink>& parLinks, map<string, double>& variants, InteractionParams& intpar)
{
    set<string> dependencies;
    for (auto& it : parLinks) {
        dependencies.clear();
        funcs::dependentVaris(it.second.expression, dependencies);
        if (dependencies.empty()) {
            it.second.linkable = false;
            continue;
        }
        double value = funcs::expressionSolve(it.second.expression, variants);
        if (isnan(value)) {
            it.second.linkable = false;
            continue;
        }
        switch (it.second.index1)
        {
        case 1:
            if (it.second.index2 >= intpar.data_drft.size() || it.second.index2 < 0) {
                it.second.linkable = false;
            }
            else {
                switch (it.second.index3)
                {
                case 1:
                    intpar.data_drft[it.second.index2].begin = value;
                    break;
                case 2:
                    intpar.data_drft[it.second.index2].end = value;
                    break;
                default:
                    it.second.linkable = false;
                    break;
                }
            }
            break;
        case 2:
            if (it.second.index2 >= intpar.data_loss.size() || it.second.index2 < 0) {
                it.second.linkable = false;
            }
            else {
                switch (it.second.index3)
                {
                case 1:
                    intpar.data_loss[it.second.index2].begin = value;
                    break;
                case 2:
                    intpar.data_loss[it.second.index2].end = value;
                    break;
                default:
                    it.second.linkable = false;
                    break;
                }
            }
            break;
        default:
            it.second.linkable = false;
            break;
        }
    }
    return true;
}

double parameter::dfsDepVarsCheck(string par, parametersList& pars)
{
    if (pars.paramList.count(par) == 0) {
        string obj = parameterObj["par"] + " -- " + par;
        usrData::getInstance().warnMesInsert(funcs::mesGen(obj, parameterPrompt["parNoExist"]));
        return NAN;
    }
    if (pars.paramList[par].state == paramState::available) {
        return pars.paramList[par].value;
    }
    if (pars.paramList[par].state == paramState::unavailable) {
        return NAN;
    }
    pars.paramList[par].state = paramState::checking;
    if (!funcs::isValidIdentifier(par)) {
        string obj = parameterObj["par"] + " -- " + par;
        usrData::getInstance().warnMesInsert(funcs::mesGen(obj, parameterPrompt["parNameWrong"]));
        pars.paramList[par].state = paramState::unavailable;
        pars.paramList[par].errMessage = parameterPrompt["parNameWrong"];
        return NAN;
    }

    
    funcs::dependentVaris(pars.paramList[par].expression, pars.paramList[par].dependentVariables);

    if (pars.paramList[par].dependentVariables.size() == 0) {
        pars.paramList[par].independent = true;
    }
    else {
        pars.paramList[par].independent = false;
    }

    map<string, double> variants;
    for (auto& it : pars.paramList[par].dependentVariables) {
        //当par依赖的变量不存在时，par不可用，抛出异常
        if (pars.paramList.count(it) == 0) {
            string obj = parameterObj["par"] + " -- " + par;
            usrData::getInstance().warnMesInsert(funcs::mesGen(obj, parameterPrompt["depVarisNoExist"]));
            pars.paramList[par].errMessage = parameterPrompt["depVarisNoExist"];
            pars.paramList[par].state = paramState::unavailable;
            return NAN;
        }
        //当par依赖的变量出现循环依赖时，par可用，但强制表达式置为0，设置为独立变量
        if (pars.paramList[it].state == paramState::checking) {
            pars.paramList[par].state = paramState::available;
            pars.paramList[par].expression = "0";
            pars.paramList[par].independent = true;
            pars.paramList[par].value = 0;
            return 0;
        }
        variants[it] = dfsDepVarsCheck(it, pars);
    }
    double res = funcs::expressionSolve(pars.paramList[par].expression, variants);

    if (isnan(res)) {
        string obj = parameterObj["par"] + " -- " + par;
        usrData::getInstance().warnMesInsert(funcs::mesGen(obj, parameterPrompt["unresolveExpr"]));
        pars.paramList[par].errMessage = parameterPrompt["unresolveExpr"];
        pars.paramList[par].state = paramState::unavailable;
        return NAN;
    }

    pars.paramList[par].state = paramState::available;
    pars.paramList[par].value = res;
    pars.paramList[par].errMessage = "";
    return res;
}

