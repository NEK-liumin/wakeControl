#include "tecplot.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "matrixOperation.h"
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

static int writeLoop2D(ofstream& outFile, vector<Matrix>& v)
{
	int width = 30;
	int precision = 20;
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

Tecplot2D::Tecplot2D(string& fileName, vector<string>& variName, string& titleName, int variNum)
{
	this->fileName = fileName;
	this->variName = variName;
	this->titleName = titleName;
	this->variNum = variNum;
	this->var.resize(variNum);
	x = nullptr;
	y = nullptr;
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

	if (!outFile.is_open())
	{
		std::cerr << "Cannot Open File " << fileName << "!" << std::endl;
		return 1;
	}
	if (variNum == 0)
	{
		outFile << "VARIABLES=\"x\", \"y\"" << endl;
		outFile << "ZONE T = \"" << titleName << "\"," << " I = " << nI << ", J = " << nJ << " F = BLOCK" << endl;
		writeLoop2D(outFile, *x);
		writeLoop2D(outFile, *y);
		outFile.close();
		return 0;
	}
	else
	{
		outFile << "VARIABLES=\"x\", \"y\"" << endl;
		for (int i = 0; i < variNum; ++i)
		{
			outFile << ", \"" << variName[i] << "\"" << endl;
		}
		outFile << "ZONE T = \"" << titleName << "\"," << " I = " << nI << ", J = " << nJ << " F = BLOCK" << endl;
		writeLoop2D(outFile, *x);
		writeLoop2D(outFile, *y);
		for (int i = 0; i < variNum; ++i)
		{
			writeLoop2D(outFile, *(var[i]));
		}
		outFile.close();
		return 0;
	}
	return 0;
}

int getTitle(string& fileName, string& titleName, double& u, double& theta360, int p)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(p) << u << "_" << std::fixed << std::setprecision(p) << theta360;
	titleName = oss.str();
	oss << ".dat";
	fileName = oss.str();
	return 0;
}
int getTitle(string& fileName, string& titleName, double& u, double& theta360, string& date, int p)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(p) << u << "_" << std::fixed << std::setprecision(p) << theta360;
	titleName = oss.str();
	oss << "_" << date << ".dat";
	fileName = oss.str();
	return 0;
}