#pragma once

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

