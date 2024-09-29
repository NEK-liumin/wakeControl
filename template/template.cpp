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

	double uBegin = 5, uEnd = 12;
	double thetaBegin = 0, thetaEnd = 350;
	double deltaU=1, deltaTheta=10;
	double ky=0.025, kz= 0.0175;
	double I = 0.12;
	double rho = 1.205;
	double tol = 1e-5;

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

	ofstream parameterFile("parameterTemp.csv");
	if (!parameterFile)
	{
		std::cerr << "Cannot Open File!" << std::endl;
		return 1;
	}
	parameterFile << "Value,Name,Detailed Description" << endl;
	parameterFile << uBegin << ",uBegin,Minimum Wind Speed for Calculating the Yaw Matrix" << endl;
	parameterFile << uEnd << ",uEnd,Maximum Wind Speed for Calculating the Yaw Matrix" << endl;
	parameterFile << thetaBegin << ",thetaBegin,Minimum Wind Direction for Calculating the Yaw Matrix" << endl;
	parameterFile << thetaEnd << ",thetaEnd,Maximum Wind Direction for Calculating the Yaw Matrix" << endl;
	parameterFile << deltaU << ",deltaU, Wind Speed Interval to Generate the Yaw Matrix" << endl;
	parameterFile << deltaTheta << ",deltaTheta, Wind Direction Interval to Generate the Yaw Matrix" << endl;
	parameterFile << ky << ",ky,Wake Width Growth Rate in y Direction" << endl;
	parameterFile << kz << ",kz,Wake Width Growth Rate in z Direction" << endl;
	parameterFile << I << ",I,Turbulence Intensity" << endl;
	parameterFile << rho << ",rho,Air Density" << endl;
	parameterFile << tol << ",tol,Optimal Tolerance" << endl;
	parameterFile.close();
	return 0;
}