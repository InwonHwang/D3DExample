#pragma once

#include "DataTypes.h"

class Timer
{
public:
	static void SetUp();				// __secondsFrequency, __millisecondsFrequency �ʱ�ȭ, ���ø����̼� ����� ȣ��

	Timer();
	~Timer();

	// inline �Լ� �ĺ�
	uint GetElapsedTime(){ return _curTime - _startTime; }	//��� �ð�	
	uint SetTimeScale(uint value)
	{
		if (value > 1) _timeScale = 1;		

		_timeScale = value;
	}

	void Start();	// ���� ����
	void Update();	// ���� ~ ���� �ð� ����

private:
	uint CalculateTime();				// CPU Ŭ�����κ��� �ð� ���ϱ�

private:
	static uint __millisecondsFrequency; //ms�� CPU Ŭ�� ��

	uint	_startTime;					// �ð� ���� ���� �ð�
	uint	_curTime;					// ���� ~ ����ð�	
	uint	_deltaTime;					// _curTime - startTime;
	uint	_timeScale;					// �ð� �帧 �ӵ�?
};

