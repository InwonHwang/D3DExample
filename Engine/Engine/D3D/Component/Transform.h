#pragma once

#include "..\..\Core\Core.h"
#include "Component.h"
#include "D3DComponentImlp\TransformImpl.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	template<typename T> sp<T> GetComponent();
	template<typename T> sp<T> AddComponent();

	virtual void Destroy() override;

	// friend private 함수로 바꾸기
	void Update(IDirect3DDevice9* pDevice);
	void UpdateWorldMatrix();

	void SetLocalScale(const Vector3& inScale);
	void SetLocalRotation(const Quaternion& inRotation);
	void SetLocalPosition(const Vector3& inPosition);

	void GetLocalScale(Vector3& outLocalScale) const;
	void GetLocalRotation(Quaternion& outLocalRotation) const;
	void GetLocalEulerAngle(Vector3& outLocalEulerAngle) const;
	void GetLocalPosition(Vector3& outLocalPosition) const;

	void GetScale(Vector3& outScale) const;
	void GetRotation(Quaternion& outRotation) const;
	void GetEulerAngle(Vector3& outEulerAngle) const;
	void GetPosition(Vector3& outPosition) const;

	void GetAxisX(Vector3& outAxisX) const;
	void GetAxisY(Vector3& outAxisY) const;
	void GetAxisZ(Vector3& outAxisZ) const;

	// friend private 함수로 바꾸기
	void GetMatrixWorld(D3DXMATRIX& outMatWorld) const
	{
		outMatWorld = _impl->_matLocal * _impl->_matWorldParent;
	}

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

inline void Transform::Update(IDirect3DDevice9* pDevice)
{
	_impl->Update(pDevice);
}

inline void Transform::UpdateWorldMatrix()
{
	_impl->UpdateMatrix();
}

inline void Transform::SetLocalScale(const Vector3& scale)
{
	_impl->SetLocalScale(scale);
}

inline void Transform::SetLocalRotation(const Quaternion& rotation)
{
	_impl->SetLocalRotation(rotation);
}

inline void Transform::SetLocalPosition(const Vector3& position)
{
	_impl->SetLocalPosition(position);
}

inline void Transform::GetLocalScale(Vector3& outLocalScale) const
{
	_impl->GetLocalScale(outLocalScale);
}

inline void Transform::GetLocalRotation(Quaternion& outLocalRotation) const
{
	_impl->GetLocalRotation(outLocalRotation);
}
inline void Transform::GetLocalEulerAngle(Vector3& outLocalEulerAngle) const
{
	_impl->GetLocalEulerAngle(outLocalEulerAngle);
}
inline void Transform::GetLocalPosition(Vector3& outLocalPosition) const
{
	_impl->GetLocalPosition(outLocalPosition);
}

inline void Transform::GetScale(Vector3& outScale) const
{
	_impl->GetScale(outScale);
}
inline void Transform::GetRotation(Quaternion& outRotation) const
{
	_impl->GetRotation(outRotation);
}
inline void Transform::GetEulerAngle(Vector3& outEulerAngle) const
{
	_impl->GetEulerAngle(outEulerAngle);
}
inline void Transform::GetPosition(Vector3& outPosition) const
{
	_impl->GetPosition(outPosition);
}

inline void Transform::GetAxisX(Vector3& outAxisX) const
{
	_impl->GetAxisX(outAxisX);
}
inline void Transform::GetAxisY(Vector3& outAxisY) const
{
	_impl->GetAxisY(outAxisY);
}
inline void Transform::GetAxisZ(Vector3& outAxisZ) const
{
	_impl->GetAxisZ(outAxisZ);
}