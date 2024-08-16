#include "contour.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using std::ofstream;
using std::cout;
using std::endl;
Contour::Contour(TurbCloud& turbines, bool is2D)
{
	this->turbines = &turbines;
	this->is2D = is2D;
}
int Contour::set_xy()
{
	int out = 5; // 网格向外扩大factor个风机直径的大小
	int meshSize = 10; // 代表了网格尺度，一个风机直径长度内的网格数量
	double delta;
	double xmin, xmax, ymin, ymax;
	xmin = std::min(turbines->x0);
	xmax = std::max(turbines->x0);
	ymin = std::min(turbines->y0);
	ymax = std::max(turbines->y0);

	xmin = xmin - factor * turbines->D;
	xmax = xmax + factor * turbines->D;
	ymin = ymin - factor * turbines->D;
	ymax = ymax + factor * turbines->D;

	delta = turbines->D / meshSize;
	nI = floor((xmax - xmin) / delta);
	nJ = floor((ymax - ymin) / delta);
	nK = 1;

	x.resize(nI);
	y.resize(nI);
	for (int i = 0; i < nI; ++i)
	{
		x[i].resize(nJ);
		y[i].resize(nJ);
		for (j = 0; j < nJ; ++j)
		{
			x[i][j].resize(1);
			y[i][j].resize(1);
		}
	}
	for (int i = 0; i < nI; ++i)
	{
		for (int j = 0; j < nJ; ++j)
		{
			x[i][j][0] = xmin + i * delta;
			y[i][j][0] = ymin + j * delta;
		}
	}
}

int Contour::output_xy()
{
	ofstream outFile("mesh.dat");
	if (!outFile)
	{
		std::cerr << "Cannot Open File!" << std::endl;
		return 1;
	}
	outFile << "     x          y          z" << endl;
	for (int i = 0; i < nI; ++i)
	{
		for (int j = 0; j < nJ; ++j)
		{
			for (int k = 0; i < nK; ++k)
			{
				outFile << x[i][j][k] << "     " << y[i][j][k] << endl;
			}
		}
	}
	return 0;
}