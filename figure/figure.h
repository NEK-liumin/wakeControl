#ifndef FIGURE_H
#define FIGURE_H

#include "run.h"

// �糡β����ͼ�ļ��������
class Figure
{
public:

	// 0:ֻ���һ����ͼ
	// 1:���ȫ����ͼ
	// other:�������ͼ
	int option = 0;

	vector<vector<vector<double>>> gamma360;
	
	Run run;

	Simulation simulation;

	Figure() {};

	// �����ͼ
	int outputContour();
};
#endif // !FIGURE_H
