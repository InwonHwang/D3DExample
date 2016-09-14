#pragma once
#include "stdafx.h"

class GameObject;
class IWEVector3;
class IWEQauternion;

class Transform
{	
private:
	IWEVector3 _position;
	IWEVector3 _localPosition;
	IWEQuaternion _rotation;
	IWEQuaternion _localRotation;
	IWEVector3 _scale;
	IWEVector3 _localScale;
		
	D3DXMATRIX _matPosition;
	D3DXMATRIX _matRotation;
	D3DXMATRIX _matScale;
	D3DXMATRIX _world;

	Transform * _parent;
	std::list<Transform *> _children;

public:
	Transform();
	~Transform();

	IWEVector3 getPosition() { return _position; }
	IWEVector3 getLocalPosition() { return _localPosition; }
	IWEQuaternion& getRotation() { return _rotation; }
	IWEQuaternion& getLocalRotation() { return _localRotation; }
	IWEVector3 getScale() { return _scale; }
	IWEVector3 getLocalScale() { return _localScale; }

	void setPosition(const IWEVector3&);
	void setLocalPosition(const IWEVector3&);
	void setRotation(const IWEQuaternion&);
	void setLocalRotation(const IWEQuaternion&);
	void setScale(const IWEVector3&);
	void setLocalScale(const IWEVector3&);

	void update();

};

//class Transform
//{
//public:
//	D3DXVECTOR3 position;
//	D3DXQUATERNION rotation;
//	D3DXVECTOR3 scale;
//	
//	D3DXMATRIX matrix;
//	GameObject* gameObject;
//	
//	void update();
//
//public:
//	Transform();
//	~Transform();
//	
//};

