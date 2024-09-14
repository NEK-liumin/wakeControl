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
	// 下面的函数是为了作图，输入的角度为角度制，且逆时针为正方向
	int run(Column& gamma, bool isPlot);
	// 下面这个只求尾流，不画图。且输入的gamma为弧度制，且顺时针为正方向
	// 在求最优角度过程中被调用
	int run(Column& gamma);
};
#endif // !SIMULATION_H
