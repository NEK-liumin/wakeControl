#include "simulation.h"
#include "tecplot.h"
const double PiOutOf180 = 0.01745329251994329576923690768489;
Simulation::Simulation()
{
	model = nullptr;
}

Simulation::Simulation(double& wind, double& theta360, Model& model)
	:turbines(1, 1, 10, 0, 20)
{
	double theta;

	input.setTurb(turbines);
	input.readFile();
	
	theta = (270 - theta360) / 180.0 * PI; // Ϊ��ƥ�������ļ��ķ���
	wake.setWake(turbines, wind, theta);
	this->model = &model;
}
int Simulation::setAll(double& wind, double& theta360, Model& model)
{
	double theta;
	turbines.init(1, 1, 10, 0, 20);
	input.setTurb(turbines);
	input.readFile();

	theta = (270 - theta360) / 180.0 * PI; // Ϊ��ƥ�������ļ��ķ���
	//if (wind - turbines.uMin <= 1e-2) wind = turbines.uMin + 1e-2;//���������̫С������β�����׳����⡣
	wake.setWake(turbines, wind, theta);
	this->model = &model;
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
// ��Ϊ�漰������ǰ��ķ����ƫ���ǵ����⣬���ԣ����溯����ָ��Ĵ������С��
int Simulation::run(Column& gamma, bool isPlot)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	// ����ǳ������
	// ����ָ���wake.turbines->gammaָ��ͬһ��λ��
	// ����ֻɾ��һ��
	delete wake.newTurbines.gamma;
	wake.turbines->gamma = &gamma;
	wake.newTurbines.gamma = new Column(gamma);

	// ��β��ģ���У�ƫ���ǵ��������β��ƫת�ǵ���������ͬ
	for (int i = 0; i < turbines.turbNum; ++i)
	{
		gamma[i] *= PiOutOf180;
	}
	wake.gamma2NewGamma();
	wake.getWake(*model);
	wake.restoreVel();
	if (isPlot)
	{
		cout << "The tecplot file is generating..." << endl;
		Contour contour = Contour(wake, true);
		contour.set_xy();
		contour.get_vel(*model);
		std::string fileName = "tecplot2D.dat";
		vector<std::string> varName;
		varName.resize(1);
		varName[0] = "velocity";
		Tecplot2D tecplot2D = Tecplot2D(fileName, varName, 1);
		tecplot2D.var[0] = &contour.vel;
		tecplot2D.set_xy(contour.x, contour.y);
		tecplot2D.output();
		cout << "Done!" << endl;
	}
	return 0;
}

// ��Ϊ�漰������ǰ��ķ����ƫ���ǵ����⣬���ԣ����溯����ָ��Ĵ������С��
int Simulation::run(Column& gamma, bool isPlot, string& fileName)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	// ����ǳ�����������������ڹ���Weak���͵ı���ʱ��ɣ�
	// ����ָ���wake.turbines->gammaָ��ͬһ��λ��
	// ����ֻɾ��һ��
	if (wake.newTurbines.gamma != nullptr)
	{
		delete wake.newTurbines.gamma;
		wake.newTurbines.gamma = nullptr;
		wake.turbines->gamma = nullptr;
	}
	
	wake.turbines->gamma = new Column(gamma);

	wake.newTurbines.gamma = new Column(gamma);

	// ��β��ģ���У�ƫ���ǵ��������β��ƫת�ǵ���������ͬ

	for (int i = 0; i < turbines.turbNum; ++i)
	{
		(*wake.turbines->gamma)[i] = gamma[i] * PiOutOf180;
	}
	wake.gamma2NewGamma();
	wake.getWake(*model);
	wake.restoreVel();
	// printA(wake.vel);
	if (isPlot)
	{
		// cout << "The tecplot file is generating..." << endl;
		Contour contour = Contour(wake, true);
		contour.set_xy();
		contour.get_vel(*model);
		vector<std::string> varName;
		varName.resize(1);
		varName[0] = "velocity";
		Tecplot2D tecplot2D = Tecplot2D(fileName, varName, 1);
		tecplot2D.var[0] = &contour.vel;
		tecplot2D.set_xy(contour.x, contour.y);
		tecplot2D.output();
		// cout << "Done!" << endl;
	}
	delete wake.newTurbines.gamma; // ��ֹ�ڴ�й¶
	wake.newTurbines.gamma = nullptr;
	delete wake.turbines->gamma;
	wake.turbines->gamma = nullptr;
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
	run(gamma);
	wake.newTurbines.getPower(wake.newVel);
	wake.newPower2Power();
	f_i = wake.turbines->power_i;
	return 0;
}