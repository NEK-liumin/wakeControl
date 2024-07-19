#ifndef QP_H
#define	QP_H
#include "matrixOperation.h"

// min f = 1/2xHx + px +q
class QP
{
public:
	int size_x;
	Column x;
	double f;
	Matrix H;
	Column p;
	double q;
	QP(){}
	QP(int size_x)
	{
		this->size_x = size_x;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;
	}
};
// min f = 1/2xHx + px +q
// s.t.AIx >= bI
class ICQP :public QP
{
public:
	int size_I;
	Matrix AI;
	Column bI;
	Column factorI;
	Column y; // киЁз╠Да©

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
		getOneColumn(factorI, size_I);
		getOneColumn(y, size_I);
		getAlphaA(y, 0.01);
	}
};
// min f = 1/2xHx + px +q
// s.t.AEx = bE
class ECQP :public QP
{
public:
	int size_E;
	Matrix AE;
	Column bE;
	Column factorE;

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
		getZeroColumn(factorE, size_E);
	}
};
// min f = 1/2xHx + px +q
// s.t.AIx >= bI
// s.t.AEx = bE
class EICQP :public QP
{
public:
	int size_I;
	Matrix AI;
	Column bI;
	Column factorI;
	Column y;

	int size_E;
	Matrix AE;
	Column bE;
	Column factorE;

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
		getZeroColumn(factorE, size_E);

		getZeroMatrix(AI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getOneColumn(factorI, size_I);
		getOneColumn(y, size_I);
		// getAlphaA(y, 0.01);
	}
};
#endif // !QP_H
