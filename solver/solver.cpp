#include "solver.h"
#include "iostream"
#include "algorithm"
#include <cmath>
using std::cout;
using std::endl;
using std::min;
using std::max;

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
		isSolvable = Ax_b_solver(problem.lambda, AAT, A_Hxplusp);
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
	isSolvable = Ax_b_solver(problem.lambda, AAT, A_Hxplusp);
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
	Column lambda;
	Column mu;
};

class Delta
{
public:
	Column x;
	Column y;
	Column lambda;
	Column mu;
};
// nu.x = Hx + p - AETlambda - AITmu
// nu.y = tau * Y^-1 * e - mu
// nu.lambda = AEx - bE
// nu.mu = bI - AIx + y
int getNuBeforePre(Nu& nu, EICQP problem)
{
	Column Hx;
	Column AETlambda;
	Column AITmu;
	getAB(Hx, problem.H, problem.x);
	getATB(AETlambda, problem.AE, problem.lambda);
	getATB(AITmu, problem.AI, problem.mu);
	getAPlusB(nu.x, Hx, problem.p);
	getAMinusB(nu.x, AETlambda);
	getAMinusB(nu.x, AITmu);

	Column AEx;
	getAB(AEx, problem.AE, problem.x);
	getAMinusB(nu.lambda, AEx, problem.bE);

	Column AIx;
	getAB(AIx, problem.AI, problem.x);
	getAMinusB(nu.mu, problem.bI, AIx);
	getAPlusB(nu.mu, problem.y);
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
	getAMinusB(nu.y ,tauYinverse_e, problem.mu);
	return 0;
}

int getHk(Matrix& Hk, EICQP& problem, Matrix& Yinverse, Matrix& M)
{
	getATB(Hk, problem.AI, Yinverse);
	getAB(Hk, M);
	getAB(Hk, problem.AI);
	getAPlusB(Hk, problem.H);
	return 0;
}

int getQP_pre(ECQP& QP_pre, EICQP& problem, Matrix& Hk, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QP_pre.H = Hk;

	Matrix YinverseM;
	Column YinverseMNU_mu;
	getAB(YinverseM, Yinverse, M);
	getAB(YinverseMNU_mu, YinverseM, nu.mu);
	Column YinverseMNU_muMinusmu;
	getAMinusB(YinverseMNU_muMinusmu, YinverseMNU_mu, problem.mu);
	getATB(QP_pre.p, problem.AI, YinverseMNU_muMinusmu);
	getAlphaA(QP_pre.p, -1);
	getAPlusB(QP_pre.p, nu.x);

	QP_pre.AE = problem.AE;
	QP_pre.bE = nu.lambda;
	getAlphaA(QP_pre.bE, -1);
	return 0;
}

int getStep(double& alphaP, double& alphaD, Delta& delta, EICQP& problem)
{
	alphaP = LARGE;
	alphaD = LARGE;
	double eta = 1 - 1e-3;
	for (int i = 0; i < problem.size_I; ++i)
	{
		if (delta.y[i] < 0)
		{
			alphaP = min(alphaP, -problem.y[i] / delta.y[i]);
		}
		if (delta.mu[i] < 0)
		{
			alphaD = min(alphaD, -problem.mu[i] / delta.mu[i]);
		}
	}
	alphaP = min(1.0, eta * alphaP);
	alphaD = min(1.0, eta * alphaD);
	return 0;
}

int getTau(double& tau, EICQP& problem, Delta delta_pre, double& alphaP_pre, double& alphaD_pre)
{
	double tauk;
	double tau_pre;
	getATB(tauk, problem.mu, problem.y);
	tauk = tauk * 1.0 / problem.size_I;
	Column alphaDeltamu, alphaDeltay;
	getAlphaA(alphaDeltamu, delta_pre.mu, alphaD_pre);
	getAlphaA(alphaDeltay, delta_pre.y, alphaP_pre);
	getAPlusB(alphaDeltamu, problem.mu);
	getAPlusB(alphaDeltay, problem.y);
	getATB(tau_pre, alphaDeltamu, alphaDeltay);
	tau_pre = tau_pre * 1.0 / problem.size_I;
	double sigma;
	sigma = pow(tau_pre / tauk, 3);

	tau = sigma * tauk;
	return 0;
}

