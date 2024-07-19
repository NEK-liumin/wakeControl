#ifndef SOLVER_H
#define SOLVER_H

#include "matrixOperation.h"
#include "QP.h"

bool Lx_b_solver(Column& x, Matrix& L, Column& b);

bool Dx_b_solver(Column& x, Matrix& D, Column& b);

bool LTx_b_solver(Column& x, Matrix& LT, Column& b);

bool Ax_b_solver(Column& x, Matrix& A, Column& b);

bool LDL_solver(QP& problem);

bool QR_solver(ECQP& problem);

// Dual path tracking algorithm with prediction correction
// 带预测校正的对偶路径跟踪算法
bool DPTPC_solver(EICQP& problem, double tol);
#endif // !SOLVER_H
