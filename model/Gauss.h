#ifndef GAUSS_H
#define GAUSS_H
#include "model.h"
#include <math.h>
// [1] Experimental and theoretical study of wind turbine wakes in yawed conditions
// [2] Control-oriented model for secondary effects of wake steering
class Gauss :public model
{
public:
	int getC(double& C, double& Ct, double& sigmay0, double& sigmay, double& sigmaz0, double& sigmaz)
	{
		C = 1 - sqrt(1 - (sigmay0 * sigmaz0) * Ct / sigmay / sigmaz);
		return 0;
	}
	int getUg(double& Ug, double& Uinf,double& C, double& y0, double& zh, double& y, double& z, double& delta, double& sigmay, double& simgaz)
	{
		Ug = Uinf * (1 - C * exp(-((y - y0 - delta) * (y - y0 - delta)) / (2 * sigmay * sigmay))
				  * exp(-((z - zh) * (z - zh)) / (2 * sigmaz * sigmaz));
		return 0;
	}
	int getSigma0(double& sigmaz0, double& sigmay0, double& D, double& uR, double& Uinf, double& u0, double& gamma)
	{
		sigmaz0 = D * 0.5 * sqrt(uR / (Uinf + u0));
		sigmay0 = sigmaz0 * gamma;
		return 0;
	}
	int getSigma(double& sigmaz, double& sigmay, double& sigmaz0, dobule& sigmay0, double& x0, double& x)
	{
		sigmaz = kz * (x - x0) + sigmaz0;
		sigmay = ky * (x - x0) + sigmay0;
		return 0;
	}
	int getu0(double& u0, double& Uinf, double& ct)
	{
		u0 = Uinf * sqrt(1 - ct);
		return 0;
	}
	int getVel(Column& vel, TurbCloud& turbine, double& uWind)
	{
		for (int i = 0; i < turbine.turbNum; ++i)
		{
			vel[i] = uWind;
		}
		 
	}
};


#endif // !GAUSS_H
