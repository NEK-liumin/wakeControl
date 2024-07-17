#include "matrixOperation.h"
#include "iostream"
#include "iomanip"
#include <cmath>

using std::cout;
using std::endl;

int getZeroMatrix(Matrix& result, int m, int n)
{
	result = Matrix(m, vector<double>(n, 0));
	return 0;
}

int getUnitMatrix(Matrix& result, int m)
{
	result = Matrix(m, vector<double>(m, 0));
	for (int i = 0; i < m; ++i)
	{
		result[i][i] = 1;
	}
	return 0;
}

int getUnitColumn(Column& result, int m, int i)
{
	result.resize(m);
	for (int j = 0; j < i - 1; ++j)
	{
		result[j] = 0;
	}
	result[i - 1] = 1;
	for (int j = i; j < m; ++j)
	{
		result[j] = 0;
	}
	return 0;
}

int getAlphaA(Matrix& result, Matrix& A, double alpha)
{
	int m = A.size();
	int n = A[0].size();
	result.resize(m);
	for (int i = 0; i < m; ++i)
	{
		result[i].resize(n);
		for (int j = 0; j < n; ++j)
		{
			result[i][j] = alpha * A[i][j];
		}
	}
	return 0;
}

int getAlphaA(Matrix& A, double alpha)
{
	int m = A.size();
	int n = A[0].size();
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			A[i][j] *= alpha;
		}
	}
	return 0;
}

int getAlphaA(Column& result, Column& A, double alpha)
{
	int m = A.size();
	result.resize(m);
	for (int i = 0; i < m; ++i)
	{
		result[i] = alpha * A[i];
	}
	return 0;
}

int getAlphaA(Column& A, double alpha)
{
	int m = A.size();
	for (int i = 0; i < m; ++i)
	{
		A[i] *= alpha;
	}
	return 0;
}

int getNormalizedA(Column& result, Column& A)
{
	int m = A.size();
	result.resize(m);
	double normal = norm(A);
	for (int i = 0; i < m; ++i)
	{
		result[i] = A[i] / normal;
	}
	return 0;
}

int getNormalizedA(Column& A)
{
	int m = A.size();
	double normal = norm(A);
	for (int i = 0; i < m; ++i)
	{
		A[i] /= normal;
	}
	return 0;
}

int getAB(Matrix& result, Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (n1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(n2);
		for (int j = 0; j < n2; ++j)
		{
			result[i][j] = 0;
			for (int k = 0; k < n1; ++k)
			{
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return 0;
}

int getAB(Matrix& A, Matrix& B)
{
	Matrix result;
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (n1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(n2);
		for (int j = 0; j < n2; ++j)
		{
			result[i][j] = 0;
			for (int k = 0; k < n1; ++k)
			{
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	A = result;
	return 0;
}

int getAB(Column& result, Matrix& A, Column& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	if (n1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i] = 0;
		for (int j = 0; j < n1; ++j)
		{
			result[i] += A[i][j] * B[j];
		}
	}
	return 0;
}

int getATB(Matrix& result, Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (m1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(n1);
	for (int i = 0; i < n1; ++i)
	{
		result[i].resize(n2);
		for (int j = 0; j < n2; ++j)
		{
			result[i][j] = 0;
			for (int k = 0; k < m1; ++k)
			{
				result[i][j] += A[k][i] * B[k][j];
			}
		}
	}
	return 0;
}

int getATB(Column& result, Matrix& A, Column& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(n1);
	for (int i = 0; i < n1; ++i)
	{
		result[i] = 0;
		for (int j = 0; j < m1; ++j)
		{
			result[i] += A[j][i] * B[j];
		}
	}
	return 0;
}

int getATB(double& result, Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "转置相乘的两个列矢不匹配" << endl;
		return 0;
	}
	result = 0;
	for (int i = 0; i < m1; ++i)
	{
		result += A[i] * B[i];
	}
	return 0;
}

int getABT(Matrix& result, Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相乘的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(m2);
		for (int j = 0; j < m2; ++j)
		{
			result[i][j] = A[i] * B[j];
		}
	}
	return 0;
}

int getAPlusB(Matrix& result, Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (m1 != m2 || n1 != n2)
	{
		cout << "相加的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(n1);
		for (int j = 0; j < n1; ++j)
		{
			result[i][j] = A[i][j] + B[i][j];
		}
	}
	return 0;
}

int getAPlusB(Column& result, Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相加的两个列矢不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i] = A[i] + B[i];
	}
	return 0;
}

int getAPlusB(Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (m1 != m2 || n1 != n2)
	{
		cout << "相加的两个矩阵不匹配" << endl;
		return 0;
	}
	for (int i = 0; i < m1; ++i)
	{
		for (int j = 0; j < n1; ++j)
		{
			A[i][j] = A[i][j] + B[i][j];
		}
	}
	return 0;
}

int getAPlusB(Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相加的两个列矢不匹配" << endl;
		return 0;
	}
	for (int i = 0; i < m1; ++i)
	{
		A[i] = A[i] + B[i];
	}
	return 0;
}

int getAMinusB(Matrix& result, Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (m1 != m2 || n1 != n2)
	{
		cout << "相减的两个矩阵不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(n1);
		for (int j = 0; j < n1; ++j)
		{
			result[i][j] = A[i][j] - B[i][j];
		}
	}
	return 0;
}

int getAMinusB(Column& result, Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相减的两个列矢不匹配" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i] = A[i] - B[i];
	}
	return 0;
}

