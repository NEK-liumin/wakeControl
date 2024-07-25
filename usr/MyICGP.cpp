#include "usr.h"

MyICGP::MyICGP(int size_x, int size_i)
{
	this->size_x = size_x;
	this->size_i = size_i;
	getZeroColumn(x, size_x);
	f = 0;
	getZeroColumn(g, size_x);
	// H一般用不到
	getZeroColumn(ci, size_i);
	getZeroMatrix(Ji, size_i, size_x);
	getZeroColumn(mu, size_i);
}

void MyICGP::set_size(int size_x, int size_i)
{
	this->size_x = size_x;
	this->size_i = size_i;
}

void MyICGP::init()
{
	getZeroColumn(x, size_x);
	getOneColumn(mu, size_i);
}

void MyICGP::init(int size_x, int size_i, Column& x, Column& mu)
{
	this->size_x = size_x;
	this->size_i = size_i;
	this->x = x;
	this->mu = mu;
}

void MyICGP::set_g()
{

}

void MyICGP::set_g(Column& g)
{
	this->g = g;
}

void MyICGP::set_H()
{

}

void MyICGP::set_H(Matrix& H)
{
	this->H = H;
}

void MyICGP::get_f()
{

}

void MyICGP::get_ci()
{

}

void MyICGP::set_Ji()
{

}
void MyICGP::set_Ji(Matrix& Ji)
{
	this->Ji = Ji;
}