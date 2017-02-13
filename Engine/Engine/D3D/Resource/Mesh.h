#pragma once
#include "..\..\Core\Core.h"
#include "..\ResourceBase\D3DResourceBase.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	sp<VertexBuffer> _vb;
	sp<IndexBuffer>	 _ib;

	bool _isSkinned;
	int  _vertexCount;
	int  _primitiveCount;
};

