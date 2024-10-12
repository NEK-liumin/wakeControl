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
	TurbCloud* turbines = nullptr;
	Column vel; // 每个风机处的速度
	double u, theta; // 来流风速大小及方向
	Wake();
	Wake(TurbCloud& turbines, double& u, double& theta);
	int setWake(TurbCloud& turbines, double& u, double& theta);
	int getNewCloud();
	int getWake(double& meshVel, double& x, double& y, double& z, Model& model); // 求该处的尾流损失 wake deflection here
	int getWake(Model& model);

// 排好序的风机
	TurbCloud newTurbines;
// 排好序的风机处的风速
	Column newVel;
	int gamma2NewGamma();
// 重新排列每个风机的偏航角
	int newGamma2Gamma();
// 重新排列每个风机的功率
	int newPower2Power();
// 重新排列每个风机处的速度
	int restoreVel();

private:
	vector<Pair> index;
	int xyzMin();
	int xyz_transformation(double& x, double& y, double& z);
	double xmin, ymin, zmin;
	double x1, y1, z1;

};
#endif // !WAKE_H
