#include "figure.h"
#include "solver.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>

using std::string;

int Figure::outputContour()
{
	int p = 1; // 名称保留小数点位数
	std::filesystem::path fileName;
	std::filesystem::path outputPath = "output";
	std::filesystem::path tecfile = "tec";
	std::filesystem::path fullPath;

	fullPath = outputPath / tecfile;

	// string fullPathName = fullPath.string();

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
		Simulation simulation(run.uBegin, run.thetaBegin, run.model);
		Column gamma360;
		std::ostringstream oss;
		cout << "Please Enter Wind Speed" << endl;
		std::cin >> u;
		cout << "Please Enter Wind Direction" << endl;
		std::cin >> theta360;
		run.yaw.reset(u, theta360, run.rho, run.model, run.randomRange);

		SQPIC_solver(run.yaw, run.tol);
		run.yaw.outputGamma(gamma360);

		oss << std::fixed << std::setprecision(p) << u << "_" << std::fixed << std::setprecision(p) << theta360 << ".dat";
		fileName = oss.str();
		fullPath = outputPath / tecfile / fileName;
		string fullPathName = fullPath.string();
		cout << fullPath.string() << " is output, please wait..." << endl;

		simulation.setAll(u, theta360, run.model);
		simulation.run(gamma360, true, fullPathName);
		cout << endl;
		cout << "Done!" << endl;
		return 0;
	}
	if (option == 1)
	{
		double u, theta360;
		double smallGamma = 2;
		Simulation simulation(run.uBegin, run.thetaBegin, run.model);
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

				run.yaw.reset(u, theta360, run.rho, run.model, run.randomRange);
				SQPIC_solver(run.yaw, run.tol);

				run.yaw.outputGamma(gamma360[i][j]);

				for (int k = 0; k < run.yaw.size_x; ++k)
				{
					if (abs(gamma360[i][j][k]) < smallGamma)
					{
						gamma360[i][j][k] = 0;
					}
				}
				std::ostringstream oss;
				oss << std::fixed << std::setprecision(p) << u << "_" << std::fixed << std::setprecision(p) << theta360 << ".dat";
				fileName = oss.str();
				fullPath = outputPath / tecfile / fileName;
				string fullPathName = fullPath.string();
				simulation.setAll(u, theta360, run.model);
				simulation.run(gamma360[i][j], true, fullPathName);// 改名字

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