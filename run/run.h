#ifndef RUN_H
#define RUN_H

#include "Blondel.h"
#include "Blondel2.h"
#include "Gauss3.h"
#include "usr.h"
#include "matrixOperation.h"

// ƫ������ļ��������
class Run
{
public:
	// ƫ������
	vector<vector<vector<double>>> gamma360;
	vector<double> u, theta360;
	Column uFull, thetaFull;
	// ÿ�����ٺͷ����£��糡ƫ��ǰ����ܷ��繦�ʣ��Լ�ƫ�����ܷ��繦�ʵ�������
	Matrix P0, P, deltaP;
	// ÿ�����ٺͷ����£�ÿ̨���ƫ��ǰ��ķ��繦�ʣ��Լ�ƫ���󷢵繦�ʵ�������
	vector<Matrix> P0_i, P_i, deltaP_i;
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
	// Ĭ�ϵ����ƫ����
	double maxGamma = 30;
	// Ĭ�ϵ�β��ģ��
	Blondel2 model = Blondel2(ky, kz, I);
	// Gauss3 model = Gauss3(ky, kz, I);
	// Ĭ�ϵĳ�ʼ�����Χ��-randomRange,randomRange��
	double randomRange = 1.0;
	// ���õ�ƫ���Ǿ���ֵС��smallGamma��ȡ0
	double smallGamma = 0;
	// 
	Bool m_isWork;
	// Ĭ�ϵ��Ż�����
	Yaw yaw = Yaw(uBegin, thetaBegin, m_isWork, rho, model, maxGamma, randomRange);
	// �������ھ������
	double uIn = 0;
	double uOut = yaw.simulation.turbines.uMax;

	Run();
	Run(double& uBegin, double& uEnd, double& deltaU, double& deltaTheta);

	// ��ȡ�����ļ�
	int readParameter();
	// ����ƫ������
	int getMatrix();

	// ���ƫ�����󣬲���ע���ٺͷ���
	int outputMatrix(bool isTranspose);

	// �������ƫ�����󣬲���ע���ٺͷ���
	int outputAbsMatrix(bool isTranspose);

	int extendMatrix(Matrix& result, Matrix& source, Column& resulti, Column& resultj, Column& sourcei, Column& sourcej);
};

#endif // !MAINLOOP_H