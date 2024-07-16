#ifndef MATRIXOPERATION_H
#define MATRIXOPERATION_H
#include<vector>
using std::vector;
using Matrix = vector<vector<double>>;
using Column = vector<double>;

int getAB(Matrix& result, Matrix& A, Matrix& B);

int getAB(Column& result, Matrix& A, Column& B);

int getAPlusB(Matrix& result, Matrix& A, Matrix& B);

int getAPlusB(Column& result, Column& A, Column& B);

int getAPlusB(Matrix& A, Matrix& B);

int getAPlusB(Column& A, Column& B);

int getAMinusB(Matrix& result, Matrix& A, Matrix& B);

int getAMinusB(Column& result, Column& A, Column& B);

int getAMinusB(Matrix& A, Matrix& B);

int getAMinusB(Column& A, Column& B);

int printA(Matrix& A);

int printA(Column& A);
#endif // ! MATRIXOPERATION_H
