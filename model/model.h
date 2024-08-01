#ifndef MODEL_H
#define MODEL_H
#include "turbCloud.h"
#include "matrixOperation.h"

class model
{
public:
	double kz; // ��ֱ�����ϵ�β��������
	double ky; // չ���ϵ�β��������
	double I;  // ����ǿ��
	virtual int getWake(Column& vel, TurbCloud& trubine, double& uWind) { return 0; }
};

#endif // ! MODEL_H