int getAMinusB(Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (m1 != m2 || n1 != n2)
	{
		cout << "相减的两个矩阵不匹配" << endl;
		return 0;
	}
	for (int i = 0; i < m1; ++i)
	{
		for (int j = 0; j < n1; ++j)
		{
			A[i][j] = A[i][j] - B[i][j];
		}
	}
	return 0;
}

int getAMinusB(Column& A, Column& B)
{
	int m1 = A.size();
	int m2 = B.size();
	if (m1 != m2)
	{
		cout << "相减的两个列矢不匹配" << endl;
		return 0;
	}
	for (int i = 0; i < m1; ++i)
	{
		A[i] = A[i] - B[i];
	}
	return 0;
}

double norm(Column& A)
{
	double sum = 0;
	for (double elem : A)
	{
		sum += elem * elem;
	}
	return sqrt(sum);
}

int getFirstColumn(Column& result, Matrix& A)
{
	int m = A.size();
	result.resize(m);
	for (int i = 0; i < m; ++i)
	{
		result[i] = A[i][0];
	}
	return 0;
}

int getReducedMatrix(Matrix& result, Matrix& A)
{
	int m = A.size();
	int n = A[0].size();
	result.resize(m - 1);
	for (int i = 0; i < m - 1; ++i)
	{
		result[i].resize(n - 1);
		for (int j = 0; j < n - 1; ++j)
		{
			result[i][j] = A[i + 1][j + 1];
		}
	}
	return 0;
}

int getExtendedMatrix(Matrix& result, Matrix& A, int n)
{
	int m1 = A.size();
	int n1 = A[0].size();
	if (m1 != n1)
	{
		cout << "扩展函数只能扩展方阵" << endl;
		return 0;
	}
	if (n < m1)
	{
		cout << "扩展后的方阵不能比扩展之前小" << endl;
		return 0;
	}
	if (n == m1)
	{
		result = A;
		return 0;
	}
	result.resize(n);
	for (int i = 0; i < n; ++i)
	{
		result[i].resize(n);
		for (int j = 0; j < n; ++j)
		{
			if (i == j && i < n - m1)
			{
				result[i][j] = 1;
			}
			else if (i < n - m1 || j < n - m1)
			{
				result[i][j] = 0;
			}
			else
			{
				result[i][j] = A[i - n + m1][j - n + m1];
			}
		}
	}
	return 0;
}

int getExtendedMatrix(Matrix& A, int n)
{
	Matrix result;
	int m1 = A.size();
	int n1 = A[0].size();
	if (m1 != n1)
	{
		cout << "扩展函数只能扩展方阵" << endl;
		return 0;
	}
	if (n < m1)
	{
		cout << "扩展后的方阵不能比扩展之前小" << endl;
		return 0;
	}
	if (n == m1)
	{
		result = A;
		return 0;
	}
	result.resize(n);
	for (int i = 0; i < n; ++i)
	{
		result[i].resize(n);
		for (int j = 0; j < n; ++j)
		{
			if (i == j && i < n - m1)
			{
				result[i][j] = 1;
			}
			else if (i < n - m1 || j < n - m1)
			{
				result[i][j] = 0;
			}
			else
			{
				result[i][j] = A[i - n + m1][j - n + m1];
			}
		}
	}
	A = result;
	return 0;
}

int getExtendedColumn(Column& result, Column& A, int n)
{
	int m = A.size();
	result.resize(n);
	for (int i = 0; i < n - m; ++i)
	{
		result[i] = 0;
	}
	for (int i = n - m; i < n; ++i)
	{
		result[i] = A[i - n + m];
	}
	return 0;
}

bool getIsHeadColumn(Column& A)
{
	int m = A.size();
	for (int i = 1; i < m; ++i)
	{
		if (abs(A[i]) > SMALL)
		{
			return false;
		}
	}
	return true;
}

int getH(Matrix& H, Matrix& A)
{
	int m = A.size();
	Column firstColumn;
	getFirstColumn(firstColumn, A);
	Matrix I;
	getUnitMatrix(I, m);

	if (getIsHeadColumn(firstColumn))
	{
		H = I;
		return 0;
	}
	double alpha = norm(firstColumn);
	Column e;
	getUnitColumn(e, m, 1);
	Column u;
	getAlphaA(e, alpha);
	getAMinusB(u, firstColumn, e);
	getNormalizedA(u);
	Matrix uuT;
	getABT(uuT, u, u);
	getAlphaA(uuT, 2);
	getAMinusB(H, I, uuT);
	return 0;
}

int QR_decomposition(Matrix& Q, Matrix& R, Matrix& A)
{
	int m = A.size();
	int n = A[0].size();
	if (m < n)
	{
		cout << "QR分解的矩阵，列数必须小于等于行数！" << endl;
		return 0;
	}
	getUnitMatrix(Q, m);
	getZeroMatrix(R, m, n);
	Matrix Atemp = A;
	Matrix Htemp;
	Matrix HA;
	for (int i = 0; i < n; ++i)
	{
		getH(Htemp, Atemp);
		getAB(HA, Htemp, Atemp);
		getExtendedColumn(R[i], HA[0], n);
		getReducedMatrix(Atemp, HA);
		getExtendedMatrix(Htemp, m);
		getAB(Q, Htemp);
	}
	return 0;
}

int printA(Matrix& A)
{
	int width = 15;
	int precision = 5;

	int m = A.size();
	int n = A[0].size();
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << std::setw(width) << std::setprecision(precision) << A[i][j];
		}
		cout << endl;
	}
	return 0;
}

int printA(Column& A)
{
	int width = 15;
	int precision = 5;
	int m = A.size();
	for (int i = 0; i < m; ++i)
	{
		cout << std::setw(width) << std::setprecision(precision) << A[i] << endl;
	}
	return 0;
}