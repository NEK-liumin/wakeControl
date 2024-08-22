#ifndef INPUT_H
#define INPUT_H

#include"turbCloud.h"

class Input
{
public:
	TurbCloud* turbines;
	Input() {};
	Input(TurbCloud& turbines)
	{
		this->turbines = &turbines;
	}
	int readFile();
};
#endif // !INPUT_H