#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// 函数用于分割字符串
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// 函数用于将字符串转换为double
double stringToDouble(const std::string& s) {
    return std::stod(s);
}

// 函数用于将字符串转换为int
int stringToInt(const std::string& s) {
    return std::stoi(s);
}

int Input::readFile()
{
    std::ifstream file("turbinesInfo.csv");
    if (!file.is_open())
    {
        std::cerr << "无法打开风机配置文件" << std::endl;
        return 1;
    }
    std::string line;
    
    // 读取风机数量
    std::getline(file, line);
    std::getline(file, line);
    turbines->turbNum = std::stoi(line);

    // 读取风机类型数量
    std::getline(file, line);
    std::getline(file, line);
    turbines->turbTypeNum = std::stoi(line);
    // 读取风速阶梯数量
    std::getline(file, line);
    std::getline(file, line);
    turbines->uNum = std::stoi(line);
    // 读取最小、最大风速
    std::getline(file, line);
    std::getline(file, line);
    turbines->uMin = std::stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    turbines->uMax = std::stoi(line);

    // 将最小最大之间的风速平均分成uNum-1份，并存入uWind数组
    getZeroColumn(turbines->uWind, turbines->uNum);
    for (int i = 0; i < turbines->uNum; ++i)
    {
        turbines->uWind[i] = turbines->uMin + (turbines->uMax - turbines->uMin) / (turbines->uNum - 1) * i;
    }

    turbines->x0.resize(turbines->turbNum);
    turbines->y0.resize(turbines->turbNum);
    turbines->z0.resize(turbines->turbNum);
    turbines->D.resize(turbines->turbNum);
    turbines->turbType.resize(turbines->turbNum);

    // 读取每个风机的位置、叶轮直径、风机种类
    std::getline(file, line);
    for (int i = 0; i < turbines->turbNum; ++i)
    {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string value;
        
        std::getline(ss, value, ',');
        turbines->x0[i] = std::stod(value);

        std::getline(ss, value, ',');
        turbines->y0[i] = std::stod(value);

        std::getline(ss, value, ',');
        turbines->z0[i] = std::stod(value);

        std::getline(ss, value, ',');
        turbines->D[i] = std::stod(value);

        std::getline(ss, value);
        turbines->turbType[i] = std::stod(value);
    }

    //读取每种风力发电机的功率曲线和推力曲线
    turbines->Cp.resize(turbines->turbTypeNum);
    turbines->Ct.resize(turbines->turbTypeNum);

    std::getline(file, line);
    for (int i = 0; i < turbines->turbTypeNum; ++i)
    {
        turbines->Cp[i].resize(turbines->uNum);
        turbines->Ct[i].resize(turbines->uNum);

        for (int j = 0; j < 2; ++j)
        {
            std::getline(file, line);
        }
        for (int j = 0; j < turbines->uNum; ++j)
        {
            std::getline(file, line);
            std::stringstream ss(line);
            std::string value;

            std::getline(ss, value, ',');

            std::getline(ss, value, ',');
            turbines->Cp[i][j] = std::stod(value);

            std::getline(ss, value);
            turbines->Ct[i][j] = std::stod(value);
        }
    }
	return 0;
}