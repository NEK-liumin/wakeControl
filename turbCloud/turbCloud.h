#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum;
	// ���ԭ����
	Column x0;
	Column y0;
	Column z0;

	// ����������
	double cutIn;
	// ����г�����
	double cutOut;
	// ���ٱ�ƽ���ֳ�uNum-1��
	vector<int> uNum;
	Matrix uWind;
	// ��ͬ�����·���Ĺ���ϵ��(ÿ�������ÿ�������µĹ���ϵ����ͬ�������Ƕ�ά)
	Matrix Cp;
	

	int setPosi(int turbNum, Column& x0, Column& y0, Column& z0);

	int setCp(int uNum, Matrix& uWind, Matrix& Cp);
	int setCp(vector<int> uNum, Matrix& uWind, Matrix& Cp);
	int getCp(double& cp_i, double& velo, int i);
};
#endif // !TURBCLOUD_H
