#include "simulation.h"
#include "tecplot.h"
Simulation::Simulation(double& wind, double& theta360, Column& gamma, Model& model, bool isPlot)
{
	double theta;
	if (isPlot)
	{
		turbines = TurbCloud(1, 1, 10, 0, 20);
		input = Input(turbines);
		input.readFile();
		if (gamma.size() != turbines.turbNum)
		{
			cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
			cout << "The number of wind turbines: " << turbines.turbNum << endl;
			cout << "The number of input yaw angle: " << gamma.size() << endl;
		}
		else
		{
			for (int i = 0; i < turbines.turbNum; ++i)
			{
				turbines.gamma[i] = -gamma[i] / 180.0 * PI;
			}
			theta = theta360 / 180 * PI;
			Wake wake = Wake(turbines, wind, theta);
			wake.getWake(model);

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
	}
	else
	{
		turbines = TurbCloud(1, 1, 10, 0, 20);
		input = Input(turbines);
		input.readFile();
		if (gamma.size() != turbines.turbNum)
		{
			cout << "The number of input yaw angle does not match the number of wind turbines" << endl;
			cout << "The number of wind turbines: " << turbines.turbNum << endl;
			cout << "The number of input yaw angle: " << gamma.size() << endl;
		}
		else
		{
			turbines.gamma = gamma;
			theta = theta360 / 180 * PI;
			Wake wake = Wake(turbines, wind, theta);
			wake.getWake(model);
		}
	}
}