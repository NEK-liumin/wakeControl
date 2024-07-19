#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H
#include<vector>
using std::vector;
using Matrix = vector<vector<double>>;
using Column = vector<double>;

const double SMALL = 1e-12;
const double LARGE = 1e200;

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

int printA(Matrix& A);

int printA(Column& A);
#endif // ! MATRIXOPERATION_H
