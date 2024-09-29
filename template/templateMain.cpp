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
	double isParameterFile;
	double uBegin, uEnd;
	double thetaBegin, thetaEnd;
	double deltaU, deltaTheta;
	double ky, kz;
	cout << "=============Template File Generator==============" << endl;
	cout << "Please Enter Wind Turbine Number:" << endl;
	cin >> turbNum;
	cout << "Please Enter Wind Turbine Type Number:" << endl;
	cin >> turbTypeNum;
	cout << "Please Enter Wind Step Number:" << endl;
	cin >> uNum;
	cout << "Please Enter Min And Max Wind Speed:" << endl;
	cin >> uMin >> uMax;
	TurbCloud turbines = TurbCloud(turbNum, turbTypeNum, uNum, uMin, uMax);
	generateTemplate(turbines);
	cout << "turbinesInfoTemp and parameterTemp file Generation Done!" << endl;
	cout << endl;
	std::cin.get();
	return 0;
}