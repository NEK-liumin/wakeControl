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
	cout << "========turbinesInfoTemp�ļ�������=========" << endl;
	cout << "������������" << endl;
	cin >> turbNum;
	cout << "��������������" << endl;
	cin >> turbTypeNum;
	cout << "���ٽ�����" << endl;
	cin >> uNum;
	cout << "��С���ٺ�������" << endl;
	cin >> uMin >> uMax;
	TurbCloud turbines = TurbCloud(turbNum, turbTypeNum, uNum, uMin, uMax);
	generateTestDate(turbines);
	cout << "=======turbinesInfoTemp�ļ����ɽ���========" << endl;
	getchar();
	return 0;
}