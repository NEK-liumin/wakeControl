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
	
	theta = (270 - theta360) / 180.0 * PI; // 为了匹配输入文件的风向
	wake.setWake(turbines, wind, theta);
	this->model = &model;
}
int Simulation::setAll(double& wind, double& theta360, Model& model)
{
	double theta;
	turbines.init(1, 1, 10, 0, 20);
	input.setTurb(turbines);
	input.readFile();

	theta = (270 - theta360) / 180.0 * PI; // 为了匹配输入文件的风向
	//if (wind - turbines.uMin <= 1e-2) wind = turbines.uMin + 1e-2;//如果环境风太小，计算尾流容易出问题。
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
	// 为了避免在计算最优角度期间反复计算对应的原始风机角度，不重新排列角度
	// 也不重新排列风机速度。
	// 后续的功率计算也在已经排列后的wake.newTurbines基础上计算
	return 0;
}
// 因为涉及到排序前后的风机的偏航角的问题，所以，下面函数中指针的处理必须小心
int Simulation::run(Column& gamma, bool isPlot)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	// 由于浅拷贝，
	// 以下指针和wake.turbines->gamma指向同一个位置
	// 所以只删除一次
	delete wake.newTurbines.gamma;
	wake.turbines->gamma = &gamma;
	wake.newTurbines.gamma = new Column(gamma);

	// 在尾流模型中，偏航角的正方向和尾流偏转角的正方向相同
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

// 因为涉及到排序前后的风机的偏航角的问题，所以，下面函数中指针的处理必须小心
int Simulation::run(Column& gamma, bool isPlot, string& fileName)
{
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
	// 由于浅拷贝，（拷贝过程在构造Weak类型的变量时完成）
	// 以下指针和wake.turbines->gamma指向同一个位置
	// 所以只删除一次
	if (wake.newTurbines.gamma != nullptr)
	{
		delete wake.newTurbines.gamma;
		wake.newTurbines.gamma = nullptr;
		wake.turbines->gamma = nullptr;
	}
	
	wake.turbines->gamma = new Column(gamma);

	wake.newTurbines.gamma = new Column(gamma);

	// 在尾流模型中，偏航角的正方向和尾流偏转角的正方向相同

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
	delete wake.newTurbines.gamma; // 防止内存泄露
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
// 先获取排序后风机，每台风机的发电功率
// 然后，对功率重新排序，得到原本的功率
int Simulation::run(Column& f_i, Column& gamma)
{
	run(gamma);
	wake.newTurbines.getPower(wake.newVel);
	wake.newPower2Power();
	f_i = wake.turbines->power_i;
	return 0;
}