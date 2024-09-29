#ifndef BLONDEL_H
#define BLONDEL_H
#include "model.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;

// 为了与文献保持一致，将参数无量纲化。后缀Bar表示无量纲化

// Blondel F, Cathelain M.An alternative form of the super - Gaussian
// wind turbine wake model[J].Wind Energy Science Discussions, 2020, 2020: 1 - 16.
class Blondel :public Model
{
private:
	const double as = 0.17;
	const double bs = 0.005;
	const double cs = 0.2;
	const double pNW = -1;
	const double af = 3.11;
	const double bf = -0.68;
	const double cf = 2.41;
public:
	Blondel(double& ky, double& kz, double& I)
	{
		this->ky = ky;
		this->kz = kz;
		this->I = I;
	}
	// 近尾流修正
	/*int get_k(double& k, double& cNW, double& x, double& xTurb, double& D)
	{
		k = 1.0 / (cNW * (1 + (x - xTurb) / D));
		return 0;
	}
	int get_Cnear(double& C, double& k, double& Ct, double& sigma)
	{
		double temp;
		temp = 8 * (sigma / D + k) * (sigma / D + k);
		C = 1 - sqrt(1 - Ct / temp);
		return 0;
	}*/

	// 仅用于尾流大小计算
	int get_sigmaBar(double& sigmaBar, double& beta, double& D, double& x, double& xTurb)
	{
		sigmaBar = (as * I + bs) * (x - xTurb) / D + cs * sqrt(beta);
		return 0;
	}
	int get_beta(double& beta, double& Ct)
	{
		beta = 0.5 * (1 + sqrt(1 - Ct)) / sqrt(1 - Ct);
		return 0;
	}
	/*int get_a(double& a, double& Ct)
	{
		a = 0.5 * (1 - sqrt(1 - Ct));
		return 0;
	}
	int get_cNW(double& cNW, double& a, double& Ct, double& beta)
	{
		double temp;
		temp = 8 * (1 - (1 - a) * (1 - a));
		cNW = sqrt(Ct / temp) - cs * sqrt(beta);
		return 0;
	}*/

	int get_n(double& n, double& x, double& xTurb, double& D)
	{
		n = af * exp(bf * (x - xTurb) / D) + cf;
		return 0;
	}

	int get_C(double& C, double& n, double& Ct, double& sigmaBar)
	{
		double temp1, temp2, temp3, temp4;
		temp1 = pow(2, 2 / n - 1);
		temp2 = pow(2, 4 / n - 2);
		temp3 = n * Ct;
		temp4 = 16 * Gamma(2 / n) * pow(sigmaBar, 4 / n);
		//temp4 = 16 * 1.0 * pow(sigmaBar, 4 / n);
		if (temp2 - temp3 / temp4 < 0)C = temp1;
		else C = temp1 - sqrt(temp2 - temp3 / temp4);
		return 0;
	}

