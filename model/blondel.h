#ifndef BLONDEL_H
#define BLONDEL_H
#include "model.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;
// ���ģ�����棬û���漰��ur�Ĺ�ʽ��
// ���ǣ�ǰ������з������ô���ܲ��Ժ���ķ����Ӱ����
// ���ԣ�����Ҫ��ur�ŵ�uinf���棬Ȼ����β��
// ���ң���Ҫ���������ټ�ȥ�������β��
// Ȼ����������ʧ�ĵ���ȥ����β���ĵ���
// Ӧ���������ɡ���������������
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
	// ��β������
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
	int get_sigma(double& sigma, double& beta, double& D, double& x)
	{
		sigma = D * ((as * I + bs) * x / D + cs * sqrt(beta));
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
	int get_n(double& n, double& x, double& D)
	{
		n = af * exp(bf * x / D) + cf;
		return 0;
	}

	int get_C(double& C, double& n, double& Ct, double& sigma, double& D)
	{
		double temp1, temp2, temp3, temp4;
		temp1 = pow(2, 2 / (n - 1));
		temp2 = pow(2, 4 / (n - 2));
		temp3 = n * Ct;
		temp4 = 16 * Gamma(2 / n) * pow(sigma / D, 4 / n);
		C = temp1 - sqrt(temp2 - temp3 / temp4);
		return 0;
	}
	int getUg(double& Ug, double& Uinf, double& C, double& yTurb, double& zTurb, double& y, double& z, double& delta, double& sigma)
	{
		Ug = Uinf * (1 - C * exp(-((y - yTurb - delta) * (y - yTurb - delta)) / (2 * sigma * sigma))
			* exp(-((z - zTurb) * (z - zTurb)) / (2 * sigma * sigma)));

		return 0;
	}

};
#endif // !GLONDEL_H
