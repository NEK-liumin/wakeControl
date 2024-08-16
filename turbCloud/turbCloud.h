#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum; // 风机数量
	int turbTypeNum; // 风机种类数
	// 风机叶轮直径
	Column D;
	// 风机原坐标
	Column x0;
	Column y0;
	Column z0;

	Column gamma; // 风机的偏航角度

	vector<int> turbType;

	// 由于在同一个风场，假设不同种类风机的风速区间一致
	// 风速被平均分成uNum-1份
	double uMin, uMax; // 最小风速和最大风速
	int uNum;
	Column uWind;
	// 不同风速下风机的功率系数(每种风机，每个风速下的功率系数不同，所以是二维)
	Matrix Cp;
	// 不同风速下风机的推力系数
	Matrix Ct;

	TurbCloud() {};

	TurbCloud(int turbNum, int turbTypeNum, int uNum, double uMin, double uMax)
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
		getZeroColumn(gamma, turbNum);
		turbType.resize(turbNum);

		getZeroColumn(uWind, uNum);

		for (int i = 0; i < uNum; ++i)
		{
			uWind[i] = uMin + (uMax - uMin) / (uNum - 1) * i;
		}

		getZeroMatrix(Cp, turbTypeNum, uNum);
		getZeroMatrix(Ct, turbTypeNum, uNum);


	}
	
	int setD(Column& D);
	int setD(double& D);

	int setPosi(int turbNum, Column& x0, Column& y0, Column& z0);

	int setCoef(Matrix& Cp, Matrix& Ct);
	int getCp(double& cp_i, double& velo, int& type_i);
	int getCt(double& ct_i, double& velo, int& type_i);
	
	int turbPrint();
};
#endif // !TURBCLOUD_H
