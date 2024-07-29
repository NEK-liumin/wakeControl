#ifndef TURBPOSITION_H
#define TURBPOSITION_H

#include "matrixOperation.h"

class Turbxyz
{
public:
	int turbNum;
	// 风机原坐标
	Column x0;
	Column y0;
	Column z0;

// 根据风向进行坐标转换后的坐标

	Column x;
	Column y;
	Column z;

	// 默认只有一台风机
	Turbxyz()
	{
		turbNum = 1;
		getZeroColumn(x0, 1);
		getZeroColumn(y0, 1);
		getZeroColumn(z0,1);
		getZeroColumn(x, 1);
		getZeroColumn(y, 1);
		getZeroColumn(z, 1);
	}

	Turbxyz(int turbNum)
	{
		this->turbNum = turbNum;
		getZeroColumn(x0, turbNum);
		getZeroColumn(y0, turbNum);
		getZeroColumn(z0, turbNum);
		getZeroColumn(x, turbNum);
		getZeroColumn(y, turbNum);
		getZeroColumn(z, turbNum);
	}

	void init(int turbNum, Column& x0, Column& y0, Column& z0)
	{
		this->turbNum = turbNum;
		this->x0 = x0;
		this->y0 = y0;
		this->z0 = z0;
		this->x = x0;
		this->y = y0;
		this->z = z0;
	}
	

	// x0 -> x
	// y0 -> y
	// z0 -> z
	void setNewPosi(double theta);
};
#endif // !TURBPOSITION_H
