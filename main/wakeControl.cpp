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
	
	int size_x = 5;
	int size_E = 2;
	int size_I = 8;
	ICQP icqp = ICQP(size_x, size_I);
	printA(icqp.x);
	cout << endl;
	printA(icqp.H);
	cout << endl;
	printA(icqp.p);
	cout << endl;
	printA(icqp.cI);
	cout << endl;
	printA(icqp.bI);
	printA(icqp.factorI);
	return 0;
}