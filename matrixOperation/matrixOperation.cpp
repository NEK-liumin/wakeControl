#include "matrixOperation.h"
#include "iostream"
#include "iomanip"
#include <cmath>

using std::cout;
using std::endl;

int interpolation(double& result, Matrix& A, int& ni, int& nj, double& factori, double& factorj)
{
	if (ni >= A.size() || nj >= A[0].size())
	{
		cout << "ni >= A.size() || nj >= A[0].size()" << endl;
		return 0;
	}
	double x00, x01, x10, x11;
	if (ni == A.size() - 1)
	{
		if (nj == A[0].size() - 1)
		{
			x00 = A[ni][nj];
			x01 = A[ni][nj];
			x10 = A[ni][nj];
			x11 = A[ni][nj];
		}
		else
		{
			x00 = A[ni][nj];
			x01 = A[ni][nj + 1];
			x10 = A[ni][nj];
			x11 = A[ni][nj + 1];
		}
	}
	else
	{
		if (nj == A[0].size() - 1)
		{
			x00 = A[ni][nj];
			x01 = A[ni][nj];
			x10 = A[ni + 1][nj];
			x11 = A[ni + 1][nj];
		}
		else
		{
			x00 = A[ni][nj];
			x01 = A[ni][nj + 1];
			x10 = A[ni + 1][nj];
			x11 = A[ni + 1][nj + 1];
		}
	}
	result = factori * factorj * x00 + (1 - factori) * factorj * x10 + factori * (1 - factorj) * x01 + (1 - factori) * (1 - factorj) * x11;
	return 0;
}

int findx(int& index, double& factor, Column& data, double& x)
{
	int length = data.size();
	if (length <= 1)
	{
		cout << "Column is too small and cannot interpret" << endl;
		return 0;
	}

	int begin = data[0];
	int end = data[length - 1];
	double delta = data[1] - data[0];

	if (x < data[0])
	{
		index = 0;
		factor = 1;
		return 0;
	}

	if (x > data[length - 1])
	{
		index = length - 1;
		factor = 1;
		return 0;
	}

	index = floor((x - begin) / delta);
	if (index < length - 1)
	{
		factor = (data[index + 1] - x) / delta;
	}
	else if (index == length - 1)
	{
		factor = 1.0;
	}
	return 0;
}

int getUniformA(Column& A, double& first, double& delta, int n)
{
	if (n < 2)
	{
		cout << "the Size of A is too small" << endl;
		return 0;
	}
	A.resize(n);
	for (int i = 0; i < n; ++i)
	{
		A[i] = first + i * delta;
	}
	return 0;
}

int getZeroMatrix(Matrix& result, int m, int n)
{
	result = Matrix(m, vector<double>(n, 0));
	return 0;
}

int getZeroColumn(Column& result, int m)
{
	result = Column(m, 0);
	return 0;
}

int getOneColumn(Column& result, int m)
{
	result = Column(m, 1);
	return 0;
}

int column2Diagnoal(Matrix& result, Column& A)
{
	int m = A.size();
	result.resize(m);
	for (int i = 0; i < m; ++i)
	{
		result[i].resize(m);
		for (int j = 0; j < m; ++j)
		{
			result[i][j] = 0;
		}
	}
	for (int i = 0; i < m; ++i)
	{
		result[i][i] = A[i];
	}
	return 0;
}

