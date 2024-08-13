﻿#ifndef NCQP_H
#define	NCQP_H
#include "matrixOperation.h"
#include "iostream"
using std::cout;
using std::endl;

// Non-Constrain Quadratic Programming
// 无约束二次规划问题
// min f = 1/2xTHx + pTx +q
class NCQP
{
public:
	int size_x;
	Column x;
	double f;
	Matrix H;
	Column p;
	double q;
	NCQP()
	{
		size_x = 1;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;
	}
	NCQP(int size_x)
	{
		this->size_x = size_x;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;
	}
	int init()
	{
		getZeroColumn(x, size_x);
		return 0;
	}
	int set_f()
	{
		Column Hx;
		double xTHx;
		getAB(Hx, H, x);
		getATB(xTHx, x, Hx);
		double pTx;
		getATB(pTx, p, x);
		f = 0.5 * xTHx + pTx + q;
		return 0;
	}
	int QPprint()
	{
		cout << "======以下是当前无约束二次规划问题的详细信息======" << endl;
		cout << "变量个数：" << size_x << endl;
		cout << "海森矩阵" << endl;
		printA(H);
		cout << "矩阵p" << endl;
		printA(p);
		cout << "常数项" << endl;
		cout << q << endl;
		cout << "当前点" << endl;
		printA(x);
		set_f();
		cout << "当前的目标函数值：" << f << endl;
		return 0;
	}
};

// Inequality Constrain Quadratic Programming
// 不等式约束二次规划问题
// min f = 1/2xTHx + pTx +q
// s.t.AIx >= bI
class ICQP :public NCQP
{
public:
	int size_I;
	Matrix AI;
	Column bI;
	Column mu;
	Column y; // 松弛变量

	ICQP(int size_x, int size_I)
	{
		this->size_x = size_x;
		this->size_I = size_I;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(AI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getOneColumn(mu, size_I);
		getOneColumn(y, size_I);
		getAlphaA(y, 0.01);
	}
	int init()
	{
		getZeroColumn(x, size_x);
		getOneColumn(mu, size_I);
		getOneColumn(y, size_I);
		return 0;
	}
	int set_f()
	{
		Column Hx;
		double xTHx;
		getAB(Hx, H, x);
		getATB(xTHx, x, Hx);
		double pTx;
		getATB(pTx, p, x);
		f = 0.5 * xTHx + pTx + q;
		return 0;
	}
	int QPprint()
	{
		cout << "======以下是当前不等式约束二次规划问题的详细信息======" << endl;
		cout << "变量个数：" << size_x << endl;
		cout << "不等式约束个数：" << size_I << endl;
		cout << "海森矩阵" << endl;
		printA(H);
		cout << "矩阵p" << endl;
		printA(p);
		cout << "常数项" << endl;
		cout << q << endl;
		cout << "AI" << endl;
		printA(AI);
		cout << "bI" << endl;
		printA(bI);
		cout << "mu" << endl;
		printA(mu);
		cout << "y" << endl;
		printA(y);
		cout << "当前点" << endl;
		printA(x);
		set_f();
		cout << "当前的目标函数值" << endl;
		cout << f << endl;
		return 0;
	}
};

// Equality Constrain Quadratic Programming
// 等式约束二次规划问题
// min f = 1/2xTHx + pTx +q
// s.t.AEx = bE
class ECQP :public NCQP
{
public:
	int size_E;
	Matrix AE;
	Column bE;
	Column lambda;

	ECQP(int size_x, int size_E)
	{
		this->size_x = size_x;
		this->size_E = size_E;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(AE, size_E, size_x);
		getZeroColumn(bE, size_E);
		getZeroColumn(lambda, size_E);
	}
	int init()
	{
		getZeroColumn(x, size_x);
		getZeroColumn(lambda, size_E);
		return 0;
	}
	int set_f()
	{
		Column Hx;
		double xTHx;
		getAB(Hx, H, x);
		getATB(xTHx, x, Hx);
		double pTx;
		getATB(pTx, p, x);
		f = 0.5 * xTHx + pTx + q;
		return 0;
	}
	int QPprint()
	{
		cout << "======以下是当前等式约束二次规划问题的详细信息======" << endl;
		cout << "变量个数：" << size_x << endl;
		cout << "等式约束个数：" << size_E << endl;
		cout << "海森矩阵" << endl;
		printA(H);
		cout << "矩阵p" << endl;
		printA(p);
		cout << "常数项" << endl;
		cout << q << endl;
		cout << "AE" << endl;
		printA(AE);
		cout << "bE" << endl;
		printA(bE);
		cout << "lambda" << endl;
		printA(lambda);
		cout << "当前点" << endl;
		printA(x);
		set_f();
		cout << "当前的目标函数值" << endl;
		cout << f << endl;
		return 0;
	}
};

// Equality and Inequality Constrain Quadratic Programming
// 等式及不等式约束二次规划问题
// min f = 1/2xTHx + pTx +q
// s.t.AIx >= bI
// s.t.AEx = bE
class EICQP :public NCQP
{
public:
	int size_I;
	Matrix AI;
	Column bI;
	Column mu;
	Column y;

	int size_E;
	Matrix AE;
	Column bE;
	Column lambda;

	EICQP(int size_x, int size_E, int size_I)
	{
		this->size_x = size_x;
		this->size_E = size_E;
		this->size_I = size_I;

		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(AE, size_E, size_x);
		getZeroColumn(bE, size_E);
		getZeroColumn(lambda, size_E);

		getZeroMatrix(AI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getOneColumn(mu, size_I);
		getOneColumn(y, size_I);
	}
	int init()
	{
		getZeroColumn(x, size_x);
		getOneColumn(lambda, size_E);
		getOneColumn(mu, size_I);
		getOneColumn(y, size_I);
		return 0;
	}
	int set_f()
	{
		Column Hx;
		double xTHx;
		getAB(Hx, H, x);
		getATB(xTHx, x, Hx);
		double pTx;
		getATB(pTx, p, x);
		f = 0.5 * xTHx + pTx + q;
		return 0;
	}
	int QPprint()
	{
		cout << "======以下是当前等式及不等式约束二次规划问题的详细信息======" << endl;
		cout << "变量个数：" << size_x << endl;
		cout << "等式约束个数：" << size_E << endl;
		cout << "不等式约束个数：" << size_I << endl;
		cout << "海森矩阵" << endl;
		printA(H);
		cout << "矩阵p" << endl;
		printA(p);
		cout << "常数项" << endl;
		cout << q << endl;
		cout << "AE" << endl;
		printA(AE);
		cout << "bE" << endl;
		printA(bE);
		cout << "lambda" << endl;
		printA(lambda);
		cout << "AI" << endl;
		printA(AI);
		cout << "bI" << endl;
		printA(bI);
		cout << "mu" << endl;
		printA(mu);
		cout << "y" << endl;
		printA(y);
		cout << "当前点" << endl;
		printA(x);
		set_f();
		cout << "当前的目标函数值" << endl;
		cout << f << endl;
		return 0;
	}
};
#endif // !QP_H
