#include "run.h"

#include "iostream"
#include "head.h"
#include <chrono>
#include <fstream>
#include <filesystem>

#include <sstream>
#include <string>

Run::Run()
{
	readParameter();
}
Run::Run(double& uBegin, double& uEnd, double& deltaU, double& deltaTheta)
{
	this->uBegin = uBegin;
	this->uEnd = uEnd;
	this->deltaU = deltaU;
	this->deltaTheta = deltaTheta;
}

int Run::readParameter()
{
	std::ifstream file("parameter.csv");
	if (!file.is_open())
	{
		std::cerr << "parameter.csv is not found and default parameters are used!" << std::endl;
		return 1;
	}
	std::string line;

	std::getline(file, line); // 第一行跳过

	int lineCount = 0;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		double firstNumber;
		if (ss >> firstNumber)
		{
			lineCount++;
			switch (lineCount)
			{
			case 1:
				uBegin = firstNumber;
				break;
			case 2:
				uEnd = firstNumber;
				break;
			case 3:
				thetaBegin = firstNumber;
				break;
			case 4:
				thetaEnd = firstNumber;
				break;
			case 5:
				deltaU = firstNumber;
				break;
			case 6:
				deltaTheta = firstNumber;
				break;
			case 7:
				ky = firstNumber;
				break;
			case 8:
				kz = firstNumber;
				break;
			case 9:
				I = firstNumber;
				break;
			case 10:
				rho = firstNumber;
				break;
			case 11:
				tol = firstNumber;
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

int Run::getMatrix()
{
	double smallGamma = 2; // 当得到偏航角绝对值小于该值，取0
	cout << "The wind turbine yaw matrix is being calculated..." << endl;
	int nLoopU = floor((uEnd - uBegin) / deltaU) + 1;
	int nLoopTheta = floor((thetaEnd - thetaBegin) / deltaTheta) + 1;
	int nLoop = nLoopU * nLoopTheta;
	int count = 0;

	gamma360.resize(nLoopU);
	P0.resize(nLoopU);
	P.resize(nLoopU);
	deltaP.resize(nLoopU);

	for (int i = 0; i < nLoopU; ++i)
	{
		gamma360[i].resize(nLoopTheta);

		P0[i].resize(nLoopTheta);
		P[i].resize(nLoopTheta);
		deltaP[i].resize(nLoopTheta);

		for (int j = 0; j < nLoopTheta; ++j)
		{
			gamma360[i][j].resize(yaw.size_x);
		}
	}
	for (int i = 0; i < nLoopU; ++i)
	{
		for (int j = 0; j < nLoopTheta; ++j)
		{
			u = uBegin + i * deltaU;
			theta360 = thetaBegin + j * deltaTheta;

			yaw.reset(u, theta360, rho, model);
			SQPIC_solver(yaw, tol);

			yaw.outputGamma(gamma360[i][j]);

			for (int k = 0; k < yaw.size_x; ++k)
			{
				if (abs(gamma360[i][j][k]) < smallGamma)
				{
					gamma360[i][j][k] = 0;
				}
			}

			P0[i][j] = yaw.initialPower();
			P[i][j] = yaw.power();
			deltaP[i][j] = yaw.power() - yaw.initialPower();

			count++;
			cout << "\rProgress: " << std::setw(4) << count << "/" << nLoop << "(" << std::setw(6) << std::setprecision(3) << count * 1.0 / nLoop * 100 << "%)";
			cout.flush();
		}
	}
	cout << endl;
	return 0;
}

int Run::outputMatrix()
{
	double theta_i = 0, u_i = 0;
	std::filesystem::path outputDir("output");
	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}
	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	std::string P0name = (outputDir / "P0.csv").string();
	std::string Pname = (outputDir / "P.csv").string();
	std::string deltaPname = (outputDir / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (std::to_string(i + 1) + ".csv")).string();
		std::ofstream file(filename);
		if (file.is_open())
		{
			file << "U \\ Theta,";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				theta_i = k * deltaTheta;
				file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
				if (k < nLoopTheta - 1)
				{
					file << ",";
				}
			}
			file << std::endl;
			for (int j = 0; j < nLoopU; ++j)
			{
				u_i = uBegin + deltaU * j;
				file << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
				for (int k = 0; k < nLoopTheta; ++k)
				{
					file << std::setw(6) << std::setprecision(2) << std::fixed << gamma360[j][k][i];
					if (k < nLoopTheta - 1)
					{
						file << ",";
					}
				}
				file << std::endl;
			}
			std::cout << "Turbine " << i + 1 << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}

	if (P0file.is_open())
	{
		P0file << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				P0file << ",";
			}
		}
		P0file << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				P0file << std::setw(6) << std::setprecision(2) << std::fixed << P0[j][k];
				if (k < nLoopTheta - 1)
				{
					P0file << ",";
				}
			}
			P0file << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		Pfile << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				Pfile << ",";
			}
		}
		Pfile << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				Pfile << std::setw(6) << std::setprecision(2) << std::fixed << P[j][k];
				if (k < nLoopTheta - 1)
				{
					Pfile << ",";
				}
			}
			Pfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		deltaPfile << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				deltaPfile << ",";
			}
		}
		deltaPfile << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << deltaP[j][k];
				if (k < nLoopTheta - 1)
				{
					deltaPfile << ",";
				}
			}
			deltaPfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();

	return 0;
}

