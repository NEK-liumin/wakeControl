#include "usr.h"
#include <math.h>
MyEICGP3::MyEICGP3(int size_x, int size_e, int size_i)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
	getZeroColumn(x, size_x);
	f = 0;
	getZeroColumn(g, size_x);
	// H一般用不到
	getZeroColumn(ce, size_e);
	getZeroMatrix(Je, size_e, size_x);
	getZeroColumn(lambda, size_e);

	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getOneColumn(mu, size_i);
}

void MyEICGP3::set_size(int size_x, int size_e, int size_i)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
}

void MyEICGP3::init()
{
	getZeroColumn(x, size_x);
	getZeroColumn(lambda, size_e);
	getOneColumn(mu, size_i);
}

void MyEICGP3::init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
	this->x = x;
	this->lambda = lambda;
	this->mu = mu;
}

void MyEICGP3::set_g()
{
	g[0] = exp(x[0]) * (3 * x[0] * x[0] + x[1] * x[1] + 2 * x[0] * x[1] + 6 * x[0] + 8 * x[1] + 1);
	g[1] = exp(x[0]) * (2 * x[0] + 2 * x[1] + 2);
}

void MyEICGP3::set_g(Column& g)
{
	this->g = g;
}

void MyEICGP3::set_H()
{

}

void MyEICGP3::set_H(Matrix& H)
{
	this->H = H;
}

void MyEICGP3::get_f()
{
	f = exp(x[0]) * (3 * x[0] * x[0] + x[1] * x[1] + 2 * x[0] * x[1] + 2 * x[1] + 1);
}

void MyEICGP3::get_ce()
{
	ce[0] = -x[0] * x[1] + x[0] + x[1] - 1.5;
}

void MyEICGP3::set_Je()
{
	Je[0][0] = -x[1] + 1;
	Je[0][1] = -x[0] + 1;
}
void MyEICGP3::set_Je(Matrix& Je)
{
	this->Je = Je;
}

void MyEICGP3::get_ci()
{
	ci[0] = -x[0] - x[1];
	ci[1] = x[0] * x[1] + 10;
	ci[2] = x[0] +10;
	ci[3] = -x[0] + 10;
	ci[4] = x[1] + 10;
	ci[5] = -x[1] + 10;
}

void MyEICGP3::set_Ji()
{
	Ji[0][0] = -1;
	Ji[0][1] = -1;
	Ji[1][0] = x[1];
	Ji[1][1] = x[0];
	Ji[2][0] = 1;
	Ji[2][1] = 0;
	Ji[3][0] = -1;
	Ji[3][1] = 0;
	Ji[4][0] = 0;
	Ji[4][1] = 1;
	Ji[5][0] = 0;
	Ji[5][1] = -1;
}
void MyEICGP3::set_Ji(Matrix& Ji)
{
	this->Ji = Ji;
}