#include "iostream"
#include "head.h"
using std::cout;
using std::endl;
using std::cin;

int main()
{
	int turbNum;
	int turbTypeNum;
	int uNum;
	double uMin;
	double uMax;
	cout << "========turbinesInfoTemp文件生成器=========" << endl;
	cout << "请输入风机数量" << endl;
	cin >> turbNum;
	cout << "请输入风机种类数" << endl;
	cin >> turbTypeNum;
	cout << "风速阶梯数" << endl;
	cin >> uNum;
	cout << "最小风速和最大风速" << endl;
	cin >> uMin >> uMax;
	TurbCloud turbines = TurbCloud(turbNum, turbTypeNum, uNum, uMin, uMax);
	generateTestDate(turbines);
	cout << "=======turbinesInfoTemp文件生成结束========" << endl;
	getchar();
	return 0;
}