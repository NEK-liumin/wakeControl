#ifndef GOP_H
#define GOP_H

// ����һ����Ż�����
// define General Optimization Problem

// non-constrain General Optimization Problem
class NCGOP
{
public:
	int size_x;
	Column x;
	double f;
	Column g;
};
// Equality Constrain General Optimization Problem
class ECGOP :public NCGOP
{
public:

};
// Enequality Constrain General Optimization Problem
class ICGOP :public NCGOP
{
public:

};
// Equality and Inequality Constrain General Optimization Problem
class EICGOP :public NCGOP
{
public:

};

#endif // !GOP_H
