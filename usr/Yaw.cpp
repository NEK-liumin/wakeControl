#include "usr.h"
#include <random>
#include <chrono>


static double getRandom(double left, double right)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<double>distribution(left, right);
	return distribution(generator);
}

const double PiOutOf180 = 0.01745329251994329576923690768489;
Yaw::Yaw(int size_x, int size_i)
{
	this->size_x = size_x;
	this->size_i = size_i;
	getZeroColumn(x, size_x);
	f0 = 0;
	f = 0;
	rho = 1.225;
	getZeroColumn(g, size_x);
	// H一般用不到
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);
}

Yaw::Yaw(double& wind, double& theta360, Bool& isWork, double& rho, Model& model, double& maxGamma, double& randomRange)
{
	isWork.resize(simulation.turbines.turbNum);
	for (int i = 0; i < simulation.turbines.turbNum; ++i)isWork[i] = '1';
	simulation.reset(wind, theta360, isWork, model);
	this->rho = rho;
	
	this->size_x = simulation.turbines.turbNum;
	this->size_i = simulation.turbines.turbNum * 2;
	this->minGamma.resize(size_x);
	this->maxGamma.resize(size_x);
	for (int i = 0; i < size_x; ++i)
	{
		this->minGamma[i] = -abs(maxGamma) * PiOutOf180;
		this->maxGamma[i] = abs(maxGamma) * PiOutOf180;
	}
	getZeroColumn(x, size_x);
	xLeft = x;
	xRight = x;
	f = 0;
	getZeroColumn(g, size_x);
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);

	simulation.run(f0, x);
	f0 /= -1.0;
	for (int i = 0; i < size_x; ++i)
	{
		x[i] = getRandom(-randomRange * PiOutOf180, randomRange * PiOutOf180);
	}
}

Yaw::Yaw(double& wind, double& theta360, Bool& isWork, double& rho, Model& model, Column& minGamma, Column& maxGamma, Column& randomRangeLeft, Column& randomRangeRight)
{
	isWork.resize(simulation.turbines.turbNum);
	for (int i = 0; i < simulation.turbines.turbNum; ++i)isWork[i] = '1';
	simulation.reset(wind, theta360, isWork, model);
	simulation.sortColumn(this->minGamma, minGamma);
	simulation.sortColumn(this->maxGamma, maxGamma);
	simulation.sortColumn(this->randomRangeLeft, randomRangeLeft);
	simulation.sortColumn(this->randomRangeRight, randomRangeRight);

	this->rho = rho;

	this->size_x = simulation.turbines.turbNum;
	this->size_i = simulation.turbines.turbNum * 2;
	this->minGamma.resize(size_x);
	this->maxGamma.resize(size_x);

	getAlphaA(this->minGamma, PiOutOf180);
	getAlphaA(this->maxGamma, PiOutOf180);
	getAlphaA(this->randomRangeLeft, PiOutOf180);
	getAlphaA(this->randomRangeRight, PiOutOf180);

	getZeroColumn(x, size_x);
	xLeft = x;
	xRight = x;
	f = 0;
	getZeroColumn(g, size_x);
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);

	simulation.run(f0, x);

	f0 /= -1.0;
	for (int i = 0; i < size_x; ++i)
	{
		x[i] = getRandom(this->randomRangeLeft[i], this->randomRangeRight[i]);
	}
}

int Yaw::reset(double& wind, double& theta360, Bool& isWork, double& rho, Model& model, double& maxGamma, double& randomRange)
{
	simulation.reset(wind, theta360, isWork, model);
	this->rho = rho;
	
	this->size_x = simulation.turbines.turbNum;
	this->size_i = simulation.turbines.turbNum * 2;
	this->maxGamma.resize(size_x);
	for (int i = 0; i < size_x; ++i)
	{
		this->minGamma[i] = -abs(maxGamma) * PiOutOf180;
		this->maxGamma[i] = abs(maxGamma) * PiOutOf180;
	}
	getZeroColumn(x, size_x);
	xLeft = x;
	xRight = x;
	f = 0;
	getZeroColumn(g, size_x);
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);

	simulation.run(f0, x);

	f0 /= -1.0;
	for (int i = 0; i < size_x; ++i)
	{
		x[i] = getRandom(-randomRange * PiOutOf180, randomRange * PiOutOf180);
	}
	set_g();
	return 0;
}

