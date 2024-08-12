#include "turbCloudGeneration.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <matrixOperation.h>
#include "turbCloud.h"
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

int generateTestDate(TurbCloud& turbines)
{
// ����һ��36̨���
// ����һ�����ַ��
	turbines = TurbCloud(36, 3, 10, 0, 20);

	// ������������Ҷ��ֱ��150m
	// ���������������500m
	// �������������ʾ��������Ų�
	for (int i = 0; i < turbines.turbNum; ++i)
	{
		turbines.D[i] = 150;
	}
	int m = 6;
	int n = 6;

	int id;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			id = i * n + j;
			turbines.x0[id] = i * 500;
			turbines.y0[id] = j * 500;
			turbines.z0[id] = 0;
			if ((i + j) % 3 == 0) // ���Ū�ã��ѷ���ֳ�����
			{
				turbines.turbType[id] = 0;
			}
			else if((i + j) % 3 == 1)
			{
				turbines.turbType[id] = 1;
			}
			else
			{
				turbines.turbType[id] = 2;
			}
		}
	}

	
	ofstream outFile("turbinesInfo.csv");

	if (!outFile)
	{
		std::cerr << "�޷����ļ�" << std::endl;
		return 1;
	}
	outFile << "̨��" << endl;
	outFile << turbines.turbNum << endl;
	outFile << "������" << endl;
	outFile << turbines.turbTypeNum << endl;
	outFile << "���ٽ�������" << endl;
	outFile << turbines.uNum << endl;
	outFile << "��С����" << endl;
	outFile << turbines.uMin << endl;
	outFile << "������" << endl;
	outFile << turbines.uMax << endl;
	outFile << "x,y,z,D,type" << endl;

	for (int i = 0; i < turbines.turbNum; i++)
	{
		outFile << turbines.x0[i] << " , " << turbines.y0[i] << " , " << turbines.z0[i] << " , " << turbines.D[i] << " , " << turbines.turbType[i] << endl;
	}
	outFile << "Cp��Ct����" << endl;
	
	for (int i = 0; i < turbines.turbTypeNum; ++i)
	{
		outFile << "�� " << i << " �ַ��" << endl;
		outFile << "����,Cp,Ct" << endl;
		for (int j = 0; j < turbines.uNum; ++j)
		{
			outFile << turbines.uWind[j] << "," << turbines.Cp[i][j] << "," << turbines.Ct[i][j] << endl;
		}
	}

	outFile.close();
	return 0;
}