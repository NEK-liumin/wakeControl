#include "wake.h"
#include <algorithm>
#include <math.h>

using std::vector;
using std::pair;
using Pair = pair<double, int>;
using std::sort;

bool compare(Pair& a, Pair& b)
{
	return a.first < b.first;
}

int Wake::getNewCloud()
{
	auto ox = min_element(turbCloud->x0.begin(), turbCloud->x0.end());
	auto oy = min_element(turbCloud->y0.begin(), turbCloud->y0.end());
	auto oz = min_element(turbCloud->z0.begin(), turbCloud->z0.end());

	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		double xTemp, yTemp;
		xTemp = turbCloud->x0[i] - *ox;
		yTemp = turbCloud->y0[i] - *oy;

		newCloud.x0[i] = xTemp * cos(theta) + yTemp * sin(theta);
		newCloud.y0[i] = -xTemp * sin(theta) + yTemp * cos(theta);
		newCloud.z0[i] = turbCloud->z0[i] - *oz;
	}

	vector<Pair> index(turbCloud->turbNum);
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		index[i] = { newCloud.x0[i],i };
	}
	sort(index.begin(), index.end(), compare);

	Column temp1;

	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp1[i] = newCloud.x0[index[i].second];
	}
	newCloud.x0 = temp1;
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp1[i] = newCloud.y0[index[i].second];
	}
	newCloud.y0 = temp1;
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp1[i] = newCloud.z0[index[i].second];
	}
	newCloud.z0 = temp1;

	vector<int> temp2;
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp2[i] = newCloud.uNum[index[i].second];
	}
	newCloud.uNum = temp2;

	Matrix temp3;
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp3[i] = newCloud.uWind[index[i].second];
	}
	newCloud.uWind = temp3;
	for (int i = 0; i < turbCloud->turbNum; ++i)
	{
		temp3[i] = newCloud.Cp[index[i].second];
	}
	newCloud.Cp = temp3;
	return 0;
}