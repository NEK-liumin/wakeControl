#include "excel.h"
#include <fstream>
#include "iostream"

#include "head.h"
#include <chrono>
#include <filesystem>
#include <sstream>
#include <string>

static bool splitString(const std::string& input, std::string& before, std::string& after) {
	size_t pos = input.find(',');

	// 检查逗号位置
	if (pos == std::string::npos || pos == 0 || pos == input.length() - 1) {
		return false; // 没有逗号或逗号在首尾
	}

	before = input.substr(0, pos);
	after = input.substr(pos + 1);
	return true;
}

int writeExcel(string& fileName, string& varName, Column& titleTop, Column& titleLeft, Matrix& content, int width, int precision)
{
	std::ofstream file(fileName);
	int nColumn = titleTop.size();
	int nLine = titleLeft.size();

	if (nLine != content.size() || nColumn != content[0].size())
	{
		std::cout << "The number of entered variables does not match!" << std::endl;
		return 1;
	}

	if (file.is_open())
	{
		file << varName << ",";
		for (int k = 0; k < nColumn; ++k)
		{
			file << std::setw(width) << std::setprecision(precision) << std::fixed << titleTop[k];
			if (k < nColumn - 1)
			{
				file << ",";
			}
		}
		file << std::endl;
		for (int j = 0; j < nLine; ++j)
		{
			file << std::setw(width) << std::setprecision(precision) << std::fixed << titleLeft[j] << ",";
			for (int k = 0; k < nColumn; ++k)
			{
				file << std::setw(width) << std::setprecision(precision) << std::fixed << content[j][k];
				if (k < nColumn - 1)
				{
					file << ",";
				}
			}
			file << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << fileName << std::endl;
		return 1;
	}
	file.close();
	return 0;
}

int writeExcel(string& fileName, string& varName, Column& titleLeft, Column& content, int width, int precision)
{
	std::ofstream file(fileName);
	int nLine = titleLeft.size();
	if (nLine != content.size())
	{
		std::cout << "The number of entered variables does not match!" << std::endl;
		return 1;
	}

	string topName, bottomName;
	if (!splitString(varName, topName, bottomName))
	{
		std::cout << "varName must contain two names separated by commas!" << endl;
		return 1;
	}

	if (file.is_open())
	{
		file << varName << std::endl;
		for (int i = 0; i < nLine; ++i)
		{
			file << std::setw(width) << std::setprecision(precision) << std::fixed << titleLeft[i] << ",";
			file << std::setw(width) << std::setprecision(precision) << std::fixed << content[i] << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to create file: " << fileName << std::endl;
		return 1;
	}
	file.close();
	return 0;
}

int writeExcel(string& fileName, Column& titleTop, string& varName, Column& content, int width, int precision)
{
	std::ofstream file(fileName);
	int nColumn = titleTop.size();

	if (nColumn != content.size())
	{
		std::cout << "The number of entered variables does not match!" << std::endl;
		return 1;
	}

	string topName, bottomName;
	if (!splitString(varName, topName, bottomName))
	{
		std::cout << "varName must contain two names separated by commas!" << endl;
		return 1;
	}

	if (file.is_open())
	{
		file << topName;
		for (int i = 0; i < nColumn; ++i)
		{
			file << "," << std::setw(width) << std::setprecision(precision) << std::fixed << titleTop[i];
		}
		cout << std::endl;
		file << bottomName;
		for (int i = 0; i < nColumn; ++i)
		{
			file << "," << std::setw(width) << std::setprecision(precision) << std::fixed << content[i];
		}
		cout << std::endl;
	}
	else
	{
		std::cerr << "Failed to create file: " << fileName << std::endl;
		return 1;
	}
	file.close();
	return 0;
}