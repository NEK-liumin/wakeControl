#ifndef RUN_H
#define RUN_H

#include "Blondel.h"
#include "Blondel2.h"
#include "Gauss3.h"
#include "usr.h"
#include "matrixOperation.h"

// 偏航矩阵的计算与输出
class Run
{
public:
	// 偏航矩阵
	vector<vector<vector<double>>> gamma360;
	vector<double> u, theta360;
	Column uFull, thetaFull;
	// 每个风速和风向下，风场偏航前后的总发电功率，以及偏航后总发电功率的增加量
	Matrix P0, P, deltaP;
	// 每个风速和风向下，每台风机偏航前后的发电功率，以及偏航后发电功率的增加量
	vector<Matrix> P0_i, P_i, deltaP_i;
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
	// 默认的最大偏航角
	double maxGamma = 30;
	// 默认的尾流模型
	Blondel2 model = Blondel2(ky, kz, I);
	// Gauss3 model = Gauss3(ky, kz, I);
	// 默认的初始随机范围（-randomRange,randomRange）
	double randomRange = 1.0;
	// 当得到偏航角绝对值小于smallGamma，取0
	double smallGamma = 0;
	// 
	Bool m_isWork;
	// 默认的优化问题
	Yaw yaw = Yaw(uBegin, thetaBegin, m_isWork, rho, model, maxGamma, randomRange);
	// 以下用于矩阵输出
	double uIn = 0;
	double uOut = yaw.simulation.turbines.uMax;

	Run();
	Run(double& uBegin, double& uEnd, double& deltaU, double& deltaTheta);

	// 读取参数文件
	int readParameter();
	// 计算偏航矩阵
	int getMatrix();

	// 输出偏航矩阵，并标注风速和风向
	int outputMatrix(bool isTranspose);

	// 输出绝对偏航矩阵，并标注风速和风向
	int outputAbsMatrix(bool isTranspose);

	int extendMatrix(Matrix& result, Matrix& source, Column& resulti, Column& resultj, Column& sourcei, Column& sourcej);
};

#endif // !MAINLOOP_H