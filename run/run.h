#ifndef RUN_H
#define RUN_H

#include "Blondel.h"
#include "usr.h"
#include "matrixOperation.h"

class Run
{
public:
	// ƫ������
	vector<vector<vector<double>>> gamma360;
	// ÿ�����ٺͷ����£��糡ƫ��ǰ����ܷ��繦�ʣ��Լ�ƫ�����ܷ��繦�ʵ�������
	Matrix P0, P, deltaP;
	// ֻ����������ƫ������
	double uBegin = 5, uEnd = 12; 
	double thetaBegin = 0, thetaEnd = 350;
	// ���ټ��Ĭ��1m/s���Ƕȼ��Ĭ��10��
	double deltaU = 1.0, deltaTheta = 10; 
	// Ĭ�ϵ�β�����������
	double ky = 0.025;
	double kz = 0.0175;
	// Ĭ�ϵ�����ǿ��
	double I = 0.12;
	// Ĭ�ϵ��ܶȣ�����ʹ���˹������ߣ����Բ������ã�
	double rho = 1.225;
	// Ĭ�ϵ��Ż��ݲ�
	double tol = 1e-5;
	// Ĭ�ϵķ��ٺͷ��򣨺�׺360��ʾ�Ƕ��ƣ�
	double u = uBegin, theta360 = 0;
	// Ĭ�ϵ�β��ģ��
	Blondel model = Blondel(ky, kz, I);
	// Ĭ�ϵ��Ż�����
	Yaw yaw = Yaw(u, theta360, rho, model);

	Run();
	Run(double& uBegin, double& uEnd, double& deltaU, double& deltaTheta);

	// ��ȡ�����ļ�
	int readParameter();
	// ����ƫ������
	int getMatrix();

	// ���ƫ�����󣬲���ע���ٺͷ���
	int outputMatrix();

	// �������ƫ�����󣬲���ע���ٺͷ���
	int outputAbsMatrix();

	// ���ת�ú��ƫ�����󣬲���ע���ٺͷ���
	int outputMatrixT();

	// ���ת�ú�ľ���ƫ�����󣬲���ע���ٺͷ���
	int outputAbsMatrixT();
};

#endif // !MAINLOOP_H