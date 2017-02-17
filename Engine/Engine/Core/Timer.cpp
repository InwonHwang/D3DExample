#include "Timer.h"
#include <Windows.h>

uint Timer::__millisecondsFrequency = 0;

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

uint Timer::CalculateTime()
{
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return (temp.LowPart / __millisecondsFrequency) * _timeScale; // ms시간 리턴
}


void Timer::Start()
{
	if (_startTime == 0)
		_startTime = CalculateTime();
}

void Timer::Update()
{
	_curTime = CalculateTime();
}
