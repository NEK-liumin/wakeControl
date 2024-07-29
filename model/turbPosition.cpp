#include "turbPosition.h"
using std::min_element;
#include <algorithm>
#include <iostream>
using::std::cout;
// 对于给定的来流风向，风向一般不是风机的x方向
// 为了方便尾流计算，将风机坐标系进行调整
// 使得风速的单位向量为(1,0,0)
// 即风从x负方向吹向x正方向
// 另外，将原风机的x,y,z三个方向上的坐标的最小值设为0点
// 即对风机进行了平移
// 使得风机坐标的坐标绝对值更小，便于减小误差
// 坐标系的转换采用先平移后旋转的方式进行
void Turbxyz::setNewPosi(double theta)
{
	auto ox = min_element(x0.begin(), x0.end());
	auto oy = min_element(y0.begin(), y0.end());
	auto oz = min_element(z0.begin(), z0.end());

	for (int i = 0; i < turbNum; ++i)
	{
		double xTemp, yTemp;
		xTemp = x0[i] - *ox;
		yTemp = y0[i] - *oy;

		x[i] =  xTemp * cos(theta) + yTemp * sin(theta);
		y[i] = -xTemp * sin(theta) + yTemp * cos(theta);
		z[i] = z0[i] - *oz;
	}
}