// wakeControl.cpp: 定义应用程序的入口点。
//

#include "wakeControl.h"
#include "head.h"
using namespace std;

int main()
{
	Wake wake{};
	wake.a = 0;
	cout << wake.a << endl;
	cout << "Hello CMake." << endl;
	return 0;
}
