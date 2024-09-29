#ifndef GAUSS3_H
#define GAUSS3_H
#include "model.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;
// [1] Experimental and theoretical study of wind turbine wakes in yawed conditions
// [2] Control-oriented model for secondary effects of wake steering

// 高斯尾流模型
class Gauss3 :public Model
{
public:
	Gauss3(double& ky, double& kz, double& I)
	{
		this->ky = ky;
		this->kz = kz;
		this->I = I;
	}
	int getC(double& C, double& Ct, double& sigmay0, double& sigmay, double& sigmaz0, double& sigmaz)
	{
		C = 1 - sqrt(1 - (sigmay0 * sigmaz0) * Ct / sigmay / sigmaz);
		return 0;
	}
	int getUg(double& Ug, double& Uinf,double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigmay, double& sigmaz)
	{
		Ug = Uinf * (1 - C * exp(-((y - yTurb - delta) * (y - yTurb - delta)) / (2 * sigmay * sigmay))
				  * exp(-((z - zTurb) * (z - zTurb)) / (2 * sigmaz * sigmaz)));
		
		return 0;
	}
	int getSigma0(double& sigmaz0, double& sigmay0, double& D, double& uR, double& Uinf, double& u0, double& gamma)
	{
		sigmaz0 = D * 0.5 * sqrt(uR / (Uinf + u0));
		sigmay0 = sigmaz0 * cos(-gamma);
		return 0;
	}
	int getSigma(double& sigmaz, double& sigmay, double& sigmaz0, double& sigmay0, double& xTurb, double& x)
	{
		/*sigmaz = kz * (x - xTurb - x0) + sigmaz0;
		sigmay = ky * (x - xTurb - x0) + sigmay0;*/
		sigmaz = kz * (x - xTurb) + sigmaz0;
		sigmay = ky * (x - xTurb) + sigmay0;
		return 0;
	}
	int getSigma(double& sigmaz, double& sigmay, double& sigmaz0, double& sigmay0, double& xTurb, double& x, double& x0)
	{
		/*sigmaz = kz * (x - xTurb - x0) + sigmaz0;
		sigmay = ky * (x - xTurb - x0) + sigmay0;*/
		sigmaz = kz * abs(x - xTurb - x0) + sigmaz0;
		sigmay = ky * abs(x - xTurb - x0) + sigmay0;
		return 0;
	}
	int getU0(double& u0, double& Uinf, double& ct)
	{
		//cout << "in Gauss3.getU0" << endl;
		//cout << ct << endl;
		u0 = Uinf * sqrt(1 - ct);
		return 0;
	}
	int getX0(double& x0, double& D, double& gamma, double& ct, double& I)
	{
		x0 = D * ((cos(-gamma) * (1 + sqrt(1 - ct))) / (1.414214 * (2.32 * I + 0.154 * (1 - sqrt(1 - ct)))));
		return 0;
	}
	int getTheta(double& theta, double& gamma, double& Ct)
	{
		theta = 0.3 * (-gamma) / cos(-gamma) * (1 - sqrt(1 - Ct * cos(-gamma)));
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
	/*int getDelta(double& delta, double& delta0, double& gamma, double& sigmay0, double& sigmaz0, double& sigmay, double& sigmaz, double& ct, double& u0, double& Uinf)
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
	}*/

	int getDelta(double& delta, double& delta0, double& gamma, double& ky, double& kz, double& sigmay, double& sigmaz, double& ct, double& D, double& theta)
	{
		double sqrtCt;
		double temp1, temp2, temp3, temp4, temp5, temp6, temp7;
		sqrtCt = sqrt(ct);
		temp1 = sqrt(cos(-gamma) / ky / kz / ct);
		temp2 = 2.9 + 1.3 * sqrt(1 - ct) - ct;
		temp3 = 1.6 + sqrtCt;
		temp4 = 1.6 - sqrtCt;
		temp5 = 1.6 * sqrt(8 * sigmay * sigmaz / D / D / cos(-gamma));
		temp6 = temp5 - sqrtCt;
		temp7 = temp5 + sqrtCt;
		delta = delta0 + D * theta / 14.7 * temp1 * temp2 * log((temp3 * temp6) / (temp4 * temp7));
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
		double theta;
		getX0(x0, D, gamma, Ct, I);

		
		// 当前位置位于风机上游，不受该风机影响
		if (x < xTurb)
		{
			vel = Uinf;
			return 0;
		}
		if (Ct <= 0)
		{
			vel = Uinf;
			return 0;
		}
		
		getU0(u0, Uinf, Ct);
		//cout << "in Gauss3.getVel" << endl;
		//cout << u0 << endl;
		getSigma0(sigmaz0, sigmay0, D, uR, Uinf, u0, gamma);
		// getSigma(sigmaz, sigmay, sigmaz0, sigmay0, xTurb, x);
		getSigma(sigmaz, sigmay, sigmaz0, sigmay0, xTurb, x, x0);
		getC(C, Ct, sigmay0, sigmay, sigmaz0, sigmaz);
		getTheta(theta, gamma, Ct);

		getDelta0(delta0, x0, theta);

		if (x - xTurb < x0)
		{
			getDelta(delta, xTurb, x, theta);
		}
		else
		{
			getDelta(delta, delta0, gamma, sigmay0, sigmaz0, sigmay, sigmaz, Ct, u0, Uinf);
		}
		

		getUg(vel, Uinf, C, yTurb, zTurb, y, z, delta, sigmay, sigmaz);
		//cout << "in Gauss3.getVel" << endl;
		//cout << vel << endl;
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
			turbs.getCt(Ct, vel[i], k);
			for (int j = i + 1; j < turbs.turbNum; ++j)
			{
				getVel(wake[j], turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], vel[i], uWind, turbs.x0[j], turbs.y0[j], turbs.z0[j]);
				//cout << "in Gauss3.getWake" << endl;
				//cout << wake[j] << endl;
				sumWake[j] += (1 - wake[j] / uWind) * (1 - wake[j] / uWind);
			}
			
			//cout << sumWake[i] << endl;
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
				turbs.getCt(Ct, turbVel[i], k);
				getVel(wake, turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], turbVel[i], uWind, x, y, z);
				sumWake += (1 - wake / uWind) * (1 - wake / uWind);
			}
		}
		meshVel = (1 - sqrt(sumWake)) * uWind;
		return 0;
	}
};


#endif // !GAUSS_H
