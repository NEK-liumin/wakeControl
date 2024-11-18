#ifndef STATISTICS_H
#define STATISTICS_H

#include "run.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using std::vector;

class Statistics
{
public:
	const double t = 8760; // 每年的小时数（24 * 365）

	// 风速和风向序列，从0到最大
	Column u, theta360;
	// 风速序列，从cutin到cutout
	Column uCut;
	// 风速和风向序列，从uBegin到uEnd（只在此范围内进行偏航）
	Column uYaw, thetaYaw;

	double uMin = 0;
	double uMax = 0; // 在readFile()中被修正
	double thetaMin = 0;
	double thetaMax = 360;
	bool isDelBadVal = true;
	int badValNum = 0;

	Run *run = nullptr;
	Bool isWork;

	Statistics();
	Statistics(Run& run);
	int setStatistics(Run& run);
	int readFile(bool isDelBadVal); // 输入true，则会删除错误数据；输入false，则不删除错误数据
	
	int get_all();
	int write_all(bool isTranspose);
private:
	// 计算风速和风向序列（最小最大风速序列、切入切出风速序列；最小最大风向序列，切入切出风向序列）
	std::filesystem::path output;
	std::filesystem::path statistics;
	std::filesystem::path absOutput;

	// 统计风速和风向的频率
	int windStatistics();
	int getUThetaColumn();

	vector<string> date, time;
	vector<double> windSpeed;
	vector<double> direction;
	vector<vector<double>> probability;
	vector<double> pWindSpeed; // 风速直方图
	vector<double> pWindTheta; // 风向玫瑰图

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

	// 输出单一统计量
	int writeSingleStatistics();
	// 不考虑尾流的每个风向下的年发电量
	int write_gPerThetaWithoutWeak();
	// 考虑尾流，不考虑偏航时每个风向的年发电量
	int write_g0PerTheta();
	// 考虑尾流，偏航时每个风向的年发电量
	int write_gPerTheta();
	// 不考虑尾流的每个风速段的年发电量
	int write_gPerUWithoutWeak();
	// 考虑尾流，不考虑偏航时每个风速段的年发电量
	int write_g0PerU();
	// 考虑尾流，偏航时每个风速段的年发电量
	int write_gPerU();
	// 不偏航的每个风速段的尾流损失
	int write_weakLoss0PerU();
	// 偏航的每个风速段的尾流损失
	int write_weakLossPerU();
	// 偏航后的每个风速段的发电量增加（按百分比计算）
	int write_gIncreasePerU();
	// 不考虑尾流的每台风机的年发电量
	int write_gPerTurbWithoutWeak();
	// 考虑尾流，不偏航时每台风机的年发电量
	int write_g0PerTurb();
	// 考虑尾流，偏航时每台风机的年发电量
	int write_gPerTurb();
	// 不偏航的每台风机的尾流损失
	int write_weakLoss0PerTurb();
	// 偏航的每台风机的尾流损失
	int write_weakLossPerTurb();
	// 偏航后的每台风机的发电量增加（按百分比计算）
	int write_gIncreasePerTurb();
	// 不考虑尾流的小时发电量时间序列
	int write_gTimeSeriseWithoutWeak();
	// 考虑尾流，不偏航时的小时发电量时间序列
	int write_g0TimeSerise();
	// 考虑尾流，偏航时的小时发电量时间序列
	int write_gTimeSerise();

	// 输出扇区统计量
	int writeSector_x(Column& x, string& title, string& fileName);
	// 输出风速段统计量
	int writeSegment_x(Column& x, string& title, string& fileName);
	// 输出各台风机统计量
	int writeTurbine_x(Column& x, string& title, string& fileName);
	// 输出时序统计量
	int writeTime_x(Column& x, string& title, string& fileName);
	// 输出列统计
	int writeColumn(Column& x, Column& y, string& titlex, string& titley, string& fileName);
	// 输出列统计
	int writeColumn(vector<string>& x, Column& y, string& titlex, string& titley, string& fileName);

	// 不考虑尾流，计算年发电量
	int get_gWithoutWeak();
	// 考虑尾流，但不偏航的年发电量
	int get_g0();
	// 考虑尾流，且偏航优化的年发电量
	int get_g();
	// 不偏航的尾流损失
	int get_weakLoss0();
	// 偏航的尾流损失
	int get_weakLoss();
	// 偏航后的发电量增加（按百分比计算）
	int get_gIncrease();
	// 不考虑尾流的每个风向下的年发电量
	int get_gPerThetaWithoutWeak();
	// 考虑尾流，不考虑偏航时每个风向的年发电量
	int get_g0PerTheta();
	// 考虑尾流，偏航时每个风向的年发电量
	int get_gPerTheta();
	// 不考虑尾流的每个风速段的年发电量
	int get_gPerUWithoutWeak();
	// 考虑尾流，不考虑偏航时每个风速段的年发电量
	int get_g0PerU();
	// 考虑尾流，偏航时每个风速段的年发电量
	int get_gPerU();
	// 不偏航的每个风速段的尾流损失
	int get_weakLoss0PerU();
	// 偏航的每个风速段的尾流损失
	int get_weakLossPerU();
	// 偏航后的每个风速段的发电量增加（按百分比计算）
	int get_gIncreasePerU();
	// 不考虑尾流的每台风机的年发电量
	int get_gPerTurbWithoutWeak();
	// 考虑尾流，不偏航时每台风机的年发电量
	int get_g0PerTurb();
	// 考虑尾流，偏航时每台风机的年发电量
	int get_gPerTurb();
	// 不偏航的每台风机的尾流损失
	int get_weakLoss0PerTurb();
	// 偏航的每台风机的尾流损失
	int get_weakLossPerTurb();
	// 偏航后的每台风机的发电量增加（按百分比计算）
	int get_gIncreasePerTurb();
	// 不考虑尾流的小时发电量时间序列
	int get_gTimeSeriseWithoutWeak();
	// 考虑尾流，不偏航时的小时发电量时间序列
	int get_g0TimeSerise();
	// 考虑尾流，偏航时的小时发电量时间序列
	int get_gTimeSerise();
};
#endif // !STATISTICS_H
