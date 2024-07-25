// wakeControl.cpp: 定义应用程序的入口点。
//
#include "iostream"
#include "head.h"
#include <chrono>
using namespace std;

int main()
{

	auto start = std::chrono::high_resolution_clock::now();

	Matrix A = { {3,2,3},{2,6,5},{3,5,8} };
	Matrix L, D, LT;
	Column x;
	Column b = { 1,2,3 };

	Matrix Q, R;


	ICQP problem = ICQP(3, 5);
	getZeroMatrix(problem.H, 3, 3);
	problem.p = { -1,-1,-5 };
	problem.q = 0;
	problem.AI = { {-3,-2,-0.25},{0,0,-1},{1,0,0},{0,1,0},{0,0,1} };
	problem.bI = { -6,-4,0,0,0 };
	problem.x = { 2.5,2.5,3 };
	double tol = 1e-4;
	PCDPF_solver(problem, tol);
	problem.QPprint();

	//double x0;
	//x0 = wilkinsonShift(1,2,3);
	//cout << x0 << endl;

	//EICQP problem = EICQP(2, 1, 2);
	//problem.x = { 0,0 };
	//problem.H = { {1,0},{0,1} };

	//problem.AE = { {1,1} };
	//problem.bE = { 2 };
	//problem.AI = { {1,0},{0,1} };
	//problem.bI = { 0,0 };
	//double tol = 1e-4;
	//DPTPC_solver(problem, tol);
	//problem.QPprint();
	
	//EICQP problem = EICQP(4, 1, 6);
	//problem.x = { 1,0,1,1 };
	//problem.H = { {1,-4,2,1},{-4,16,-8,-4},{2,-8,4,2},{1,-1,2,1} };
	//problem.p = { -1,0,7,4 };
	//problem.q = 5;
	//problem.AE = { {1,1,1,1} };
	//problem.bE = { 4 };
	//problem.AI = { {-1,-2,0,0},{0,-1,0,0},{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
	//problem.bI = { -3.5,-0.5,0,0,0,0 };
	//double tol = 1e-5;



	//PCDPF_solver(problem, tol);
	//problem.QPprint();
	// 
	// 
	// 
	//printA(problem.x);
	//cout << endl;
	//printA(problem.factorE);
	//cout << endl;
	//printA(problem.factorI);
	//cout << endl;
	//printA(problem.y);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	cout << "程序运行时间：" << duration.count() << " 秒" << endl;
	return 0;
}