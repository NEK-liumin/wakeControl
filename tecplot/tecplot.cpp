#include "tecplot.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "matrixOperation.h"
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

static int writeLoop2D(ofstream& outFile, vector<Matrix>& v)
{
	int width = 15;
	int precision = 5;
	int n = 0;
	int nI = v.size();
	int nJ = v[0].size();
	for (int j = 0; j < nJ; ++j)
	{
		for (int i = 0; i < nI; ++i)
		{
			outFile << std::setw(width) << std::setprecision(precision) << v[i][j][0] << " ";
			n++;
			if (n % 4 == 0)
			{
				outFile << endl;
			}
		}
	}
	return 0;
}

Tecplot2D::Tecplot2D(string& fileName, vector<string>& variName, int variNum)
{
	this->fileName = fileName;
	this->variName = variName;
	this->variNum = variNum;
	this->var.resize(variNum);
}

int Tecplot2D::set_xy(vector<Matrix>& x, vector<Matrix>& y)
{
	this->x = &x;
	this->y = &y;
	return 0;
}

int Tecplot2D::set_var_i(vector<Matrix>& var_i, int i)
{
	this->var[i] = &var_i;
	return 0;
}

int Tecplot2D::output()
{
	int nI = (*x).size();
	int nJ = (*x)[0].size();
	ofstream outFile(fileName);

	if (!outFile)
	{
		std::cerr << "Cannot Open File!" << std::endl;
		return 1;
	}
	if (variNum == 0)
	{
		outFile << "VARIABLES=\"x\", \"y\"" << endl;
		outFile << "ZONE I=" << nI << ",J=" << nJ << " F=BLOCK" << endl;
		writeLoop2D(outFile, *x);
		writeLoop2D(outFile, *y);
		return 0;
	}
	else
	{
		outFile << "VARIABLES=\"x\", \"y\"" << endl;
		for (int i = 0; i < variNum; ++i)
		{
			outFile << "," << variName[i] << endl;
		}
		outFile << "ZONE I=" << nI << ",J=" << nJ << " F=BLOCK" << endl;
		writeLoop2D(outFile, *x);
		writeLoop2D(outFile, *y);
		for (int i = 0; i < variNum; ++i)
		{
			writeLoop2D(outFile, *(var[i]));
		}
		return 0;
	}
	return 0;
}