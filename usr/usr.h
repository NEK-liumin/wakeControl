#ifndef USR_H
#define USR_H

#include "GP.h"

// �û��Զ�����Լ���Ż�����
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

// �û��Զ����ʽԼ���Ż�����
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

// �û��Զ��岻��ʽԼ���Ż�����
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

// �û��Զ����ʽ������ʽԼ���Ż�����
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