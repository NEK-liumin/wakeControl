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
	int getWake(double& meshVel, double& x, double& y, double& z, Column& newGamma, Model& model); // 求该处的尾流损失 wake deflection here
	int getWake(Model& model);
// 以下函数在作图中被调用
	int getWake(Column& newGamma, Model& model);

// 排好序的风机
	TurbCloud newTurbines;
// 排好序的风机处的风速
	Column newVel;
	int gamma2NewGamma(Column& newGamma, Column& gamma);
// 重新排列每个风机的偏航角
	Column newGamma2Gamma();
// 重新排列每个风机的功率
//	int newPower2Power();
// 重新排列每个风机处的速度
	int restoreVel();
// 重新排列任意输入，输入是按照位置排序的，输出是原顺序
	int restoreA(Column& result, Column& A);
// 排列任意输入，输入是原顺序，输出是按位置前后排好序的
	int sortA(Column& result, Column& A);
	int sortA(Bool& result, Bool& A);

private:
	vector<Pair> index;
	int xyzMin();
	int xyz_transformation(double& x, double& y, double& z);
	double xmin, ymin, zmin;
	double x1, y1, z1;

};
#endif // !WAKE_H
