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

	EICQP problem = EICQP(2, 1, 2);
	problem.x = { 0,0 };
	problem.H = { {1,0},{0,1} };

	problem.AE = { {1,1} };
	problem.bE = { 2 };
	problem.AI = { {1,0},{0,1} };
	problem.bI = { 0,0 };
	double tol = 1e-6;
	DPTPC_solver(problem, tol);
	problem.QPprint();
	
	//EICQP problem = EICQP(4, 1, 6);
	//problem.x = { 1,0,1,1 };
	//problem.H = { {1,-4,2,1},{-4,16,-8,-1},{2,-8,4,2},{1,-1,2,1} };
	//problem.p = { -1,0,7,4 };
	//problem.q = 5;
	//problem.AE = { {1,1,1,1} };
	//problem.bE = { 4 };
	//problem.AI = { {-1,-2,0,0},{0,-1,0,0},{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	//problem.bI = { -3.5,-0.5,0,0,0,0 };
	//double tol = 1e-30;

	//problem.QPprint();

	//DPTPC_solver(problem, tol);
	//printA(problem.x);
	//cout << endl;
	//printA(problem.factorE);
	//cout << endl;
	//printA(problem.factorI);
	//cout << endl;
	//printA(problem.y);

	return 0;
}