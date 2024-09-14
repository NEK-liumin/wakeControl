#ifndef INPUT_H
#define INPUT_H

#include"turbCloud.h"

class Input
{
public:
	TurbCloud* turbines;
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