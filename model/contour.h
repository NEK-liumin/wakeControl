#ifndef CONTOUR_H
#define CONTOUR_H
#include "turbCloud.h"
class Contour
{
private:
	TurbCloud* turbines;
	bool is2D;
	int nI, nJ, nK;
public:
	vector<vector<vector<double>>> x;
	vector<vector<vector<double>>> y;
	vector<vector<vector<double>>> z;
	Contour(TurbCloud& turbines, bool is2D);
	int set_xy();
	int set_xyz();
	int output_xy();
};
#endif // !COUTOUR_H
