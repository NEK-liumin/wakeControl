#ifndef SIMULATION_H
#define SIMULATION_H
#include "turbCloud.h"
#include "input.h"
#include "model.h"
#include "wake.h"
#include "contour.h"
using std::string;

// ���ڼ���β�����󷢵繦�ʡ�Ҳ������ͼ
class Simulation
{
public:
	TurbCloud turbines = TurbCloud(1, 1, 10, 0, 20);
	Input input;
	Wake wake;
	Model* model; //�������ͷ�ָ��ĵ�ַ����Ϊ��ַ���ⲿ����
	// turbinesInfo.csv is needed
	// input wind velocity, wind theta, yaw angles, model and isPlot
	Simulation();
	int reset(double& wind, double& theta360, Bool& isWork, Model& model);
	// �Է��ƫ���ǵ������ޡ���ʼ�����Χ��������
	int sortColumn(Column& result, Column& A);
	int sortColumn(Bool& result, Bool& A);
	// ��ͼ������ĽǶ�Ϊ�Ƕ��ƣ�(���١�����β��ģ�͡�ƫ���ǡ��ļ���������)
	int plot(double& wind, double& theta360, Bool& isWork, Model& model, Column& gamma360, string& fileName, string& titleName);
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
private:
	// ֻ��β��������ͼ���������gammaΪ�����ƣ�����ʱ��Ϊ������
	int run(Column& gamma);
	// Ϊ����ͼ������ĽǶ�Ϊ�Ƕ��ƣ�����ʱ��Ϊ�����������ַ�����Ϊ��ͼ����
	int run(Column& gamma, bool isPlot, string& fileName, string& titleName);
};
#endif // !SIMULATION_H
