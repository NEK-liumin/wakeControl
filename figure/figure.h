#ifndef FIGURE_H
#define FIGURE_H

#include "run.h"

// 风场尾流云图的计算与输出
class Figure
{
public:

	// 0:只输出一张云图
	// 1:输出全部云图
	// other:不输出云图
	int option = 0;

	vector<vector<vector<double>>> gamma360;
	
	Run run;

	Simulation simulation;

	Figure() {};

	// 输出云图
	int outputContour();
};
#endif // !FIGURE_H
