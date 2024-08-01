#ifndef SOLVER_H
#define SOLVER_H

#include "matrixOperation.h"
#include "QP.h"
#include "GP.h"

// ====================LPsolver====================

// L为下三角方阵
bool Lx_b_solver(Column& x, Matrix& L, Column& b);

// D为对角方阵
bool Dx_b_solver(Column& x, Matrix& D, Column& b);

// LT为上三角方阵
bool LTx_b_solver(Column& x, Matrix& LT, Column& b);

// 要求A对称，且特征值不能为0
bool Ax_b_solver(Column& x, Matrix& A, Column& b);

// ====================QPsolver====================

// LDL分解求无约束二次规划问题的最小值。要求H对称正定
bool LDL_solver(NCQP& problem);

// 要求H对称正定
bool QR_solver(ECQP& problem);

// Predictor-Corrector Dual Path-Following Algorithm
// 带预测校正的对偶路径跟踪算法
bool PCDPF_solver(ICQP& problem, double tol);

// Predictor-Corrector Dual Path-Following Algorithm
// 带预测校正的对偶路径跟踪算法
bool PCDPF_solver(EICQP& problem, double tol);

// ====================GPsolver====================

// Sequential Quadratic Programming
// 序列二次优化法
template<typename T>
bool SQPNC_solver(T& problem, double tol);

template<typename T>
bool SQPEC_solver(T& problem, double tol);

template<typename T>
bool SQPIC_solver(T& problem, double tol);

template<typename T>
bool SQPEIC_solver(T& problem, double tol);

#endif // !SOLVER_H