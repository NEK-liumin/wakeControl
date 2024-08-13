#include "solver.h"
#include "iostream"
#include "algorithm"
#include <cmath>
#include <usr.h>
using std::cout;
using std::endl;
using std::min;
using std::max;

int BFGS(Matrix& B, NCGP& problem, NCGP& problemNext)
{
	Column deltax;
	Column eta;
	Column gamma;

	double theta;

	double deltaxTgamma;
	Column Bdeltax;
	double deltaxTBdeltax;

	getAMinusB(deltax, problemNext.x, problem.x);
	getAMinusB(gamma, problemNext.g, problem.g);

	getATB(deltaxTgamma, deltax, gamma);
	getAB(Bdeltax, B, deltax);
	getATB(deltaxTBdeltax, deltax, Bdeltax);

	if (deltaxTgamma >= 0.2 * deltaxTBdeltax)
	{
		theta = 1;
	}
	else
	{
		theta = 0.8 * deltaxTBdeltax / (deltaxTBdeltax - deltaxTgamma);
	}

	getAlphaA(eta, Bdeltax, 1 - theta);
	Column thetaGamma;
	getAlphaA(thetaGamma, gamma, theta);
	getAPlusB(eta, thetaGamma);

	Matrix etaEtaT;
	getABT(etaEtaT, eta, eta);
	Matrix BdeltaDeltaB;
	getABT(BdeltaDeltaB, Bdeltax, Bdeltax);

	double deltaTeta;
	getATB(deltaTeta, deltax, eta);

	getAlphaA(etaEtaT, 1 / deltaTeta);
	getAlphaA(BdeltaDeltaB, 1 / deltaxTBdeltax);

	getAPlusB(B, etaEtaT);
	getAMinusB(B, BdeltaDeltaB);

	return 0;
}

int BFGS(Matrix& B, ECGP& problem, ECGP& problemNext)
{
	Column deltax;
	Column eta;
	Column gamma;
	Column deltag;
	Matrix deltaJe;
	Column deltaJeLambda;

	double theta;

	double deltaxTgamma;
	Column Bdeltax;
	double deltaxTBdeltax;

	getAMinusB(deltax, problemNext.x, problem.x);

	getAMinusB(deltag, problemNext.g, problem.g);
	getAMinusB(deltaJe, problemNext.Je, problem.Je);
	getATB(deltaJeLambda, deltaJe, problem.lambda);

	getAMinusB(gamma, problemNext.g, problem.g);
	getAMinusB(gamma, deltaJeLambda);

	getATB(deltaxTgamma, deltax, gamma);
	getAB(Bdeltax, B, deltax);
	getATB(deltaxTBdeltax, deltax, Bdeltax);

	if (deltaxTgamma >= 0.2 * deltaxTBdeltax)
	{
		theta = 1;
	}
	else
	{
		theta = 0.8 * deltaxTBdeltax / (deltaxTBdeltax - deltaxTgamma);
	}

	getAlphaA(eta, Bdeltax, 1 - theta);
	Column thetaGamma;
	getAlphaA(thetaGamma, gamma, theta);
	getAPlusB(eta, thetaGamma);

	Matrix etaEtaT;
	getABT(etaEtaT, eta, eta);
	Matrix BdeltaDeltaB;
	getABT(BdeltaDeltaB, Bdeltax, Bdeltax);

	double deltaTeta;
	getATB(deltaTeta, deltax, eta);

	getAlphaA(etaEtaT, 1 / deltaTeta);
	getAlphaA(BdeltaDeltaB, 1 / deltaxTBdeltax);

	getAPlusB(B, etaEtaT);
	getAMinusB(B, BdeltaDeltaB);

	return 0;
}

