#include "solver.h"
#include "iostream"
using std::cout;
using std::endl;

bool Lx_b_solver(Column& x, Matrix& L, Column& b)
{
	int m = L.size();
	x.resize(m);
	for (int i = 0; i < m; ++i)
	{
		if (L[i][i] == 0)
		{
			cout << "L矩阵的对角线上的第" << i + 1 << "个元素为0" << endl;
			return false;
		}
	}
	x[0] = b[0] / L[0][0];
	for (int i = 1; i < m; ++i)
	{
		x[i] = b[i];
		for (int j = 0; j < i; ++j)
		{
			x[i] -= L[i][j] * x[j];
		}
		x[i] /= L[i][i];
	}
	return true;
}

bool Dx_b_solver(Column& x, Matrix& D, Column& b)
{
	int m = D.size();
	x.resize(m);
	for (int i = 0; i < m; ++i)
	{
		if (D[i][i] == 0)
		{
			cout << "L矩阵的对角线上的第" << i + 1 << "个元素为0" << endl;
			return false;
		}
	}
	for (int i = 0; i < m; ++i)
	{
		x[i] = b[i] / D[i][i];
	}
	return true;
}

bool LTx_b_solver(Column& x, Matrix& LT, Column& b)
{
	int m = LT.size();
	x.resize(m);
	for (int i = 0; i < m; ++i)
	{
		if (LT[i][i] == 0)
		{
			cout << "LT矩阵的对角线上的第" << i + 1 << "个元素为0" << endl;
			return false;
		}
	}
	x[m - 1] = b[m - 1] / LT[m - 1][m - 1];
	for (int i = m - 2; i >= 0; --i)
	{
		x[i] = b[i];
		for (int j = m - 1; j > i; --j)
		{
			x[i] -= x[j] * LT[i][j];
		}
		x[i] /= LT[i][i];
	}
	return true;
}

bool Ax_b_solver(Column& x, Matrix& A, Column& b)
{
	Matrix L, D, LT;
	bool isSolvable;
	isSolvable = LDL_decomposition(L, D, A);
	getAT(LT, L);
	if (!isSolvable)
	{
		return false;
	}
	Column DLTx;

	isSolvable = Lx_b_solver(DLTx, L, b);
	if (!isSolvable)
	{
		return false;
	}
	Column LTx;

	isSolvable = Dx_b_solver(LTx, D, DLTx);
	if (!isSolvable)
	{
		return false;
	}

	isSolvable = LTx_b_solver(x, LT, LTx);
	if (!isSolvable)
	{
		return false;
	}
	return true;
}

bool LDL_solver(QP& problem)
{
	bool isSolvable;
	Column negative_p;
	getAlphaA(negative_p, problem.p, -1);
	isSolvable = Ax_b_solver(problem.x, problem.H, negative_p);
	if (!isSolvable)
	{
		cout << "当前问题的海森矩阵存在问题，无解" << endl;
		return false;
	}

	return true;
}

bool QR_solver(ECQP& problem)
{
	bool isSolvable;
	if (problem.size_E > problem.size_x)
	{
		cout << "等式约束的个数不能大于变量个数" << endl;
		return false;
	}
	if (problem.size_E == problem.size_x)
	{
		cout << "等式约束等于变量个数，仅靠约束即可确定变量" << endl;
		Matrix Q, R;
		QR_decomposition(Q, R, problem.AE);
		Matrix QT;
		Column QTb;
		getAT(QT, Q);
		getAB(QTb, QT, problem.bE);
		isSolvable = LTx_b_solver(problem.x, R, QTb);

		if (!isSolvable)
		{
			cout << "等式约束构成的方程组不可解" << endl;
			return false;
		}

		Matrix AAT;
		getABT(AAT, problem.AE, problem.AE);
		Column Hx, Hxplusp, A_Hxplusp;
		getAB(Hx, problem.H, problem.x);
		getAPlusB(Hxplusp, Hx, problem.p);
		getAB(A_Hxplusp, problem.AE, Hxplusp);
		isSolvable = Ax_b_solver(problem.factorE, AAT, A_Hxplusp);
		if (!isSolvable)
		{
			cout << "等式约束拉格朗日乘子不可解" << endl;
			return false;
		}
		return true;
	}
	Matrix AT;
	Matrix Q, R, RT;
	Matrix Q1, Q2, Q2T;
	getAT(AT, problem.AE);
	QR_decomposition(Q, R, AT);
	getAT(RT, R);
	splitMatrix(Q1, Q2, Q, problem.size_E);
	getAT(Q2T, Q2);
	Matrix H_temp;
	getAB(H_temp, Q2T, problem.H);
	getAB(H_temp, Q2);

	Column p_temp, RT_inverse_b;
	Matrix HQ1;
	Column HQ1RT_inverse_b;
	isSolvable = Lx_b_solver(RT_inverse_b, RT, problem.bE);
	if (!isSolvable)
	{
		cout << "等式约束存在问题" << endl;
		return false;
	}
	getAB(HQ1, problem.H, Q1);
	getAB(HQ1RT_inverse_b, HQ1, RT_inverse_b);
	getAPlusB(HQ1RT_inverse_b, problem.p);
	getAB(p_temp, Q2T, HQ1RT_inverse_b);
	getAlphaA(p_temp, -1);
	Column phi;
	isSolvable = Ax_b_solver(phi, H_temp, p_temp);
	if (!isSolvable)
	{
		cout << "当前问题不可解，可能与海森矩阵和线性约束矩阵相关" << endl;
		return false;
	}
	Column Q2phi;
	getAB(Q2phi, Q2, phi);
	getAB(problem.x, Q1, RT_inverse_b);
	getAPlusB(problem.x, Q2phi);

	Matrix AAT;
	getABT(AAT, problem.AE, problem.AE);
	Column Hx, Hxplusp, A_Hxplusp;
	getAB(Hx, problem.H, problem.x);
	getAPlusB(Hxplusp, Hx, problem.p);
	getAB(A_Hxplusp, problem.AE, Hxplusp);
	isSolvable = Ax_b_solver(problem.factorE, AAT, A_Hxplusp);
	if (!isSolvable)
	{
		cout << "当前问题等式约束的拉格朗日乘子不可解" << endl;
		return false;
	}
	return true;
}

