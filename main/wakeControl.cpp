// wakeControl.cpp: 定义应用程序的入口点。
//
#include "iostream"
#include "head.h"
using namespace std;

int main()
{
	Matrix A = { {1,2,0,1},{1,0,3,1},{1,0,3,2},{1,2,0,2} };
	Column B = { 1,2,3 };
	Matrix A1;
	Column B1;
	Matrix Q, R;
	QR_decomposition(Q, R, A);
	printA(Q);
	cout << endl;
	printA(R);
	return 0;
}