#include "MyTerrain.h"
#include "Device.h"
#include <fstream>
#include <cmath>

const DWORD MyTerrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

MyTerrain::MyTerrain(std::string heightmapFileName,
	int numVertsPerRow,
	int numVertsPerCol,
	int cellSpacing,
	float heightScale)
{
	_numVertsPerRow = numVertsPerRow;
	_numVertsPerCol = numVertsPerCol;
	_cellSpacing = cellSpacing;

	_numCellsPerRow = _numVertsPerRow - 1;
	_numCellsPerCol = _numVertsPerCol - 1;

	_width = _numCellsPerRow * _cellSpacing;
	_depth = _numCellsPerCol * _cellSpacing;

	_numVertices = _numVertsPerRow * _numVertsPerCol;
	_numTriangles = _numCellsPerRow * _numCellsPerCol * 2;

	_heightScale = heightScale;

	// load heightmap
	if (!readRawFile(heightmapFileName))
	{
		DebugBox(0, _T("readRawFile - FAILED"));
		::PostQuitMessage(0);
	}

	// scale heights
	for (int i = 0; i < _heightmap.size(); i++)
		_heightmap[i] *= heightScale;

	// compute the vertices
	if (!computeVertices())
	{
		DebugBox(0, _T("computeVertices - FAILED"));
		::PostQuitMessage(0);
	}

	// compute the indices
	if (!computeIndices())
	{
		DebugBox(0, _T("computeIndices - FAILED"));
		::PostQuitMessage(0);
	}
}

MyTerrain::~MyTerrain()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
	SAFE_RELEASE(_tex);
}

int MyTerrain::getHeightmapEntry(int row, int col)
{
	return _heightmap[row * _numVertsPerRow + col];
}

void MyTerrain::setHeightmapEntry(int row, int col, int value)
{
	_heightmap[row * _numVertsPerRow + col] = value;
}

bool MyTerrain::computeVertices()
{
	HRESULT hr = 0;

	hr = D3DDevice->CreateVertexBuffer(
		_numVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	if (FAILED(hr))
		return false;

	// coordinates to start generating vertices at
	int startX = -_width / 2;
	int startZ = _depth / 2;

	// coordinates to end generating vertices at
	int endX = _width / 2;
	int endZ = -_depth / 2;

	// compute the increment size of the texture coordinates
	// from one vertex to the next.
	float uCoordIncrementSize = 1.0f / (float)_numCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)_numCellsPerCol;

	TerrainVertex* v = 0;
	_vb->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= _cellSpacing)
	{
		int j = 0;
		for (int x = startX; x <= endX; x += _cellSpacing)
		{
			// compute the correct index into the vertex buffer and heightmap
			// based on where we are in the nested loop.
			int index = i * _numVertsPerRow + j;

			v[index] = TerrainVertex(
				(float)x,
				(float)_heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);

			j++; // next column
		}
		i++; // next row
	}

	_vb->Unlock();

	return true;
}