int BFGS(Matrix& B, ICGP& problem, ICGP& problemNext)
{
	Column deltax;
	Column eta;
	Column gamma;
	Column deltag;
	Matrix deltaJi;
	Column deltaJiMu;

	double theta;

	double deltaxTgamma;
	Column Bdeltax;
	double deltaxTBdeltax;

	getAMinusB(deltax, problemNext.x, problem.x);

	getAMinusB(deltag, problemNext.g, problem.g);
	getAMinusB(deltaJi, problemNext.Ji, problem.Ji);
	getATB(deltaJiMu, deltaJi, problem.mu);

	getAMinusB(gamma, problemNext.g, problem.g);
	getAMinusB(gamma, deltaJiMu);

	getATB(deltaxTgamma, deltax, gamma);
	getAB(Bdeltax, B, deltax);
	getATB(deltaxTBdeltax, deltax, Bdeltax);

	if (deltaxTgamma >= 0.2 * deltaxTBdeltax)
	{
		theta = 1;
	}
	else
	{
		theta = 0.8 * deltaxTBdeltax / (deltaxTBdeltax - deltaxTgamma);
	}

	getAlphaA(eta, Bdeltax, 1 - theta);
	Column thetaGamma;
	getAlphaA(thetaGamma, gamma, theta);
	getAPlusB(eta, thetaGamma);

	Matrix etaEtaT;
	getABT(etaEtaT, eta, eta);
	Matrix BdeltaDeltaB;
	getABT(BdeltaDeltaB, Bdeltax, Bdeltax);

	double deltaTeta;
	getATB(deltaTeta, deltax, eta);

	getAlphaA(etaEtaT, 1 / deltaTeta);
	getAlphaA(BdeltaDeltaB, 1 / deltaxTBdeltax);

	getAPlusB(B, etaEtaT);
	getAMinusB(B, BdeltaDeltaB);

	return 0;
}

int BFGS(Matrix& B, EICGP& problem, EICGP& problemNext)
{
	Column deltax;
	Column eta;
	Column gamma;
	Column deltag;
	Matrix deltaJe;
	Column deltaJeLambda;
	Matrix deltaJi;
	Column deltaJiMu;

	double theta;

	double deltaxTgamma;
	Column Bdeltax;
	double deltaxTBdeltax;

	getAMinusB(deltax, problemNext.x, problem.x);

	getAMinusB(deltag, problemNext.g, problem.g);
	getAMinusB(deltaJe, problemNext.Je, problem.Je);
	getATB(deltaJeLambda, deltaJe, problem.lambda);
	getAMinusB(deltaJi, problemNext.Ji, problem.Ji);
	getATB(deltaJiMu, deltaJi, problem.mu);

	getAMinusB(gamma, problemNext.g, problem.g);
	getAMinusB(gamma, deltaJeLambda);
	getAMinusB(gamma, deltaJiMu);

	getATB(deltaxTgamma, deltax, gamma);
	getAB(Bdeltax, B, deltax);
	getATB(deltaxTBdeltax, deltax, Bdeltax);

	if (deltaxTgamma >= 0.2 * deltaxTBdeltax)
	{
		theta = 1;
	}
	else
	{
		theta = 0.8 * deltaxTBdeltax / (deltaxTBdeltax - deltaxTgamma);
	}

	getAlphaA(eta, Bdeltax, 1 - theta);
	Column thetaGamma;
	getAlphaA(thetaGamma, gamma, theta);
	getAPlusB(eta, thetaGamma);

	Matrix etaEtaT;
	getABT(etaEtaT, eta, eta);
	Matrix BdeltaDeltaB;
	getABT(BdeltaDeltaB, Bdeltax, Bdeltax);

	double deltaTeta;
	getATB(deltaTeta, deltax, eta);

	getAlphaA(etaEtaT, 1 / deltaTeta);
	getAlphaA(BdeltaDeltaB, 1 / deltaxTBdeltax);

	getAPlusB(B, etaEtaT);
	getAMinusB(B, BdeltaDeltaB);

	return 0;
}

// T只能是NCGP的子类
template<typename T>
bool SQPNC_solver(T& problem, double tol)
{
	double normal;
	T problemNext = problem;
	NCQP subProblem = NCQP(problem.size_x);
	Matrix B;
	getUnitMatrix(B, problem.size_x);
	problem.set_g();

	do
	{
		subProblem.H = B;
		subProblem.p = problem.g;
		LDL_solver(subProblem);
		getAPlusB(problemNext.x, problem.x, subProblem.x);
		problemNext.set_g();
		BFGS(B, problem, problemNext);
		problem = problemNext;
		normal = norm(subProblem.x);
	} while (normal >tol);
	problem.get_f();
	return true;
}

