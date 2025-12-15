#include "funcsTwo.h"

vector<double> funcsTwo::fixedNumEvenSeg(double start, double end, int num)
{
    if (end<=start) {
        return vector<double>(1, start);
    }
    if (num <= 0) {
        return { start,end };
    }
    vector<double> res(1, start);
    double val = start;
    double delta = (end - start) / num;

    for (int i = 0; i < num-1; ++i) {
        res.push_back(val + delta);
        val = res.back();
    }
    res.push_back(end);
    return res;
}

vector<double> funcsTwo::fixedLenEvenSeg(double start, double end, double length)
{
    if (end <= start) {
        return vector<double>(1,start);
    }
    if (length <= 0) {
        return { start,end };
    }
    vector<double> res;
    for (int i = 0; start + length * i < end- ctlData::getInstance().dftValue.doubleEpsilon; ++i) {
        res.push_back(start + length * i);
    }
    res.push_back(end);
    return res;
}

vector<double> funcsTwo::linearDistri(double value, int num, bool flag, double start)
{
    vector<double>res;
    if (value <= 0 || num <= 0 || start > value) {
        return vector<double>();
    }
    if (num == 1) {
        return {value};
    }
    if (start * num > value) {
        flag = !flag;
    }
    if (isnan(start)) {
        ++num;
        start = 0;
    }
    else {
        res.push_back(start);
    }
    int divisor = 0;
    for (int i = 1; i < num; ++i) {
        divisor += i;
    }
    double interval = (value - start* num) / divisor;
    for (int i = 1; i < num; ++i) {
        res.push_back(start + interval* i);
        //start = res.back();
    }
    if (!flag) {
        reverse(res.begin(), res.end());
    }
    return res;
}

vector<double> funcsTwo::defaultDistriWrapper(double value, int num)
{
    return funcsTwo::linearDistri(value, num, true, NAN);
}
