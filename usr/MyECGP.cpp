#include "usr.h"

MyECGP::MyECGP(int size_x, int size_e)
{
	this->size_x = size_x;
	this->size_e = size_e;
	getZeroColumn(x, size_x);
	f = 0;
	getZeroColumn(g, size_x);
	// H一般用不到
	getZeroColumn(ce, size_e);
	getZeroMatrix(Je, size_e, size_x);
	getZeroColumn(lambda, size_e);
}

void MyECGP::set_size(int size_x, int size_e)
{
	this->size_x = size_x;
	this->size_e = size_e;
}

void MyECGP::init()
{
	getZeroColumn(x, size_x);
	getZeroColumn(lambda, size_e);
}

void MyECGP::init(int size_x, int size_e, Column& x, Column& lambda)
{
	this->size_x = size_x;
	this->size_e = size_e;
	this->x = x;
	this->lambda = lambda;
}

void MyECGP::set_g()
{

}

void MyECGP::set_g(Column& g)
{
	this->g = g;
}

void MyECGP::set_H()
{

}

void MyECGP::set_H(Matrix& H)
{
	this->H = H;
}

void MyECGP::get_f()
{

}

void MyECGP::get_ce()
{

}

void MyECGP::set_Je()
{

}
void MyECGP::set_Je(Matrix& Je)
{
	this->Je = Je;
}