int diagonalInverse(Matrix& result, Matrix& A)
{
	int m = A.size();
	result = A;
	for (int i = 0; i < m; ++i)
	{
		result[i][i] = 1 / A[i][i];
	}
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

int getAT(Matrix& result, Matrix& A)
{
	int m = A.size();
	int n = A[0].size();
	result.resize(n);
	for (int i = 0; i < n; ++i)
	{
		result[i].resize(m);
		for (int j = 0; j < m; ++j)
		{
			result[i][j] = A[j][i];
		}
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

int getABT(Matrix& result, Matrix& A, Matrix& B)
{
	int m1 = A.size();
	int n1 = A[0].size();
	int m2 = B.size();
	int n2 = B[0].size();
	if (n1 != n2)
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
			result[i][j] = 0;
			for (int k = 0; k < n1; ++k)
			{
				result[i][j] += A[i][k] * B[j][k];
			}
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

int splitMatrix(Matrix& result1, Matrix& result2, Matrix& A, int n)
{
	int m1 = A.size();
	int n1 = A[0].size();
	result1.resize(m1);
	result2.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result1[i].resize(n);
		result2[i].resize(n1 - n);
		for (int j = 0; j < n; ++j)
		{
			result1[i][j] = A[i][j];
		}
		for (int j = 0; j < n1 - n; ++j)
		{
			result2[i][j] = A[i][j + n];
		}
	}
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
	if (m > n)
	{
		Column zeroColumn;
		getZeroColumn(zeroColumn, n);
		for (int i = n; i < m; ++i)
		{
			R[i] = zeroColumn;
		}
	}
	return 0;
}

bool LDL_decomposition(Matrix& L, Matrix& D, Matrix& A)
{

	int m = A.size();
	if (m <= 0)
	{
		cout << "LDL分解的矩阵至少需要含有一个元素！" << endl;
		return false;
	}
	int n = A[0].size();
	if (m != n)
	{
		cout << "LDL分解的矩阵必须是方阵" << endl;
		return false;
	}
	getUnitMatrix(L, m);
	getZeroMatrix(D, m, n);

	for (int j = 0; j < n; ++j) {
		D[j][j] = A[j][j];
		for (int k = 0; k < j; ++k) {
			D[j][j] -= L[j][k] * L[j][k] * D[k][k];
		}

		// 处理D[j][j]为零的情况
		if (D[j][j] == 0) {
			cout << "Matrix has a zero pivot at " << j+1 << ". LDL decomposition may not be possible." << endl;
			return false;
		}

		for (int i = j + 1; i < n; ++i) {
			L[i][j] = A[i][j];
			for (int k = 0; k < j; ++k) {
				L[i][j] -= L[i][k] * L[j][k] * D[k][k];
			}
			L[i][j] /= D[j][j];
		}
	}
	return true;
}

int printA(Matrix A)
{
	// 为方便展示，将绝对值很小的量显示为0
	int width = 15;
	int precision = 5;

	int m = A.size();
	int n = A[0].size();
	double eps = 1e-5;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (abs(A[i][j]) < eps)
				A[i][j] = 0;
			cout << std::setw(width) << std::setprecision(precision) << A[i][j];
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}

int printA(Column A)
{
	// 为方便展示，将绝对值很小的量显示为0
	int width = 15;
	int precision = 5;
	int m = A.size();
	double eps = 1e-5;
	for (int i = 0; i < m; ++i)
	{
		if (abs(A[i]) < eps)
			A[i] = 0;
		cout << std::setw(width) << std::setprecision(precision) << A[i] << endl;
	}
	cout << endl;
	return 0;
}

int printA(Bool A)
{
	int m = A.size();
	for (int i = 0; i < m; ++i)
	{
		cout << A[i] << endl;
	}
	cout << endl;
	return 0;
}

const double g = 7;
const double lanczos_coefficients[] = {
	0.99999999999980993,
	676.5203681218851,
	-1259.1392167224028,
	771.32342877765313,
	-176.61502916214059,
	12.507343278686905,
	-0.13857109526572012,
	9.9843695780195716e-6,
	1.5056327351493116e-7
};
double Gamma(double z)
{
	if (z < 0.5) {
		// 使用Gamma函数反射公式
		return PI / (sin(PI * z) * Gamma(1 - z));
	}
	else {
		z -= 1;
		double x = lanczos_coefficients[0];
		for (int i = 1; i < sizeof(lanczos_coefficients) / sizeof(double); ++i) {
			x += lanczos_coefficients[i] / (z + i);
		}
		double t = z + g + 0.5;
		return sqrt(2 * PI) * pow(t, z + 0.5) * exp(-t) * x;
	}
}