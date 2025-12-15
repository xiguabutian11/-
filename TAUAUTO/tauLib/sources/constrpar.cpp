#include"tauLib/headers/constpar.h"
#include <iostream>
std::string dtos(double d)
{
    stringstream os;
    if (os << setprecision(15) << d)
    {
        return os.str();
    }
    return to_string(d);
}

void string2tchar(std::string& src, TCHAR* buf)
{
#ifdef UNICODE  
    swprintf(buf, MAX_PATH, TEXT("%S"), src.c_str());//%S宽字符  
#else  
    _stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());//%s单字符
#endif
}

const std::string currentDateTime(int style) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    if (style == 0) {
        strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
    }
    else {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    }
    
    return buf;
}

bool isnum(std::string s)
{
    if (s.size() == 0) return false;
    for (unsigned int i = 0; i < s.size(); i++)
    {
        int dotflag = 0;
        if (int(s[i]) > 57 || int(s[i]) < 48)
        {
            if (int(s[i]) == 32 && (i == 0 || s[i - 1] == ' '))
            {
                continue;
            }
            else if (int(s[i]) == 46 && dotflag == 0)
            {
                dotflag = 1;
                continue;
            }
            else if (int(s[i]) == 45 && (i == 0 || (i < s.size() - 1 && int(s[i - 1]) == 69)))
            {
                continue;
            }
            else if (int(s[i]) == 69 && i < s.size() - 1 && i>0 && s.find(".", i) == s.npos)
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

//bool isNum(std::string s)
//{
//    char* end = 0;
//    double val = strtod(s.c_str(), &end);
//    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
//}

ifstream& seek_to_line(ifstream& in, int line)
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

double*** arry3(int x, int y, int z,double data)
{
    double*** name;//创建三重指针
    name = new double** [x];//为第一维分配空间
    for (int i = 0; i < x; i++)
    {
        name[i] = new double* [y];//为第二维分配空间
        for (int j = 0; j < y; j++)
            name[i][j] = new double[z];//为第三维分配空间
    }
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
            for (int k = 0; k < z; k++)
                name[i][j][k] = data;
                ;//初始化
    /*for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            for (int k = 0; k < z; k++)
            {
                cout << *(*(*(name + i) + j) + k);//输出检验
            }
            cout << endl;
        }
        cout << endl << endl;
    }*/
    return name;
}
int arrydelete(double*** ary,int x, int y )
{
   
    for (int i = 0; i <x; i++)//释放内存空间    
        for (int j = 0; j < y; j++)
            delete[]ary[i][j];
    for (int i = 0; i < x; i++)
        delete[]ary[i];
    delete[]ary;
    return 0;
}

double** arry2(int x, int y, double data)
{
    double** name;//创建三重指针
    name = new double* [x];//为第一维分配空间
    for (int i = 0; i < x; i++)
    {
        name[i] = new double[y];//为第二维分配空间
    }
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
                name[i][j] = data;
    // *(*(name + i) + j) = data;
    ;//初始化
   /* for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
           
                cout << *(*(name + i) + j);//输出检验
           
        }
        cout << endl << endl;
    }*/
    return name;
}
int arrydelete(double** ary, int x)
{
    for (int i = 0; i < x; i++)
        delete[]ary[i];
    delete[]ary;
    return 0;
}

double* arry1(int x, double data)
{
    double *name= new double[x];//为第二维分配空间

     for (int i = 0; i < x; i++)
        name[i] = data;
     /*for (int i = 0; i < x; i++)
     {
         cout << *(name + i);//输出检验
     }
     cout << endl;*/
     return name;
}
int arrydelete(double* ary)
{
    delete[]ary;
    return 0;
}
