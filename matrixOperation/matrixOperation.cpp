#include "matrixOperation.h"
#include "iostream"
#include "iomanip"

using std::cout;
using std::endl;

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