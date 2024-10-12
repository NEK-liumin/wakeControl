#ifndef STATISTICS_H
#define STATISTICS_H

#include "run.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class Statistics
{
public:
	const double t = 8760; // 每年的小时数（24 * 365）

	vector<string> date, time;
	vector<double> windSpeed;
	vector<double> direction;
	vector<vector<double>> probability;
	vector<double> pWindSpeed; // 风速直方图
	vector<double> pWindTheta; // 风向玫瑰图

	// 风速和风向序列，从0到最大
	Column u, theta360;
	// 也是风速和风向序列，从0

	// g means annual generation
	double gWithoutWeak = 0; // 不考虑尾流的年发电量
	double g0 = 0; // 考虑尾流，但不偏航的年发电量
	double g = 0; // 考虑尾流，且偏航优化的年发电量

	double weakLoss0 = 0; // 不偏航的尾流损失
	double weakLoss = 0; // 偏航后的尾流损失
	double gIncrease = 0; // 偏航后的发电量增加（按百分比计算）

	Column gPerThetaWithoutWeak; // 不考虑尾流的每个风向下的年发电量
	Column g0PerTheta; // 考虑尾流，不偏航时每个风向下的年发电量
	Column gPerTheta; // 考虑尾流，偏航时每个风向下的年发电量

	Column gPerUWithoutWeak; // 不考虑尾流的每个风速段的年发电量
	Column g0PerU; // 考虑尾流，不考虑偏航时每个风速段的年发电量
	Column gPerU; // 考虑尾流，偏航时每个风速段的年发电量

	Column weakLoss0PerU; // 不偏航的每个风速段的尾流损失
	Column weakLossPerU; // 偏航的每个风速段的尾流损失
	Column gIncreasePerU; // 偏航后的每个风速段的发电量增加（按百分比计算）

	Column gPerTurbWithoutWeak; // 不考虑尾流的每台风机的年发电量
	Column g0PerTurb; // 考虑尾流，不偏航时每台风机的年发电量
	Column gPerTurb; // 考虑尾流，偏航时每台风机的年发电量

	Column weakLoss0PerTurb; // 不偏航的每台风机的尾流损失
	Column weakLossPerTurb; // 偏航的每台风机的尾流损失
	Column gIncreasePerTurb; // 偏航后的每台风机的发电量增加（按百分比计算）

	Column gTimeSeriseWithoutWeak; // 不考虑尾流的小时发电量时间序列
	Column g0TimeSerise; // 考虑尾流，不偏航时的小时发电量时间序列
	Column gTimeSerise; // 考虑尾流，偏航时的小时发电量时间序列

	double uMin = 0;
	double uMax = 0; // 在readFile()中被修正
	double thetaMin = 0;
	double thetaMax = 360;

	Run *run = nullptr;

	Statistics();
	int setStatistisc(Run& run);
	int readFile(bool isDelBadVal); // 输入true，则会删除错误数据；输入false，则提示数据错误并退出
	// 统计风速和风向的频率
	int windStatistics(bool isTranspose);
	// 统计风电场的总功率
	int powerStatistics(bool isTranspose);
	// 统计每台风机的功率
	int power_iStatistics(bool isTranspose);
	// 统计年发电量
	int annualPowerGeneration();
};
#endif // !STATISTICS_H
