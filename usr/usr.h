#ifndef USR_H
#define USR_H

#include "GP.h"

// 用户自定义无约束优化问题
class MyNCGP :public NCGP
{
public:
	MyNCGP(int size_x);
	void set_size(int size_x);
	void init();
	void init(int size_x, Column& x);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
};

// 用户自定义等式约束优化问题
class MyECGP :public ECGP
{
public:
	MyECGP(int size_x, int size_e);
	void set_size(int size_x, int size_e);
	void init();
	void init(int size_x, int size_e, Column& x, Column& lambda);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
	void get_ce();
	void set_Je();
	void set_Je(Matrix& Je);
};

// 用户自定义不等式约束优化问题
class MyICGP :public ICGP
{
public:
	MyICGP(int size_x, int size_i);
	void set_size(int size_x, int size_i);
	void init();
	void init(int size_x, int size_i, Column& x, Column& mu);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
	void get_ci();
	void set_Ji();
	void set_Ji(Matrix& Ji);
};

// 用户自定义等式及不等式约束优化问题
class MyEICGP :public EICGP
{
public:
	MyEICGP(int size_x, int size_e, int size_i);
	void set_size(int size_x, int size_e, int size_i);
	void init();
	void init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
	void get_ce();
	void set_Je();
	void set_Je(Matrix& Je);
	void get_ci();
	void set_Ji();
	void set_Ji(Matrix& Ji);
};

class MyEICGP2 :public EICGP
{
public:
	MyEICGP2(int size_x, int size_e, int size_i);
	void set_size(int size_x, int size_e, int size_i);
	void init();
	void init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
	void get_ce();
	void set_Je();
	void set_Je(Matrix& Je);
	void get_ci();
	void set_Ji();
	void set_Ji(Matrix& Ji);
};

class MyEICGP3 :public EICGP
{
public:
	MyEICGP3(int size_x, int size_e, int size_i);
	void set_size(int size_x, int size_e, int size_i);
	void init();
	void init(int size_x, int size_e, int size_i, Column& x, Column& lambda, Column& mu);
	void set_g();
	void set_g(Column& g);
	void set_H();
	void set_H(Matrix& H);
	void get_f();
	void get_ce();
	void set_Je();
	void set_Je(Matrix& Je);
	void get_ci();
	void set_Ji();
	void set_Ji(Matrix& Ji);
};

#endif // !USR_H