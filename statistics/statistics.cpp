#include "statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "excel.h"
#include <algorithm>

Statistics::Statistics()
{

}

int Statistics::setStatistisc(Run& run)
{
	this->run = &run;
	return 0;
}

int Statistics::readFile(bool isDelBadVal)
{

	std::ifstream timeseries("input/timeseries.txt");
	if (!timeseries.is_open())
	{
		std::cerr << "Error opening timeseries.txt!" << std::endl;
		return 1;
	}
	string line;
	while (std::getline(timeseries, line))
	{
		std::stringstream ss(line);
		std::string date0, time0; // ��ʱ�������������
		double u, theta; // ��ʱ��������ŷ��ٷ���

		ss >> date0 >> time0 >> u >> theta;

		date.push_back(date0);
		time.push_back(time0);
		windSpeed.push_back(u);
		direction.push_back(theta);
	}
	timeseries.close();

	if (isDelBadVal)
	{
		vector<size_t> indicesToRemove;

		for (size_t i = 0; i < windSpeed.size(); ++i)
		{
			if (windSpeed[i] <= 0 || windSpeed[i] > 60 || direction[i] < 0 || direction[i] > 360)
			{
				indicesToRemove.push_back(i);
			}
		}
		for (size_t i = indicesToRemove.size(); i > 0; --i)
		{
			size_t index = indicesToRemove[i - 1];
			date.erase(date.begin() + index);
			time.erase(time.begin() + index);
			windSpeed.erase(windSpeed.begin() + index);
			direction.erase(direction.begin() + index);
		}
		auto uMaxTemp = std::max_element(windSpeed.begin(), windSpeed.end());
		uMax = *uMaxTemp;
	}
	else
	{
		int k = 0;
		std::ofstream badValue("input/badValue.log");
		if (!badValue.is_open())
		{
			std::cerr << "Error creating badValue.log" << std::endl;
			return 1;
		}
		for (size_t i = 0; i < windSpeed.size(); ++i)
		{
			if (windSpeed[i] <= 0 || windSpeed[i]> 60 || direction[i] < 0 || direction[i] > 360)
			{
				badValue << date[i] << " " << time[i] << " "
					<< std::fixed << std::setprecision(2) << windSpeed[i] << " "
					<< std::fixed << std::setprecision(2) << direction[i] << std::endl;
				k++;
			}
		}
		badValue.close();
		if (k > 0)
		{
			std::cout << "Data in the timeseries.txt file is faulty. Check the badValue.log" << std::endl;
			return 0;
		}
		auto uMaxTemp = std::max_element(windSpeed.begin(), windSpeed.end());
		uMax = *uMaxTemp;
	}

	return 0;
}