class Nu
{
public:
	Column x;
	Column y;
	Column factorE;
	Column factorI;
};

class Delta
{
public:
	Column x;
	Column y;
	Column factorE;
	Column factorI;
};

int getNuBeforePre(Nu& nu, EICQP problem)
{
	Column Hx;
	Column AETFactorE;
	Column AITFactorI;
	getAB(Hx, problem.H, problem.x);
	getATB(AETFactorE, problem.AE, problem.factorE);
	getATB(AITFactorI, problem.AI, problem.factorI);
	getAPlusB(nu.x, Hx, problem.p);
	getAMinusB(nu.x, AETFactorE);
	getAMinusB(nu.x, AITFactorI);

	Column AEx;
	getAB(AEx, problem.AE, problem.x);
	getAMinusB(nu.factorE, AEx, problem.bE);

	Column AIx;
	getAB(AIx, problem.AI, problem.x);
	getAMinusB(nu.factorI, problem.bI, AIx);
	getAPlusB(nu.factorI, problem.y);
	return 0;
}

int getNuAfterPre(Nu& nu, EICQP& problem, double tau, Matrix& Yinverse)
{
	Column e;
	getOneColumn(e, problem.size_I);
	Matrix tauYInverse;
	getAlphaA(tauYInverse, Yinverse, tau);
	Column tauYinverse_e;
	getAB(tauYinverse_e, tauYInverse, e);
	getAMinusB(tauYinverse_e, problem.factorI);
	return 0;
}

int getHk(Matrix& Hk, EICQP& problem, Matrix& Yinverse, Matrix& M)
{
	getATB(Hk, problem.AI, Yinverse);
	getAB(Hk, M);
	getAB(Hk, problem.AI);
	getAPlusB(Hk, problem.H);
}

int getQP_pre(ECQP& QP_pre, EICQP& problem, Matrix& Hk, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QP_pre.H = Hk;

	Matrix YinverseM;
	Column YinverseMNU_factorI;
	getAB(YinverseM, Yinverse, M);
	getAB(YinverseMNU_factorI, YinverseM, nu.factorI);
	Column YinverseMNU_factorIMinusFactorI;
	getAMinusB(YinverseMNU_factorIMinusFactorI, YinverseMNU_factorI, problem.factorI);
	getATB(QP_pre.p, problem.AI, YinverseMNU_factorIMinusFactorI);
	getAlphaA(QP_pre.p, -1);
	getAPlusB(QP_pre.p, nu.x);

	QP_pre.AE = problem.AE;
	QP_pre.bE = nu.factorE;
	getAlphaA(QP_pre.bE, -1);
	return 0;
}

int getQP_cor(ECQP& QP_cor, EICQP& problem, Matrix& Hk, Nu& nu)
{
	QP_cor.H = Hk;
	getATB(QP_cor.p, problem.AI, nu.y);
	getAlphaA(QP_cor.p, -1);
	QP_cor.AE = problem.AE;
	getZeroColumn(QP_cor.bE, problem.size_E);
}

int getDelta_pre(Delta& delta_pre, EICQP& problem, ECQP& QP_pre, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QR_solver(QP_pre);
	delta_pre.x = QP_pre.x;
	delta_pre.factorE = QP_pre.factorE;

	Column AIdeltax;
	getAB(AIdeltax, problem.AI, delta_pre.x);
	getAMinusB(delta_pre.y, AIdeltax, nu.factorI);

	Matrix yIM;
	Column yIMdeltay;
	getAB(yIM, Yinverse, M);
	getAB(yIMdeltay, yIM, delta_pre.y);
	getAPlusB(delta_pre.factorI, problem.factorI, yIMdeltay);
	getAlphaA(delta_pre.factorI, -1);
	return 0;
}

int getDelta_cor(Delta& delta_cor, EICQP& problem, ECQP& QP_cor, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QR_solver(QP_cor);
	delta_cor.x = QP_cor.x;
	delta_cor.factorE = QP_cor.factorE;
	getAB(delta_cor.y, problem.AI, delta_cor.x);
	Matrix YIM;
	getAB(YIM, Yinverse, M);
	getAB(delta_cor.factorI, YIM, delta_cor.y);
	getAlphaA(delta_cor.factorI, -1);
	getAPlusB(delta_cor.factorI, nu.y);
}

bool DPTPC_solver(EICQP& problem, double tol)
{
	return true;
}