#pragma once

#include "..\..\Core\Core.h"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();

	virtual void Destroy() override {}

	template<typename T>
	sp<T> GetComponent();

	template<typename T>
	sp<T> AddComponent();

	void Clear();

private:
	std::vector<sp<Component>> _components;
	
};

template<typename T>
inline sp<T> Transform::GetComponent()
{	
	for (auto c : _components)
	{
		sp<T> ret = boost::dynamic_pointer_cast<T>(c);
		if (ret)
		{
			Debug::MsgBox(0, _T("ok"));
			return ret;
		}
		else
		{
			Debug::MsgBox(0, _T("failed"));
		}
	}
	
	return nullptr;
}

//나중에 관리자를 만들어서 할당할 예정.
template<typename T>
inline sp<T> Transform::AddComponent()
{
	T* c = Memory<T>::OrderedAlloc(sizeof(T));	
	sp<T> component(c, Memory<T>::OrderedFree);
	_components.push_back(component);

	return component;
}

inline void Transform::Clear()
{
	for (auto c : _components)
	{
		c->Destroy();		
	}
	_components.clear();
}

