#include "wakeControlOnline.h"
#include "solver.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "tecplot.h"

int WakeControlOnline::getIsWork(Bool& isWork)
{
	this->isWork = isWork;
	return 0;
}

int WakeControlOnline::getGammaRange(double& theta360, Column& currentAbsGamma)
{
	int turbNum = simulation.turbines.turbNum;
	minGamma.resize(turbNum);
	maxGamma.resize(turbNum);
	double gammaLimit = m_run.maxGamma;
	if (currentAbsGamma.size() != turbNum)
	{
		cout << "currentAbsGamma.size() != turbNum" << endl;
		return 0;
	}
	for (int i = 0; i < turbNum; ++i)
	{
		if (currentAbsGamma[i] >= theta360 && currentAbsGamma[i] - theta360 >= 180)
		{
			minGamma[i] = -5;
			maxGamma[i] = abs(gammaLimit);
		}
		else if (currentAbsGamma[i] >= theta360 && currentAbsGamma[i] - theta360 < 180)
		{
			minGamma[i] = -abs(gammaLimit);
			maxGamma[i] = 5;
		}
		else if (currentAbsGamma[i] < theta360 && theta360 - currentAbsGamma[i] >= 180)
		{
			minGamma[i] = -abs(gammaLimit);
			maxGamma[i] = 5;
		}
		else if(currentAbsGamma[i] < theta360 && theta360 - currentAbsGamma[i] < 180)
		{
			minGamma[i] = -5;
			maxGamma[i] = abs(gammaLimit);
		}
		else
		{
			cout << "Unknown problems!" << endl;
			minGamma[i] = -abs(gammaLimit);
			maxGamma[i] = abs(gammaLimit);
		}
	}
	return 0;
}

int WakeControlOnline::getRandomRange(double& theta360, Column& currentAbsGamma)
{
	int turbNum = simulation.turbines.turbNum;
	randomRangeLeft.resize(turbNum);
	randomRangeRight.resize(turbNum);
	double gammaLimit = m_run.maxGamma;
	if (currentAbsGamma.size() != turbNum)
	{
		cout << "currentAbsGamma.size() != turbNum" << endl;
		return 0;
	}
	for (int i = 0; i < turbNum; ++i)
	{
		if (currentAbsGamma[i] >= theta360)
		{
			randomRangeLeft[i] = -abs(m_run.randomRange);
			randomRangeRight[i] = 0;
		}
		else
		{
			randomRangeLeft[i] = 0;
			randomRangeRight[i] = abs(m_run.randomRange);
		}
	}
	return 0;
}

int WakeControlOnline::getGamma(Column& gamma360, double& u, double& theta360)
{
	m_run.yaw.reset(u, theta360, isWork, m_run.rho, m_run.model, minGamma, maxGamma, randomRangeLeft, randomRangeRight);
	SQPIC_solver(m_run.yaw, m_run.tol);
	m_run.yaw.outputGamma(gamma360);
	return 0;
}
int WakeControlOnline::power(Column& gamma360)
{
	double p0, p, increase;
	p0 = m_run.yaw.initialPower();
	p = m_run.yaw.power();
	if (p0 != 0)increase = (p - p0) / p0;
	else increase = 0;
	cout << " 优化前功率: " << p0 << " kw" << endl;
	cout << " 优化后功率: " << p << " kw" << endl;
	cout << " 优化提升: " << increase * 100 << "%" << endl;
	cout << endl;
	cout << " 每台风机的偏航角度 " << endl;
	for (int i = 0; i < m_run.yaw.size_x; ++i)
	{
		cout << m_run.yaw.simulation.turbines.turbName[i] << "    " << gamma360[i] << endl;
	}
	return 0;
}

int WakeControlOnline::run(Column& nextGamma, double& u, double& theta360, Column& currentAbsGamma, Bool& isWork, string& date)
{
	int p = 1; // 名称保留小数点位数
	std::filesystem::path outputPath = "output";
	std::filesystem::path tecfile = "tecOnline";
	std::filesystem::path fullPath;

	fullPath = outputPath / tecfile;

	if (!std::filesystem::exists(outputPath))
	{
		std::filesystem::create_directory(outputPath);
	}

	if (!std::filesystem::exists(fullPath))
	{
		std::filesystem::create_directory(fullPath);
	}
	string fileName, titleName;
	getIsWork(isWork);
	getGammaRange(theta360, currentAbsGamma);
	getRandomRange(theta360, currentAbsGamma);
	getGamma(nextGamma, u, theta360);
	getTitle(fileName, titleName, u, theta360, date, p);

	fullPath = outputPath / tecfile / fileName;
	string fullPathName = fullPath.string();
	simulation.plot(u, theta360, isWork, m_run.model, nextGamma, fullPathName, titleName);
	return 0;
}