int getQP_cor(ECQP& QP_cor, EICQP& problem, Matrix& Hk, Nu& nu)
{
	QP_cor.H = Hk;
	getATB(QP_cor.p, problem.AI, nu.y);
	getAlphaA(QP_cor.p, -1);
	QP_cor.AE = problem.AE;
	getZeroColumn(QP_cor.bE, problem.size_E);
	return 0;
}

int getDelta_pre(Delta& delta_pre, EICQP& problem, ECQP& QP_pre, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QR_solver(QP_pre);
	delta_pre.x = QP_pre.x;
	delta_pre.lambda = QP_pre.lambda;

	Column AIdeltax;
	getAB(AIdeltax, problem.AI, delta_pre.x);
	getAMinusB(delta_pre.y, AIdeltax, nu.mu);

	Matrix yIM;
	Column yIMdeltay;
	getAB(yIM, Yinverse, M);
	getAB(yIMdeltay, yIM, delta_pre.y);
	getAPlusB(delta_pre.mu, problem.mu, yIMdeltay);
	getAlphaA(delta_pre.mu, -1);
	return 0;
}

int getDelta_cor(Delta& delta_cor, EICQP& problem, ECQP& QP_cor, Nu& nu, Matrix& Yinverse, Matrix& M)
{
	QR_solver(QP_cor);
	delta_cor.x = QP_cor.x;
	delta_cor.lambda = QP_cor.lambda;
	getAB(delta_cor.y, problem.AI, delta_cor.x);
	Matrix YIM;
	getAB(YIM, Yinverse, M);
	getAB(delta_cor.mu, YIM, delta_cor.y);
	getAlphaA(delta_cor.mu, -1);
	getAPlusB(delta_cor.mu, nu.y);
	return 0;
}

int getDelta(Delta& delta, Delta& delta_pre, Delta& delta_cor)
{
	getAPlusB(delta.x, delta_pre.x, delta_cor.x);
	getAPlusB(delta.y, delta_pre.y, delta_cor.y);
	getAPlusB(delta.lambda, delta_pre.lambda, delta_cor.lambda);
	getAPlusB(delta.mu, delta_pre.mu, delta_cor.mu);
	return 0;
}

int getNewPoint(EICQP& problem, Delta& delta, double& alphaP, double& alphaD)
{
	getAlphaA(delta.x, alphaP);
	getAlphaA(delta.y, alphaP);
	getAlphaA(delta.lambda, alphaD);
	getAlphaA(delta.mu, alphaD);
	getAPlusB(problem.x, delta.x);
	getAPlusB(problem.y, delta.y);
	getAPlusB(problem.lambda, delta.lambda);
	getAPlusB(problem.mu, delta.mu);
	return 0;
}

int getNewInterval(double& interval, EICQP& problem)
{
	getATB(interval, problem.mu, problem.y);
	return 0;
}

bool PCDPF_solver(EICQP& problem, double tol)
{
	ECQP QP_pre = ECQP(problem.size_x, problem.size_E);
	ECQP QP_cor = ECQP(problem.size_x, problem.size_E);

	Matrix Y, YInverse, M;
	Nu nu;
	Matrix Hk;
	Delta delta, delta_pre, delta_cor;
	double alphaP_pre, alphaP;
	double alphaD_pre, alphaD;
	double tau;
	double interval;
	int k = 0;
	do
	{
		column2Diagnoal(Y, problem.y);
		diagonalInverse(YInverse, Y);
		column2Diagnoal(M, problem.mu);
	
		getNuBeforePre(nu, problem);
	
		getHk(Hk, problem, YInverse, M);

		getQP_pre(QP_pre, problem, Hk, nu, YInverse, M);

		getDelta_pre(delta_pre, problem, QP_pre, nu, YInverse, M);

		getStep(alphaP_pre, alphaD_pre, delta_pre, problem);
	
		getTau(tau, problem, delta_pre, alphaP_pre, alphaD_pre);
		getNuAfterPre(nu, problem, tau, YInverse);
		getQP_cor(QP_cor, problem, Hk, nu);
		
		getDelta_cor(delta_cor, problem, QP_cor, nu, YInverse, M);
		
		
		getDelta(delta, delta_pre, delta_cor);

		getStep(alphaP, alphaD, delta, problem);

		getNewPoint(problem, delta, alphaP, alphaD);

		getNewInterval(interval, problem);

	} while (interval > tol);

	return true;
}