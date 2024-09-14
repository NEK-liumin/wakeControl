#ifndef SIMULATION_H
#define SIMULATION_H
#include "turbCloud.h"
#include "input.h"
#include "model.h"
#include "wake.h"
#include "contour.h"
class Simulation
{
public:
	TurbCloud turbines;
	Input input;
	Wake wake;
	Model* model;
	// turbinesInfo.csv is needed
	// input wind velocity, wind theta, yaw angles, model and isPlot
	Simulation();
	Simulation(double& wind, double& theta360, Model& model);
	int setAll(double& wind, double& theta360, Model& model);
	// ����ĺ�����Ϊ����ͼ������ĽǶ�Ϊ�Ƕ��ƣ�����ʱ��Ϊ������
	int run(Column& gamma, bool isPlot);
	// �������ֻ��β��������ͼ���������gammaΪ�����ƣ���˳ʱ��Ϊ������
	// �������ŽǶȹ����б�����
	int run(Column& gamma);
};
#endif // !SIMULATION_H
