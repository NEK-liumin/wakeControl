#include "simulation.h"
#include "tecplot.h"
Simulation::Simulation()
{
	turbines = TurbCloud(1, 1, 10, 0, 20);
	input = Input(turbines);
	input.readFile();
}
int Simulation::run(double& wind, double& theta360, Column& gamma, Model& model, bool isPlot)
{
	double theta;
	if (gamma.size() != turbines.turbNum)
	{
		cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
		cout << "The number of wind turbines: " << turbines.turbNum << endl;
		cout << "The number of input yaw angle: " << gamma.size() << endl;
		return 0;
	}
// 在尾流模型中，偏航角的正方向和尾流偏转角的正方向相反。这里加一个负号用以调整
	for (int i = 0; i < turbines.turbNum; ++i)
	{
		turbines.gamma[i] = -gamma[i] / 180.0 * PI;
	}
	theta = theta360 / 180 * PI;
	Wake wake = Wake(turbines, wind, theta);
	wake.getWake(model);

	if (isPlot)
	{
		Contour contour = Contour(wake, true);
		contour.set_xy();
		contour.get_vel(model);
		std::string fileName = "tecplot2D.dat";
		vector<std::string> varName;
		varName.resize(1);
		varName[0] = "velocity";
		Tecplot2D tecplot2D = Tecplot2D(fileName, varName, 1);
		tecplot2D.var[0] = &contour.vel;
		tecplot2D.set_xy(contour.x, contour.y);
		tecplot2D.output();
	}
	return 0;
}