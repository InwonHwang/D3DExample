#include "Animation.h"


Animation::Animation()
	: IResourceItem(),
	_animationSet(NULL),
	_animationData(NULL)
{	
}

Animation::~Animation()
{		
	SAFE_DELETE_ARRAY(_animationData);
	SAFE_RELEASE(_animationSet)
}


void Animation::loadAnimationKey(LPD3DXFILEDATA xfileData, void* pData)
{
	BYTE* buffer = NULL;
	DWORD size = 0;
	HRESULT hr = 0;
	ANIMATIONDATA* animData = static_cast<ANIMATIONDATA *>(pData);

	if (SUCCEEDED(hr = xfileData->Lock(&size, (LPCVOID*)&buffer)))
	{
		DWORD keyType = ((DWORD*)buffer)[0];
		DWORD keyCount = ((DWORD*)buffer)[1];
		animData->NumKeys = keyCount;

		switch (keyType)
		{
		case 0:
		{
			animData->RotationKeys = new D3DXKEY_QUATERNION[keyCount];
			BYTE* tempBuffer = buffer + (sizeof(DWORD) * 2);

			for (DWORD i = 0; i < keyCount; ++i)
			{
				DWORD* time = (DWORD *)tempBuffer;
				D3DXQUATERNION* rotation = (D3DXQUATERNION *)(tempBuffer + (sizeof(DWORD) * 2));
				animData->RotationKeys[i].Time = (FLOAT)*time;
				animData->RotationKeys[i].Value = *rotation;
				tempBuffer += sizeof(D3DXQUATERNION) + sizeof(DWORD) * 2;
			}
		} break;
		case 1:
		{
			animData->ScaleKeys = new D3DXKEY_VECTOR3[keyCount];
			BYTE* tempBuffer = buffer + (sizeof(DWORD) * 2);

			for (DWORD i = 0; i < keyCount; ++i)
			{
				DWORD* time = (DWORD *)tempBuffer;
				D3DXVECTOR3* scale = (D3DXVECTOR3 *)(tempBuffer + (sizeof(DWORD) * 2));
				animData->PositionKeys[i].Time = (FLOAT)*time;
				animData->PositionKeys[i].Value = *scale;
				tempBuffer += sizeof(D3DXVECTOR3) + sizeof(DWORD) * 2;
			}
		} break;
		case 2:
		{
			animData->PositionKeys = new D3DXKEY_VECTOR3[keyCount];
			BYTE* tempBuffer = buffer + (sizeof(DWORD) * 2);

			for (DWORD i = 0; i < keyCount; ++i)
			{
				DWORD* time = (DWORD *)tempBuffer;
				D3DXVECTOR3* position = (D3DXVECTOR3 *)(tempBuffer + (sizeof(DWORD) * 2));
				animData->PositionKeys[i].Time = (FLOAT)*time;
				animData->PositionKeys[i].Value = *position;
				tempBuffer += sizeof(D3DXVECTOR3) + sizeof(DWORD) * 2;
			}
		} break;
		case 4:
		{
			animData->MatrixKeys = new D3DXKEY_MATRIX[keyCount];
			animData->ScaleKeys = new D3DXKEY_VECTOR3[keyCount];
			animData->RotationKeys = new D3DXKEY_QUATERNION[keyCount];
			animData->PositionKeys = new D3DXKEY_VECTOR3[keyCount];


			BYTE* tempBuffer = buffer + (sizeof(DWORD) * 2);

			for (DWORD i = 0; i < keyCount; ++i)
			{
				DWORD* time = (DWORD *)tempBuffer;
				D3DXMATRIX* matrix = (D3DXMATRIX *)(tempBuffer + (sizeof(DWORD) * 2));
				animData->MatrixKeys[i].Time = (FLOAT)*time;
				animData->MatrixKeys[i].Value = *matrix;

				animData->ScaleKeys[i].Time = animData->MatrixKeys[i].Time;
				animData->RotationKeys[i].Time = animData->MatrixKeys[i].Time;
				animData->PositionKeys[i].Time = animData->MatrixKeys[i].Time;
				D3DXMatrixDecompose(&animData->ScaleKeys[i].Value,
					&animData->RotationKeys[i].Value,
					&animData->PositionKeys[i].Value,
					&animData->MatrixKeys[i].Value);

				animData->RotationKeys[i].Value.x = -animData->RotationKeys[i].Value.x;
				animData->RotationKeys[i].Value.y = -animData->RotationKeys[i].Value.y;
				animData->RotationKeys[i].Value.z = -animData->RotationKeys[i].Value.z;
				animData->RotationKeys[i].Value.w = animData->RotationKeys[i].Value.w;

				tempBuffer += sizeof(D3DXMATRIX) + sizeof(DWORD) * 2;
			}
		} break;
		}

		xfileData->Unlock();
	}
}

