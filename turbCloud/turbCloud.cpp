#include "turbCloud.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <iomanip>
using std::ofstream;
using std::cout;
using std::endl;

int TurbCloud::init(int turbNum, int turbTypeNum, int uNum, double uMin, double uMax)
{
	this->turbNum = turbNum;
	this->turbTypeNum = turbTypeNum;
	this->uNum = uNum;
	this->uMin = uMin;
	this->uMax = uMax;
	getZeroColumn(D, turbNum);
	getZeroColumn(x0, turbNum);
	getZeroColumn(y0, turbNum);
	getZeroColumn(z0, turbNum);
	turbType.resize(turbNum);

	getZeroColumn(uWind, uNum);

	for (int i = 0; i < uNum; ++i)
	{
		uWind[i] = uMin + (uMax - uMin) / (uNum - 1) * i;
	}

	getZeroMatrix(Cp, turbTypeNum, uNum);
	getZeroMatrix(Ct, turbTypeNum, uNum);
	return 0;
}
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
	if (velo_i < uWind[0])
	{
		cp_i = velo_i * Cp[type_i][0] / uWind[0];
		return 0;
	}
	if (velo_i > uWind[uNum - 1])
	{
		cp_i = 0;
		return 0;
	}
	if (velo_i == uWind[0])
	{
		cp_i = Cp[type_i][0];
		return 0;
	}
	//cout << "in getcp" << endl;
	//cout << velo_i << endl;
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
		ct_i = 0;
		// cout << "风机无法在该风速下进行工作" << endl;
		return 0;
	}
	if (velo_i == uWind[0])
	{
		ct_i = Ct[type_i][0];
		if (ct_i >= 0.95)
		{
			ct_i = 0.95;
		}
		return 0;
	}
	if (velo_i == uWind[uNum - 1])
	{
		ct_i = Ct[type_i][uNum - 1];
		if (ct_i >= 0.95)
		{
			ct_i = 0.95;
		}
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
	if (ct_i < 0)
	{
		ct_i = 0;
	}
	if (ct_i >= 0.95)
	{
		ct_i = 0.95;
	}
	return 0;
}
int TurbCloud::getPower(double& power, Column& vel, double& rho)
{
	power = 0.0;
	double cp_i;
	for (int i = 0; i < turbNum; ++i)
	{
		getCp(cp_i, vel[i], turbType[i]);
		power += 0.5 * cp_i * rho * (0.25 * PI * D[i] * D[i]) * pow(vel[i] * cos((*gamma)[i]), 3);
	}
	return 0;
}

bool TurbCloud::isYaw(Column& vel, int& i)
{
	double pc_i;
	double vel_axial;
	if (vel[i] < uWind[0])
	{
		pc_i = 0;
	}
	else
	{
		vel_axial = vel[i] * cos((*gamma)[i]);
		getCp(pc_i, vel_axial, turbType[i]);
	}


	if (pc_i > 0)return true;
	else return false;
}

int TurbCloud::getPower(double& power, Column& vel)
{
	power = 0.0;
	double pc_i; // 这里实际上调用的是功率曲线，而不是功率系数曲线
	double vel_axial;
	for (int i = 0; i < turbNum; ++i)
	{
		// 为了能在切入速度处能正常计算，采用以下策略：
		// 1. 如果风机处的风速小于切入速度，当前风机的功率取0；
		// 2. 如果风机处的风速大于等于切入速度，只是风机偏航后，风速的分量小于切入速度
		//    则会让风机的功率在0和切入速度处的功率之间插值
		if (vel[i] < uWind[0])
		{
			power += 0;
		}
		else
		{
			vel_axial = vel[i] * cos((*gamma)[i]);
			getCp(pc_i, vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
			power += pc_i; // 单位是kw
		}
		// 为避免功率不连续，不采用上述策略
		//vel_axial = vel[i] * cos((*gamma)[i]);
		//getCp(pc_i, vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		//power += pc_i; // 单位是kw
		
	}
	return 0;
}

int TurbCloud::getPower(Column& power_i, Column& vel)
{
	double vel_axial;
	power_i.resize(turbNum);
	for (int i = 0; i < turbNum; ++i)
	{
		// 为了能在切入速度处能正常计算，采用以下策略：
		// 1. 如果风机处的风速小于切入速度，当前风机的功率取0；
		// 2. 如果风机处的风速大于等于切入速度，只是风机偏航后，风速的分量小于切入速度
		//    则会让风机的功率在0和切入速度处的功率之间插值
		if (vel[i] < uWind[0])
		{
			power_i[i] = 0;
		}
		else
		{
			vel_axial = vel[i] * cos((*gamma)[i]);
			getCp(power_i[i], vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		}
		// 为避免功率不连续，不采用上述策略
		//vel_axial = vel[i] * cos((*gamma)[i]);
		//getCp(pc_i, vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		//power += pc_i; // 单位是kw

	}
	return 0;
}

int TurbCloud::getHypothesisPower(Column& power_i, double& vel)
{
	power_i.resize(turbNum);
	for (int i = 0; i < turbNum; ++i)
	{
		// 为了能在切入速度处能正常计算，采用以下策略：
		// 1. 如果风机处的风速小于切入速度，当前风机的功率取0；
		// 2. 如果风机处的风速大于等于切入速度，只是风机偏航后，风速的分量小于切入速度
		//    则会让风机的功率在0和切入速度处的功率之间插值
		if (vel < uWind[0])
		{
			power_i[i] = 0;
		}
		else
		{
			getCp(power_i[i], vel, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		}
		// 为避免功率不连续，不采用上述策略
		//vel_axial = vel[i] * cos((*gamma)[i]);
		//getCp(pc_i, vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		//power += pc_i; // 单位是kw

	}
	return 0;
}

int TurbCloud::getHypothesisPower(double& power, double& vel)
{
	power = 0.0;
	double pc_i; // 这里实际上调用的是功率曲线，而不是功率系数曲线
	for (int i = 0; i < turbNum; ++i)
	{
		// 为了能在切入速度处能正常计算，采用以下策略：
		// 1. 如果风机处的风速小于切入速度，当前风机的功率取0；
		// 2. 如果风机处的风速大于等于切入速度，只是风机偏航后，风速的分量小于切入速度
		//    则会让风机的功率在0和切入速度处的功率之间插值
		if (vel < uWind[0])
		{
			power += 0;
		}
		else
		{
			getCp(pc_i, vel, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
			power += pc_i; // 单位是kw
		}
		// 为避免功率不连续，不采用上述策略
		//vel_axial = vel[i] * cos((*gamma)[i]);
		//getCp(pc_i, vel_axial, turbType[i]); // 只是名字是getcp而已，实际上得到的是功率
		//power += pc_i; // 单位是kw

	}
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