int Statistics::windStatistics(bool isTranspose)
{
	std::filesystem::path output("output");
	std::filesystem::path statistics("statistics");
	std::filesystem::path absOutput;
	absOutput = output / statistics;

	if (!std::filesystem::exists(absOutput))
	{
		std::filesystem::create_directories(absOutput);
	}

	int nVel = 0, nTheta = 0;
	nVel = (uMax - uMin) / run->deltaU;
	nTheta = (thetaMax - thetaMin) / run->deltaTheta;

	u.resize(nVel);
	theta360.resize(nTheta);

	getZeroMatrix(probability, nVel, nTheta);

	for (int i = 0; i < nVel; ++i)
	{
		u[i] = uMin + (i + 0.5) * run->deltaU;
	}
	for (int j = 0; j < nTheta; ++j)
	{
		theta360[j] = thetaMin + (j + 0.5) * run->deltaTheta;
	}

	for (int i = 0; i < date.size(); ++i)
	{
		int j = floor(windSpeed[i] / run->deltaU);
		int k = floor(direction[i] / run->deltaTheta);
		if (j <= 0)j = 0;
		if (j >= nVel - 1)j = nVel - 1;
		if (k <= 0)k = 0;
		if (k >= nTheta - 1)k = nTheta - 1;
		probability[j][k]++;
	}
	for (int i = 0; i < nVel; ++i)
	{
		for (int j = 0; j < nTheta; ++j)
		{
			probability[i][j] /= date.size();
		}
	}

	pWindSpeed.resize(nVel);
	pWindTheta.resize(nTheta);

	for (int i = 0; i < nVel; ++i)
	{
		pWindSpeed[i] = 0;
	}
	for (int i = 0; i < nTheta; ++i)
	{
		pWindTheta[i] = 0;
	}
	for (int i = 0; i < nVel; ++i)
	{
		for (int j = 0; j < nTheta; ++j)
		{
			pWindSpeed[i] += probability[i][j];
			pWindTheta[j] += probability[i][j];
		}
	}

	string possibleName = (absOutput / "possible.csv").string();
	string pUName = (absOutput / "pWindSpeed.csv").string();
	string pThetaName = (absOutput / "pWindTheta.csv").string();

	std::ofstream possible(possibleName);
	std::ofstream pU(pUName);
	std::ofstream pTheta(pThetaName);

	if (isTranspose)
	{
		string varName = "Theta\\U";
		Matrix probabilityT;
		getAT(probabilityT, probability);
		writeExcel(possibleName, varName, u, theta360, probabilityT, 10, 5);
	}
	else
	{
		string varName = "U\\Theta";
		writeExcel(possibleName, varName, theta360, u, probability, 10, 5);
	}
	string varName = "U, probability";
	writeExcel(pUName, varName, u, pWindSpeed, 10, 5);
	varName = "Theta,probility";
	writeExcel(pThetaName, varName, theta360, pWindTheta, 10, 5);
	return 0;
}

