#include "turbCloud.h"
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
		cout << "����ķ������������������ƥ��" << endl;
		return 0;
	}
	if (x0.size() != y0.size() || x0.size() != z0.size())
	{
		cout << "xyz�����������һ��" << endl;
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
		cout << "����޷��ڸ÷����½��й���" << endl;
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
	// �᲻�ϸ��ݷ��λ�ô��������ٶȼ����书��ϵ��������ϵ��
	// ���ô����϶�
	// ���˲���ѭ��
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
		cout << "����޷��ڸ÷����½��й���" << endl;
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
	// �᲻�ϸ��ݷ��λ�ô��������ٶȼ����书��ϵ��������ϵ��
	// ���ô����϶�
	// ���˲���ѭ��
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
		std::cerr << "�޷����ļ�" << std::endl;
		return 1;
	}
	outFile << "̨��" << endl;
	outFile << this ->turbNum << endl;
	outFile << "������" << endl;
	outFile << this->turbTypeNum << endl;
	outFile << "���ٽ�������" << endl;
	outFile << this->uNum << endl;
	outFile << "��С����" << endl;
	outFile << this->uMin << endl;
	outFile << "������" << endl;
	outFile << this->uMax << endl;
	outFile << "x,y,z,D,type" << endl;

	for (int i = 0; i < this->turbNum; i++)
	{
		outFile << this->x0[i] << " , " << this->y0[i] << " , " << this->z0[i] << " , " << this->D[i] << " , " << this->turbType[i] << endl;
	}
	outFile << "Cp��Ct����" << endl;

	for (int i = 0; i < this->turbTypeNum; ++i)
	{
		outFile << "�� " << i << " �ַ��" << endl;
		outFile << "����,Cp,Ct" << endl;
		for (int j = 0; j < this->uNum; ++j)
		{
			outFile << this->uWind[j] << "," << this->Cp[i][j] << "," << this->Ct[i][j] << endl;
		}
	}

	outFile.close();
}