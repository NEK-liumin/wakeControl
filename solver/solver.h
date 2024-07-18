#ifndef SOLVER_H
#define SOLVER_H

#include "matrixOperation.h"
bool Lx_b_solver(Column& x, Matrix& L, Column& b);

bool Dx_b_solver(Column& x, Matrix& D, Column& b);

bool LTx_b_solver(Column& x, Matrix& LT, Column& b);

bool Ax_b_solver(Column& x, Matrix& A, Column& b);

#endif // !SOLVER_H