int Yaw::reset(double& wind, double& theta360, Bool& isWork, double& rho, Model& model, Column& minGamma, Column& maxGamma, Column& randomRangeLeft, Column& randomRangeRight)
{
	simulation.reset(wind, theta360, isWork, model);
	simulation.sortColumn(this->minGamma, minGamma);
	simulation.sortColumn(this->maxGamma, maxGamma);
	simulation.sortColumn(this->randomRangeLeft, randomRangeLeft);
	simulation.sortColumn(this->randomRangeRight, randomRangeRight);

	this->rho = rho;
	this->size_x = simulation.turbines.turbNum;
	this->size_i = simulation.turbines.turbNum * 2;
	this->maxGamma.resize(size_x);
	getAlphaA(this->minGamma, PiOutOf180);
	getAlphaA(this->maxGamma, PiOutOf180);
	getAlphaA(this->randomRangeLeft, PiOutOf180);
	getAlphaA(this->randomRangeRight, PiOutOf180);

	getZeroColumn(x, size_x);
	xLeft = x;
	xRight = x;
	f = 0;
	getZeroColumn(g, size_x);
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);

	simulation.run(f0, x);
	f0 /= -1.0;
	for (int i = 0; i < size_x; ++i)
	{
		x[i] = getRandom(this->randomRangeLeft[i], this->randomRangeRight[i]);
	}
	set_g();
	return 0;
}

void Yaw::set_size(int size_x, int size_i)
{
	this->size_x = size_x;
	this->size_i = size_i;
}

void Yaw::init()
{
	getZeroColumn(x, size_x);
	getOneColumn(mu, size_i);
}

void Yaw::init(int size_x, int size_i, Column& x, Column& mu)
{
	this->size_x = size_x;
	this->size_i = size_i;
	this->x = x;
	this->mu = mu;
}

void Yaw::set_g()
{
	double delta = 0.01;
	double fLeft, fRight;
	xLeft = x;
	xRight = x;
	for (int i = 0; i < size_x; ++i)
	{
// 注意，以下，求梯度的过程中必须在末尾恢复xLeft[i]和xRight[i]的值
		xLeft[i] = xLeft[i] - delta;
		xRight[i] = xRight[i] + delta;
		simulation.run(fLeft, xLeft);
		simulation.run(fRight, xRight);
		if (f0 != 0)
		{
			fLeft /= f0;
			fRight /= f0;
		}
		else
		{
			fLeft = 0;
			fRight = 0;
		}
			
		g[i] = (fRight - fLeft) / delta / 2.0;
		xLeft[i] = xLeft[i] + delta;
		xRight[i] = xRight[i] - delta;
	}
}

void Yaw::set_g(Column& g)
{
	this->g = g;
}

void Yaw::set_H()
{

}

void Yaw::set_H(Matrix& H)
{
	this->H = H;
}

void Yaw::get_f()
{
	simulation.run(f, x);
	if (f0 != 0)
		f /= f0; // 用初始发电量的负值对方程进行无量纲化；梯度同理
	else
		f = 0;
}

void Yaw::get_ci()
{
	for (int i = 0; i < size_x; ++i)
	{
		ci[i] = x[i] - minGamma[i];
	}
	for (int i = size_x; i < size_i; ++i)
	{
		ci[i] = -x[i - size_x] + maxGamma[i - size_x];
	}
}

void Yaw::set_Ji()
{
	getZeroMatrix(Ji, size_i, size_x);
	for (int i = 0; i < size_x; ++i)
	{
		Ji[i][i] = 1.0;
	}
	for (int i = size_x; i < size_i; ++i)
	{
		Ji[i][i - size_x] = -1.0;
	}
}
void Yaw::set_Ji(Matrix& Ji)
{
	this->Ji = Ji;
}

int Yaw::outputGamma(Column& gamma360)
{
	// 根据每个风机输出的功率，校正偏航角
	// 如果风机没有输出功率，偏航角为0
	for (int i = 0; i < size_x; ++i)
	{
		if (!simulation.wake.newTurbines.isYaw(simulation.wake.newVel, i))
		{
			(*simulation.wake.newTurbines.gamma)[i] = 0;
		}
	}
	gamma360 = simulation.wake.newGamma2Gamma();
	for (int i = 0; i < simulation.turbines.turbNum; ++i)
	{
		gamma360[i] /= PiOutOf180;
	}
	return 0;
}

double Yaw::power()
{
	get_f();
	return abs(f * f0);
}

double Yaw::initialPower()
{
	return -f0;
}

Column Yaw::power_i()
{
	Column f_i;
	getZeroColumn(f_i, size_x);
	simulation.run(f_i, x);
	return f_i;
}

Column Yaw::initialPower_i()
{
	Column f0_i, x0;
	getZeroColumn(f0_i, size_x);
	getZeroColumn(x0, size_x);
	simulation.run(f0_i, x0);
	return f0_i;
}

