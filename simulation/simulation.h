#ifndef SIMULATION_H
#define SIMULATION_H
#include "turbCloud.h"
#include "input.h"
#include "model.h"
#include "wake.h"
#include "contour.h"
using std::string;

// 用于计算尾流和求发电功率。也可以作图
class Simulation
{
public:
	TurbCloud turbines = TurbCloud(1, 1, 10, 0, 20);
	Input input;
	Wake wake;
	Model* model; //析构后不释放指向的地址。因为地址从外部传入
	// turbinesInfo.csv is needed
	// input wind velocity, wind theta, yaw angles, model and isPlot
	Simulation();
	int reset(double& wind, double& theta360, Bool& isWork, Model& model);
	// 对风机偏航角的上下限、初始随机范围进行排序
	int sortColumn(Column& result, Column& A);
	int sortColumn(Bool& result, Bool& A);
	// 作图。输入的角度为角度制，(风速、风向、尾流模型、偏航角、文件名、域名)
	int plot(double& wind, double& theta360, Bool& isWork, Model& model, Column& gamma360, string& fileName, string& titleName);
	// 只求尾流，不画图。且输入的gamma为弧度制，且逆时针为正方向
	// 在求最优角度过程中被调用,且返回总发电功率
	int run(double& f, Column& gamma);
	//  只求尾流，不画图。且输入的gamma为弧度制，且逆时针为正方向
	// 返回每台风机的发电功率，用于统计
	int run(Column& f_i, Column& gamma);
	// 假设没有尾流影响，计算总的发电功率
	int hypotheticalRun(double& fHypothetical);
	// 假设没有尾流影响，计算每台风机的发电功率
	int hypotheticalRun(Column& f_iHypothetical);
private:
	// 只求尾流，不画图。且输入的gamma为弧度制，且逆时针为正方向
	int run(Column& gamma);
	// 为了作图，输入的角度为角度制，且逆时针为正方向。输入字符串作为云图名称
	int run(Column& gamma, bool isPlot, string& fileName, string& titleName);
};
#endif // !SIMULATION_H
