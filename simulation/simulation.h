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
	// turbinesInfo.csv is needed
	// input wind velocity, wind theta, yaw angles, model and isPlot
	// then, simulation run!
	Simulation(double& wind, double& theta360, Column& gamma, Model& model, bool isPlot);
};
#endif // !SIMULATION_H
