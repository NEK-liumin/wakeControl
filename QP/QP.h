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
	QP(int& size_x)
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
// s.t.cIx = bI
class ICQP :public QP
{
public:
	int size_I;
	Matrix cI;
	Column bI;
	Column factorI;

	ICQP(int& size_x, int& size_I)
	{
		this->size_x = size_x;
		this->size_I = size_I;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(cI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getZeroColumn(factorI, size_I);
	}
};
// min f = 1/2xHx + px +q
// s.t.cEx = bE
class ECQP :public QP
{
public:
	int size_E;
	Matrix cE;
	Column bE;
	Column factorE;

	ECQP(int& size_x, int& size_E)
	{
		this->size_x = size_x;
		this->size_E = size_E;
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(cE, size_E, size_x);
		getZeroColumn(bE, size_E);
		getZeroColumn(factorE, size_E);
	}
};
// min f = 1/2xHx + px +q
// s.t.cIx = bI
// s.t.cEx = bE
class EICQP :public QP
{
public:
	int size_I;
	Matrix cI;
	Column bI;
	Column factorI;

	int size_E;
	Matrix cE;
	Column bE;
	Column factorE;

	EICQP(int& size_x, int& size_E, int& size_I)
	{
		this->size_x = size_x;
		this->size_E = size_E;
		this->size_I = size_I;

		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(cE, size_E, size_x);
		getZeroColumn(bE, size_E);
		getZeroColumn(factorE, size_E);

		getZeroMatrix(cI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getZeroColumn(factorI, size_I);
	}
};
#endif // !QP_H
