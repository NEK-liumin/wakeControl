#ifndef EXCEL_H
#define EXCEL_H

#include <string>
#include <vector>
#include "matrixOperation.h"
using std::vector;
using std::string;

// 输出二维矩阵
int writeExcel(string& fileName, string& varName, Column& titleTop, Column& titleLeft, Matrix& content, int width, int precision);

// 输出两列
int writeExcel(string& fileName, string& varName, Column& titleLeft, Column& content, int width, int precision);
int writeExcel(string& fileName, string& varName, vector<string>& titleLeft, Column& content, int width, int precision);
// 输出两行
int writeExcel(string& fileName, Column& titleTop, string& varName, Column& content, int width, int precision);
#endif // !EXCEL_H
