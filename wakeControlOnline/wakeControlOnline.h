#ifndef WAKECONTROLONLINE_H
#define WAKECONTROLONLINE_H
#include "matrixOperation.h"
#include "run.h"

// 该类用于在线偏航
class WakeControlOnline
{
public:
	WakeControlOnline() {};
	// 计算偏航角度（输出结果为角度制，逆时针为正方向）
	int run(Column& nextGamma, double& u, double& theta360, Column& currentAbsGamma, Bool& isWork, string& date);
	// 显示优化前后总功率
	int power(Column& nextGamma);
private:
	bool isPlot = true; // 是否输出图像
	Run m_run;
	Simulation simulation;

	Bool isWork;
	Column minGamma, maxGamma;
	Column randomRangeLeft, randomRangeRight;
	
	// 不考虑是否解缆。假设风机采用最近的方式进行偏航对风
	// nexGamma：输出风机的相对偏航角。为与WT工具保持一致，逆时针为正方向
	// currentAbsGamma：当前风机的绝对偏航角。风机面向正北为0，顺时针为正方向，单位为度
	// isWork：当前风机是否正常工作。
	// date：日期
	int getIsWork(Bool& isWork);
	int getGammaRange(double& theta360, Column& currentAbsGamma);
	int getRandomRange(double& theta360, Column& currentAbsGamma);
	int getGamma(Column& gamma360, double& u, double& theta360);
};
#endif // !YAWONLINE_H
