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