	// 如果输入无穷远处的Uinf似乎不太合理，输入风机轮毂处的风速试试
	// 风速为U，风机的偏航角为gamma
	int get_Uw(double& Uw, double& U, double& gamma, double& D, double& n, double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigmaBar)
	{
		double rBar2; // r*r
		rBar2 = ((y - yTurb - delta) * (y - yTurb - delta) + (z - zTurb) * (z - zTurb)) / D / D;
		Uw = U * cos(gamma) * (1 - C * exp(-pow(rBar2, n / 2) / (2 * sigmaBar * sigmaBar)));
		return 0;
	}
	int get_deltaU(double& deltaU, double& U, double& gamma, double& D, double& n, double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigmaBar)
	{
		double rBar2; // r*r
		rBar2 = ((y - yTurb - delta) * (y - yTurb - delta) + (z - zTurb) * (z - zTurb)) / D / D;
		deltaU = U * cos(gamma) * C * exp(-pow(rBar2, n / 2) / (2 * sigmaBar * sigmaBar));
		return 0;
	}
	int get_x0(double& x0, double& D, double& gamma, double& Ct, double& I)
	{
		x0 = D * ((cos(-gamma) * (1 + sqrt(1 - Ct))) / (1.414214 * (2.32 * I + 0.154 * (1 - sqrt(1 - Ct)))));
		return 0;
	}
	// 仅用于偏转角度的计算
	int get_sigma(double& sigmay, double& sigmaz, double& D, double& gamma, double& xTurb, double& x, double& x0)
	{
		/*sigmaz = kz * (x - xTurb - x0) + sigmaz0;
		sigmay = ky * (x - xTurb - x0) + sigmay0;*/
		sigmaz = kz * (x - xTurb - x0) + 0.35355339059327376 * D; // 1 / sqrt(8)
		sigmay = ky * (x - xTurb - x0) + 0.35355339059327376 * cos(-gamma) * D;
		return 0;
	}
	int get_theta0(double& theta0, double& gamma, double& Ct)
	{
		theta0 = 0.3 * (-gamma) / cos(-gamma) * (1 - sqrt(1 - Ct * cos(-gamma)));
		return 0;
	}
	int get_delta0(double& delta0, double& x0, double& theta0)
	{
		delta0 = x0 * theta0;
		return 0;
	}
	int get_delta(double& delta, double& xTurb, double& x, double& theta0)
	{
		delta = (x - xTurb) * theta0;
		return 0;
	}
	int get_delta(double& delta, double& delta0, double& gamma, double& ky, double& kz, double& sigmay, double& sigmaz, double& ct, double& D, double& theta0)
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
		delta = delta0 + D * theta0 / 14.7 * temp1 * temp2 * log((temp3 * temp6) / (temp4 * temp7));
		return 0;
	}

	int deltaUafterTurbine(double& deltaU, double& D, double& Ct, double& xTurb, double& yTurb, double& zTurb, double& gamma, double& U, double& x, double& y, double& z)
	{
		double beta;
		double sigmaBar;
		double n;
		double sigmaz, sigmay;
		double x0;
		double C;
		double delta0, delta;
		double theta0;

		get_x0(x0, D, gamma, Ct, I);

		// 当前位置位于风机上游，不受该风机影响
		if (x < xTurb)
		{
			deltaU = 0.0;
			return 0;
		}
		if (Ct <= 0)
		{
			deltaU = 0.0;
			return 0;
		}

		// 尾流大小相关
		get_beta(beta, Ct);
		get_sigmaBar(sigmaBar, beta, D, x, xTurb);
		get_n(n, x, xTurb, D);
		get_C(C, n, Ct, sigmaBar);

		// 尾流偏转角度相关
		get_sigma(sigmay, sigmaz, D, gamma, xTurb, x, x0);
		get_theta0(theta0, gamma, Ct);
		get_delta0(delta0, x0, theta0);

		if (x - xTurb < x0)
		{
			get_delta(delta, xTurb, x, theta0);
		}
		else
		{
			get_delta(delta, delta0, gamma, ky, kz, sigmay, sigmaz, Ct, D, theta0);
		}

		get_deltaU(deltaU, U, gamma, D, n, C, yTurb, zTurb, y, z, delta, sigmaBar);
		return 0;
	}
	int getWake(Column& vel, TurbCloud& turbs, double& uWind)
	{
		double velAxial;
		Column deltaU(turbs.turbNum, 0);
		Column sumDeltaU(turbs.turbNum, 0);
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			vel[i] = uWind;
		}
		// 风机有顺序，i越小越靠前
		// 对于第i个循环，先求所有在第i个风机后面的风机受第i个风机的影响
		// 所有在第i个风机前面的风机，不会被第i个风机影响
		// 所以，到了第i个循环中，第i个风机受到前面风机的影响已经计算完成
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			double Ct;
			int k = turbs.turbType[i];
			velAxial = vel[i] * cos((*turbs.gamma)[i]);
			turbs.getCt(Ct, velAxial, k);
			for (int j = i + 1; j < turbs.turbNum; ++j)
			{
				deltaUafterTurbine(deltaU[j], turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], vel[i], turbs.x0[j], turbs.y0[j], turbs.z0[j]);
				sumDeltaU[j] += deltaU[j] * deltaU[j];
			}

			//cout << sumWake[i] << endl;
			vel[i] = uWind - sqrt(sumDeltaU[i]);
		}
		return 0;
	}
	int getWake(double& meshVel, Column& turbVel, TurbCloud& turbs, double& uWind, double& x, double& y, double& z)
	{
		double velAxial;
		double deltaU = 0;
		double sumDeltaU = 0;
		meshVel = uWind;
		for (int i = 0; i < turbs.turbNum; ++i)
		{
			if (x > turbs.x0[i])
			{
				double Ct;
				int k = turbs.turbType[i];
				velAxial = turbVel[i] * cos((*turbs.gamma)[i]);
				turbs.getCt(Ct, velAxial, k);
				deltaUafterTurbine(deltaU, turbs.D[i], Ct, turbs.x0[i], turbs.y0[i], turbs.z0[i], (*turbs.gamma)[i], turbVel[i], x, y, z);
				sumDeltaU += deltaU * deltaU;
			}
		}
		meshVel = uWind - sqrt(sumDeltaU);
		return 0;
	}
};
#endif // !GLONDEL_H
