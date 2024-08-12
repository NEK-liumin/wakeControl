#ifndef TURBCLOUD_H
#define TURBCLOUD_H
#include "matrixOperation.h"

class TurbCloud
{
public:
	int turbNum; // �������
	int turbTypeNum; // �������
	// ���Ҷ��ֱ��
	Column D;
	// ���ԭ����
	Column x0;
	Column y0;
	Column z0;

	Column gamma; // �����ƫ���Ƕ�

	vector<int> turbType;

	// ������ͬһ���糡�����費ͬ�������ķ�������һ��
	// ���ٱ�ƽ���ֳ�uNum-1��
	double uMin, uMax; // ��С���ٺ�������
	int uNum;
	Column uWind;
	// ��ͬ�����·���Ĺ���ϵ��(ÿ�ַ����ÿ�������µĹ���ϵ����ͬ�������Ƕ�ά)
	Matrix Cp;
	// ��ͬ�����·��������ϵ��
	Matrix Ct;

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
