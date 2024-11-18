#ifndef STATISTICS_H
#define STATISTICS_H

#include "run.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
using std::string;
using std::vector;

class Statistics
{
public:
	const double t = 8760; // ÿ���Сʱ����24 * 365��

	// ���ٺͷ������У���0�����
	Column u, theta360;
	// �������У���cutin��cutout
	Column uCut;
	// ���ٺͷ������У���uBegin��uEnd��ֻ�ڴ˷�Χ�ڽ���ƫ����
	Column uYaw, thetaYaw;

	double uMin = 0;
	double uMax = 0; // ��readFile()�б�����
	double thetaMin = 0;
	double thetaMax = 360;
	bool isDelBadVal = true;
	int badValNum = 0;

	Run *run = nullptr;
	Bool isWork;

	Statistics();
	Statistics(Run& run);
	int setStatistics(Run& run);
	int readFile(bool isDelBadVal); // ����true�����ɾ���������ݣ�����false����ɾ����������
	
	int get_all();
	int write_all(bool isTranspose);
private:
	// ������ٺͷ������У���С���������С������г��������У���С���������У������г��������У�
	std::filesystem::path output;
	std::filesystem::path statistics;
	std::filesystem::path absOutput;

	// ͳ�Ʒ��ٺͷ����Ƶ��
	int windStatistics();
	int getUThetaColumn();

	vector<string> date, time;
	vector<double> windSpeed;
	vector<double> direction;
	vector<vector<double>> probability;
	vector<double> pWindSpeed; // ����ֱ��ͼ
	vector<double> pWindTheta; // ����õ��ͼ

	// g means annual generation
	double gWithoutWeak = 0; // ������β�����귢����
	double g0 = 0; // ����β��������ƫ�����귢����
	double g = 0; // ����β������ƫ���Ż����귢����

	double weakLoss0 = 0; // ��ƫ����β����ʧ
	double weakLoss = 0; // ƫ�����β����ʧ
	double gIncrease = 0; // ƫ����ķ��������ӣ����ٷֱȼ��㣩

	Column gPerThetaWithoutWeak; // ������β����ÿ�������µ��귢����
	Column g0PerTheta; // ����β������ƫ��ʱÿ�������µ��귢����
	Column gPerTheta; // ����β����ƫ��ʱÿ�������µ��귢����

	Column gPerUWithoutWeak; // ������β����ÿ�����ٶε��귢����
	Column g0PerU; // ����β����������ƫ��ʱÿ�����ٶε��귢����
	Column gPerU; // ����β����ƫ��ʱÿ�����ٶε��귢����

	Column weakLoss0PerU; // ��ƫ����ÿ�����ٶε�β����ʧ
	Column weakLossPerU; // ƫ����ÿ�����ٶε�β����ʧ
	Column gIncreasePerU; // ƫ�����ÿ�����ٶεķ��������ӣ����ٷֱȼ��㣩

	Column gPerTurbWithoutWeak; // ������β����ÿ̨������귢����
	Column g0PerTurb; // ����β������ƫ��ʱÿ̨������귢����
	Column gPerTurb; // ����β����ƫ��ʱÿ̨������귢����

	Column weakLoss0PerTurb; // ��ƫ����ÿ̨�����β����ʧ
	Column weakLossPerTurb; // ƫ����ÿ̨�����β����ʧ
	Column gIncreasePerTurb; // ƫ�����ÿ̨����ķ��������ӣ����ٷֱȼ��㣩

	Column gTimeSeriseWithoutWeak; // ������β����Сʱ������ʱ������
	Column g0TimeSerise; // ����β������ƫ��ʱ��Сʱ������ʱ������
	Column gTimeSerise; // ����β����ƫ��ʱ��Сʱ������ʱ������

