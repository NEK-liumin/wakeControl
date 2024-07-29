#include "turbCloud.h"
#include <iostream>
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

int TurbCloud::setCp(int uNum, Matrix& uWind, Matrix& Cp)
{
	for (int i = 0; i < turbNum; ++i)
	{
		this->uNum[i] = uNum;
	}
	this->uWind = uWind;
	this->Cp = Cp;
	return 0;
}
int TurbCloud::setCp(vector<int> uNum, Matrix& uWind, Matrix& Cp)
{
	this->uNum = uNum;
	this->uWind = uWind;
	this->Cp = Cp;
	return 0;
}

int TurbCloud::getCp(double& cp_i, double& velo, int i)
{
	if (velo < uWind[i][0] || velo > uWind[i][uNum[i] - 1])
	{
		cout << "风机无法在该风速下进行工作" << endl;
		return 0;
	}
	int j;
	for (j = 0; j < uNum[i] - 1; ++j)
	{
		if (velo >= uWind[i][j] && velo <= uWind[i][j + 1])
		{
			break;
		}
	}
	double delta1 = velo - uWind[i][j];
	double delta2 = uWind[i][j + 1] - velo;
	cp_i = (Cp[i][j] * delta2 + Cp[i][j + 1] * delta1) / (delta1 + delta2);
	return 0;
}