// T只能是ECGP的子类
template<typename T>
bool SQPEC_solver(T& problem, double tol)
{
	double normal;
	T problemNext = problem;
	ECQP subProblem = ECQP(problem.size_x, problem.size_e);
	Matrix B;
	getUnitMatrix(B, problem.size_x);
	problem.set_g();
	problem.set_Je();
	problem.get_ce();
	do
	{
		subProblem.H = B;
		subProblem.p = problem.g;
		subProblem.AE = problem.Je;
		getAlphaA(subProblem.bE, problem.ce, -1);
		QR_solver(subProblem);
		getAPlusB(problemNext.x, problem.x, subProblem.x);
		problemNext.lambda = subProblem.lambda;
		problemNext.set_g();
		problemNext.set_Je();
		problemNext.get_ce();
		BFGS(B, problem, problemNext);
		problem = problemNext;
		normal = norm(subProblem.x);
	} while (normal > tol);
	problem.get_f();
	return true;
}

// T只能是ICGP的子类
template<typename T>
bool SQPIC_solver(T& problem, double tol)
{
	double normal;
	T problemNext = problem;
	ICQP subProblem = ICQP(problem.size_x, problem.size_i);
	Matrix B;
	getUnitMatrix(B, problem.size_x);
	problem.set_g();
	problem.set_Ji();
	problem.get_ci();
	do
	{
		subProblem.init();
		subProblem.H = B;
		subProblem.p = problem.g;
		subProblem.AI = problem.Ji;
		getAlphaA(subProblem.bI, problem.ci, -1);
		PCDPF_solver(subProblem, tol);
		getAPlusB(problemNext.x, problem.x, subProblem.x);
		problemNext.mu = subProblem.mu;
		problemNext.set_g();
		problemNext.set_Ji();
		problemNext.get_ci();
		BFGS(B, problem, problemNext);
		problem = problemNext;
		normal = norm(subProblem.x);
	} while (normal > tol);
	problem.get_f();
	return true;
}
// 这个T只能是EICGP的子类
template<typename T>
bool SQPEIC_solver(T& problem, double tol)
{
	double normal;
	T problemNext = problem;
	EICQP subProblem = EICQP(problem.size_x, problem.size_e, problem.size_i);
	Matrix B;
	getUnitMatrix(B, problem.size_x);
	problem.set_g();
	problem.set_Je();
	problem.get_ce();
	problem.set_Ji();
	problem.get_ci();
	int k = 0;
	do
	{
		subProblem.init();
		subProblem.H = B;
		subProblem.p = problem.g;
		subProblem.AE = problem.Je;
		getAlphaA(subProblem.bE, problem.ce, -1);
		subProblem.AI = problem.Ji;
		getAlphaA(subProblem.bI, problem.ci, -1);
		// 必须把下面的tol设置成很小的数字，例如1e-20，才能得到比较准确的拉格朗日参数lambda和mu
		// 不知道为什么
		// 但奇怪的是，直接利用传入的tol也能得到正确的x
		// 可能lambda和mu的计算对初值敏感
		PCDPF_solver(subProblem, 1e-20);
		getAPlusB(problemNext.x, problem.x, subProblem.x);
		problemNext.lambda = subProblem.lambda;
		problemNext.mu = subProblem.mu;
		problemNext.set_g();
		problemNext.set_Je();
		problemNext.get_ce();
		problemNext.set_Ji();
		problemNext.get_ci();
		BFGS(B, problem, problemNext);
		problem = problemNext;
		normal = norm(subProblem.x);
	} while (normal > tol);
	problem.get_f();
	return true;
}

// ===================================================================
template bool SQPNC_solver<MyNCGP>(MyNCGP& problem, double tol);

template bool SQPEC_solver<MyECGP>(MyECGP& problem, double tol);

template bool SQPIC_solver<MyICGP>(MyICGP& problem, double tol);

template bool SQPEIC_solver<MyEICGP>(MyEICGP& problem, double tol);
template bool SQPEIC_solver<MyEICGP2>(MyEICGP2& problem, double tol);
template bool SQPEIC_solver<MyEICGP3>(MyEICGP3& problem, double tol);