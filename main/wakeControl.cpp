// wakeControl.cpp: 定义应用程序的入口点。
//
#include "iostream"
#include "head.h"
using namespace std;

int main()
{
	Matrix A = { {1,2},{3,4} };
	Matrix B = { {1,2},{3,4} };
	Matrix C;
	Column D = { 1,2 };
	Column E;
	Column F = { 3,4 };
	getAMinusB(D, F);
	printA(D);
	return 0;
}