#include "simulation.h"
#include "tecplot.h"
const double PiOutOf180 = 0.01745329251994329576923690768489;
Simulation::Simulation()
{
	double wind = 0, theta360 = 0;
	model = nullptr;
	double theta;

	input.setTurb(turbines);
	input.readFile();
	theta = (270 - theta360) / 180.0 * PI; // Ϊ��ƥ�������ļ��ķ���
	wake.setWake(turbines, wind, theta);
}

int Simulation::reset(double& wind, double& theta360, Bool& isWork, Model& model)
{
	double theta;
	Bool newIsWork;
	sortColumn(newIsWork, isWork);
	theta = (270 - theta360) / 180.0 * PI; // Ϊ��ƥ�������ļ��ķ���
	wake.setWake(turbines, wind, theta);
	turbines.isWork = isWork;
	this->wake.newTurbines.isWork = newIsWork;
	this->model = &model;
	return 0;
}
int Simulation::sortColumn(Column& result, Column& A)
{
	wake.sortA(result, A);
	return 0;
}
int Simulation::sortColumn(Bool& result, Bool& A)
{
	wake.sortA(result, A);
	return 0;
}
int Simulation::run(Column& gamma)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	wake.newTurbines.gamma = &gamma;
	wake.getWake(*model);
	// Ϊ�˱����ڼ������ŽǶ��ڼ䷴�������Ӧ��ԭʼ����Ƕȣ����������нǶ�
	// Ҳ���������з���ٶȡ�
	// �����Ĺ��ʼ���Ҳ���Ѿ����к��wake.newTurbines�����ϼ���
	return 0;
}

int Simulation::run(Column& gamma, bool isPlot, string& fileName, string& titleName)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	Column m_gamma, m_newGamma;
	m_gamma = gamma;
	m_newGamma = gamma;
	for (int i = 0; i < turbines.turbNum; ++i)
	{
		m_gamma[i] = gamma[i] * PiOutOf180;
	}
	wake.gamma2NewGamma(m_newGamma, m_gamma);
	wake.getWake(m_newGamma, *model);
	wake.restoreVel(); // ���û�ã�Ӧ�ã�

	if (isPlot)
	{
		Contour contour = Contour(wake, true);
		contour.set_xy();
		contour.get_vel(m_newGamma, *model);
		vector<std::string> varName;
		varName.resize(1);
		varName[0] = "velocity";
		Tecplot2D tecplot2D = Tecplot2D(fileName, varName, titleName, 1);
		tecplot2D.var[0] = &contour.vel;
		tecplot2D.set_xy(contour.x, contour.y);
		tecplot2D.output();
		// cout << "Done!" << endl;
	}
	return 0;
}

int Simulation::run(double& f, Column& gamma)
{
	run(gamma);
	wake.newTurbines.getPower(f, wake.newVel);
	return 0;
}
// �Ȼ�ȡ���������ÿ̨����ķ��繦��
// Ȼ�󣬶Թ����������򣬵õ�ԭ���Ĺ���
int Simulation::run(Column& f_i, Column& gamma)
{
	Column f_iNew;
	run(gamma);
	wake.newTurbines.getPower(f_iNew, wake.newVel);
	wake.restoreA(f_i, f_iNew);
	return 0;
}

int Simulation::hypotheticalRun(double& fHypothetical)
{
	wake.newTurbines.getHypothesisPower(fHypothetical, wake.u);
	return 0;
}

int Simulation::hypotheticalRun(Column& f_iHypothetical)
{
	Column f_iHypotheticalNew;
	wake.newTurbines.getHypothesisPower(f_iHypotheticalNew, wake.u);
	wake.restoreA(f_iHypothetical, f_iHypotheticalNew);
	return 0;
}

int Simulation::plot(double& wind, double& theta360, Bool& isWork, Model& model, Column& gamma360, string& fileName, string& titleName)
{
	reset(wind, theta360, isWork, model);
	run(gamma360, true, fileName, titleName);
	return 0;
}