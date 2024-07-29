#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum;
	// 风机原坐标
	Column x0;
	Column y0;
	Column z0;

	// 风机切入风速
	double cutIn;
	// 风机切出风速
	double cutOut;
	// 风速被平均分成uNum-1份
	vector<int> uNum;
	Matrix uWind;
	// 不同风速下风机的功率系数(每个风机，每个风速下的功率系数不同，所以是二维)
	Matrix Cp;
	

	int setPosi(int turbNum, Column& x0, Column& y0, Column& z0);

	int setCp(int uNum, Matrix& uWind, Matrix& Cp);
	int setCp(vector<int> uNum, Matrix& uWind, Matrix& Cp);
	int getCp(double& cp_i, double& velo, int i);
};
#endif // !TURBCLOUD_H
