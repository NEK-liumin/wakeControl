#include "usr.h"

MyNCGP::MyNCGP(int size_x)
{
	this->size_x = size_x;
	getZeroColumn(x, size_x);
	f = 0;
	getZeroColumn(g, size_x);
	// H一般用不到
}

void MyNCGP::set_size(int size_x)
{
	this->size_x = size_x;
}

void MyNCGP::init()
{
	getZeroColumn(x, size_x);
}

void MyNCGP::init(int size_x, Column& x)
{
	this->size_x = size_x;
	this->x = x;
}

void MyNCGP::set_g()
{

}

void MyNCGP::set_g(Column& g)
{
	this->g = g;
}

void MyNCGP::set_H()
{

}

void MyNCGP::set_H(Matrix& H)
{
	this->H = H;
}

void MyNCGP::get_f()
{

}