#ifndef TECPLOT_H
#define TECPLOT_H
#include <string>
#include <vector>
#include "matrixOperation.h"
using std::string;
using std::vector;

int getTitle(string& fileName, string& titleName, double& u, double& theta360, int p);
int getTitle(string& fileName, string& titleName, double& u, double& theta360, string& date, int p);

class Tecplot2D
{
public:
	string fileName;
	string titleName;
	vector<string> variName;//变量名
	int variNum; // 输出变量的个数
	vector<vector<vector<double>>>* x, * y;
	vector<vector<vector<vector<double>>>*> var;
	Tecplot2D(string& fileName, vector<string>& variName, string& titleName, int variNum);
	int set_xy(vector<Matrix>& x, vector<Matrix>& y);
	int set_var_i(vector<Matrix>& var_i, int i);
	int output();
};

class Tecplot3D
{
public:

};
#endif // !TECPLOT_H
