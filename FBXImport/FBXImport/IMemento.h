#pragma once
#include <fbxsdk.h>

class IMemento abstract
{
public:	
	virtual ~IMemento() {}

	virtual void Load(FbxNode& fbxNode, void* pData) abstract;	// pData 추가 적인 정보를 보내기 위해 Animation Load 할때 AnimationLayer 정보가 필요함.
	virtual void Save(FbxNode& fbxNode) abstract;
};

//template<typename T>
//class IMemento abstract
//{
//public:
//	virtual ~IMemento() {}
//
//	virtual void Load(FbxNode* fbxNode) abstract;
//	virtual void Save(FbxNode* fbxNode) abstract;
//
//	virtual T* GetData() abstract;
//
//private:
//	T _data;
//};

