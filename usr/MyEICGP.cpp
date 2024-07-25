#include "usr.h"

MyEICGP::MyEICGP(int size_x, int size_e, int size_i)
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
	getZeroColumn(mu, size_i);
}

void MyEICGP::set_size(int size_x, int size_e, int size_i)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
}

void MyEICGP::init()
{
	getZeroColumn(x, size_x);
	getZeroColumn(lambda, size_e);
	getOneColumn(mu, size_i);
}

void MyEICGP::init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->size_i = size_i;
	this->x = x;
	this->lambda = lambda;
	this->mu = mu;
}

void MyEICGP::set_g()
{

}

void MyEICGP::set_g(Column& g)
{
	this->g = g;
}

void MyEICGP::set_H()
{

}

void MyEICGP::set_H(Matrix& H)
{
	this->H = H;
}

void MyEICGP::get_f()
{

}

void MyEICGP::get_ce()
{

}

void MyEICGP::set_Je()
{

}
void MyEICGP::set_Je(Matrix& Je)
{
	this->Je = Je;
}

void MyEICGP::get_ci()
{

}

void MyEICGP::set_Ji()
{

}
void MyEICGP::set_Ji(Matrix& Ji)
{
	this->Ji = Ji;
}