int Statistics::powerStatistics(bool isTranspose)
{
	std::filesystem::path output("output");
	std::filesystem::path subOutput("statistics");
	std::filesystem::path outputDir;
	outputDir = output / subOutput;

	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directory(output);
	}

	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}

	std::string P0name = (outputDir / "P0.csv").string();
	std::string Pname = (outputDir / "P.csv").string();
	std::string deltaPname = (outputDir / "deltaP.csv").string();

	std::ofstream P0file(P0name);
	std::ofstream Pfile(Pname);
	std::ofstream deltaPfile(deltaPname);

	if (isTranspose)
	{
		if (P0file.is_open())
		{
			string varName = "Theta \\ U";
			Matrix P0T;
			getAT(P0T, run->P0);
			writeExcel(P0name, varName, run->u, run->theta360, P0T, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << P0name << std::endl;
		}
		P0file.close();

		if (Pfile.is_open())
		{
			string varName = "Theta \\ U";
			Matrix PT;
			getAT(PT, run->P);
			writeExcel(Pname, varName, run->u, run->theta360, PT, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << Pname << std::endl;
		}
		Pfile.close();

		if (deltaPfile.is_open())
		{
			string varName = "Theta \\ U";
			Matrix deltaPT;
			getAT(deltaPT, run->deltaP);
			writeExcel(deltaPname, varName, run->u, run->theta360, deltaPT, 6, 2);
		}
		else
		{
			std::cerr << "Failed to create file: " << deltaPname << std::endl;
		}
		deltaPfile.close();
		return 0;
	}
	// �����ת�ã�����
	if (P0file.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(P0name, varName, run->theta360, run->u, run->P0, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << P0name << std::endl;
	}
	P0file.close();

	if (Pfile.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(Pname, varName, run->theta360, run->u, run->P, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << Pname << std::endl;
	}
	Pfile.close();

	if (deltaPfile.is_open())
	{
		string varName = "U \\ Theta";
		writeExcel(deltaPname, varName, run->theta360, run->u, run->deltaP, 6, 2);
	}
	else
	{
		std::cerr << "Failed to create file: " << deltaPname << std::endl;
	}
	deltaPfile.close();
	return 0;
}

int Statistics::power_iStatistics(bool isTranspose)
{
	std::filesystem::path output("output");
	std::filesystem::path subOutput("statistics");
	std::filesystem::path subSubOutput("powerPerTurbine");
	std::filesystem::path outputDir;
	outputDir = output / subOutput / subSubOutput;

	if (!std::filesystem::exists(output))
	{
		std::filesystem::create_directory(output);
	}

	if (!std::filesystem::exists(output / subOutput))
	{
		std::filesystem::create_directory(output / subOutput);
	}

	if (!std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directory(outputDir);
	}

	for (int i = 0; i < run->yaw.size_x; ++i)
	{
		std::string P0_iName = (outputDir / ("P0_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream P0file(P0_iName);
		std::string P_iName = (outputDir / ("P_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream Pfile(P_iName);
		std::string deltaP_iName = (outputDir / ("deltaP_" + std::to_string(i + 1) + ".csv")).string();
		std::ofstream deltaPfile(deltaP_iName);
		if (P0file.is_open() && Pfile.is_open() && deltaPfile.is_open())
		{
			if (isTranspose)
			{
				string varName = "Theta \\ U";
				Matrix deltaP_i;
				Matrix P0_iT, P_iT, deltaP_iT;

				getAMinusB(deltaP_i, run->P_i[i], run->P0_i[i]);
				
				getAT(P0_iT, run->P0_i[i]);
				getAT(P_iT, run->P_i[i]);
				getAT(deltaP_iT, deltaP_i);

				writeExcel(P0_iName, varName, run->u, run->theta360, P0_iT, 10, 5);
				writeExcel(P_iName, varName, run->u, run->theta360, P_iT, 10, 5);
				writeExcel(deltaP_iName, varName, run->u, run->theta360, deltaP_iT, 10, 5);
			}
			else
			{
				string varName = "U \\ Theta";
				Matrix deltaP_i;
				getAMinusB(deltaP_i, run->P_i[i], run->P0_i[i]);
				
				writeExcel(P0_iName, varName, run->theta360, run->u, run->P0_i[i], 10, 5);
				writeExcel(P_iName, varName, run->theta360, run->u, run->P_i[i], 10, 5);
				writeExcel(deltaP_iName, varName, run->theta360, run->u, deltaP_i, 10, 5);
			}

		}
		else
		{
			std::cerr << "Failed to create file: " << P0_iName << ", " << P_iName << " and " << deltaP_iName << std::endl;
		}
	}

	return 0;
}

int Statistics::annualPowerGeneration()
{
	double uCutIn = run->yaw.simulation.turbines.uMin;
	double uCutOut = run->yaw.simulation.turbines.uMax;
	double deltaU = run->deltaU;
	double deltaTheta = run->deltaTheta;
	int nVel = floor((uCutOut - uCutIn) / deltaU) + 1;
	int nTheta = floor((thetaMax - thetaMin) / deltaTheta);
	// ������ٷ������
	// ��possibility��ͬ������ĸ����Ǵ������ٶȿ�ʼ�����г��ٶ�Ϊֹ
	// �������ֻ��Ϊ�ֲ�������ͳ���귢�����ã���������
	Matrix probability2;
	getZeroMatrix(probability2, nVel, nTheta);
	for (int i = 0; i < date.size(); ++i)
	{
		double j = -1, k = -1;
		if (windSpeed[i]>=uCutIn && windSpeed[i]<=uCutOut);
		{
			j = (windSpeed[i] + 0.5 * deltaU - uCutIn) / deltaU;
			if (j <= 0)j = 0;
			if (j >= nVel - 1)j = nVel - 1;
		}
		if (direction[i] > thetaMax - 0.5 * deltaTheta)
		{
			direction[i] -= thetaMax;
		}
		k = (direction[i] + 0.5 * deltaTheta - thetaMin) / deltaTheta;
		if (k <= 0)k = 0;
		if (k >= nTheta - 1)k = nTheta - 1;
		if (j >= 0)probability2[j][k] += 1;
	}
	getAlphaA(probability2, 1.0 / date.size());
	printA(probability2);
	return 0;
}