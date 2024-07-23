#ifndef QP_H
#define	QP_H
#include "matrixOperation.h"
#include "iostream"
using std::cout;
using std::endl;
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
	int init()
	{
		getZeroColumn(x, size_x);
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;
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
		cout << "======�����ǵ�ǰ��Լ�����ι滮�������ϸ��Ϣ======" << endl;
		cout << "����������" << size_x << endl;
		cout << "��ɭ����" << endl;
		printA(H);
		cout << "����p" << endl;
		printA(p);
		cout << "������" << endl;
		cout << q << endl;
		cout << "��ǰ��" << endl;
		printA(x);
		set_f();
		cout << "��ǰ��Ŀ�꺯��ֵ��" << f << endl;
		return 0;
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
	Column mu;
	Column y; // �ɳڱ���

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
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(AI, size_I, size_x);
		getZeroColumn(bI, size_I);
		getOneColumn(mu, size_I);
		getOneColumn(y, size_I);
		getAlphaA(y, 0.01);
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
		cout << "======�����ǵ�ǰ����ʽԼ�����ι滮�������ϸ��Ϣ======" << endl;
		cout << "����������" << size_x << endl;
		cout << "����ʽԼ��������" << size_I << endl;
		cout << "��ɭ����" << endl;
		printA(H);
		cout << "����p" << endl;
		printA(p);
		cout << "������" << endl;
		cout << q << endl;
		cout << "AI" << endl;
		printA(AI);
		cout << "bI" << endl;
		printA(bI);
		cout << "mu" << endl;
		printA(mu);
		cout << "y" << endl;
		printA(y);
		cout << "��ǰ��" << endl;
		printA(x);
		set_f();
		cout << "��ǰ��Ŀ�꺯��ֵ" << endl;
		cout << f << endl;
		return 0;
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
		f = 0;
		getUnitMatrix(H, size_x);
		getZeroColumn(p, size_x);
		q = 0;

		getZeroMatrix(AE, size_E, size_x);
		getZeroColumn(bE, size_E);
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
		cout << "======�����ǵ�ǰ��ʽԼ�����ι滮�������ϸ��Ϣ======" << endl;
		cout << "����������" << size_x << endl;
		cout << "��ʽԼ��������" << size_E << endl;
		cout << "��ɭ����" << endl;
		printA(H);
		cout << "����p" << endl;
		printA(p);
		cout << "������" << endl;
		cout << q << endl;
		cout << "AE" << endl;
		printA(AE);
		cout << "bE" << endl;
		printA(bE);
		cout << "lambda" << endl;
		printA(lambda);
		cout << "��ǰ��" << endl;
		printA(x);
		set_f();
		cout << "��ǰ��Ŀ�꺯��ֵ" << endl;
		cout << f << endl;
		return 0;
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
		//getAlphaA(mu, 0.1);
		//getZeroColumn(factorI, size_I);
		getOneColumn(y, size_I);
		// getAlphaA(y, 10);
	}
	int init()
	{
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
		cout << "======�����ǵ�ǰ��ʽ������ʽԼ�����ι滮�������ϸ��Ϣ======" << endl;
		cout << "����������" << size_x << endl;
		cout << "��ʽԼ��������" << size_E << endl;
		cout << "����ʽԼ��������" << size_I << endl;
		cout << "��ɭ����" << endl;
		printA(H);
		cout << "����p" << endl;
		printA(p);
		cout << "������" << endl;
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
		cout << "��ǰ��" << endl;
		printA(x);
		set_f();
		cout << "��ǰ��Ŀ�꺯��ֵ" << endl;
		cout << f << endl;
		return 0;
	}
};
#endif // !QP_H
