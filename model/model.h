#ifndef MODEL_H
#define MODEL_H
#include "turbCloud.h"
#include "matrixOperation.h"

class model
{
public:
	double kz; // 垂直方向上的尾流膨胀率
	double ky; // 展向上的尾流膨胀率
	double I;  // 湍流强度
	virtual int getWake(Column& vel, TurbCloud& trubine, double& uWind) { return 0; }
};

#endif // ! MODEL_H
