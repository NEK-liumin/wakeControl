#ifndef USR_H
#define USR_H

#include "GP.h"

class MyNCGP :public NCGP
{
public:
	void set_size(int size_x);
	void init();
	void init(int size_x, Column& x);
	void set_x();
	void set_x(Column& x);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Column& H);
	void get_f();
};
class MyECGP :public ECGP
{
public:

};
class MyICGP :public ICGP
{
public:

};
class MyEICGP :public EICGP
{
public:

};
#endif // !USR_H
