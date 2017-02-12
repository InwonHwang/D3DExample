#pragma once

#include "..\..\Core\Core.h"
#include "Component.h"
#include "TransformImpl.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	template<typename T> sp<T> GetComponent();
	template<typename T> sp<T> AddComponent();

	virtual void Destroy() override;

	// friend private 함수로 바꾸기
	void Update(IDirect3DDevice9& device);
	void UpdateWorldMatrix();
	//

	void SetLocalScale(const Vector3& scale);
	void SetLocalRotation(const Quaternion& rotation);
	void SetLocalPosition(const Vector3& position);

	Vector3 GetLocalScale() const;
	Quaternion GetLocalRotation() const;
	Vector3 GetLocalEulerAngle() const;
	Vector3 GetLocalPosition() const;

	Vector3 GetScale() const;
	Quaternion GetRotation() const;
	Vector3 GetEulerAngle() const;
	Vector3 GetPosition() const;

	Vector3 GetAxisX() const;
	Vector3 GetAxisY() const;
	Vector3 GetAxisZ() const;

	// friend private 함수로 바꾸기
	D3DXMATRIX GetMatrix() const;

private:
	std::vector<sp<Component>>*	_components;
	sp<TransformImpl>			_impl;			//공유될 가능성 0
};

template<typename T>
inline sp<T> Transform::GetComponent()
{	
	for (auto c : *_components)
	{
		sp<T> ret = boost::dynamic_pointer_cast<T>(c);

		if (ret) return ret;		
	}
	
	return nullptr;
}

//나중에 관리자를 만들어서 할당할 예정.
template<typename T>
inline sp<T> Transform::AddComponent()
{
	T* c = Memory<T>::OrderedAlloc(sizeof(T));	
	sp<T> component(c, Memory<T>::OrderedFree);
	_components->push_back(component);

	return component;
}

inline Vector3 Transform::GetLocalScale() const
{
	return _impl->GetLocalScale();
}

inline Quaternion Transform::GetLocalRotation() const
{
	return _impl->GetLocalRotation();
}

inline Vector3 Transform::GetLocalEulerAngle() const
{
	return _impl->GetLocalEulerAngle();
}

inline Vector3 Transform::GetLocalPosition() const
{
	return _impl->GetLocalPosition();
}

inline Vector3 Transform::GetScale() const
{
	return _impl->GetScale();
}

inline Quaternion Transform::GetRotation() const
{
	return _impl->GetRotation();
}

inline Vector3 Transform::GetEulerAngle() const
{
	return _impl->GetEulerAngle();
}

inline Vector3 Transform::GetPosition() const
{
	return _impl->GetPosition();
}

inline Vector3 Transform::GetAxisX() const
{
	return _impl->GetAxisX();
}

inline Vector3 Transform::GetAxisY() const
{
	return _impl->GetAxisY();
}

inline Vector3 Transform::GetAxisZ() const
{
	return _impl->GetAxisZ();
}

inline D3DXMATRIX Transform::GetMatrix() const
{
	return _impl->GetMatrix();
}



