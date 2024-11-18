#include "contour.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Gauss.h"
#include <algorithm>

using std::ofstream;
using std::cout;
using std::endl;
Contour::Contour(Wake& wake, bool is2D)
{
	this->wake = &wake;
	this->is2D = is2D;
	nI = 0;
	nJ = 0;
	nK = 0;
}
int Contour::set_xy()
{
	int Cspread = 5; // 网格向外扩大factor个风机直径的大小
	int meshSize = 10; // 代表了网格尺度，一个风机直径长度内的网格数量
	double delta;
	double xmin, xmax, ymin, ymax;
	auto xmin_it = std::min_element((*wake).turbines->x0.begin(), (*wake).turbines->x0.end());
	auto xmax_it = std::max_element((*wake).turbines->x0.begin(), (*wake).turbines->x0.end());
	auto ymin_it = std::min_element((*wake).turbines->y0.begin(), (*wake).turbines->y0.end());
	auto ymax_it = std::max_element((*wake).turbines->y0.begin(), (*wake).turbines->y0.end());

	auto Dmax = std::max_element((*wake).turbines->D.begin(), (*wake).turbines->D.end());

	xmin = *xmin_it - Cspread * (*Dmax);
	xmax = *xmax_it + Cspread * (*Dmax);
	ymin = *ymin_it - Cspread * (*Dmax);
	ymax = *ymax_it + Cspread * (*Dmax);

	/*cout << xmin << endl;
	cout << xmax << endl;
	cout << xmax - xmin << endl;
	cout << ymin << endl;
	cout << ymax << endl;
	cout << ymax - ymin << endl;*/

	delta = *Dmax / meshSize;

	// cout << delta << endl;
	nI = floor((xmax - xmin) / delta);
	nJ = floor((ymax - ymin) / delta);
	nK = 1;

	/*cout << nI << endl;
	cout <<nJ<<endl;*/

	x.resize(nI);
	y.resize(nI);
	z.resize(nI);
	for (int i = 0; i < nI; ++i)
	{
		x[i].resize(nJ);
		y[i].resize(nJ);
		z[i].resize(nJ);
		for (int j = 0; j < nJ; ++j)
		{
			x[i][j].resize(1);
			y[i][j].resize(1);
			z[i][j].resize(1);
		}
	}
	for (int i = 0; i < nI; ++i)
	{
		for (int j = 0; j < nJ; ++j)
		{
			x[i][j][0] = xmin + i * delta;
			y[i][j][0] = ymin + j * delta;
			z[i][j][0] = 0;
		}
	}
	return 0;
}
// 获取每个网格点的速度
int Contour::get_vel(Column& newGamma, Model& model)
{
	vel.resize(nI);
	for (int i = 0; i < nI; ++i)
	{
		vel[i].resize(nJ);
		for (int j = 0; j < nJ; ++j)
		{
			vel[i][j].resize(nK);
			for (int k = 0; k < nK; ++k)
			{
				wake->getWake(vel[i][j][k], x[i][j][k], y[i][j][k], z[i][j][k], newGamma, model);
			}
		}
	}
	return 0;
}

int Contour::output_xy()
{
	int width = 15;
	int precision = 5;
	ofstream outFile("mesh.dat");
	if (!outFile)
	{
		std::cerr << "Cannot Open File!" << std::endl;
		return 1;
	}
	cout << nI << endl;
	cout << nJ << endl;
	cout << nK << endl;
	cout << x.size() << endl;
	cout << x[0].size() << endl;
	cout << x[0][0].size() << endl;
	outFile << "              x                     y" << endl;

	for (int i = 0; i < nI; ++i)
	{
		for (int j = 0; j < nJ; ++j)
		{
			for (int k = 0; k < nK; ++k)
			{
				outFile << std::setw(width) << std::setprecision(precision) << x[i][j][k]
					<< std::setw(width) << std::setprecision(precision) << y[i][j][k]
					<< endl;
			}
		}
	}
	return 0;
}

