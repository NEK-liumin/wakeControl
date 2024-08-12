#include "turbCloudGeneration.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <matrixOperation.h>
#include "turbCloud.h"
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

int generateTestDate(TurbCloud& turbines)
{
// 假设一共36台风机
// 假设一共三种风机
	turbines = TurbCloud(36, 3, 10, 0, 20);

	// 假设风力发电机叶轮直径150m
	// 假设风力发电机间隔500m
	// 假设风力发电机呈举行网格排布
	for (int i = 0; i < turbines.turbNum; ++i)
	{
		turbines.D[i] = 150;
	}
	int m = 6;
	int n = 6;

	int id;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			id = i * n + j;
			turbines.x0[id] = i * 500;
			turbines.y0[id] = j * 500;
			turbines.z0[id] = 0;
			if ((i + j) % 3 == 0) // 随便弄得，把风机分成三种
			{
				turbines.turbType[id] = 0;
			}
			else if((i + j) % 3 == 1)
			{
				turbines.turbType[id] = 1;
			}
			else
			{
				turbines.turbType[id] = 2;
			}
		}
	}

	
	ofstream outFile("turbinesInfo.csv");

	if (!outFile)
	{
		std::cerr << "无法打开文件" << std::endl;
		return 1;
	}
	outFile << "台数" << endl;
	outFile << turbines.turbNum << endl;
	outFile << "类型数" << endl;
	outFile << turbines.turbTypeNum << endl;
	outFile << "风速阶梯数量" << endl;
	outFile << turbines.uNum << endl;
	outFile << "最小风速" << endl;
	outFile << turbines.uMin << endl;
	outFile << "最大风速" << endl;
	outFile << turbines.uMax << endl;
	outFile << "x,y,z,D,type" << endl;

	for (int i = 0; i < turbines.turbNum; i++)
	{
		outFile << turbines.x0[i] << " , " << turbines.y0[i] << " , " << turbines.z0[i] << " , " << turbines.D[i] << " , " << turbines.turbType[i] << endl;
	}
	outFile << "Cp、Ct曲线" << endl;
	
	for (int i = 0; i < turbines.turbTypeNum; ++i)
	{
		outFile << "第 " << i << " 种风机" << endl;
		outFile << "风速,Cp,Ct" << endl;
		for (int j = 0; j < turbines.uNum; ++j)
		{
			outFile << turbines.uWind[j] << "," << turbines.Cp[i][j] << "," << turbines.Ct[i][j] << endl;
		}
	}

	outFile.close();
	return 0;
}