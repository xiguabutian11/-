#include"constpar.h"
std::string dtos1(double d)
{
    stringstream os;
    if (os << setprecision(15) << d)
    {
        return os.str();
    }
    return to_string(d);
}

void string2tchar1(std::string& src, TCHAR* buf)
{
#ifdef UNICODE  
    swprintf(buf, MAX_PATH, TEXT("%S"), src.c_str());//%S宽字符  
#else  
    _stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str());//%s单字符
#endif
}

const std::string currentDateTime1() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
    return buf;
}

bool isnum1(std::string s)
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

ifstream& seek_to_line1(ifstream& in, int line)
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

double*** arry31(int x, int y, int z, double data)
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
int arrydelete1(double*** ary, int x, int y)
{

    for (int i = 0; i < x; i++)//释放内存空间    
        for (int j = 0; j < y; j++)
            delete[]ary[i][j];
    for (int i = 0; i < x; i++)
        delete[]ary[i];
    delete[]ary;
    return 0;
}

double** arry21(int x, int y, double data)
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
int arrydelete1(double** ary, int x)
{
    for (int i = 0; i < x; i++)
        delete[]ary[i];
    delete[]ary;
    return 0;
}

double* arry11(int x, double data)
{
    double* name = new double[x];//为第二维分配空间

    for (int i = 0; i < x; i++)
        name[i] = data;
    /*for (int i = 0; i < x; i++)
    {
        cout << *(name + i);//输出检验
    }
    cout << endl;*/
    return name;
}
int arrydelete1(double* ary)
{
    delete[]ary;
    return 0;
}
