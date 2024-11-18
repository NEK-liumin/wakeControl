#include "figure.h"
#include "solver.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <tecplot.h>

using std::string;

int Figure::getGamma(Column& gamma360, double& u, double& theta360)
{
	Bool isWork;
	isWork.resize(run.yaw.size_x);
	for (int i = 0; i < run.yaw.size_x; ++i)
	{
		isWork[i] = '1';
	}
	run.yaw.reset(u, theta360, isWork, run.rho, run.model, run.maxGamma, run.randomRange);
	SQPIC_solver(run.yaw, run.tol);
	run.yaw.outputGamma(gamma360);
	return 0;
}

int Figure::outputContour()
{
	int p = 1; // 名称保留小数点位数
	std::filesystem::path outputPath = "output";
	std::filesystem::path tecfile = "tec";
	std::filesystem::path fullPath;

	fullPath = outputPath / tecfile;

	string fileName;
	string titleName;

	Bool isWork;
	isWork.resize(run.yaw.size_x);
	for (int i = 0; i < run.yaw.size_x; ++i)
	{
		isWork[i] = '1';
	}

	if (!std::filesystem::exists(outputPath))
	{
		std::filesystem::create_directory(outputPath);
	}

	if (!std::filesystem::exists(fullPath))
	{
		std::filesystem::create_directory(fullPath);
	}

	cout << "0: only 1 contour is output" << endl;
	cout << "1: all contours are output" << endl;
	cout << "other: no contour is output" << endl;
	cout << endl;
	cout << "Please enter a selection:" << endl;
	std::cin >> option;

	if (option != 0 && option != 1)
	{
		return 0;
	}

	if (option == 0)
	{
		double u, theta360;
		Column gamma360;
		cout << "Please Enter Wind Speed" << endl;
		std::cin >> u;
		cout << "Please Enter Wind Direction" << endl;
		std::cin >> theta360;

		getGamma(gamma360, u, theta360);

		getTitle(fileName, titleName, u, theta360, p);

		fullPath = outputPath / tecfile / fileName;
		string fullPathName = fullPath.string();
		cout << fullPath.string() << " is output, please wait..." << endl;

		simulation.plot(u, theta360, isWork, run.model, gamma360, fullPathName, titleName);

		cout << endl;
		cout << "Done!" << endl;
		return 0;
	}
	if (option == 1)
	{
		double u, theta360;
		cout << "It will take a long time to output all contours, please wait..." << endl;
		int nLoopU = floor((run.uEnd - run.uBegin) / run.deltaU) + 1;
		int nLoopTheta = floor((run.thetaEnd - run.thetaBegin) / run.deltaTheta) + 1;
		int nLoop = nLoopU * nLoopTheta;
		int count = 0;

		gamma360.resize(nLoopU);

		for (int i = 0; i < nLoopU; ++i)
		{
			gamma360[i].resize(nLoopTheta);

			for (int j = 0; j < nLoopTheta; ++j)
			{
				gamma360[i][j].resize(run.yaw.size_x);
			}
		}
		for (int i = 0; i < nLoopU; ++i)
		{
			for (int j = 0; j < nLoopTheta; ++j)
			{
				u = run.uBegin + i * run.deltaU;
				theta360 = run.thetaBegin + j * run.deltaTheta;

				getGamma(gamma360[i][j], u, theta360);
				getTitle(fileName, titleName, u, theta360, p);
				fullPath = outputPath / tecfile / fileName;
				string fullPathName = fullPath.string();

				simulation.plot(u, theta360, isWork, run.model, gamma360[i][j], fullPathName, titleName);

				count++;
				cout << "\rProgress: " << std::setw(4) << count << "/" << nLoop << "(" << std::setw(6) << std::setprecision(3) << count * 1.0 / nLoop * 100 << "%)";
				cout.flush();
			}
		}
		cout << endl;
		cout << "Done!" << endl;
		return 0;
	}

	return 0;
}