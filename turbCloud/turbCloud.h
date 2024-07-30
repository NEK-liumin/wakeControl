#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum;
	// ���Ҷ��ֱ��
	Column D;
	// ���ԭ����
	Column x0;
	Column y0;
	Column z0;

	Column gamma; // �����ƫ���Ƕ�

	// ����������
	double cutIn;
	// ����г�����
	double cutOut;
	// ���ٱ�ƽ���ֳ�uNum-1��
	vector<int> uNum;
	Matrix uWind;
	// ��ͬ�����·���Ĺ���ϵ��(ÿ�������ÿ�������µĹ���ϵ����ͬ�������Ƕ�ά)
	Matrix Cp;
	// ��ͬ�����·��������ϵ��
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
