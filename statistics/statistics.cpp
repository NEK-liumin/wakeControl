#include "statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "excel.h"
#include <algorithm>
#include <locale>
#include <codecvt>

int progress(int& count, int n)
{
	count++;
	cout << "\rProgress: " << std::setw(4) << count << "/" << n << "(" << std::setw(6) << std::setprecision(3) << count * 1.0 / n * 100 << "%)";
	return 0;
}

int Statistics::getUThetaColumn()
{
	int nVel = 0, nTheta = 0;
	nVel = floor((uMax - uMin) / run->deltaU) + 1;
	nTheta = floor((thetaMax - thetaMin) / run->deltaTheta);
	// 最小最大序列
	thetaMax = run->thetaEnd;
	getUniformA(u, uMin, run->deltaU, nVel);
	getUniformA(theta360, thetaMin, run->deltaTheta, nTheta);
	// 切入切出序列
	double ufirst = run->yaw.simulation.turbines.uMin;
	double ulast = run->yaw.simulation.turbines.uMax;
	nVel = floor((ulast - ufirst) / run->deltaU) + 1;
	getUniformA(uCut, ufirst, run->deltaU, nVel);
	// 偏航范围序列
	uYaw = run->u;
	thetaYaw = run->theta360;
	return 0;
}

Statistics::Statistics()
{
	output = "output";
	statistics = "statistics";
	absOutput = output / statistics;
	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directories(output);
	}
	if (!std::filesystem::exists(absOutput))
	{
		std::filesystem::create_directories(absOutput);
	}
}

int Statistics::setStatistics(Run& run)
{
	this->run = &run;
	isWork.resize(run.yaw.size_x);
	for (int i = 0; i < run.yaw.size_x; ++i)isWork[i] = '1';
	output = "output";
	statistics = "statistics";
	absOutput = output / statistics;
	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directories(output);
	}
	if (!std::filesystem::exists(absOutput))
	{
		std::filesystem::create_directories(absOutput);
	}
	return 0;
}
Statistics::Statistics(Run& run)
{
	this->run = &run;
	isWork.resize(run.yaw.size_x);
	for (int i = 0; i < run.yaw.size_x; ++i)isWork[i] = '1';
	output = "output";
	statistics = "statistics";
	absOutput = output / statistics;
	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directories(output);
	}
	if (!std::filesystem::exists(absOutput))
	{
		std::filesystem::create_directories(absOutput);
	}
}

int Statistics::readFile(bool isDelBadVal)
{
	this->isDelBadVal = isDelBadVal;
	std::ifstream timeseries("input/timeseries.txt");
	if (!timeseries.is_open())
	{
		std::cerr << "Error opening timeseries.txt!" << std::endl;
		return 1;
	}
	string line;
	while (std::getline(timeseries, line))
	{
		std::stringstream ss(line);
		std::string date0, time0; // 临时变量，存放日期
		double u, theta; // 临时变量，存放风速风向

		ss >> date0 >> time0 >> u >> theta;

		date.push_back(date0);
		time.push_back(time0);
		windSpeed.push_back(u);
		direction.push_back(theta);
	}
	timeseries.close();

	badValNum = 0;
	std::ofstream badValue("input/badValue.log");
	if (!badValue.is_open())
	{
		std::cerr << "Error creating badValue.log" << std::endl;
		return 1;
	}
	for (size_t i = 0; i < windSpeed.size(); ++i)
	{
		if (windSpeed[i] <= 0 || windSpeed[i] > 60 || direction[i] < 0 || direction[i] > 360)
		{
			badValue << date[i] << " " << time[i] << " "
				<< std::fixed << std::setprecision(2) << windSpeed[i] << " "
				<< std::fixed << std::setprecision(2) << direction[i] << std::endl;
			badValNum++;
		}
	}
	badValue.close();
	if (badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
	}

	if (isDelBadVal)
	{
		vector<size_t> indicesToRemove;

		for (size_t i = 0; i < windSpeed.size(); ++i)
		{
			if (windSpeed[i] <= 0 || windSpeed[i] > 80 || direction[i] < 0 || direction[i] > 360)
			{
				indicesToRemove.push_back(i);
			}
		}
		for (size_t i = indicesToRemove.size(); i > 0; --i)
		{
			size_t index = indicesToRemove[i - 1];
			date.erase(date.begin() + index);
			time.erase(time.begin() + index);
			windSpeed.erase(windSpeed.begin() + index);
			direction.erase(direction.begin() + index);
		}
	}
	
	auto uMaxTemp = std::max_element(windSpeed.begin(), windSpeed.end());
	uMax = *uMaxTemp;
	
	getUThetaColumn();
	return 0;
}

