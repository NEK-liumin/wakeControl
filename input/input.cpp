#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// �������ڷָ��ַ���
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// �������ڽ��ַ���ת��Ϊdouble
double stringToDouble(const std::string& s) {
    return std::stod(s);
}

// �������ڽ��ַ���ת��Ϊint
int stringToInt(const std::string& s) {
    return std::stoi(s);
}

int Input::readFile()
{
    std::ifstream file("turbinesInfo.csv");
    if (!file.is_open())
    {
        std::cerr << "�޷��򿪷�������ļ�" << std::endl;
        return 1;
    }
    std::string line;
    
    // ��ȡ�������
    std::getline(file, line);
    std::getline(file, line);
    turbines->turbNum = std::stoi(line);

    // ��ȡ�����������
    std::getline(file, line);
    std::getline(file, line);
    turbines->turbTypeNum = std::stoi(line);
    // ��ȡ���ٽ�������
    std::getline(file, line);
    std::getline(file, line);
    turbines->uNum = std::stoi(line);
    // ��ȡ��С��������
    std::getline(file, line);
    std::getline(file, line);
    turbines->uMin = std::stoi(line);
    std::getline(file, line);
    std::getline(file, line);
    turbines->uMax = std::stoi(line);

    // ����С���֮��ķ���ƽ���ֳ�uNum-1�ݣ�������uWind����
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

    // ��ȡÿ�������λ�á�Ҷ��ֱ�����������
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

    //��ȡÿ�ַ���������Ĺ������ߺ���������
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