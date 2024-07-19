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
	
	column2Diagnoal(L, b);
	printA(L);
	return 0;
}