#ifndef SIMULATION_H
#define SIMULATION_H
#include "turbCloud.h"
#include "input.h"
#include "model.h"
#include "wake.h"
#include "contour.h"
using std::string;

class Simulation
{
public:
	TurbCloud turbines;
	Input input;
	Wake wake;
	Model* model; //�������ͷ�ָ��ĵ�ַ����Ϊ��ַ���ⲿ����
	// turbinesInfo.csv is needed
	// input wind velocity, wind theta, yaw angles, model and isPlot
	Simulation();
	Simulation(double& wind, double& theta360, Model& model);
	int setAll(double& wind, double& theta360, Model& model);
	// Ϊ����ͼ������ĽǶ�Ϊ�Ƕ��ƣ�����ʱ��Ϊ������(�ú��������������ɵ���)
	int run(Column& gamma, bool isPlot);
	// Ϊ����ͼ������ĽǶ�Ϊ�Ƕ��ƣ�����ʱ��Ϊ�����������ַ�����Ϊ��ͼ����
	int run(Column& gamma, bool isPlot, string& fileName);
	// ֻ��β��������ͼ���������gammaΪ�����ƣ�����ʱ��Ϊ������
	// �������ŽǶȹ����б�����
	int run(Column& gamma);
	// ֻ��β��������ͼ���������gammaΪ�����ƣ�����ʱ��Ϊ������
	// �������ŽǶȹ����б�����,�ҷ����ܷ��繦��
	int run(double& f, Column& gamma);
	//  ֻ��β��������ͼ���������gammaΪ�����ƣ�����ʱ��Ϊ������
	// ����ÿ̨����ķ��繦�ʣ�����ͳ��
	int run(Column& f_i, Column& gamma);
	// ����û��β��Ӱ�죬�����ܵķ��繦��
	int hypotheticalRun(double& fHypothetical);
	// ����û��β��Ӱ�죬����ÿ̨����ķ��繦��
	int hypotheticalRun(Column& f_iHypothetical);
};
#endif // !SIMULATION_H
