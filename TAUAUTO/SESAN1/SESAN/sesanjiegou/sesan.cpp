#include "sesan.h"

using namespace std;

// 检查文件是否存在
bool pathExists(const string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0);
}

void sesan() {
    //string paramFile = "C:/Users/liu/Desktop/HUZUOYONG/XIGUA/SESAN/input.txt";  // 参数文件路径
    //string workpath = "C:/Users/liu/Desktop/HUZUOYONG/XIGUA/SESAN";    // 工作目录
    string aiDatFile;
    StructParams1 structParams;
    DispersionParams1 dispersionParams;

    // 1. 读取参数文件
    if (!GetStructData1(structParams, paramFile)) {
        cerr << "参数文件读取失败!" << endl;
        
    }
    /*cout << "参数文件读取成功!" << endl;*/

    // 2. 生成 ai.dat 文件
    string timeFlag = "20250317";  // 时间标记
    aiDatFile = WriteEigenParfile1(structParams, workpath, timeFlag);
   /* cout << "ai.dat 文件生成成功: " << aiDatFile << endl;*/

    // 3. 进行数据检查
    if (!EigenDataCheck1(structParams)) {
        cerr << "数据检查失败!" << endl;
      
    }
    /*cout << "数据检查通过!" << endl;*/

    // 4. 进行本征计算并读取结果
    if (!Eigen_file1(dispersionParams, aiDatFile, workpath)) {
        cerr << "本征计算失败!" << endl;
       
    }
    /*cout << "本征计算完成，结果已提取!" << endl;*/

    // 6. 导出结果数据
   /* cout << "开始导出数据..." << endl;*/
    string outputFile = workpath + "/output.txt";
    string BWoutputFile = workpath + "/bwoutput.txt";
   /* cout << "打开文件..." << endl;*/
    WriteData1(dispersionParams, outputFile);
    /*cout << "写入数据结束" << std::endl;*/
    WriteDataBW1(dispersionParams, BWoutputFile);
    /*cout << "数据导出成功: " << outputFile << endl;*/

    // 7. 合并输入参数和输出数据到CSV文件
    string csvpath = workpath + "\\CSV";
    string csvFile = csvpath + "_combined.csv";

    createDirectories(csvpath);

    if (directoryExists(csvpath)) {
        /*cout << "文件夹创建成功: " << csvpath << endl;*/
    }
    else {
        /*cout << "文件夹创建失败" << endl;*/
    }

    if (!pathExists(csvFile)) {
        ofstream createCsvFile(csvFile);
        if (!createCsvFile.is_open()) {
            cerr << "无法创建CSV文件: " << csvFile << endl;
            
        }
        createCsvFile.close();
    }

    ofstream csvOutput;

    // 检查CSV文件是否存在，如果不存在则添加表头
    try {
        // 检查文件是否为空
        bool needHeader = true;
        ifstream checkFile(csvFile);
        if (checkFile.is_open()) {
            checkFile.seekg(0, ios::end);
            if (checkFile.tellg() > 0) {
                needHeader = false;
            }
            checkFile.close();
        }

        // 打开文件进行追加
        csvOutput.open(csvFile, ios::app);
        if (!csvOutput.is_open()) {
            throw runtime_error("无法打开CSV文件进行写入");
        }

        // 如果文件为空，添加表头
        if (needHeader) {
            csvOutput << "a,thi,L,c,del,eps,pol,"
                << "f,vp,phi,kc,alfa\n";
        }

        // 读取输出文件的数据
        ifstream outputData(outputFile);
        if (!outputData.is_open()) {
            throw runtime_error("无法打开输出文件进行读取");
        }

        string line;
        // 跳过表头
        getline(outputData, line);

        // 读取每一行输出数据并与输入参数合并
        while (getline(outputData, line)) {
            stringstream ss(line);
            double f, vp, phi, kc, alfa;
            if (!(ss >> f >> vp >> phi >> kc >> alfa)) {
                cerr << "警告：无法解析行: " << line << endl;
                continue;
            }

            csvOutput << structParams.swsr << ","
                << (structParams.swsro - structParams.swsr) << ","
                << structParams.pitch << ","
                << structParams.c << ","
                << structParams.helixwidth << ","
                << structParams.eps << ","
                << structParams.pol << ","
                << f << ","
                << vp << ","
                << phi << ","
                << kc << ","
                << alfa << "\n";
        }

        outputData.close();
        csvOutput.close();
        /*cout << "数据已合并到CSV文件: " << csvFile << endl;*/

    }
    catch (const exception& e) {
        cerr << "处理CSV文件时发生错误: " << e.what() << endl;
        if (csvOutput.is_open()) csvOutput.close();
        
    }

    
}
