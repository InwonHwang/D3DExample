#pragma once

// ���� ������ ��� �� ����ȭ �ʿ�
template <class T>
class Singleton
{
protected:
	Singleton() {}

public:
	virtual ~Singleton() {}

	virtual bool Init() = 0;
	virtual void Clear() = 0;

	static T* Instance()	
	{			
		static T inst;

		return &inst;
	}
};
