#include "solver.h"
#include "iostream"
#include "algorithm"
#include <cmath>
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

bool SQP_solver(NCGP& problem, double tol)
{
	double normal;
	NCGP problemNext = problem;
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

	return true;
}

bool SQP_solver(ECGP& problem, double tol)
{
	double normal;
	ECGP problemNext = problem;
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
	return true;
}

bool SQP_solver(ICGP& problem, double tol)
{
	double normal;
	ICGP problemNext = problem;
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
	return true;
}

bool SQP_solver(EICGP& problem, double tol)
{
	double normal;
	EICGP problemNext = problem;
	EICQP subProblem = EICQP(problem.size_x, problem.size_e, problem.size_i);
	Matrix B;
	getUnitMatrix(B, problem.size_x);
	problem.set_g();
	problem.set_Je();
	problem.get_ce();
	problem.set_Ji();
	problem.get_ci();
	do
	{
		subProblem.init();
		subProblem.H = B;
		subProblem.p = problem.g;
		subProblem.AE = problem.Je;
		getAlphaA(subProblem.bE, problem.ce, -1);
		subProblem.AI = problem.Ji;
		getAlphaA(subProblem.bI, problem.ci, -1);
		PCDPF_solver(subProblem, tol);
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
	return true;
}