void Animation::loadAnimationName(LPD3DXFILEDATA xfileData, void* pData)
{
	DWORD size = 0;
	HRESULT hr = 0;
	ANIMATIONDATA* animData = static_cast<ANIMATIONDATA *>(pData);

	if (SUCCEEDED(hr = xfileData->GetName(NULL, &size)))
	{
		if (size > 0) {
			animData->Name = new char[size];
			xfileData->GetName(animData->Name, &size);
		}
	}
}

void Animation::save()
{

}

void Animation::loadChild(LPD3DXFILEDATA xfileData, void* pData)
{
	HRESULT hr = 0;
	GUID type;

	hr = xfileData->GetType(&type);

	ANIMATIONDATA* animData = static_cast<ANIMATIONDATA *>(pData);

	if (type == TID_D3DRMAnimationKey)
	{
		loadAnimationKey(xfileData, pData);
	}
	else if (type == TID_D3DRMAnimationOptions)
	{
	}
	else if (type == TID_D3DRMFrame)
	{
		loadAnimationName(xfileData, pData);
	}

	if (type == TID_D3DRMAnimation)
	{
		SIZE_T childCount = 0;
		xfileData->GetChildren(&childCount);

		for (SIZE_T i = 0; i < childCount; i++)
		{
			LPD3DXFILEDATA data = NULL;

			if (SUCCEEDED(hr = xfileData->GetChild(i, &data)))
			{
				loadChild(data, animData);
				SAFE_RELEASE(data);
			}
		}
	}
}

void Animation::load(LPD3DXFILEDATA xfileData)
{
	HRESULT hr = 0;
	SIZE_T childCount = 0;
	xfileData->GetChildren(&childCount);

	_animationData = new ANIMATIONDATA[childCount];
	_numAnimation = childCount;

	for (SIZE_T i = 0; i < childCount; i++)
	{
		LPD3DXFILEDATA data = NULL;

		if (SUCCEEDED(hr = xfileData->GetChild(i, &data)))
		{
			loadChild(data, &_animationData[i]);
			SAFE_RELEASE(data);
		}
	}
}

void Animation::load(TCHAR *)
{


}

void Animation::create(char *animName, float speed, bool loop)
{
	D3DXPLAYBACK_TYPE playbackType;
	if (loop)
		playbackType = D3DXPLAY_LOOP;
	else
		playbackType = D3DXPLAY_ONCE;

	HRESULT hr = 0;

	if (!_animationData)
		hr = D3DXCreateKeyframedAnimationSet(animName, speed, playbackType, 160, 0, NULL, &_animationSet);
	else
	{
		hr = D3DXCreateKeyframedAnimationSet(animName, speed, playbackType, _numAnimation, 0, NULL, &_animationSet);

		for (DWORD i = 0; i < _numAnimation; i++)
		{
			_animationSet->RegisterAnimationSRTKeys(_animationData[i].Name,
				_animationData[i].NumKeys, _animationData[i].NumKeys, _animationData[i].NumKeys,
				_animationData[i].ScaleKeys, _animationData[i].RotationKeys, _animationData[i].PositionKeys,
				&_animationData[i].Index);
		}
	}
}
