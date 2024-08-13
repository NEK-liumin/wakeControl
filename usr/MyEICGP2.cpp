#include "usr.h"

MyEICGP2::MyEICGP2(int size_x, int size_e, int size_i)
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

void MyEICGP2::set_size(int size_x, int size_e, int size_i)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
}

void MyEICGP2::init()
{
	getZeroColumn(x, size_x);
	getZeroColumn(lambda, size_e);
	getOneColumn(mu, size_i);
}

void MyEICGP2::init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
	this->x = x;
	this->lambda = lambda;
	this->mu = mu;
}

void MyEICGP2::set_g()
{
	g[0] = 2 * x[0];
	g[1] = 1;
}

void MyEICGP2::set_g(Column& g)
{
	this->g = g;
}

void MyEICGP2::set_H()
{

}

void MyEICGP2::set_H(Matrix& H)
{
	this->H = H;
}

void MyEICGP2::get_f()
{
	f = x[0] * x[0] + x[1];
}

void MyEICGP2::get_ce()
{
	ce[0] = x[0] * x[0] + x[1] * x[1] - 9;
}

void MyEICGP2::set_Je()
{
	Je[0][0] = 2 * x[0];
	Je[0][1] = 2 * x[1];
}
void MyEICGP2::set_Je(Matrix& Je)
{
	this->Je = Je;
}

void MyEICGP2::get_ci()
{
	ci[0] = x[0] - 1;
	ci[1] = -x[0] + 5;
	ci[2] = x[1] - 2;
	ci[3] = -x[1] + 4;
}

void MyEICGP2::set_Ji()
{
	Ji[0][0] = 1;
	Ji[0][1] = 0;
	Ji[1][0] = -1;
	Ji[1][1] = 0;
	Ji[2][0] = 0;
	Ji[2][1] = 1;
	Ji[3][0] = 0;
	Ji[3][1] = -1;
}
void MyEICGP2::set_Ji(Matrix& Ji)
{
	this->Ji = Ji;
}