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
	cout << Gamma(1) << endl;
	
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

// 定义空气密度
	double rho = 1.225;
// 定义偏航角
	int turbNum = 33; // 风机数量。必须与输入文件中的风机数量一致
	Column gamma360;
	gamma360.resize(turbNum);
	for (int i = 0; i < turbNum; ++i)
	{
		gamma360[i] = 30.0;
	}
// 定义尾流模型
	double ky = 0.025;
	double kz = 0.0175;
	double I = 0.12;
	Gauss3 model = Gauss3(ky, kz, I);
// 定义风速、风向
	double wind = 6;
	double theta360 = 0.0;
// 是否输出云图
	bool isPlot = true;
// 计算尾流
	//cout << "开始进行尾流计算" << endl;
	//Simulation simulation(wind, theta360, model);
	//simulation.run(gamma360, isPlot);
	//cout << "尾流计算结束" << endl;
// 定义优化问题
	Yaw yaw = Yaw(wind, theta360, rho, model);

	/*for (int i = 0; i < 36; i++)
	{
		yaw.x[i] = 0.0;
	}*/
	// yaw.get_f();
	//cout << yaw.f << endl;
	double tol = 1.0e-5;
	SQPIC_solver(yaw, tol);
	//cout << "balabala" << endl;
	//printA(yaw.x);
	yaw.outputGamma(gamma360);
	printA(gamma360);
	cout << "Total power of wind field without yawing: " << yaw.initialPower() << " kw" << endl;
	cout << "Total power of wind field with yawing: " << yaw.power() << " kw" << endl;
	cout << "Power generation increased by " << (yaw.power() - yaw.initialPower()) / yaw.initialPower() * 100 << "% !" << endl;
    Simulation simulation(wind, theta360, model);
    simulation.run(gamma360, isPlot);
	//double power;
	//simulation.turbines.getPower(power, simulation.wake.vel);
	//cout << power << endl;

	/*printA(yaw.g);
	cout << yaw.f << endl;*/
	/*printA(yaw.ci);
	printA(yaw.Ji);*/
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	cout << "程序运行时间：" << duration.count() << " 秒" << endl;
	
	std::cin.get();
	return 0;
}