// wakeControl.cpp: 定义应用程序的入口点。
//
#include "iostream"
#include "head.h"
#include <chrono>
using namespace std;

void test0()
{
	double tol = 1e-4;
	MyEICGP2 problem = MyEICGP2(2, 1, 4);
	problem.x = { 3.5,3 };
	SQPEIC_solver(problem, tol);
	cout << "最小值位于" << endl;
	printA(problem.x);
	cout << "目标函数的最小值是：" << problem.f << endl;
}

void test1()
{
	double tol = 1e-6;
	MyEICGP3 problem = MyEICGP3(2, 1, 6);
	problem.x = { -3,-3 };
	SQPEIC_solver(problem, tol);
	cout << "最小值位于" << endl;
	printA(problem.x);
	cout << "目标函数的最小值是：" << problem.f << endl;
}

void test2()
{
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
}

int main()
{
	cout << "开始进行偏航矩阵计算" << endl;
	auto start = std::chrono::high_resolution_clock::now();
	// https://www.bilibili.com/video/BV1XT411x7ig/?spm_id_from=333.788&vd_source=22351688aa00db029a949c880636bc36
	// 例12.3
	//test0();
	// 
	// 例12.4
	//test1();
	// 
	// https://www.bilibili.com/video/BV1JK4y1z7AA/?spm_id_from=333.337.search-card.all.click&vd_source=22351688aa00db029a949c880636bc36
	// 例9.8
	//test2();

	TurbCloud turbines = TurbCloud(36, 3, 10, 0, 20);
	// generateTestDate(turbines);
	
	Input input = Input(turbines);
	input.readFile();
	turbines.turbPrint();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	cout << "程序运行时间：" << duration.count() << " 秒" << endl;
	cout << "计算结束" << endl;
	std::cin.get();
	return 0;
}