#pragma once
#include <Windows.h>

typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;

class Timer
{
private:
	static UINT32 __millisecondsFrequency; //ms�� CPU Ŭ�� ��

	UINT32	_startTime;					// �ð� ���� ���� �ð�
	UINT32	_curTime;					// ���� ~ ����ð�	
	UINT32	_deltaTime;					// _curTime - startTime;
	UINT32	_timeScale;					// �ð� �帧 �ӵ�?
	
	UINT32 calculateTime();				// CPU Ŭ�����κ��� �ð� ���ϱ�
public:
	static void SetUp();				// __secondsFrequency, __millisecondsFrequency �ʱ�ȭ, ���ø����̼� ����� ȣ��

	Timer();
	~Timer();

	// inline �Լ� �ĺ�
	UINT32 getElapsedTime() { return _curTime - _startTime; }	//��� �ð�	
	UINT32 setTimeScale(UINT32 value)
	{
		if (value > 1) _timeScale = 1;		

		_timeScale = value;
	}

	void start();	// ���� ����
	void update();	// ���� ~ ���� �ð� ����
};

extern Timer AppTimer;
