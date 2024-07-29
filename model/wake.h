#ifndef WAKE_H
#define WAKE_H
#include "turbCloud.h"

class Wake
{
public:
	TurbCloud* turbCloud;

 // ����ת�������з��������Ϣ position of wind turbine
	TurbCloud newCloud;

	double x, y, z; // ��ô���β����ʧ wake deflection here
	double u, theta; // �������ٴ�С������
	double ky, kz; // 
	double gamma; // ���ƫ���� yaw angle

	int getNewCloud();

};
#endif // !WAKE_H
