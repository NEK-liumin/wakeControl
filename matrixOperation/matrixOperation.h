#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H
#include<vector>

// 除了矩阵运算之外，也提供了计算gamma函数的代码Gamma(double&x)
// 为区分风机偏航的角度，G大写

using std::vector;
using Matrix = vector<vector<double>>;
using Column = vector<double>;
// 输入字符类型的vector来表示bool类型，0表示false，1表示true
using Bool = std::vector<char>;

const double SMALL = 1e-16;
const double LARGE = 1e20;
const double PI = 3.1415926;

// 插值
int interpolation(double& result, Matrix& A, int& ni, int& nj, double& factori, double& factorj);

// 已知有序序列（均匀），已知x，求x在当前序列中的最近的左边值以及插值时需要的系数
int findx(int& index, double& factor, Column& data, double& x);

// 获得均匀分布的数列
int getUniformA(Column& A, double& first, double& delta, int n);

int getZeroMatrix(Matrix& result, int m, int n);

int getZeroColumn(Column& result, int m);

int getOneColumn(Column& result, int m);

int column2Diagnoal(Matrix& result, Column& A);

int diagonalInverse(Matrix& result, Matrix& A);

int getUnitMatrix(Matrix& result, int m);

int getUnitColumn(Column& result, int m, int i);

int getAlphaA(Matrix& result, Matrix& A, double alpha);

int getAlphaA(Matrix& A, double alpha);

int getAlphaA(Column& result, Column& A, double alpha);

int getAlphaA(Column& A, double alpha);

int getNormalizedA(Column& result, Column& A);

int getNormalizedA(Column& A);

int getAT(Matrix& result, Matrix& A);

int getAB(Matrix& result, Matrix& A, Matrix& B);

int getAB(Matrix& A, Matrix& B);

int getAB(Column& result, Matrix& A, Column& B);

int getATB(Matrix& result, Matrix& A, Matrix& B);

int getATB(Column& result, Matrix& A, Column& B);

int getATB(double& result, Column& A, Column& B);

int getABT(Matrix& result, Column& A, Column& B);

int getABT(Matrix& result, Matrix& A, Matrix& B);

int getAPlusB(Matrix& result, Matrix& A, Matrix& B);

int getAPlusB(Column& result, Column& A, Column& B);

int getAPlusB(Matrix& A, Matrix& B);

int getAPlusB(Column& A, Column& B);

int getAMinusB(Matrix& result, Matrix& A, Matrix& B);

int getAMinusB(Column& result, Column& A, Column& B);

int getAMinusB(Matrix& A, Matrix& B);

int getAMinusB(Column& A, Column& B);

double norm(Column& A);

int getFirstColumn(Column& result, Matrix& A);

int getReducedMatrix(Matrix& result, Matrix& A);

int getExtendedMatrix(Matrix& result, Matrix& A, int n);

int getExtendedMatrix(Matrix& A, int n);

int getExtendedColumn(Column& result, Column& A, int n);

bool getIsHeadColumn(Column& A);

int splitMatrix(Matrix& result1, Matrix& result2, Matrix& A, int n);

int QR_decomposition(Matrix& Q, Matrix& R, Matrix& A);

bool LDL_decomposition(Matrix& L, Matrix& D, Matrix& A);

int printA(Matrix A);

int printA(Column A);

int printA(Bool A);

// 计算gamma函数
// Gamma(z) = (z-1)!
double Gamma(double z);

#endif // ! MATRIXOPERATION_H
