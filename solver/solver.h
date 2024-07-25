#ifndef SOLVER_H
#define SOLVER_H

#include "matrixOperation.h"
#include "QP.h"
#include "GP.h"

// ====================LPsolver====================

// LΪ�����Ƿ���
bool Lx_b_solver(Column& x, Matrix& L, Column& b);

// DΪ�ԽǷ���
bool Dx_b_solver(Column& x, Matrix& D, Column& b);

// LTΪ�����Ƿ���
bool LTx_b_solver(Column& x, Matrix& LT, Column& b);

// Ҫ��A�Գƣ�������ֵ����Ϊ0
bool Ax_b_solver(Column& x, Matrix& A, Column& b);

// ====================QPsolver====================

// LDL�ֽ�����Լ�����ι滮�������Сֵ��Ҫ��H�Գ�����
bool LDL_solver(NCQP& problem);

// Ҫ��H�Գ�����
bool QR_solver(ECQP& problem);

// Predictor-Corrector Dual Path-Following Algorithm
// ��Ԥ��У���Ķ�ż·�������㷨
bool PCDPF_solver(ICQP& problem, double tol);

// Predictor-Corrector Dual Path-Following Algorithm
// ��Ԥ��У���Ķ�ż·�������㷨
bool PCDPF_solver(EICQP& problem, double tol);

// ====================GPsolver====================

// Sequential Quadratic Programming
// ���ж����Ż���
bool SQP_solver(NCGP& problem, double tol);

bool SQP_solver(ECGP& problem, double tol);

bool SQP_solver(ICGP& problem, double tol);

bool SQP_solver(EICGP& problem, double tol);

#endif // !SOLVER_H