#pragma once
#include <fbxsdk.h>

class IMemento abstract
{
public:	
	virtual ~IMemento() {}

	virtual void Load(FbxNode& fbxNode) abstract;
	virtual void Save(FbxNode& fbxNode) abstract;		
	virtual void* GetData() abstract;
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

