#include <iostream>
#include "Timer.h"

using namespace std;

int main()
{
	Timer::SetUp();

	Timer time;

	time.start();

	while (true)
	{
		if (getchar() == 'a') break;

		time.update();
		cout << time.getElapsedTime() / 1000 << "s" << endl;
	}
	
}