bool MyTerrain::computeIndices()
{
	HRESULT hr = 0;

	hr = D3DDevice->CreateIndexBuffer(
		_numTriangles * 3 * sizeof(WORD), // 3 indices per triangle
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	if (FAILED(hr))
		return false;

	WORD* indices = 0;
	_ib->Lock(0, 0, (void**)&indices, 0);

	// index to start of a group of 6 indices that describe the
	// two triangles that make up a quad
	int baseIndex = 0;

	// loop through and compute the triangles of each quad
	for (int i = 0; i < _numCellsPerCol; i++)
	{
		for (int j = 0; j < _numCellsPerRow; j++)
		{
			indices[baseIndex] = i   * _numVertsPerRow + j;
			indices[baseIndex + 1] = i   * _numVertsPerRow + j + 1;
			indices[baseIndex + 2] = (i + 1) * _numVertsPerRow + j;

			indices[baseIndex + 3] = (i + 1) * _numVertsPerRow + j;
			indices[baseIndex + 4] = i   * _numVertsPerRow + j + 1;
			indices[baseIndex + 5] = (i + 1) * _numVertsPerRow + j + 1;

			// next quad
			baseIndex += 6;
		}
	}

	_ib->Unlock();

	return true;
}

bool MyTerrain::loadTexture(std::tstring fileName)
{
	HRESULT hr = 0;

	hr = D3DXCreateTextureFromFile(
		D3DDevice,
		fileName.c_str(),
		&_tex);

	if (FAILED(hr))
		return false;

	return true;
}

bool MyTerrain::genTexture(D3DXVECTOR3* directionToLight)
{
	// Method fills the top surface of a texture procedurally.  Then
	// lights the top surface.  Finally, it fills the other mipmap
	// surfaces based on the top surface data using D3DXFilterTexture.

	HRESULT hr = 0;

	// texel for each quad cell
	int texWidth = _numCellsPerRow;
	int texHeight = _numCellsPerCol;

	// create an empty texture
	hr = D3DXCreateTexture(
		D3DDevice,
		texWidth, texHeight,
		0, // create a complete mipmap chain
		0, // usage
		D3DFMT_X8R8G8B8,// 32 bit XRGB format
		D3DPOOL_MANAGED, &_tex);

	if (FAILED(hr))
		return false;

	D3DSURFACE_DESC textureDesc;
	_tex->GetLevelDesc(0 /*level*/, &textureDesc);

	// make sure we got the requested format because our code 
	// that fills the texture is hard coded to a 32 bit pixel depth.
	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(0/*lock top surface*/, &lockedRect,
		0 /* lock entire tex*/, 0/*flags*/);

	DWORD* imageData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < texHeight; i++)
	{
		for (int j = 0; j < texWidth; j++)
		{
			D3DXCOLOR c;

			// get height of upper left vertex of quad.
			float height = (float)getHeightmapEntry(i, j) / _heightScale;

			if ((height) < 42.5f) 		 c = D3DCOLOR_XRGB(255, 249, 157);
			else if ((height) < 85.0f)	 c = D3DCOLOR_XRGB(124, 197, 118);
			else if ((height) < 127.5f) c = D3DCOLOR_XRGB(0, 166, 81);
			else if ((height) < 170.0f) c = D3DCOLOR_XRGB(25, 123, 48);
			else if ((height) < 212.5f) c = D3DCOLOR_XRGB(115, 100, 87);
			else	                     c = D3DCOLOR_XRGB(255, 255, 255);

			// fill locked data, note we divide the pitch by four because the
			// pitch is given in bytes and there are 4 bytes per DWORD.
			imageData[i * lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

	if (!lightTerrain(directionToLight))
	{
		DebugBox(0, _T("lightTerrain() - FAILED"));		
		return false;
	}

	hr = D3DXFilterTexture(
		_tex,
		0, // default palette
		0, // use top level as source level
		D3DX_DEFAULT); // default filter

	if (FAILED(hr))
	{
		DebugBox(0, _T("D3DXFilterTexture() - FAILED"));
		return false;
	}

	return true;
}

bool MyTerrain::lightTerrain(D3DXVECTOR3* directionToLight)
{
	HRESULT hr = 0;

	D3DSURFACE_DESC textureDesc;
	_tex->GetLevelDesc(0 /*level*/, &textureDesc);

	// make sure we got the requested format because our code that fills the
	// texture is hard coded to a 32 bit pixel depth.
	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	_tex->LockRect(
		0,          // lock top surface level in mipmap chain
		&lockedRect,// pointer to receive locked data
		0,          // lock entire texture image
		0);         // no lock flags specified

	DWORD* imageData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < textureDesc.Height; i++)
	{
		for (int j = 0; j < textureDesc.Width; j++)
		{
			// index into texture, note we use the pitch and divide by 
			// four since the pitch is given in bytes and there are 
			// 4 bytes per DWORD.
			int index = i * lockedRect.Pitch / 4 + j;

			// get current color of quad
			D3DXCOLOR c(imageData[index]);

			// shade current quad
			c *= computeShade(i, j, directionToLight);;

			// save shaded color
			imageData[index] = (D3DCOLOR)c;
		}
	}

	_tex->UnlockRect(0);

	return true;
}

float MyTerrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight)
{
	// get heights of three vertices on the quad
	float heightA = getHeightmapEntry(cellRow, cellCol);
	float heightB = getHeightmapEntry(cellRow, cellCol + 1);
	float heightC = getHeightmapEntry(cellRow + 1, cellCol);

	// build two vectors on the quad
	D3DXVECTOR3 u(_cellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, -_cellSpacing);

	// find the normal by taking the cross product of two
	// vectors on the quad.
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, directionToLight);

	if (cosine < 0.0f)
		cosine = 0.0f;

	return cosine;
}

