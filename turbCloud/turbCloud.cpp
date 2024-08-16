﻿#include "turbCloud.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <iomanip>
using std::ofstream;
using std::cout;
using std::endl;

int TurbCloud::setPosi(int turbNum, Column& x0, Column& y0, Column& z0)
{
	if (x0.size() != turbNum || y0.size() != turbNum || z0.size() != turbNum)
	{
		cout << "输入的风机数量与坐标数量不匹配" << endl;
		return 0;
	}
	if (x0.size() != y0.size() || x0.size() != z0.size())
	{
		cout << "xyz坐标的数量不一致" << endl;
		return 0;
	}
	this->turbNum = turbNum;
	this->x0 = x0;
	this->y0 = y0;
	this->z0 = z0;
	return 0;
}

int TurbCloud::setCoef(Matrix& Cp, Matrix& Ct)
{
	this->Cp = Cp;
	this->Ct = Ct;
	return 0;
}

int TurbCloud::getCp(double& cp_i, double& velo_i, int& type_i)
{
	if (velo_i < uWind[0] || velo_i > uWind[uNum - 1])
	{
		cout << "风机无法在该风速下进行工作" << endl;
		return 0;
	}
	if (velo_i == uWind[0])
	{
		cp_i = Cp[type_i][0];
		return 0;
	}
	if (velo_i == uWind[uNum - 1])
	{
		cp_i = Cp[type_i][uNum - 1];
		return 0;
	}
	// 会不断根据风机位置处的流体速度计算其功率系数和推力系数
	// 调用次数较多
	// 不宜采用循环
	double delta;
	delta = (uWind[uNum - 1] - uWind[0]) / (uNum - 1);

	int j;
	j = floor((velo_i - uWind[0]) / delta);

	double delta1 = velo_i - uWind[j];
	double delta2 = uWind[j + 1] - velo_i;
	cp_i = (Cp[type_i][j] * delta2 + Cp[type_i][j + 1] * delta1) / delta;
	return 0;
}

int TurbCloud::getCt(double& ct_i, double& velo_i, int& type_i)
{
	if (velo_i < uWind[0] || velo_i > uWind[uNum - 1])
	{
		cout << "风机无法在该风速下进行工作" << endl;
		return 0;
	}
	if (velo_i == uWind[0])
	{
		ct_i = Ct[type_i][0];
		return 0;
	}
	if (velo_i == uWind[uNum - 1])
	{
		ct_i = Ct[type_i][uNum - 1];
		return 0;
	}
	// 会不断根据风机位置处的流体速度计算其功率系数和推力系数
	// 调用次数较多
	// 不宜采用循环
	double delta;
	delta = (uWind[uNum - 1] - uWind[0]) / (uNum - 1);

	int j;
	j = floor((velo_i - uWind[0]) / delta);

	double delta1 = velo_i - uWind[j];
	double delta2 = uWind[j + 1] - velo_i;
	ct_i = (Ct[type_i][j] * delta2 + Ct[type_i][j + 1] * delta1) / delta;
	return 0;
}

int TurbCloud::turbPrint()
{
	ofstream outFile("turbinesInfo_check.csv");

	if (!outFile)
	{
		std::cerr << "无法打开文件" << std::endl;
		return 1;
	}
	outFile << "台数" << endl;
	outFile << this ->turbNum << endl;
	outFile << "类型数" << endl;
	outFile << this->turbTypeNum << endl;
	outFile << "风速阶梯数量" << endl;
	outFile << this->uNum << endl;
	outFile << "最小风速" << endl;
	outFile << this->uMin << endl;
	outFile << "最大风速" << endl;
	outFile << this->uMax << endl;
	outFile << "x,y,z,D,type" << endl;

	for (int i = 0; i < this->turbNum; i++)
	{
		outFile << this->x0[i] << " , " << this->y0[i] << " , " << this->z0[i] << " , " << this->D[i] << " , " << this->turbType[i] << endl;
	}
	outFile << "Cp、Ct曲线" << endl;

	for (int i = 0; i < this->turbTypeNum; ++i)
	{
		outFile << "第 " << i << " 种风机" << endl;
		outFile << "风速,Cp,Ct" << endl;
		for (int j = 0; j < this->uNum; ++j)
		{
			outFile << this->uWind[j] << "," << this->Cp[i][j] << "," << this->Ct[i][j] << endl;
		}
	}

	outFile.close();
	return 0;
}