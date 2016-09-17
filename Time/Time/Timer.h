#pragma once
#include <Windows.h>

typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;

class Timer
{
private:
	static UINT32 __millisecondsFrequency; //ms당 CPU 클럭 수

	UINT32	_startTime;					// 시간 측정 시작 시간
	UINT32	_curTime;					// 시작 ~ 현재시간	
	UINT32	_deltaTime;					// _curTime - startTime;
	UINT32	_timeScale;					// 시간 흐름 속도?
	
	UINT32 calculateTime();				// CPU 클럭수로부터 시간 구하기
public:
	static void SetUp();				// __secondsFrequency, __millisecondsFrequency 초기화, 애플리케이션 실행시 호출

	Timer();
	~Timer();

	// inline 함수 후보
	UINT32 getElapsedTime() { return _curTime - _startTime; }	//경과 시간	
	UINT32 setTimeScale(UINT32 value)
	{
		if (value > 1) _timeScale = 1;		

		_timeScale = value;
	}

	void start();	// 측정 시작
	void update();	// 시작 ~ 현재 시간 변경
};

extern Timer AppTimer;