	// �����һͳ����
	int writeSingleStatistics();
	// ������β����ÿ�������µ��귢����
	int write_gPerThetaWithoutWeak();
	// ����β����������ƫ��ʱÿ��������귢����
	int write_g0PerTheta();
	// ����β����ƫ��ʱÿ��������귢����
	int write_gPerTheta();
	// ������β����ÿ�����ٶε��귢����
	int write_gPerUWithoutWeak();
	// ����β����������ƫ��ʱÿ�����ٶε��귢����
	int write_g0PerU();
	// ����β����ƫ��ʱÿ�����ٶε��귢����
	int write_gPerU();
	// ��ƫ����ÿ�����ٶε�β����ʧ
	int write_weakLoss0PerU();
	// ƫ����ÿ�����ٶε�β����ʧ
	int write_weakLossPerU();
	// ƫ�����ÿ�����ٶεķ��������ӣ����ٷֱȼ��㣩
	int write_gIncreasePerU();
	// ������β����ÿ̨������귢����
	int write_gPerTurbWithoutWeak();
	// ����β������ƫ��ʱÿ̨������귢����
	int write_g0PerTurb();
	// ����β����ƫ��ʱÿ̨������귢����
	int write_gPerTurb();
	// ��ƫ����ÿ̨�����β����ʧ
	int write_weakLoss0PerTurb();
	// ƫ����ÿ̨�����β����ʧ
	int write_weakLossPerTurb();
	// ƫ�����ÿ̨����ķ��������ӣ����ٷֱȼ��㣩
	int write_gIncreasePerTurb();
	// ������β����Сʱ������ʱ������
	int write_gTimeSeriseWithoutWeak();
	// ����β������ƫ��ʱ��Сʱ������ʱ������
	int write_g0TimeSerise();
	// ����β����ƫ��ʱ��Сʱ������ʱ������
	int write_gTimeSerise();

	// �������ͳ����
	int writeSector_x(Column& x, string& title, string& fileName);
	// ������ٶ�ͳ����
	int writeSegment_x(Column& x, string& title, string& fileName);
	// �����̨���ͳ����
	int writeTurbine_x(Column& x, string& title, string& fileName);
	// ���ʱ��ͳ����
	int writeTime_x(Column& x, string& title, string& fileName);
	// �����ͳ��
	int writeColumn(Column& x, Column& y, string& titlex, string& titley, string& fileName);
	// �����ͳ��
	int writeColumn(vector<string>& x, Column& y, string& titlex, string& titley, string& fileName);

	// ������β���������귢����
	int get_gWithoutWeak();
	// ����β��������ƫ�����귢����
	int get_g0();
	// ����β������ƫ���Ż����귢����
	int get_g();
	// ��ƫ����β����ʧ
	int get_weakLoss0();
	// ƫ����β����ʧ
	int get_weakLoss();
	// ƫ����ķ��������ӣ����ٷֱȼ��㣩
	int get_gIncrease();
	// ������β����ÿ�������µ��귢����
	int get_gPerThetaWithoutWeak();
	// ����β����������ƫ��ʱÿ��������귢����
	int get_g0PerTheta();
	// ����β����ƫ��ʱÿ��������귢����
	int get_gPerTheta();
	// ������β����ÿ�����ٶε��귢����
	int get_gPerUWithoutWeak();
	// ����β����������ƫ��ʱÿ�����ٶε��귢����
	int get_g0PerU();
	// ����β����ƫ��ʱÿ�����ٶε��귢����
	int get_gPerU();
	// ��ƫ����ÿ�����ٶε�β����ʧ
	int get_weakLoss0PerU();
	// ƫ����ÿ�����ٶε�β����ʧ
	int get_weakLossPerU();
	// ƫ�����ÿ�����ٶεķ��������ӣ����ٷֱȼ��㣩
	int get_gIncreasePerU();
	// ������β����ÿ̨������귢����
	int get_gPerTurbWithoutWeak();
	// ����β������ƫ��ʱÿ̨������귢����
	int get_g0PerTurb();
	// ����β����ƫ��ʱÿ̨������귢����
	int get_gPerTurb();
	// ��ƫ����ÿ̨�����β����ʧ
	int get_weakLoss0PerTurb();
	// ƫ����ÿ̨�����β����ʧ
	int get_weakLossPerTurb();
	// ƫ�����ÿ̨����ķ��������ӣ����ٷֱȼ��㣩
	int get_gIncreasePerTurb();
	// ������β����Сʱ������ʱ������
	int get_gTimeSeriseWithoutWeak();
	// ����β������ƫ��ʱ��Сʱ������ʱ������
	int get_g0TimeSerise();
	// ����β����ƫ��ʱ��Сʱ������ʱ������
	int get_gTimeSerise();
};
#endif // !STATISTICS_H
