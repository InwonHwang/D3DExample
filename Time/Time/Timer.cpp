#include "Timer.h"

UINT32 Timer::__millisecondsFrequency = 0;

Timer::Timer()
	:	_startTime(0),
		_deltaTime(0),
		_timeScale(1)
{
}

Timer::~Timer()
{
}

void Timer::SetUp()
{	
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	__millisecondsFrequency = frequency.LowPart / 1000;
}

UINT32 Timer::calculateTime()
{
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return (temp.LowPart / __millisecondsFrequency) * _timeScale; // ms시간 리턴
}


void Timer::start()
{
	if (_startTime == 0);
		_startTime = calculateTime();
}

void Timer::update()
{
	_curTime = calculateTime();
}
