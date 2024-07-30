#ifndef WAKE_H
#define WAKE_H
#include "turbCloud.h"

class Wake
{
public:
	TurbCloud* turbCloud;

 // ����ת�������з��������Ϣ position of wind turbine
	TurbCloud newCloud;

	Column vel; // ÿ����������ٶ�
	double x, y, z; // ��ô���β����ʧ wake deflection here
	double u, theta; // �������ٴ�С������
	double ky, kz; // 
	double gamma; // ���ƫ���� yaw angle

	int getNewCloud();
	virtual int getWake() { return 0; };

};
#endif // !WAKE_H
