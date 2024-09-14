#include "usr.h"

const double minusPiOutOf180 = -0.01745329251994329576923690768489;
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

Yaw::Yaw(double& wind, double& theta360, double& rho, Model& model)
	// :simulation(wind, theta360, model)
{
	double norm_g;
	double tol = 1e-3;
	simulation.setAll(wind, theta360, model);
	this->rho = rho;
	this->size_x = simulation.turbines.turbNum;
	this->size_i = simulation.turbines.turbNum * 2;
	getZeroColumn(x, size_x);
	xLeft = x;
	xRight = x;
	f = 0;
	getZeroColumn(g, size_x);
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);
	simulation.run(x);
	// simulation.wake.newTurbines.getPower(f0, simulation.wake.newVel, rho);
	simulation.wake.newTurbines.getPower(f0, simulation.wake.newVel);
	f0 /= -1.0;
	set_g();
	norm_g = norm(g);
	printA(g);
	if (norm_g < tol)
	{
		int k = 0;
		do
		{
			k++;
			theta360 += 1.0;
			simulation.setAll(wind, theta360, model);
			simulation.run(x);
			set_g();
			norm_g = norm(g);
		} while (norm_g < tol && k < 3);
		if (k >= 3)
		{
			cout << "gradient is too small and simulation can not run!" << endl;
		}
	}
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
	/*simulation.run(x);
	simulation.wake.newTurbines.getPower(f, simulation.wake.newVel, rho);*/
	double delta = 0.0001;
	double fLeft, fRight;
	xLeft = x;
	xRight = x;
	for (int i = 0; i < size_x; ++i)
	{
		xLeft[i] = xLeft[i] - delta;
		xRight[i] = xRight[i] + delta;
		simulation.run(xLeft);
		simulation.wake.newTurbines.getPower(fLeft, simulation.wake.newVel, rho);
		simulation.run(xRight);
		simulation.wake.newTurbines.getPower(fRight, simulation.wake.newVel, rho);
		fLeft /= f0;
		fRight /= f0;
		g[i] = (fRight - fLeft) / delta / 2.0;
		/*if (abs(g[i]) > 0.1)
		{
			cout << "i = " << i << ", g[i] = " << g[i] << endl;
		}*/
		//g[i] = (fRight - f) / delta;
		/*if (abs(g[i]) < SMALL)
		{
			double gleft, gright;
			gleft = (f - fLeft) / delta;
			gright = (fRight - f) / delta;
			if (abs(gleft) > abs(gright))
			{
				g[i] = gleft;
			}
			else
			{
				g[i] = gright;
			}
		}*/
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
	simulation.run(x);
	// simulation.wake.newTurbines.getPower(f, simulation.wake.newVel, rho);
	simulation.wake.newTurbines.getPower(f, simulation.wake.newVel);
	f /= f0; // 让值小一点，看看行不行
}

void Yaw::get_ci()
{
	for (int i = 0; i < size_x; ++i)
	{
		ci[i] = x[i] + PI / 6.0;
	}
	for (int i = size_x; i < size_i; ++i)
	{
		ci[i] = -x[i - size_x] + PI / 6.0;
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
	simulation.wake.newGamma2Gamma();
	gamma360 = *simulation.turbines.gamma;
	for (int i = 0; i < simulation.turbines.turbNum; ++i)
	{
		gamma360[i] /= minusPiOutOf180;
	}
	return 0;
}