bool MyTerrain::readRawFile(std::string fileName)
{
	// Restriction: RAW file dimensions must be >= to the
	// dimensions of the terrain.  That is a 128x128 RAW file
	// can only be used with a terrain constructed with at most
	// 128x128 vertices.

	// A height for each vertex
	std::vector<BYTE> in(_numVertices);

	std::ifstream inFile(fileName.c_str(), std::ios_base::binary);

	if (!inFile)
		return false;

	inFile.read(
		(char*)&in[0], // buffer
		in.size());// number of bytes to read into buffer

	inFile.close();

	// copy BYTE vector to int vector
	_heightmap.resize(_numVertices);

	for (int i = 0; i < in.size(); i++)
		_heightmap[i] = in[i];

	return true;
}

float MyTerrain::getHeight(float x, float z)
{
	// Translate on xz-plane by the transformation that takes
	// the terrain START point to the origin.
	x = ((float)_width / 2.0f) + x;
	z = ((float)_depth / 2.0f) - z;

	// Scale down by the transformation that makes the 
	// cellspacing equal to one.  This is given by 
	// 1 / cellspacing since; cellspacing * 1 / cellspacing = 1.
	x /= (float)_cellSpacing;
	z /= (float)_cellSpacing;

	// From now on, we will interpret our positive z-axis as
	// going in the 'down' direction, rather than the 'up' direction.
	// This allows to extract the row and column simply by 'flooring'
	// x and z:

	float col = ::floorf(x);
	float row = ::floorf(z);

	// get the heights of the quad we're in:
	// 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D

	float A = getHeightmapEntry(row, col);
	float B = getHeightmapEntry(row, col + 1);
	float C = getHeightmapEntry(row + 1, col);
	float D = getHeightmapEntry(row + 1, col + 1);

	//
	// Find the triangle we are in:
	//

	// Translate by the transformation that takes the upper-left
	// corner of the cell we are in to the origin.  Recall that our 
	// cellspacing was nomalized to 1.  Thus we have a unit square
	// at the origin of our +x -> 'right' and +z -> 'down' system.
	float dx = x - col;
	float dz = z - row;

	// Note the below compuations of u and v are unneccessary, we really
	// only need the height, but we compute the entire vector to emphasis
	// the books discussion.
	float height = 0.0f;
	if (dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {A}, plus the heights
						  // found by interpolating on each vector u and v.
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B

						  // Linearly interpolate on each vector.  The height is the vertex
						  // height the vectors u and v originate from {D}, plus the heights
						  // found by interpolating on each vector u and v.
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}

bool MyTerrain::draw(D3DXMATRIX* world, bool drawTris)
{
	HRESULT hr = 0;

	if (D3DDevice)
	{
		D3DDevice->SetTransform(D3DTS_WORLD, world);

		D3DDevice->SetStreamSource(0, _vb, 0, sizeof(TerrainVertex));
		D3DDevice->SetFVF(TerrainVertex::FVF);
		D3DDevice->SetIndices(_ib);

		D3DDevice->SetTexture(0, _tex);

		// turn off lighting since we're lighting it ourselves
		D3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		hr = D3DDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			_numVertices,
			0,
			_numTriangles);

		D3DDevice->SetRenderState(D3DRS_LIGHTING, true);

		if (drawTris)
		{
			D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr = D3DDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				_numVertices,
				0,
				_numTriangles);

			D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		if (FAILED(hr))
			return false;
	}

	return true;
}

