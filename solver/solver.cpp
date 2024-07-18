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