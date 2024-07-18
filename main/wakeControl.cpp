// wakeControl.cpp: 定义应用程序的入口点。
//
#include "iostream"
#include "head.h"
using namespace std;

int main()
{
	Matrix A = { {3,2,3},{2,6,5},{3,5,8} };
	Matrix L, D, LT;
	Column x;
	Column b = { 1,2,3 };
	
	ECQP problem = ECQP(3, 3);
	getUnitMatrix(problem.H, 3);
	problem.p = { 2,1,-1 };
	problem.q = 3;
	problem.AE = { {3,-1,2},{-2,4,0},{-4,3,8} };
	problem.bE = { 7,12,10 };
	QR_solver(problem);
	printA(problem.x);
	cout << endl;
	printA(problem.factorE);
	return 0;
}