#include "TerrainData.h"
#include "..\Description\D3DDescription.h"

extern ResourceManager resourceManager;

TerrainData::TerrainData(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool),
	_vb(nullptr),
	_ib(nullptr)
{
}


TerrainData::~TerrainData()
{
	
}

void TerrainData::Destroy()
{
	_vb.reset();
	_ib.reset();
	_heightMap.reset();
}

bool TerrainData::Create(IDirect3DDevice9& device, int width, const std::vector<BYTE>& height)
{
	assert(width % 2 == 0);

	_vb = resourceManager.Create<VertexBuffer>(); // VertexBuffer Pool로 VertexBuffer만들기	
	_ib = resourceManager.Create<IndexBuffer>(); // IndexBuffer Pool로 IndexBuffer만들기	

	_width = width + 1;	// QuadTree를 만들 때 분할 과정중 겹치는 부분이 없게 만들기 위해 ( 짝수일 때 처음 분할때 겹치는 부분 발생 )
	_verticeCount = _width * _width;

	_heightMap.reset(new HeightMapVec);
	_heightMap->resize(_verticeCount);

	int sizeVB = _verticeCount * sizeof(STATICMESHVERTEX);
	int sizeIB = (_width - 1) * (_width - 1) * 2 * sizeof(INDEX);
	float coordIncrementSize = 1.0f / (float)_width;	// uv값 증가략

	if (!_vb->CreateVertexBuffer(device, sizeVB, 0, D3DPOOL_MANAGED))
		return false;	

	if (!_ib->CreateIndexBuffer(device, sizeIB, 0, D3DPOOL_DEFAULT))
		return false;

	if (!_vb->CreateVertexDeclaration(device, StaticMeshDec))	
		return false;

	int indexVertex;
	int indexHeightMap;

	void* pData = nullptr;
	if (_vb->Lock(0, sizeVB, &pData, 0))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

		for (int z = 0; z < width; ++z)
		{
			for (int x = 0; x < width; ++x)
			{
				indexVertex = x + z * _width; //
				indexHeightMap = x + z * width;	 // HeightMap Index;

				STATICMESHVERTEX vertex;
				float posX = (float)(x - width / 2);
				float posZ = (float)(z - width / 2);
				vertex.Position = Vector3(posX, height[indexHeightMap] * 0.1f, posZ);
				vertex.TexCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);

				pVertice[indexVertex] = vertex;
				_heightMap->data()[indexVertex] = vertex.Position;
			}
		}

		for (int z = 0; z < _width; ++z)
		{
			indexVertex = width + z * _width;
			pVertice[width + z * _width].Position = _heightMap->data()[width + z * _width - 1] + Vector3(1, 0, 0);
			_heightMap->data()[indexVertex] = pVertice[indexVertex].Position;
		}

		for (int x = 0; x < _width; ++x)
		{
			indexVertex = x + width * _width;
			pVertice[indexVertex].Position = _heightMap->data()[x + (width - 1) * _width] +Vector3(0, 0, 1);
			_heightMap->data()[indexVertex] = pVertice[indexVertex].Position;
		}

		_vb->Unlock();
	}

	return true;

	//assert(width % 2 == 0);
	//	
	//_width = width;
	//_vb = resourceManager.Create<VertexBuffer>(); // VertexBuffer Pool로 VertexBuffer만들기	
	//_ib = resourceManager.Create<IndexBuffer>(); // IndexBuffer Pool로 IndexBuffer만들기

	//_verticeCount = width * width;
	//_heightMap.reset(new HeightMapVec);
	//_heightMap->resize(_verticeCount);

	//int sizeVB = _verticeCount * sizeof(STATICMESHVERTEX);
	//int sizeIB = (width - 1) * (width - 1) * 2 * sizeof(INDEX);

	//float coordIncrementSize = 1.0f / (float)width;	// uv값 증가략

	//if (!_vb->CreateVertexBuffer(device, sizeVB, 0, D3DPOOL_MANAGED))
	//	return false;

	//if (!_ib->CreateIndexBuffer(device, sizeIB, 0, D3DPOOL_DEFAULT))
	//	return false;

	//if (_vb->CreateVertexDeclaration(device, StaticMeshDec) == false)
	//	return false;

	//void* pData = nullptr;
	//if(_vb->Lock(0, sizeVB, &pData, 0))
	//{
	//	STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

	//	for (int z = 0; z < width; ++z)
	//	{
	//		for (int x = 0; x < width; ++x)
	//		{				
	//			int index = x + z * width;

	//			STATICMESHVERTEX vertex;
	//			float posX = (float)(x - width / 2);
	//			float posZ = (float)(z - width / 2);
	//			vertex.Position = Vector3(posX, height[index] * 0.1f, posZ);
	//			vertex.TexCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);				

	//			pVertice[index] = vertex;
	//			_heightMap->data()[index] = vertex.Position;
	//		}
	//	}

	//	_vb->Unlock();
	//}

	//pData = nullptr;
	//if (_ib->Lock(0, sizeIB, &pData, 0))
	//{
	//	INDEX* pIndice = static_cast<INDEX *>(pData);

	//	for (int z = 0; z < width - 1 ; ++z)
	//	{
	//		for (int x = 0; x < width - 1; ++x)
	//		{
	//			INDEX index;
	//			
	//			index._0 = x + z * width;
	//			index._1 = (x + 1) + z * width;
	//			index._2 = x + (z + 1) * width;

	//			*pIndice++ =  index;

	//			index._0 = x + (z + 1) * width;
	//			index._1 = (x + 1) + z * width;
	//			index._2 = (x + 1) + (z + 1) * width;
	//			

	//			*pIndice++ = index;
	//		}
	//	}

	//	_ib->Unlock();
	//}
	//
	//return true;
}

bool TerrainData::Create(IDirect3DDevice9& device, const String& fileName)
{
#ifdef _UNICODE
	std::string path;
	path.assign(fileName.begin(), fileName.end());
#elif
	std::string path = fileName;
#endif

	std::ifstream stream;
	stream.open(path.c_str(), std::ios::binary);

	if (stream.fail())
		return false;

	// vertex 개수 구하기
	stream.seekg(0, std::ios::end);
	int verticeCount = (int)stream.tellg();
	stream.seekg(0, std::ios::beg);
	
	
	int width = (int)Mathf::Sqrt((float)verticeCount);	// 터레인 Plane 가로 정보 정사각형으로만 만들예정.
	std::vector<BYTE> HeightMapVec(verticeCount);	// 높이맵 정보
	stream.read((char*)&HeightMapVec[0], verticeCount);

	stream.close();

	Create(device, width, HeightMapVec);

	return true;
}