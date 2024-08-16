#ifndef WAKE_H
#define WAKE_H
#include "turbCloud.h"
#include "model.h"
#include "Gauss.h"
using std::pair;
using Pair = pair<double, int>;

class Wake
{
public:
	TurbCloud* turbines;

 // 坐标转换后所有风机所有信息 position of wind turbine
	TurbCloud newTurbines;

	Column vel; // 每个风机处的速度
	double x, y, z; // 求该处的尾流损失 wake deflection here
	double u, theta; // 来流风速大小及方向
	Wake(TurbCloud& turbines, double& u, double& theta);
	int getNewCloud();
	int getWake(Model& model);

	int restoreGamma();
private:
	vector<Pair> index;
};
#endif // !WAKE_H
