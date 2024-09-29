#ifndef RUN_H
#define RUN_H

#include "Blondel.h"
#include "usr.h"
#include "matrixOperation.h"

class Run
{
public:
	// 偏航矩阵
	vector<vector<vector<double>>> gamma360;
	// 每个风速和风向下，风场偏航前后的总发电功率，以及偏航后总发电功率的增加量
	Matrix P0, P, deltaP;
	// 只计算此区间的偏航矩阵
	double uBegin = 5, uEnd = 12; 
	double thetaBegin = 0, thetaEnd = 350;
	// 风速间隔默认1m/s，角度间隔默认10度
	double deltaU = 1.0, deltaTheta = 10; 
	// 默认的尾流宽度增长率
	double ky = 0.025;
	double kz = 0.0175;
	// 默认的湍流强度
	double I = 0.12;
	// 默认的密度（由于使用了功率曲线，所以不起作用）
	double rho = 1.225;
	// 默认的优化容差
	double tol = 1e-5;
	// 默认的风速和风向（后缀360表示角度制）
	double u = uBegin, theta360 = 0;
	// 默认的尾流模型
	Blondel model = Blondel(ky, kz, I);
	// 默认的优化问题
	Yaw yaw = Yaw(u, theta360, rho, model);

	Run();
	Run(double& uBegin, double& uEnd, double& deltaU, double& deltaTheta);

	// 读取参数文件
	int readParameter();
	// 计算偏航矩阵
	int getMatrix();

	// 输出偏航矩阵，并标注风速和风向
	int outputMatrix();

	// 输出绝对偏航矩阵，并标注风速和风向
	int outputAbsMatrix();

	// 输出转置后的偏航矩阵，并标注风速和方向
	int outputMatrixT();

	// 输出转置后的绝对偏航矩阵，并标注风速和风向
	int outputAbsMatrixT();
};

#endif // !MAINLOOP_H