int Statistics::windStatistics()
{
	int nVel = u.size();
	int nTheta = theta360.size();
	getZeroMatrix(probability, nVel, nTheta);

	for (int i = 0; i < date.size(); ++i)
	{
		int j = floor(windSpeed[i] / run->deltaU + 0.5);
		int k = floor(direction[i] / run->deltaTheta + 0.5);
		if (j <= 0)j = 0;
		if (j >= nVel - 1)j = nVel - 1;
		if (k <= 0)k = 0;
		if (k > nTheta - 1)k = 0;
		probability[j][k]++;
	}
	for (int i = 0; i < nVel; ++i)
	{
		for (int j = 0; j < nTheta; ++j)
		{
			probability[i][j] /= date.size();
		}
	}

	getZeroColumn(pWindSpeed, nVel);
	getZeroColumn(pWindTheta, nTheta);

	for (int i = 0; i < nVel; ++i)
	{
		for (int j = 0; j < nTheta; ++j)
		{
			pWindSpeed[i] += probability[i][j];
			pWindTheta[j] += probability[i][j];
		}
	}
	return 0;
}
int Statistics::get_gWithoutWeak()
{
	gWithoutWeak = 0;
	double gi;
	if (pWindSpeed.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		run->yaw.simulation.wake.turbines->getHypothesisPower(gi, u[i]);
		gWithoutWeak += gi * pWindSpeed[i];
	}
	gWithoutWeak *= t;
	return 0;
}
int Statistics::get_g0()
{
	g0 = 0;
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	
	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
			gi = yaw.initialPower();
			g0 += gi * probability[i][j];
		}
	}
	g0 *= t;
	return 0;
}
int Statistics::get_g()
{
	g = 0;
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			if (u[i] < uCut[0] || u[i] > uCut[uCut.size() - 1])
			{
				gi = 0;
				g += gi * probability[i][j];
			}
			else if (u[i] < uYaw[0] || u[i]>uYaw[uYaw.size() - 1])
			{
				yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
				gi = yaw.initialPower();
				g += gi * probability[i][j];
			}
			else
			{
				int ni, nj;
				double fi, fj;
				findx(ni, fi, uYaw, u[i]);
				findx(nj, fj, thetaYaw, theta360[j]);
				interpolation(gi, run->P, ni, nj, fi, fj);
				g += gi * probability[i][j];
			}
		}
	}
	g *= t;
	return 0;
}
int Statistics::get_weakLoss0()
{
	if (gWithoutWeak > 0)
	{
		weakLoss0 = (gWithoutWeak - g0) / gWithoutWeak;
	}
	else
		weakLoss0 = 0;
	return 0;
}
int Statistics::get_weakLoss()
{
	if (gWithoutWeak > 0)
	{
		weakLoss = (gWithoutWeak - g) / gWithoutWeak;
	}
	else
	{
		weakLoss = 0;
	}
	return 0;
}
int Statistics::get_gIncrease()
{
	gIncrease = (g - g0) / g0;
	return 0;
}
int Statistics::get_gPerThetaWithoutWeak()
{
	int n = theta360.size();
	getZeroColumn(gPerThetaWithoutWeak, n);
	double gi;
	if (probability.size() != u.size())
	{
		cout << "probability.size() != u.size()" << endl;
		return 0;
	}
	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			run->yaw.simulation.wake.turbines->getHypothesisPower(gi, u[i]);
			gPerThetaWithoutWeak[j] += gi * probability[i][j];
		}
	}
	getAlphaA(gPerThetaWithoutWeak, t);
	return 0;
}
int Statistics::get_g0PerTheta()
{
	int n = theta360.size();
	getZeroColumn(g0PerTheta, n);
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);

	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}

	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
			gi = yaw.initialPower();
			g0PerTheta[j] += gi * probability[i][j];
		}
	}
	getAlphaA(g0PerTheta, t);
	return 0;
}
int Statistics::get_gPerTheta()
{
	int n = theta360.size();
	getZeroColumn(gPerTheta, n);
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			if (u[i] < uCut[0] || u[i] > uCut[uCut.size() - 1])
			{
				gi = 0;
				gPerTheta[j] += gi * probability[i][j];
			}
			else if (u[i] < uYaw[0] || u[i]>uYaw[uYaw.size() - 1])
			{
				yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
				gi = yaw.initialPower();
				gPerTheta[j] += gi * probability[i][j];
			}
			else
			{
				int ni, nj;
				double fi, fj;
				findx(ni, fi, uYaw, u[i]);
				findx(nj, fj, thetaYaw, theta360[j]);
				interpolation(gi, run->P, ni, nj, fi, fj);
				gPerTheta[j] += gi * probability[i][j];
			}
		}
	}
	getAlphaA(gPerTheta, t);
	return 0;
}
int Statistics::get_gPerUWithoutWeak()
{
	int n = u.size();
	getZeroColumn(gPerUWithoutWeak, n);
	double gi;
	if (probability.size() != u.size())
	{
		cout << "probability.size() != u.size()" << endl;
		return 0;
	}
	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			run->yaw.simulation.wake.turbines->getHypothesisPower(gi, u[i]);
			gPerUWithoutWeak[i] += gi * probability[i][j];
		}
	}
	getAlphaA(gPerUWithoutWeak, t);
	return 0;
}
int Statistics::get_g0PerU()
{
	int n = u.size();
	getZeroColumn(g0PerU, n);
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);

	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}

	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
			gi = yaw.initialPower();
			g0PerU[i] += gi * probability[i][j];
		}
	}
	getAlphaA(g0PerU, t);
	return 0;
}
int Statistics::get_gPerU()
{
	int n = u.size();
	getZeroColumn(gPerU, n);
	double gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			if (u[i] < uCut[0] || u[i] > uCut[uCut.size() - 1])
			{
				gi = 0;
				gPerU[i] += gi * probability[i][j];
			}
			else if (u[i] < uYaw[0] || u[i]>uYaw[uYaw.size() - 1])
			{
				yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
				gi = yaw.initialPower();
				gPerU[i] += gi * probability[i][j];
			}
			else
			{
				int ni, nj;
				double fi, fj;
				findx(ni, fi, uYaw, u[i]);
				findx(nj, fj, thetaYaw, theta360[j]);
				interpolation(gi, run->P, ni, nj, fi, fj);
				gPerU[i] += gi * probability[i][j];
			}
		}
	}
	getAlphaA(gPerU, t);
	return 0;
}
int Statistics::get_weakLoss0PerU()
{
	getAMinusB(weakLoss0PerU, gPerUWithoutWeak, g0PerU);
	for (int i = 0; i < weakLoss0PerU.size(); ++i)
	{
		if (gPerUWithoutWeak[i] > 0)
		{
			weakLoss0PerU[i] = weakLoss0PerU[i] / gPerUWithoutWeak[i];
		}
		else
		{
			weakLoss0PerU[i] = 0;
		}
	}
	return 0;
}
int Statistics::get_weakLossPerU()
{
	getAMinusB(weakLossPerU, gPerUWithoutWeak, gPerU);
	for (int i = 0; i < weakLoss0PerU.size(); ++i)
	{
		if (gPerUWithoutWeak[i] > 0)
		{
			weakLossPerU[i] = weakLossPerU[i] / gPerUWithoutWeak[i];
		}
		else
		{
			weakLossPerU[i] = 0;
		}
	}
	return 0;
}
int Statistics::get_gIncreasePerU()
{
	getAMinusB(gIncreasePerU, weakLoss0PerU, weakLossPerU);
	return 0;
}
int Statistics::get_gPerTurbWithoutWeak()
{
	int n = run->yaw.simulation.turbines.turbNum;
	getZeroColumn(gPerTurbWithoutWeak, n);
	Column gi;
	if (probability.size() != u.size())
	{
		cout << "probability.size() != u.size()" << endl;
		return 0;
	}
	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			run->yaw.simulation.wake.turbines->getHypothesisPower(gi, u[i]);
			getAlphaA(gi, probability[i][j]);
			getAPlusB(gPerTurbWithoutWeak, gi);
		}
	}
	getAlphaA(gPerTurbWithoutWeak, t);
	return 0;
}
int Statistics::get_g0PerTurb()
{
	int n = run->yaw.simulation.turbines.turbNum;
	getZeroColumn(g0PerTurb, n);
	Column gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);

	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}

	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
			gi = yaw.initialPower_i();
			getAlphaA(gi, probability[i][j]);
			getAPlusB(g0PerTurb, gi);
		}
	}
	getAlphaA(g0PerTurb, t);
	return 0;
}
int Statistics::get_gPerTurb()
{
	int n = run->yaw.simulation.turbines.turbNum;
	getZeroColumn(gPerTurb, n);
	Column gi;
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	if (probability.size() != u.size())
	{
		cout << "pWindSpeed.size() != u.size()" << endl;
		return 0;
	}

	if (probability[0].size() != theta360.size())
	{
		cout << "probability[0].size() != theta360.size()" << endl;
		return 0;
	}
	for (int i = 0; i < u.size(); ++i)
	{
		for (int j = 0; j < theta360.size(); ++j)
		{
			if (u[i] < uCut[0] || u[i] > uCut[uCut.size() - 1])
			{
				getZeroColumn(gi, n);
				getAlphaA(gi, probability[i][j]);
				getAPlusB(gPerTurb, gi);
			}
			else if (u[i] < uYaw[0] || u[i]>uYaw[uYaw.size() - 1])
			{
				yaw.reset(u[i], theta360[j], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
				gi = yaw.initialPower_i();
				getAlphaA(gi, probability[i][j]);
				getAPlusB(gPerTurb, gi);
			}
			else
			{
				int ni, nj;
				double fi, fj;
				findx(ni, fi, uYaw, u[i]);
				findx(nj, fj, thetaYaw, theta360[j]);
				for (int k = 0; k < n; ++k)
				{
					interpolation(gi[k], run->P_i[k], ni, nj, fi, fj);
				}
				getAlphaA(gi, probability[i][j]);
				getAPlusB(gPerTurb, gi);
			}
		}
	}
	getAlphaA(gPerTurb, t);
	return 0;
}
int Statistics::get_weakLoss0PerTurb()
{
	getAMinusB(weakLoss0PerTurb, gPerTurbWithoutWeak, g0PerTurb);
	for (int i = 0; i < weakLoss0PerTurb.size(); ++i)
	{
		if (gPerTurbWithoutWeak[i] > 0)
		{
			weakLoss0PerTurb[i] = weakLoss0PerTurb[i] / gPerTurbWithoutWeak[i];
		}
		else
		{
			weakLoss0PerTurb[i] = 0;
		}
	}
	return 0;
}
int Statistics::get_weakLossPerTurb()
{
	getAMinusB(weakLossPerTurb, gPerTurbWithoutWeak, gPerTurb);
	for (int i = 0; i < weakLoss0PerTurb.size(); ++i)
	{
		if (gPerTurbWithoutWeak[i] > 0)
		{
			weakLossPerTurb[i] = weakLossPerTurb[i] / gPerTurbWithoutWeak[i];
		}
		else
		{
			weakLossPerTurb[i] = 0;
		}
	}
	return 0;
}
int Statistics::get_gIncreasePerTurb()
{
	getAMinusB(gIncreasePerTurb, weakLoss0PerTurb, weakLossPerTurb);
	return 0;
}
int Statistics::get_gTimeSeriseWithoutWeak()
{
	if (isDelBadVal && badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
	}
	int n = date.size();
	getZeroColumn(gTimeSeriseWithoutWeak, n);

	for (int i = 0; i < n; ++i)
	{
		run->yaw.simulation.wake.turbines->getHypothesisPower(gTimeSeriseWithoutWeak[i], windSpeed[i]);
	}
	return 0;
}
int Statistics::get_g0TimeSerise()
{
	if (isDelBadVal && badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
	}
	int n = date.size();
	getZeroColumn(g0TimeSerise, n);
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	for (int i = 0; i < n; ++i)
	{
		yaw.reset(windSpeed[i], direction[i], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
		g0TimeSerise[i] = yaw.initialPower();
	}
	return 0;
}
int Statistics::get_gTimeSerise()
{
	if (isDelBadVal && badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
	}
	int n = date.size();
	getZeroColumn(gTimeSerise, n);
	Yaw yaw(run->uBegin, run->thetaBegin, isWork, run->rho, run->model, run->maxGamma, run->randomRange);
	for (int i = 0; i < n; ++i)
	{
		if (windSpeed[i] < uCut[0] || windSpeed[i] > uCut[uCut.size() - 1])
		{
			gTimeSerise[i] = 0;
		}
		
		else if (windSpeed[i] < uYaw[0] || windSpeed[i]>uYaw[uYaw.size() - 1])
		{
			yaw.reset(windSpeed[i], direction[i], isWork, run->rho, run->model, run->maxGamma, run->randomRange);
			gTimeSerise[i] = yaw.initialPower();
		}
		else
		{
			int ni, nj;
			double fi, fj;
			findx(ni, fi, uYaw, windSpeed[i]);
			findx(nj, fj, thetaYaw, direction[i]);
			interpolation(gTimeSerise[i], run->P, ni, nj, fi, fj);
		}
	}
	return 0;
}

int Statistics::get_all()
{
	int nLoop = 25;
	int count = 0;
	cout << "Statistics are being taken..." << endl;
	windStatistics();progress(count, nLoop);
	get_gWithoutWeak(); progress(count, nLoop);
	get_g0(); progress(count, nLoop);
	get_g(); progress(count, nLoop);
	get_weakLoss0(); progress(count, nLoop);
	get_weakLoss(); progress(count, nLoop);
	get_gIncrease(); progress(count, nLoop);
	get_gPerThetaWithoutWeak(); progress(count, nLoop);
	get_g0PerTheta(); progress(count, nLoop);
	get_gPerTheta(); progress(count, nLoop);
	get_gPerUWithoutWeak(); progress(count, nLoop);
	get_g0PerU(); progress(count, nLoop);
	get_gPerU(); progress(count, nLoop);
	get_weakLoss0PerU(); progress(count, nLoop);
	get_weakLossPerU(); progress(count, nLoop);
	get_gIncreasePerU(); progress(count, nLoop);
	get_gPerTurbWithoutWeak(); progress(count, nLoop);
	get_g0PerTurb(); progress(count, nLoop);
	get_gPerTurb(); progress(count, nLoop);
	get_weakLoss0PerTurb(); progress(count, nLoop);
	get_weakLossPerTurb(); progress(count, nLoop);
	get_gIncreasePerTurb(); progress(count, nLoop);
	get_gTimeSeriseWithoutWeak(); progress(count, nLoop);
	get_g0TimeSerise(); progress(count, nLoop);
	get_gTimeSerise(); progress(count, nLoop);
	cout << endl;
	
	return 0;
}

int Statistics::writeColumn(Column& x, Column& y, string& titlex, string& titley, string& fileName)
{
	int nx = x.size();
	int ny = y.size();
	if (nx != ny)
	{
		cout << "nx != ny" << endl;
		return 0;
	}
	string varName = titlex + "," + titley;

	writeExcel(fileName, varName, x, y, 10, 5);
	
	return 0;
}
int Statistics::writeColumn(vector<string>& x, Column& y, string& titlex, string& titley, string& fileName)
{
	int nx = x.size();
	int ny = y.size();
	if (nx != ny)
	{
		cout << "nx != ny" << endl;
		return 0;
	}
	string varName = titlex + "," + titley;
	writeExcel(fileName, varName, x, y, 10, 5);
	return 0;
}
int Statistics::writeSector_x(Column& x, string& title, string& fileName)
{
	string titleLeft = "Theta";
	writeColumn(theta360, x, titleLeft, title, fileName);
	return 0;
}
int Statistics::writeSegment_x(Column& x, string& title, string& fileName)
{
	string titleLeft = "U";
	writeColumn(u, x, titleLeft, title, fileName);
	return 0;
}
int Statistics::writeTurbine_x(Column& x, string& title, string& fileName)
{
	string titleLeft = "name";
	writeColumn(run->yaw.simulation.turbines.turbName, x, titleLeft, title, fileName);
	return 0;
}
int Statistics::writeTime_x(Column& x, string& title, string& fileName)
{
	string titleLeft = "Date";
	vector<string>date_all;
	int n = date.size();
	date_all.resize(n);
	for (int i = 0; i < n; ++i)
	{
		date_all[i] = date[i] + " " + time[i];
	}
	writeColumn(date_all, x, titleLeft, title, fileName);
	return 0;
}

int Statistics::writeSingleStatistics()
{
	string name = (absOutput / "年发电量.csv").string();
	std::ofstream file(name);
	if (file.is_open())
	{
		file << gWithoutWeak << ", Annual output without wake" << endl;
		file << g0 << ", Annual output with wake but without yawing" << endl;
		file << g << ", Annual output with wake and yawing" << endl;
		file << weakLoss0 << ", Annual wake loss without yawing" << endl;
		file << weakLoss << ", Annual wake loss with yawing" << endl;
		file << gIncrease << ", Annual increased output with yawing" << endl;
	}
	file.close();
	return 0;
}
int Statistics::write_gPerThetaWithoutWeak()
{
	string name = (absOutput / "不同风向的年发电量（不考虑尾流）.csv").string();
	string title = "Annual output";
	writeSector_x(gPerThetaWithoutWeak, title, name);
	return 0;
}
int Statistics::write_g0PerTheta()
{
	string name = (absOutput / "不同风向的年发电量（考虑尾流但不偏航）.csv").string();
	string title = "Annual output";
	writeSector_x(g0PerTheta, title, name);
	return 0;
}
int Statistics::write_gPerTheta()
{
	string name = (absOutput / "不同风向的年发电量（考虑尾流且偏航）.csv").string();
	string title = "Annual output";
	writeSector_x(gPerTheta, title, name);
	return 0;
}
int Statistics::write_gPerUWithoutWeak()
{
	string name = (absOutput / "不同风速的年发电量（不考虑尾流）.csv").string();
	string title = "Annual output";
	writeSegment_x(gPerUWithoutWeak, title, name);
	return 0;
}
int Statistics::write_g0PerU()
{
	string name = (absOutput / "不同风速的年发电量（考虑尾流但不偏航）.csv").string();
	string title = "Annual output";
	writeSegment_x(g0PerU, title, name);
	return 0;
}
int Statistics::write_gPerU()
{
	string name = (absOutput / "不同风速的年发电量（偏航后）.csv").string();
	string title = "Annual output";
	writeSegment_x(gPerU, title, name);
	return 0;
}
int Statistics::write_weakLoss0PerU()
{
	string name = (absOutput / "不同风速的尾流损失（不偏航）.csv").string();
	string title = "Weak loss";
	writeSegment_x(weakLoss0PerU, title, name);
	return 0;
}
int Statistics::write_weakLossPerU()
{
	string name = (absOutput / "不同风速的尾流损失（偏航）.csv").string();
	string title = "Weak loss";
	writeSegment_x(weakLossPerU, title, name);
	return 0;
}
int Statistics::write_gIncreasePerU()
{
	string name = (absOutput / "不同风速偏航后尾流损失降低.csv").string();
	string title = "Output Increase";
	writeSegment_x(gIncreasePerU, title, name);
	return 0;
}
int Statistics::write_gPerTurbWithoutWeak()
{
	string name = (absOutput / "不同风机年发电量（不考虑尾流）.csv").string();
	string title = "Annual output";
	writeTurbine_x(gPerTurbWithoutWeak, title, name);
	return 0;
}
int Statistics::write_g0PerTurb()
{
	string name = (absOutput / "不同风机年发电量（考虑尾流但不偏航）.csv").string();
	string title = "Annual output";
	writeTurbine_x(g0PerTurb, title, name);
	return 0;
}
int Statistics::write_gPerTurb()
{
	string name = (absOutput / "不同风机年发电量（偏航后）.csv").string();
	string title = "Annual output";
	writeTurbine_x(gPerTurb, title, name);
	return 0;
}
int Statistics::write_weakLoss0PerTurb()
{
	string name = (absOutput / "不同风机尾流损失（不偏航）.csv").string();
	string title = "Weak loss";
	writeTurbine_x(weakLoss0PerTurb, title, name);
	return 0;
}
int Statistics::write_weakLossPerTurb()
{
	string name = (absOutput / "不同风机尾流损失（偏航）.csv").string();
	string title = "Weak loss";
	writeTurbine_x(weakLossPerTurb, title, name);
	return 0;
}
int Statistics::write_gIncreasePerTurb()
{
	string name = (absOutput / "不同风机偏航后尾流损失降低.csv").string();
	string title = "output Increase";
	writeTurbine_x(gIncreasePerTurb, title, name);
	return 0;
}
int Statistics::write_gTimeSeriseWithoutWeak()
{
	string name = (absOutput / "时序总功率（不考虑尾流）.csv").string();
	string title = "Power Generation";
	writeTime_x(gTimeSeriseWithoutWeak, title, name);
	return 0;
}
int Statistics::write_g0TimeSerise()
{
	string name = (absOutput / "时序总功率（考虑尾流但不偏航）.csv").string();
	string title = "Power Generation";
	writeTime_x(g0TimeSerise, title, name);
	return 0;
}
int Statistics::write_gTimeSerise()
{
	string name = (absOutput / "时序总功率（偏航后）.csv").string();
	string title = "Power Generation";
	writeTime_x(gTimeSerise, title, name);
	return 0;
}

int Statistics::write_all(bool isTranspose)
{
	string possibleName = (absOutput / "风速风向概率矩阵.csv").string();
	string pUName = (absOutput / "风速概率曲线.csv").string();
	string pThetaName = (absOutput / "风向概率曲线.csv").string();

	/*std::ofstream possible(possibleName);
	std::ofstream pU(pUName);
	std::ofstream pTheta(pThetaName);*/

	if (isTranspose)
	{
		string varName = "Theta\\U";
		Matrix probabilityT;
		getAT(probabilityT, probability);
		writeExcel(possibleName, varName, u, theta360, probabilityT, 10, 5);
	}
	else
	{
		string varName = "U\\Theta";
		writeExcel(possibleName, varName, theta360, u, probability, 10, 5);
	}
	string varName = "U, probability";
	writeExcel(pUName, varName, u, pWindSpeed, 10, 5);
	varName = "Theta,probability";
	writeExcel(pThetaName, varName, theta360, pWindTheta, 10, 5);
	/////////////////////////////////////////////////////////////////////

	std::string P0name = (absOutput / "不同风速风向风场发电功率（不偏航）.csv").string();
	std::string Pname = (absOutput / "不同风速风向风场发电功率（偏航）.csv").string();
	std::string deltaPname = (absOutput / "不同风速风向偏航后风场发电功率增加.csv").string();

	if (isTranspose)
	{
		varName = "Theta \\ U";
		Matrix P0T;
		getAT(P0T, run->P0);
		writeExcel(P0name, varName, run->u, run->theta360, P0T, 6, 2);

		Matrix PT;
		getAT(PT, run->P);
		writeExcel(Pname, varName, run->u, run->theta360, PT, 6, 2);

		Matrix deltaPT;
		getAT(deltaPT, run->deltaP);
		writeExcel(deltaPname, varName, run->u, run->theta360, deltaPT, 6, 2);
	}
	// 如果不转置，以下
	else
	{
		varName = "U \\ Theta";
		writeExcel(P0name, varName, run->theta360, run->u, run->P0, 6, 2);
		writeExcel(Pname, varName, run->theta360, run->u, run->P, 6, 2);
		writeExcel(deltaPname, varName, run->theta360, run->u, run->deltaP, 6, 2);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	std::filesystem::path subSubOutput("不同风机发电功率");
	std::filesystem::path outputDir;
	outputDir = absOutput / subSubOutput;

	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}

	for (int i = 0; i < run->yaw.size_x; ++i)
	{
		std::string P0_iName = (outputDir / ("不偏航_" + run->yaw.simulation.turbines.turbName[i] + ".csv")).string();
		std::ofstream P0file(P0_iName);
		std::string P_iName = (outputDir / ("偏航_" + run->yaw.simulation.turbines.turbName[i] + ".csv")).string();
		std::ofstream Pfile(P_iName);
		std::string deltaP_iName = (outputDir / ("偏航后功率变化_" + run->yaw.simulation.turbines.turbName[i] + ".csv")).string();
		std::ofstream deltaPfile(deltaP_iName);
		if (P0file.is_open() && Pfile.is_open() && deltaPfile.is_open())
		{
			if (isTranspose)
			{
				varName = "Theta \\ U";
				Matrix deltaP_i;
				Matrix P0_iT, P_iT, deltaP_iT;

				getAMinusB(deltaP_i, run->P_i[i], run->P0_i[i]);

				getAT(P0_iT, run->P0_i[i]);
				getAT(P_iT, run->P_i[i]);
				getAT(deltaP_iT, deltaP_i);

				writeExcel(P0_iName, varName, run->u, run->theta360, P0_iT, 10, 5);
				writeExcel(P_iName, varName, run->u, run->theta360, P_iT, 10, 5);
				writeExcel(deltaP_iName, varName, run->u, run->theta360, deltaP_iT, 10, 5);
			}
			else
			{
				varName = "U \\ Theta";
				Matrix deltaP_i;
				getAMinusB(deltaP_i, run->P_i[i], run->P0_i[i]);

				writeExcel(P0_iName, varName, run->theta360, run->u, run->P0_i[i], 10, 5);
				writeExcel(P_iName, varName, run->theta360, run->u, run->P_i[i], 10, 5);
				writeExcel(deltaP_iName, varName, run->theta360, run->u, deltaP_i, 10, 5);
			}
		}
		else
		{
			std::cerr << "Failed to create file: " << P0_iName << ", " << P_iName << " and " << deltaP_iName << std::endl;
		}
	}

	writeSingleStatistics();
	write_gPerThetaWithoutWeak();
	write_g0PerTheta();
	write_gPerTheta();
	write_gPerUWithoutWeak();
	write_g0PerU();
	write_gPerU();
	write_weakLoss0PerU();
	write_weakLossPerU();
	write_gIncreasePerU();
	write_gPerTurbWithoutWeak();
	write_g0PerTurb();
	write_gPerTurb();
	write_weakLoss0PerTurb();
	write_weakLossPerTurb();
	write_gIncreasePerTurb();
	write_gTimeSeriseWithoutWeak();
	write_g0TimeSerise();
	write_gTimeSerise();
	cout << "Done!" << endl;
	return 0;
}