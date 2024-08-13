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
	int getUg(double& Ug, double& Uinf,double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigmay, double& simgaz)
	{
		Ug = Uinf * (1 - C * exp(-((y - yTurb - delta) * (y - yTurb - delta)) / (2 * sigmay * sigmay))
				  * exp(-((z - zTurb) * (z - zTurb)) / (2 * sigmaz * sigmaz));
		return 0;
	}
	int getSigma0(double& sigmaz0, double& sigmay0, double& D, double& uR, double& Uinf, double& u0, double& gamma)
	{
		sigmaz0 = D * 0.5 * sqrt(uR / (Uinf + u0));
		sigmay0 = sigmaz0 * gamma;
		return 0;
	}
	int getSigma(double& sigmaz, double& sigmay, double& sigmaz0, double& sigmay0, double& xTurb, double& x0, double& x)
	{
		sigmaz = kz * (x - xTurb - x0) + sigmaz0;
		sigmay = ky * (x - xTurb - x0) + sigmay0;
		return 0;
	}
	int getU0(double& u0, double& Uinf, double& ct)
	{
		u0 = Uinf * sqrt(1 - ct);
		return 0;
	}
	int getX0(double& x0, double& D, double& gamma, double& ct, double& I)
	{
		x0 = D * ((cos(gamma) * (1 + sqrt(1 - ct))) / (1.414214 * (2.32 * I + 0.154 * (1 - sqrt(1 - ct)))));
		return 0;
	}
	int getDelta0(double& delta0, double& gamma, double& Ct,double& x0)
	{
		double theta;
		theta = 0.3 * gamma / cos(gamma) * (1 - sqrt(1 - ct * cos(gamma)));
		delta0 = x0 * tan(theta);
		return 0;
	}
	int getDelta(double& delta, double& delta0, double& gamma, double& sigmay0, double& sigmaz0, double& sigmay, double& sigmaz, double& ct, double& u0, double& Uinf)
	{
		double C0;
		double E0;
		double sqrtCt;
		double sqrtSimma;
		C0 = 1 - u0 / Uinf;
		E0 = C0 * C0 - 3.2607121485636 * C0 + 4.18683727525826;
		sqrtCt = sqrt(ct);
		sqrtSimma = sqrt(sigmay * sigmaz / sigmay0 / sigmaz0);
		delta = delta0 + gamma * E0 / 5.2 * sqrt(sigmay0 * sigmaz0 / ky / kz / ct) * log(((1.6 + sqrtCt) * (1.6 * sqrtSimma - sqrtCt)) / ((1.6 - sqrtCt) * (1.6 * sqrtSimma + sqrtCt)));
		return 0;
	}
	int getVel(double& vel, double& D, double& Ct, double& xTurb, double& yTurb, double& zTurb, double& gamma, double& uR, double& Uinf, double&x,double&y,double&z)
	{
		double sigmaz0, sigmay0;
		double sigmaz, sigmay;
		double x0;
		double u0;
		double C;
		double delta0, delta;
		getX0(x0, D, gamma, ct, I);

		
		// 当前位置位于风机上游，不受该风机影响
		if (x < xTurb)
		{
			vel = uR;
			return 0;
		}
		// 当两台风机在x方向上很近，但在y方向和z方向上距离较远时，假设互不影响
		if (x - xTurb < x0 && (abs(y - yTurb) > 2 * D || abs(z - zTurb) > 2 * D))
		{
			vel = uR;
			return 0;
		}
		// 如果台风机距离过近，风速衰减将非常强烈
		getU0(u0, Uinf, ct);
		if (x - xTurb < x0 && abs(y-yTurb) < 2 * D && abs(z - zTurb) < 2 * D)
		{
			cout << "风机距离过近！" << endl;
			vel = u0;
			return 0;
		}
		
		getSigma0(sigmaz0, sigmay0, D, uR, Uinf, u0, gamma);
		getSigma(sigmaz, sigmay, sigmaz0, sigmay0, xTurb, x0, x);
		getC(C, Ct, sigmay0, sigmay, sigmaz0, sigmaz);
		getDelta0(delta0, gamma, Ct, x0);
		getDelta(delta, delta0, gamma, sigmay0, sigmaz0, sigmay, sigmaz, ct, u0, Uinf);
		getUg(vel, Uinf, C, yTurb, zTurb, y, z, delta, sigmay, sigmaz);
	}
	int getWake(Column& vel, TurbCloud& turbs, double& uWind)
	{
		Column wake(turbs.turbNum) = { 0 };
		Column sumWake(turbs.turbNum) = { 0 };
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			vel[i] = uWind;
		}
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			for (int j = i + 1; j < turbs.turbNum; ++j)
			{
				getVel(wake[j], turbs.D[i], turbs.Ct[i], turbs.x0[i], turbs.y0[i], turbs.z0[i], turbs.gamma[i], vel[i], uWind, turbs.x0[j], turbs.y0[j], turbs.z0[j]);
				sumWake[j] += (1 - wake[j] / uWind) * (1 - wake[j] / uWind);
			}
		}
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			vel[i] = (1 - sqrt(sumWake[i])) * uWind;
		}
		return 0;
	}
};


#endif // !GAUSS_H
