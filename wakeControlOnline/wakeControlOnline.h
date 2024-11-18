#ifndef WAKECONTROLONLINE_H
#define WAKECONTROLONLINE_H
#include "matrixOperation.h"
#include "run.h"

// ������������ƫ��
class WakeControlOnline
{
public:
	WakeControlOnline() {};
	// ����ƫ���Ƕȣ�������Ϊ�Ƕ��ƣ���ʱ��Ϊ������
	int run(Column& nextGamma, double& u, double& theta360, Column& currentAbsGamma, Bool& isWork, string& date);
	// ��ʾ�Ż�ǰ���ܹ���
	int power(Column& nextGamma);
private:
	bool isPlot = true; // �Ƿ����ͼ��
	Run m_run;
	Simulation simulation;

	Bool isWork;
	Column minGamma, maxGamma;
	Column randomRangeLeft, randomRangeRight;
	
	// �������Ƿ���¡���������������ķ�ʽ����ƫ���Է�
	// nexGamma�������������ƫ���ǡ�Ϊ��WT���߱���һ�£���ʱ��Ϊ������
	// currentAbsGamma����ǰ����ľ���ƫ���ǡ������������Ϊ0��˳ʱ��Ϊ�����򣬵�λΪ��
	// isWork����ǰ����Ƿ�����������
	// date������
	int getIsWork(Bool& isWork);
	int getGammaRange(double& theta360, Column& currentAbsGamma);
	int getRandomRange(double& theta360, Column& currentAbsGamma);
	int getGamma(Column& gamma360, double& u, double& theta360);
};
#endif // !YAWONLINE_H
