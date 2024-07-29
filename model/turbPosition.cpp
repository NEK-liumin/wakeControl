#include "turbPosition.h"
using std::min_element;
#include <algorithm>
#include <iostream>
using::std::cout;
// ���ڸ������������򣬷���һ�㲻�Ƿ����x����
// Ϊ�˷���β�����㣬���������ϵ���е���
// ʹ�÷��ٵĵ�λ����Ϊ(1,0,0)
// �����x��������x������
// ���⣬��ԭ�����x,y,z���������ϵ��������Сֵ��Ϊ0��
// ���Է��������ƽ��
// ʹ�÷��������������ֵ��С�����ڼ�С���
// ����ϵ��ת��������ƽ�ƺ���ת�ķ�ʽ����
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