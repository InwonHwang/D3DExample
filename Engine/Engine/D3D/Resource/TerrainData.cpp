#include "TerrainData.h"
#include "..\Description\D3DDescription.h"

extern ResourceManager resourceManager;

TerrainData::TerrainData(ResourceHandle handle, ResourcePoolImpl* pool)
	: ResourceItem(handle, pool)
{
	_heightMap.reset(new std::vector<Vector3>);
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

	_verticeCount = width * width;
	_primitiveCount = (width - 1) * (width - 1) * 2;
	_width = width;
	_heightMap->resize(_verticeCount);

	int sizeVB = _verticeCount * sizeof(STATICMESHVERTEX);
	int sizeIB = _primitiveCount * sizeof(INDEX);
	float coordIncrementSize = 1.0f / (float)width;	// uv값 증가략

	if (!_vb->CreateVertexBuffer(device, sizeVB, 0, D3DPOOL_MANAGED))
		return false;

	if (!_ib->CreateIndexBuffer(device, sizeIB, 0, D3DPOOL_DEFAULT))
		return false;

	if (_vb->CreateVertexDeclaration(device, StaticMeshDec) == false)
		return false;

	void* pData = nullptr;
	if(_vb->Lock(0, sizeVB, &pData, 0))
	{
		STATICMESHVERTEX* pVertice = static_cast<STATICMESHVERTEX *>(pData);

		for (int z = 0; z < width; ++z)
		{
			for (int x = 0; x < width; ++x)
			{				
				int index = z * width + x;

				STATICMESHVERTEX vertex;
				float posX = (float)(x - width / 2);
				float posZ = (float)(z - width / 2);
				vertex.Position = Vector3(posX, height[index] * 0.1f, posZ);
				vertex.TexCoord = Vector2(x * coordIncrementSize, z * coordIncrementSize);				

				pVertice[index] = vertex;
				_heightMap->data()[index] = vertex.Position;
			}
		}

		_vb->Unlock();
	}

	pData = nullptr;
	if (_ib->Lock(0, sizeIB, &pData, 0))
	{
		INDEX* pIndice = static_cast<INDEX *>(pData);

		for (int z = 0; z < width - 1; ++z)
		{
			for (int x = 0; x < width - 1; x++)
			{
				INDEX index;
				
				index._0 = x + z * width;
				index._1 = x + (z + 1) * width;
				index._2 = (x + 1) + z * width;			

				*pIndice++ =  index;

				index._0 = x + (z + 1) * width;
				index._1 = (x + 1) + (z + 1) * width;
				index._2 = (x + 1) + z * width;				

				*pIndice++ = index;
			}
		}

		_vb->Unlock();
	}
	
	return true;
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
	
	std::vector<BYTE> heightMapVec(verticeCount);	// 높이맵 정보
	int width = (int)Mathf::Sqrt((float)verticeCount);	// 터레인 Plane 가로 정보 정사각형으로만 만들예정.
	stream.read((char*)&heightMapVec[0], heightMapVec.size());

	stream.close();

	Create(device, width, heightMapVec);

	return true;
}