#pragma once

// 만약 쓰레드 사용 시 동기화 필요
template <class T>
class Singleton
{
protected:
	Singleton() {}

public:
	virtual ~Singleton() {}

	virtual bool Init() = 0;
	virtual void Release() = 0;

	static T* Instance()	
	{			
		static T inst;

		return &inst;
	}
};
