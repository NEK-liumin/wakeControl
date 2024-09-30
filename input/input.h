#ifndef INPUT_H
#define INPUT_H

#include"turbCloud.h"

class Input
{
public:
	TurbCloud* turbines; //析构不释放指向的地址。因为地址来自传入的变量
	Input()
	{
		turbines = nullptr;
	};
	Input(TurbCloud& turbines)
	{
		this->turbines = &turbines;
	}
	int setTurb(TurbCloud& turbines)
	{
		this->turbines = &turbines;
		return 0;
	}
	int readFile();
};
#endif // !INPUT_H