#ifndef GAUSS2_H
#define GAUSS2_H
#include "model.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;
// [1] Experimental and theoretical study of wind turbine wakes in yawed conditions
// [2] Control-oriented model for secondary effects of wake steering

// 高斯尾流模型
class Gauss2 :public Model
{
public:
	Gauss2(double& ky, double& kz, double& I)
	{
		this->ky = ky;
		this->kz = kz;
		this->I = I;
	}
	int getC(double& C, double& Ct, double& sigmay, double& sigmaz, double& gamma, double& D)
	{
		C= 1 - sqrt(1 - cos(gamma) * Ct / 8.0 / sigmay / sigmaz * D * D);
		return 0;
	}
	int getUg(double& Ug, double& Uinf,double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigmay, double& sigmaz)
	{
		Ug = Uinf * (1 - C * exp(-((y - yTurb - delta) * (y - yTurb - delta)) / (2 * sigmay * sigmay))
				  * exp(-((z - zTurb) * (z - zTurb)) / (2 * sigmaz * sigmaz)));
		
		return 0;
	}
	int getSigma(double& sigmay, double& sigmaz, double& xTurb, double& x, double& x0, double& gamma, double& D)
	{
		sigmay = ky * (x - xTurb - x0) + cos(gamma) / 2.828427124746 * D;
		sigmaz = kz * (x - xTurb - x0) + 0.35355339059 * D;
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
	int getTheta(double& theta, double& gamma, double& Ct)
	{
		theta = 0.3 * gamma / cos(gamma) * (1 - sqrt(1 - Ct * cos(gamma)));
		return 0;
	}
	int getDelta0(double& delta0, double& x0, double& theta)
	{
		delta0 = x0 * theta;
		return 0;
	}
	int getDelta(double& delta, double & xTurb, double& x, double& theta)
	{
		delta = (x - xTurb) * theta;
		return 0;
	}
	int getDelta(double& delta, double& delta0, double& gamma, double& ky, double& kz, double& sigmay, double& sigmaz, double& ct, double& D, double& theta)
	{
		double sqrtCt;
		double temp1, temp2, temp3, temp4, temp5, temp6, temp7;
		sqrtCt = sqrt(ct);
		temp1 = sqrt(cos(gamma) / ky / kz / ct);
		temp2 = 2.9 + 1.3 * sqrt(1 - ct) - ct;
		temp3 = 1.6 + ct;
		temp4 = 1.6 - ct;
		temp5 = 1.6 * sqrt(8 * sigmay * sigmaz / D / D / cos(gamma) - ct);
		temp6 = 1.6 * sqrt(8 * sigmay * sigmaz / D / D / cos(gamma) + ct);
		// temp6 = temp5 - sqrtCt;
		// temp7 = temp5 + sqrtCt;
		delta = delta0 + D * theta / 14.7 * temp1 * temp2 * log((temp3 * temp5) / (temp4 * temp6));
		return 0;
	}

	int getVel(double& vel, double& D, double& Ct, double& xTurb, double& yTurb, double& zTurb, double& gamma, double& uR, double& Uinf, double&x,double&y,double&z)
	{
		double sigmaz0, sigmay0;
		double sigmaz, sigmay;
		double x0;
		double C;
		double delta0, delta;
		double theta;
		// 当前位置位于风机上游，不受该风机影响
		if (x < xTurb)
		{
			vel = Uinf;
			return 0;
		}
		getX0(x0, D, gamma, Ct, I);
		getSigma(sigmay, sigmaz, xTurb, x, x0, gamma, D);
		getTheta(theta, gamma, Ct);

		if (x - xTurb < x0)
		{
			getDelta(delta, xTurb, x, theta);
		}
		else
		{
			getDelta0(delta0, x0, theta);
			getDelta(delta, delta0, gamma, ky, kz, sigmay, sigmaz, Ct, D, theta);
		}
		getC(C, Ct, sigmay0, sigmay, sigmaz0, sigmaz);
		getUg(vel, Uinf, C, yTurb, zTurb, y, z, delta, sigmay, sigmaz);
		return 0;
	}
	
	// 这里turbs中得风机必须已经按照x0从小到大排序好了
	// 然后，计算每一个风机处的风速
	// 计算尾流的时候，只计算当前风机上风向的风机的尾流影响
	// 所以，j = i + 1
	int getWake(Column& vel, TurbCloud& turbs, double& uWind)
	{
		Column wake(turbs.turbNum, 0);
		Column sumWake(turbs.turbNum, 0);
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			vel[i] = uWind;
		}
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			double Ct;
			int k = turbs.turbType[i];
			turbs.getCt(Ct, vel[i], i, k);
			for (int j = i + 1; j < turbs.turbNum; ++j)
			{
				getVel(wake[j], turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], vel[i], uWind, turbs.x0[j], turbs.y0[j], turbs.z0[j]);
				sumWake[j] += (1 - wake[j] / uWind) * (1 - wake[j] / uWind);
			}
			vel[i] = (1 - sqrt(sumWake[i])) * uWind;
		}
		return 0;
	}
	int getWake(double& meshVel, Column& turbVel, TurbCloud& turbs, double& uWind, double& x, double& y, double& z)
	{
		double wake = 0;
		double sumWake = 0;
		meshVel = uWind;
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			if (x > turbs.x0[i])
			{
				double Ct;
				int k = turbs.turbType[i];
				turbs.getCt(Ct, turbVel[i], i, k);
				getVel(wake, turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], turbVel[i], uWind, x, y, z);
				sumWake += (1 - wake / uWind) * (1 - wake / uWind);
			}
		}
		meshVel = (1 - sqrt(sumWake)) * uWind;
		return 0;
	}
};


#endif // !GAUSS_H
