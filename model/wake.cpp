#include "wake.h"
#include <algorithm>
#include <math.h>
#include "matrixOperation.h"

using std::vector;
using std::sort;

int Wake::xyzMin()
{
	auto ox = min_element(turbines->x0.begin(), turbines->x0.end());
	auto oy = min_element(turbines->y0.begin(), turbines->y0.end());
	auto oz = min_element(turbines->z0.begin(), turbines->z0.end());
	xmin = *ox;
	ymin = *oy;
	zmin = *oz;
	return 0;
}
int Wake::xyz_transformation(double& x, double& y, double& z)
{
	double xTemp, yTemp;
	xTemp = x - xmin;
	yTemp = y - ymin;
	x1 = xTemp * cos(theta) + yTemp * sin(theta);
	y1 = -xTemp * sin(theta) + yTemp * cos(theta);
	z1 = z - zmin;
	return 0;
}

Wake::Wake()
{
	theta = 0;
	turbines = nullptr;
	u = 0;
	x1 = 0;
	xmin = 0;
	y1 = 0;
	ymin = 0;
	z1 = 0;
	zmin = 0;
}
Wake::Wake(TurbCloud& turbines, double& u, double& theta)
{
	this->turbines = &turbines;
	this->newTurbines = turbines;
	this->u = u;
	this->theta = theta;
	getZeroColumn(vel, turbines.turbNum);
	getNewCloud();
	newVel = vel;
}

int Wake::setWake(TurbCloud& turbines, double& u, double& theta)
{
	this->turbines = &turbines;
	this->newTurbines = turbines;
	this->u = u;
	this->theta = theta;
	getZeroColumn(vel, turbines.turbNum);
	getNewCloud();
	newVel = vel;
	return 0;
}


bool compare(Pair& a, Pair& b)
{
	return a.first < b.first;
}

int Wake::getNewCloud()
{
	xyzMin();

	for (int i = 0; i < turbines->turbNum; ++i)
	{
		double xTemp, yTemp;
		xTemp = turbines->x0[i] - xmin;
		yTemp = turbines->y0[i] - ymin;

		newTurbines.x0[i] = xTemp * cos(theta) + yTemp * sin(theta);
		newTurbines.y0[i] = -xTemp * sin(theta) + yTemp * cos(theta);
		newTurbines.z0[i] = turbines->z0[i] - zmin;
	}

	index.resize(turbines->turbNum);
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		index[i] = { newTurbines.x0[i],i };
	}
	sort(index.begin(), index.end(), compare);

	Column temp1 = turbines->x0;

	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp1[i] = newTurbines.x0[index[i].second];
	}
	newTurbines.x0 = temp1;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp1[i] = newTurbines.y0[index[i].second];
	}
	newTurbines.y0 = temp1;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp1[i] = newTurbines.z0[index[i].second];
	}
	newTurbines.z0 = temp1;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp1[i] = turbines->D[index[i].second];
	}
	newTurbines.D = temp1;
	//不需要对偏航角度排序
	//因为偏航角是直接在newTurbines上设置的
	//但是
	//计算结束后
	//偏航角需要重新按照最初的turbines重新排序
	vector<int>temp2 = turbines->turbType;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp2[i] = turbines->turbType[index[i].second];
	}
	newTurbines.turbType = temp2;
	Bool temp3 = turbines->isWork;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		temp3[i] = turbines->isWork[index[i].second];
	}
	newTurbines.isWork = temp3;
	return 0;
}

int Wake::getWake(Model& model)
{
	model.getWake(newVel, newTurbines, u);
	return 0;
}
int Wake::getWake(Column& newGamma, Model& model)
{
	model.getWake(newVel, newTurbines, newGamma, u);
	return 0;
}
int Wake::getWake(double& meshVel, double& x, double& y, double& z, Column& newGamma, Model& model)
{
	getWake(newGamma, model);
	xyz_transformation(x, y, z);
	// x1, y1, z1是xyz点转换后的坐标
	model.getWake(meshVel, newVel, newTurbines, newGamma, u, x1, y1, z1);
	return 0;
}
// 恢复Vel的排序
int Wake::restoreVel()
{
	vel = newVel;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		int j = index[i].second;
		vel[j] = newVel[i];
	}
	return 0;
}
// 获取排列后风机的偏航角
int Wake::gamma2NewGamma(Column& newGamma, Column& gamma)
{
	sortA(newGamma, gamma);
	return 0;
}
Column Wake::newGamma2Gamma()
{
	Column gamma;
	//cout << "in newGamma2Gamma" << endl;
	if (newTurbines.gamma == nullptr)
	{
		cout << "newGamma is not defined!!!" << endl;
		return gamma;
	}
	gamma = *newTurbines.gamma;
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		int j = index[i].second;
		//cout << "i=" << i << "j=" << j << endl;
		gamma[j] = (*newTurbines.gamma)[i];
	}
	return gamma;
}

int Wake::restoreA(Column& result, Column& A)
{
	if (A.size() != turbines->turbNum)
	{
		cout << "Column can not be restored because size of A is not equal to turbine number" << endl;
		return 0;
	}
	result.resize(turbines->turbNum);
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		int j = index[i].second;
		result[j] = A[i];
	}
	return 0;
}
int Wake::sortA(Column& result, Column& A)
{
	if (A.size() != turbines->turbNum)
	{
		cout << "Column can not be sorted because size of A is not equal to turbine number" << endl;
		return 0;
	}
	result.resize(turbines->turbNum);
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		int j = index[i].second;
		result[i] = A[j];
	}
	return 0;
}
int Wake::sortA(Bool& result, Bool& A)
{
	if (A.size() != turbines->turbNum)
	{
		cout << "Column can not be sorted because size of A is not equal to turbine number" << endl;
		return 0;
	}
	result.resize(turbines->turbNum);
	for (int i = 0; i < turbines->turbNum; ++i)
	{
		int j = index[i].second;
		result[i] = A[j];
	}
	return 0;
}