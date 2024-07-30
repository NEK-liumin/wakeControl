#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum;
	// 风机叶轮直径
	Column D;
	// 风机原坐标
	Column x0;
	Column y0;
	Column z0;

	Column gamma; // 风机的偏航角度

	// 风机切入风速
	double cutIn;
	// 风机切出风速
	double cutOut;
	// 风速被平均分成uNum-1份
	vector<int> uNum;
	Matrix uWind;
	// 不同风速下风机的功率系数(每个风机，每个风速下的功率系数不同，所以是二维)
	Matrix Cp;
	// 不同风速下风机的推力系数
	Matrix Ct;
	
	int setD(Column& D);
	int setD(double& D);

	int setPosi(int turbNum, Column& x0, Column& y0, Column& z0);

	int setCoef(int uNum, Matrix& uWind, Matrix& Cp, Matrix& Ct);
	int setCoef(vector<int> uNum, Matrix& uWind, Matrix& Cp, Matrix& Ct);
	int getCp(double& cp_i, double& velo, int i);
	int getCt(double& ct_i, double& velo, int i);
	
};
#endif // !TURBCLOUD_H