int Run::outputAbsMatrix()
{
	double theta_i = 0, u_i = 0;
	double AbsGamma = 0; //风机的绝对偏转角度。风向为0，且正对风时，偏航角度为0
	std::filesystem::path outputDir("output");
	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}
	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	std::string P0name = (outputDir / "P0.csv").string();
	std::string Pname = (outputDir / "P.csv").string();
	std::string deltaPname = (outputDir / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (std::to_string(i + 1) + ".csv")).string();
		std::ofstream file(filename);
		if (file.is_open())
		{
			file << "U \\ Theta,";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				theta_i = k * deltaTheta;
				file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
				if (k < nLoopTheta - 1)
				{
					file << ",";
				}
			}
			file << std::endl;
			for (int j = 0; j < nLoopU; ++j)
			{
				u_i = uBegin + deltaU * j;
				file << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
				for (int k = 0; k < nLoopTheta; ++k)
				{
					theta_i = k * deltaTheta;
					AbsGamma = theta_i + gamma360[j][k][i];
					file << std::setw(6) << std::setprecision(2) << std::fixed << AbsGamma;
					if (k < nLoopTheta - 1)
					{
						file << ",";
					}
				}
				file << std::endl;
			}
			std::cout << "Turbine " << i + 1 << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}

	if (P0file.is_open())
	{
		P0file << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				P0file << ",";
			}
		}
		P0file << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				P0file << std::setw(6) << std::setprecision(2) << std::fixed << P0[j][k];
				if (k < nLoopTheta - 1)
				{
					P0file << ",";
				}
			}
			P0file << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		Pfile << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				Pfile << ",";
			}
		}
		Pfile << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				Pfile << std::setw(6) << std::setprecision(2) << std::fixed << P[j][k];
				if (k < nLoopTheta - 1)
				{
					Pfile << ",";
				}
			}
			Pfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		deltaPfile << "U \\ Theta,";
		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = k * deltaTheta;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i;
			if (k < nLoopTheta - 1)
			{
				deltaPfile << ",";
			}
		}
		deltaPfile << std::endl;
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + deltaU * j;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i << ",";
			for (int k = 0; k < nLoopTheta; ++k)
			{
				deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << deltaP[j][k];
				if (k < nLoopTheta - 1)
				{
					deltaPfile << ",";
				}
			}
			deltaPfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();

	return 0;
}

