#ifndef CONTOUR_H
#define CONTOUR_H
#include "turbCloud.h"
#include "wake.h"

class Contour
{
private:
	Wake* wake;
	bool is2D;
	int nI, nJ, nK;
public:
	vector<vector<vector<double>>> x;
	vector<vector<vector<double>>> y;
	vector<vector<vector<double>>> z;
	vector<vector<vector<double>>> vel;
	Contour(Wake& wake, bool is2D);
	int set_xy();
	int get_vel(Column& newGamma, Model& model);
	int output2D();
	int set_xyz();
	int output_xy();
};
#endif // !COUTOUR_H
