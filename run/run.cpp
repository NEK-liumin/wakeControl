#include "run.h"

#include "iostream"
#include "head.h"
#include <chrono>
#include <fstream>
#include <filesystem>

#include <sstream>
#include <string>
#include "excel.h"

Run::Run()
{
	readParameter();
	model.reset(ky, kz, I);
	yaw.reset(uBegin, thetaBegin, m_isWork, rho, model, maxGamma, randomRange);
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
	std::ifstream file("input/parameter.csv");
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
			case 12:
				maxGamma = firstNumber;
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
	cout << "The wind turbine yaw matrix is being calculated..." << endl;
	int nLoopU = floor((uEnd - uBegin) / deltaU) + 1;
	int nLoopTheta = floor((thetaEnd - thetaBegin) / deltaTheta) + 1;
	int nLoop = nLoopU * nLoopTheta;
	int count = 0;
	Bool isWork;


	gamma360.resize(nLoopU);
	P0.resize(nLoopU);
	P.resize(nLoopU);
	deltaP.resize(nLoopU);
	u.resize(nLoopU);

	P0_i.resize(yaw.size_x);
	P_i.resize(yaw.size_x);
	deltaP_i.resize(yaw.size_x);
	isWork.resize(yaw.size_x);

	for (int i = 0; i < yaw.size_x; ++i)
	{
		P0_i[i].resize(nLoopU);
		P_i[i].resize(nLoopU);
		deltaP_i[i].resize(nLoopU);
		for (int j = 0; j < nLoopU; ++j)
		{
			P0_i[i][j].resize(nLoopTheta);
			P_i[i][j].resize(nLoopTheta);
			deltaP_i[i][j].resize(nLoopTheta);
		}
		isWork[i] = '1';
	}

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
	theta360.resize(nLoopTheta);

	for (int i = 0; i < nLoopU; ++i)
	{
		u[i] = uBegin + i * deltaU;
	}
	for (int j = 0; j < nLoopTheta; ++j)
	{
		theta360[j] = thetaBegin + j * deltaTheta;
	}
	for (int i = 0; i < nLoopU; ++i)
	{
		for (int j = 0; j < nLoopTheta; ++j)
		{
			
			yaw.reset(u[i], theta360[j], isWork, rho, model, maxGamma, randomRange);

			SQPIC_solver(yaw, tol);

			yaw.outputGamma(gamma360[i][j]);

			for (int k = 0; k < yaw.size_x; ++k)
			{
				if (abs(gamma360[i][j][k]) < smallGamma)
				{
					gamma360[i][j][k] = 0;
				}
			}
			Column f0_i = yaw.initialPower_i();
			Column f_i = yaw.power_i();
			for (int k = 0; k < yaw.size_x; ++k)
			{
				P0_i[k][i][j] = f0_i[k];
				P_i[k][i][j] = f_i[k];
				deltaP_i[k][i][j] = f_i[k] - f0_i[k];
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

int Run::outputMatrix(bool isTranspose)
{
	std::filesystem::path output("output");
	std::filesystem::path matrix("yawMatrix");
	std::filesystem::path outputDir;
	outputDir = output / matrix;

	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directory(output);
	}

	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}

	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	int uNum = floor((uOut - uIn) / deltaU) +1;
	if ((uNum - 1) * deltaU + uIn < uOut) uNum++;
	getUniformA(uFull, uIn, deltaU, uNum);
	thetaFull = theta360;
	Matrix fullGamma, fullGammaT;
	fullGamma.resize(uNum);
	for (int i = 0; i < uNum; ++i)
	{
		fullGamma[i].resize(nLoopTheta);
	}

	if (isTranspose)
	{
		for (int i = 0; i < nLoopTurbine; ++i)
		{
			std::string filename = (outputDir / (yaw.simulation.turbines.turbName[i] + ".csv")).string();
			std::ofstream file(filename);
			if (file.is_open())
			{
				Matrix gamma,gammaT;

				gamma.resize(nLoopU);
				for (int j = 0; j < nLoopU; ++j)
				{
					gamma[j].resize(nLoopTheta);
					for (int k = 0; k < nLoopTheta; ++k)
					{
						gamma[j][k] = gamma360[j][k][i];
					}
				}
				extendMatrix(fullGamma, gamma, uFull, thetaFull, u, theta360);
				string varName = "Theta \\ U";
				// getAT(gammaT, gamma);
				getAT(fullGammaT, fullGamma);
				writeExcel(filename, varName, uFull, thetaFull, fullGammaT, 6, 2);
				std::cout << "Turbine " << yaw.simulation.turbines.turbName[i] << " yaw matrix has been output!" << endl;
			}
			else
			{
				std::cerr << "Failed to create file: " << filename << std::endl;
			}
			file.close();
		}
		return 0;
	}

	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (yaw.simulation.turbines.turbName[i] + ".csv")).string();
		std::ofstream file(filename);
		if (file.is_open())
		{
			Matrix gamma;
			gamma.resize(nLoopU);
			for (int j = 0; j < nLoopU; ++j)
			{
				gamma[j].resize(nLoopTheta);
				for (int k = 0; k < nLoopTheta; ++k)
				{
					gamma[j][k] = gamma360[j][k][i];
				}
			}
			extendMatrix(fullGamma, gamma, uFull, thetaFull, u, theta360);
			string varName = "U \\ Theta";
			writeExcel(filename, varName, thetaFull, uFull, fullGamma, 6, 2);
			std::cout << "Turbine " << yaw.simulation.turbines.turbName[i] << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}
	return 0;
}

int Run::outputAbsMatrix(bool isTranspose)
{
	double theta_i = 0, u_i = 0;
//风机的绝对偏转角度。风向为0，且正对风时，偏航角度为0
	std::filesystem::path output("output");
	std::filesystem::path matrix("yawMatrix");
	std::filesystem::path outputDir;
	outputDir = output / matrix;

	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directory(output);
	}
	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}
	int nLoopU, nLoopTheta, nLoopTurbine;
	nLoopU = gamma360.size();
	nLoopTheta = gamma360[0].size();
	nLoopTurbine = gamma360[0][0].size();

	if (isTranspose)
	{
		for (int i = 0; i < nLoopTurbine; ++i)
		{
			std::string filename = (outputDir / (yaw.simulation.turbines.turbName[i] + ".csv")).string();
			std::ofstream file(filename);
			if (file.is_open())
			{
				Matrix absGamma, absGammaT; //风机的绝对偏转角度。风向为0，且正对风时，偏航角度为0
				absGamma.resize(nLoopU);

				for (int j = 0; j < nLoopU; ++j)
				{
					absGamma[j].resize(nLoopTheta);
					for (int k = 0; k < nLoopTheta; ++k)
					{
						absGamma[j][k] = gamma360[j][k][i] + theta360[k];
					}
				}
				getAT(absGammaT, absGamma);
				string varName = "Theta \\ U";
				writeExcel(filename, varName, u, theta360, absGammaT, 6, 2);
				std::cout << "Turbine " << yaw.simulation.turbines.turbName[i] << " yaw matrix has been output!" << endl;
			}
			else
			{
				std::cerr << "Failed to create file: " << filename << std::endl;
			}
			file.close();
			return 0;
		}
	}

	for (int i = 0; i < nLoopTurbine; ++i)
	{
		std::string filename = (outputDir / (yaw.simulation.turbines.turbName[i] + ".csv")).string();
		std::ofstream file(filename);
		if (file.is_open())
		{
			Matrix absGamma; //风机的绝对偏转角度。风向为0，且正对风时，偏航角度为0
			absGamma.resize(nLoopU);

			for (int j = 0; j < nLoopU; ++j)
			{
				absGamma[j].resize(nLoopTheta);
				for (int k = 0; k < nLoopTheta; ++k)
				{
					absGamma[j][k] = gamma360[j][k][i] + theta360[k];
				}
			}
			string varName = "U \\ Theta";
			writeExcel(filename, varName, theta360, u, absGamma, 6, 2);
			std::cout << "Turbine " << yaw.simulation.turbines.turbName[i] << " yaw matrix has been output!" << endl;
		}
		else
		{
			std::cerr << "Failed to create file: " << filename << std::endl;
		}
		file.close();
	}
	return 0;
}

int Run::extendMatrix(Matrix& result, Matrix& source, Column& resulti, Column& resultj, Column& sourcei, Column& sourcej)
{
	int m1 = resulti.size();
	int n1 = resultj.size();
	int m2 = sourcei.size();
	int n2 = sourcej.size();
	if (source.size() != m2)
	{
		cout << "source.size() != sourcei.size()" << endl;
		return 0;
	}
	if (source[0].size() != n2)
	{
		cout << "source[0].size() != sourcej.size()" << endl;
		return 0;
	}
	result.resize(m1);
	for (int i = 0; i < m1; ++i)
	{
		result[i].resize(n1);
		for (int j = 0; j < n1; ++j)
		{
			if (resulti[i] < sourcei[0] || resulti[i] > sourcei[m2 - 1] || resultj[j] < sourcej[0] || resultj[j] > sourcej[n2 - 1])
			{
				result[i][j] = 0;
			}
			else
			{
				int indexi, indexj;
				double factori, factorj;
				double resultij;
				findx(indexi, factori, sourcei, resulti[i]);
				findx(indexj, factorj, sourcej, resultj[j]);
				interpolation(resultij, source, indexi, indexj, factori, factorj);
				result[i][j] = resultij;
			}
		}
	}
	return 0;
}