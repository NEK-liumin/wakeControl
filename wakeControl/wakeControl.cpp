#include "head.h"
#include <chrono>

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	// 计算并输出偏航矩阵
	Run run;
	run.getMatrix();
	run.outputMatrix(false);

	// 统计结果
	Statistics statistics(run);
	statistics.readFile(true);
	statistics.get_all();
	statistics.write_all(true);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	cout << "程序运行时间：" << duration.count() << " 秒" << endl;
	
	std::cin.get();
	return 0;
}