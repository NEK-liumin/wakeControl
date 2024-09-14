#include "template.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <matrixOperation.h>
#include "turbCloud.h"
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

int generateTemplate(TurbCloud& turbines)
{
	// 假设一共36台风机
	// 假设一共三种风机
	// turbines = TurbCloud(36, 3, 10, 0, 20);

	// 假设风力发电机叶轮直径150m
	// 假设风力发电机间隔500m
	// 假设风力发电机呈举行网格排布
	//for (int i = 0; i < turbines.turbNum; ++i)
	//{
	//	turbines.D[i] = 150;
	//}

	ofstream outFile("turbinesInfoTemp.csv");

	if (!outFile)
	{
		std::cerr << "Cannot Open File!" << std::endl;
		return 1;
	}
	outFile << "Wind Turbine Number" << endl;
	outFile << turbines.turbNum << endl;
	outFile << "Type Number" << endl;
	outFile << turbines.turbTypeNum << endl;
	outFile << "Wind Step Number" << endl;
	outFile << turbines.uNum << endl;
	outFile << "Min Wind Speed" << endl;
	outFile << turbines.uMin << endl;
	outFile << "Max Wind Speed" << endl;
	outFile << turbines.uMax << endl;
	outFile << "x,y,z,D,type" << endl;

	for (int i = 0; i < turbines.turbNum; i++)
	{
		outFile << turbines.x0[i] << " , " << turbines.y0[i] << " , " << turbines.z0[i] << " , " << turbines.D[i] << " , " << turbines.turbType[i] << endl;
	}
	outFile << "Pc and Ct" << endl;

	for (int i = 0; i < turbines.turbTypeNum; ++i)
	{
		outFile << "Wind Turbine Type " << i << endl;
		outFile << "Wind Speed,Pc,Ct" << endl;
		for (int j = 0; j < turbines.uNum; ++j)
		{
			outFile << turbines.uWind[j] << "," << turbines.Cp[i][j] << "," << turbines.Ct[i][j] << endl;
		}
	}

	outFile.close();
	return 0;
}