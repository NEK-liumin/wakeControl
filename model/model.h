#ifndef MODEL_H
#define MODEL_H
#include "turbCloud.h"
#include "matrixOperation.h"
#include <iostream>

class Model
{
public:
	double kz; // 垂直方向上的尾流膨胀率
	double ky; // 展向上的尾流膨胀率
	double I;  // 湍流强度
	int reset(double& ky, double& kz, double& I)
	{
		this->ky = ky;
		this->kz = kz;
		this->I = I;
		return 0;
	}
	virtual int getWake(Column& vel, TurbCloud& trubine, double& uWind) { return 0; }
	// 仅用于作图
	virtual int getWake(Column& vel, TurbCloud& trubine, Column& gamma, double& uWind) { return 0; }
	virtual int getWake(double& meshVel, Column& turbVel, TurbCloud& turbs, Column& gamma, double& uWind, double& x, double& y, double& z) { return 0; }
};

#endif // ! MODEL_H