int Run::outputMatrixT()
{
	double theta_i = 0, u_i = 0;
	std::filesystem::path outputDir("output");
	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}
	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	std::string P0name = (outputDir / "P0.csv").string();
	std::string Pname = (outputDir / "P.csv").string();
	std::string deltaPname = (outputDir / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	// 将偏航矩阵写入文件
	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (std::to_string(i + 1) + ".csv")).string();
		std::ofstream file(filename);

		if (file.is_open())
		{
			file << "Theta \\ U,";
			for (int j = 0; j < nLoopU; ++j)
			{
				u_i = uBegin + j * deltaU;
				file << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
				if (j < nLoopU - 1)
				{
					file << ",";
				}
			}
			file << std::endl;
			for (int k = 0; k < nLoopTheta; ++k)
			{
				theta_i = deltaTheta * k;
				file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
				for (int j = 0; j < nLoopU; ++j)
				{
					file << std::setw(6) << std::setprecision(2) << std::fixed << gamma360[j][k][i];
					if (j < nLoopU - 1)
					{
						file << ",";
					}
				}
				file << std::endl;
			}
			std::cout << "Turbine " << i + 1 << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}

	if (P0file.is_open())
	{
		P0file << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				P0file << ",";
			}
		}
		P0file << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				P0file << std::setw(6) << std::setprecision(2) << std::fixed << P0[j][k];
				if (j < nLoopU - 1)
				{
					P0file << ",";
				}
			}
			P0file << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		Pfile << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				Pfile << ",";
			}
		}
		Pfile << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				Pfile << std::setw(6) << std::setprecision(2) << std::fixed << P[j][k];
				if (j < nLoopU - 1)
				{
					Pfile << ",";
				}
			}
			Pfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		deltaPfile << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				deltaPfile << ",";
			}
		}
		deltaPfile << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << deltaP[j][k];
				if (j < nLoopU - 1)
				{
					deltaPfile << ",";
				}
			}
			deltaPfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();

	return 0;
}

int Run::outputAbsMatrixT()
{
	double theta_i = 0, u_i = 0;
	double absGamma = 0;
	std::filesystem::path outputDir("output");
	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}
	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	std::string P0name = (outputDir / "P0.csv").string();
	std::string Pname = (outputDir / "P.csv").string();
	std::string deltaPname = (outputDir / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	// 将偏航矩阵写入文件
	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (std::to_string(i + 1) + ".csv")).string();
		std::ofstream file(filename);

		if (file.is_open())
		{
			file << "Theta \\ U,";
			for (int j = 0; j < nLoopU; ++j)
			{
				u_i = uBegin + j * deltaU;
				file << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
				if (j < nLoopU - 1)
				{
					file << ",";
				}
			}
			file << std::endl;
			for (int k = 0; k < nLoopTheta; ++k)
			{
				theta_i = deltaTheta * k;
				file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
				for (int j = 0; j < nLoopU; ++j)
				{
					absGamma = theta_i + gamma360[j][k][i];
					file << std::setw(6) << std::setprecision(2) << std::fixed << absGamma;
					if (j < nLoopU - 1)
					{
						file << ",";
					}
				}
				file << std::endl;
			}
			std::cout << "Turbine " << i + 1 << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}

	if (P0file.is_open())
	{
		P0file << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				P0file << ",";
			}
		}
		P0file << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			P0file << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				P0file << std::setw(6) << std::setprecision(2) << std::fixed << P0[j][k];
				if (j < nLoopU - 1)
				{
					P0file << ",";
				}
			}
			P0file << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		Pfile << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				Pfile << ",";
			}
		}
		Pfile << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			Pfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				Pfile << std::setw(6) << std::setprecision(2) << std::fixed << P[j][k];
				if (j < nLoopU - 1)
				{
					Pfile << ",";
				}
			}
			Pfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		deltaPfile << "Theta \\ U,";
		for (int j = 0; j < nLoopU; ++j)
		{
			u_i = uBegin + j * deltaU;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << u_i;
			if (j < nLoopU - 1)
			{
				deltaPfile << ",";
			}
		}
		deltaPfile << std::endl;

		for (int k = 0; k < nLoopTheta; ++k)
		{
			theta_i = deltaTheta * k;
			deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << theta_i << ",";
			for (int j = 0; j < nLoopU; ++j)
			{
				deltaPfile << std::setw(6) << std::setprecision(2) << std::fixed << deltaP[j][k];
				if (j < nLoopU - 1)
				{
					deltaPfile << ",";
				}
			}
			deltaPfile << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();

	return 0;
}