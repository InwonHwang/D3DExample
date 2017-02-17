#pragma once

#include "DataTypes.h"

class Timer
{
public:
	static void SetUp();				// __secondsFrequency, __millisecondsFrequency 초기화, 애플리케이션 실행시 호출

	Timer();
	~Timer();

	// inline 함수 후보
	uint GetElapsedTime(){ return _curTime - _startTime; }	//경과 시간	
	uint SetTimeScale(uint value)
	{
		if (value > 1) _timeScale = 1;		

		_timeScale = value;
	}

	void Start();	// 측정 시작
	void Update();	// 시작 ~ 현재 시간 변경

private:
	uint CalculateTime();				// CPU 클럭수로부터 시간 구하기

private:
	static uint __millisecondsFrequency; //ms당 CPU 클럭 수

	uint	_startTime;					// 시간 측정 시작 시간
	uint	_curTime;					// 시작 ~ 현재시간	
	uint	_deltaTime;					// _curTime - startTime;
	uint	_timeScale;					// 시간 흐름 속도?
};

