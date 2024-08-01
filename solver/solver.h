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
template<typename T>
bool SQPNC_solver(T& problem, double tol);

template<typename T>
bool SQPEC_solver(T& problem, double tol);

template<typename T>
bool SQPIC_solver(T& problem, double tol);

template<typename T>
bool SQPEIC_solver(T& problem, double tol);

#endif // !SOLVER_H