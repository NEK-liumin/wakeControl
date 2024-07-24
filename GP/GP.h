#ifndef GP_H
#define GP_H

#include "matrixOperation.h"
#include "iostream"
using std::cout;
using std::endl;

// 定义一般的优化问题
// define General Programming

// Non-Constrain General Programming
// min f(x)
class NCGP
{
public:
	int size_x;
	Column x;
	double f;
	Column g;
	Matrix H;
	virtual void set_size(int size_x) {};
	virtual void init() {};
	virtual void init(int size_x, Column& x) {};
	virtual void set_x() {};
	virtual void set_x(Column& x) {};
	virtual void set_g() {};
	virtual void set_g(Column& g) {};
	virtual void set_H() {};
	virtual void set_H(Column& H) {};
	virtual void get_f() {};
};
// Equality Constrain General Programming
// min f(x)
// s.t.ce(x) = 0
class ECGP :public NCGP
{
public:
	int size_e;
	Column ce;
	Matrix Je;
	Column lambda;
	virtual void set_size(int size_x, int size_e) {};
	virtual void init() {};
	virtual void init(int size_x, int size_e, Column& x, Column& lambda) {};
	virtual void set_ce() {};
	virtual void set_ce(Column& ce);
	virtual void set_Je() {};
	virtual void set_Je(Column& Je);
};
// Enequality Constrain General Programming
// min f(x)
// s.t. ci(x) >=0
class ICGP :public NCGP
{
public:
	int size_i;
	Column ci;
	Matrix Ji;
	Column mu;
};
// Equality and Inequality Constrain General Programming
// min f(x)
// s.t. ce(x) = 0
// s.t. ci(x) >=0
class EICGP :public NCGP
{
public:
	int size_e, size_i;
	Column ce;
	Matrix Je;
	Column lambda;
	Column ci;
	Matrix Ji;
	Column mu;
};

#endif // !GOP_H
