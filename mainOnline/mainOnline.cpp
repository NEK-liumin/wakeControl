#include "wakeControlOnline.h"
#include "Blondel.h"
int main()
{
	int turbNum = 9;
	double u = 8, theta360 = 0;
	string date = "202411011622";

	Column nextGamma;
	Column currentAbsGamma;
	getZeroColumn(currentAbsGamma, turbNum);
	getZeroColumn(nextGamma, turbNum);
	Bool isWork;
	isWork.resize(turbNum);
	
	for (int i = 0; i < turbNum; ++i)
	{
		isWork[i] = '1';
	}
	isWork[0] = '1';
	isWork[2] = '1';
	isWork[3] = '1';
	isWork[7] = '1';
	WakeControlOnline online;
	online.run(nextGamma, u, theta360, currentAbsGamma, isWork, date);
	online.power(nextGamma);

	/*double k = 0.2985;
	double kyInfty;
	double kzInfty;
	
	double CtInfty = 0.9;

	Column IInfty, deltaInfty;
	IInfty.resize(16);
	deltaInfty.resize(16);

	for (int i = 0; i < 16; ++i)
	{
		IInfty[i] = 0.01 * i + 0.05;

		kyInfty = k * IInfty[i];
		kzInfty = k * IInfty[i];
		Blondel modelTest = Blondel(kyInfty, kzInfty, IInfty[i]);
		modelTest.deltaInfty(deltaInfty[i], CtInfty, IInfty[i]);
	}

	for (int i = 0; i < 16; ++i)
	{
		cout << IInfty[i] << "    " << abs(deltaInfty[i]) << endl;
	}*/
	
	
	

	return 0;
}