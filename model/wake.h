#ifndef WAKE_H
#define WAKE_H
#include "turbCloud.h"

class Wake
{
public:
	TurbCloud* turbCloud;

 // 坐标转换后所有风机所有信息 position of wind turbine
	TurbCloud newCloud;

	Column vel; // 每个风机处的速度
	double x, y, z; // 求该处的尾流损失 wake deflection here
	double u, theta; // 来流风速大小及方向
	double ky, kz; // 
	double gamma; // 风机偏航角 yaw angle

	int getNewCloud();
	virtual int getWake() { return 0; };

};
#endif // !WAKE_H
