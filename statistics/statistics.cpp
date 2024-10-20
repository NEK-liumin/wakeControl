#include "statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "excel.h"
#include <algorithm>

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

}

int Statistics::setStatistisc(Run& run)
{
	this->run = &run;
	return 0;
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

	if (isDelBadVal)
	{
		vector<size_t> indicesToRemove;

		for (size_t i = 0; i < windSpeed.size(); ++i)
		{
			if (windSpeed[i] <= 0 || windSpeed[i] > 60 || direction[i] < 0 || direction[i] > 360)
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
		auto uMaxTemp = std::max_element(windSpeed.begin(), windSpeed.end());
		uMax = *uMaxTemp;
	}
	else
	{
		badValNum = 0;
		std::ofstream badValue("input/badValue.log");
		if (!badValue.is_open())
		{
			std::cerr << "Error creating badValue.log" << std::endl;
			return 1;
		}
		for (size_t i = 0; i < windSpeed.size(); ++i)
		{
			if (windSpeed[i] <= 0 || windSpeed[i]> 60 || direction[i] < 0 || direction[i] > 360)
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
			return 0;
		}
		auto uMaxTemp = std::max_element(windSpeed.begin(), windSpeed.end());
		uMax = *uMaxTemp;
	}
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

int Statistics::powerStatistics()
{
	// 直接输出run.u, run.theta360, run.p, run.p0即可，无需统计
	return 0;
}

int Statistics::power_iStatistics()
{
	// 直接输出run.u, run.theta360, run.p_i, run.p0_i即可，无需统计
	return 0;
}

//int Statistics::windStaCutInOut(Matrix& prob)
//{
//	double uCutIn = run->yaw.simulation.turbines.uMin;
//	double uCutOut = run->yaw.simulation.turbines.uMax;
//	double deltaU = run->deltaU;
//	double deltaTheta = run->deltaTheta;
//	int nVel = floor((uCutOut - uCutIn) / deltaU) + 1;
//	int nTheta = floor((thetaMax - thetaMin) / deltaTheta);
//	// 计算风速风向概率
//	// 和possibility不同，这里的概率是从切入速度开始，到切出速度为止
//	// 这个概率只作为局部参数，统计年发电量用，不输出表格
//	getZeroMatrix(prob, nVel, nTheta);
//	for (int i = 0; i < date.size(); ++i)
//	{
//		double j = -1, k = -1;
//		if (windSpeed[i]>=uCutIn && windSpeed[i]<=uCutOut)
//		{
//			j = (windSpeed[i] + 0.5 * deltaU - uCutIn) / deltaU;
//			if (j <= 0)j = 0;
//			if (j >= nVel - 1)j = nVel - 1;
//		}
//		if (direction[i] > thetaMax - 0.5 * deltaTheta)
//		{
//			direction[i] -= thetaMax;
//		}
//		k = (direction[i] + 0.5 * deltaTheta - thetaMin) / deltaTheta;
//		if (k <= 0)k = 0;
//		if (k >= nTheta - 1)k = nTheta - 1;
//		if (j >= 0)prob[j][k] += 1;
//	}
//	getAlphaA(prob, 1.0 / date.size());
//	return 0;
//}

int Statistics::powerStaCutInOut(Matrix& pow)
{

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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
	
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
			yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
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
				yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	weakLoss0 = (gWithoutWeak - g0) / gWithoutWeak;
	return 0;
}
int Statistics::get_weakLoss()
{
	weakLoss = (gWithoutWeak - g) / gWithoutWeak;
	return 0;
}
int Statistics::get_gIncrease()
{
	gIncrease = weakLoss - weakLoss0;
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);

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
			yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
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
				yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);

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
			yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
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
				yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
		weakLoss0PerU[i] = weakLoss0PerU[i] / gPerUWithoutWeak[i];
	}
	return 0;
}
int Statistics::get_weakLossPerU()
{
	getAMinusB(weakLossPerU, gPerUWithoutWeak, gPerU);
	for (int i = 0; i < weakLoss0PerU.size(); ++i)
	{
		weakLossPerU[i] = weakLossPerU[i] / gPerUWithoutWeak[i];
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);

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
			yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
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
				yaw.reset(u[i], theta360[j], run->rho, run->model, run->randomRange);
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
		weakLoss0PerTurb[i] = weakLoss0PerTurb[i] / gPerTurbWithoutWeak[i];
	}
	return 0;
}
int Statistics::get_weakLossPerTurb()
{
	getAMinusB(weakLossPerTurb, gPerTurbWithoutWeak, gPerTurb);
	for (int i = 0; i < weakLoss0PerTurb.size(); ++i)
	{
		weakLossPerTurb[i] = weakLossPerTurb[i] / gPerTurbWithoutWeak[i];
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
	if (isDelBadVal || badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
		return 0;
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
	if (isDelBadVal || badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
		return 0;
	}
	int n = date.size();
	getZeroColumn(g0TimeSerise, n);
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
	for (int i = 0; i < n; ++i)
	{
		yaw.reset(windSpeed[i], direction[i], run->rho, run->model, run->randomRange);
		g0TimeSerise[i] = yaw.initialPower();
	}
	return 0;
}
int Statistics::get_gTimeSerise()
{
	if (isDelBadVal || badValNum > 0)
	{
		std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
		return 0;
	}
	int n = date.size();
	getZeroColumn(gTimeSerise, n);
	Yaw yaw(run->uBegin, run->thetaBegin, run->rho, run->model, run->randomRange);
	for (int i = 0; i < n; ++i)
	{
		if (windSpeed[i] < uCut[0] || windSpeed[i] > uCut[uCut.size() - 1])
		{
			gTimeSerise[i] = 0;
		}
		else if (u[i] < uYaw[0] || u[i]>uYaw[uYaw.size() - 1])
		{
			yaw.reset(windSpeed[i], direction[i], run->rho, run->model, run->randomRange);
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
}
int Statistics::writeFile(bool isTranspose)
{
	std::filesystem::path output("output");
	std::filesystem::path statistics("statistics");
	std::filesystem::path absOutput;
	absOutput = output / statistics;

	if (!std::filesystem::exists(absOutput))
	{
		std::filesystem::create_directories(absOutput);
	}
	string possibleName = (absOutput / "possible.csv").string();
	string pUName = (absOutput / "pWindSpeed.csv").string();
	string pThetaName = (absOutput / "pWindTheta.csv").string();

	std::ofstream possible(possibleName);
	std::ofstream pU(pUName);
	std::ofstream pTheta(pThetaName);

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
	varName = "Theta,probility";
	writeExcel(pThetaName, varName, theta360, pWindTheta, 10, 5);
	/////////////////////////////////////////////////////////////////////

	std::string P0name = (absOutput / "P0.csv").string();
	std::string Pname = (absOutput / "P.csv").string();
	std::string deltaPname = (absOutput / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	if (isTranspose)
	{
		if (P0file.is_open())
		{
			string varName = "Theta \\ U";
			Matrix P0T;
			getAT(P0T, run->P0);
			writeExcel(P0name, varName, run->u, run->theta360, P0T, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << P0name << std::endl;
		}
		P0file.close();

		if (Pfile.is_open())
		{
			string varName = "Theta \\ U";
			Matrix PT;
			getAT(PT, run->P);
			writeExcel(Pname, varName, run->u, run->theta360, PT, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << Pname << std::endl;
		}
		Pfile.close();

		if (deltaPfile.is_open())
		{
			string varName = "Theta \\ U";
			Matrix deltaPT;
			getAT(deltaPT, run->deltaP);
			writeExcel(deltaPname, varName, run->u, run->theta360, deltaPT, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << deltaPname << std::endl;
		}
		deltaPfile.close();
		return 0;
	}
	// 如果不转置，以下
	if (P0file.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(P0name, varName, run->theta360, run->u, run->P0, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(Pname, varName, run->theta360, run->u, run->P, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(deltaPname, varName, run->theta360, run->u, run->deltaP, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();
	///////////////////////////////////////////////////////////////////////////////////////
	std::filesystem::path subSubOutput("powerPerTurbine");
	std::filesystem::path outputDir;
	outputDir = absOutput / subSubOutput;

	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}

	for (int i = 0; i < run->yaw.size_x; ++i)
	{
		std::string P0_iName = (outputDir / ("P0_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream P0file(P0_iName);
		std::string P_iName = (outputDir / ("P_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream Pfile(P_iName);
		std::string deltaP_iName = (outputDir / ("deltaP_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream deltaPfile(deltaP_iName);
		if (P0file.is_open() && Pfile.is_open() && deltaPfile.is_open())
		{
			if (isTranspose)
			{
				string varName = "Theta \\ U";
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
				string varName = "U \\ Theta";
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

	return 0;
}