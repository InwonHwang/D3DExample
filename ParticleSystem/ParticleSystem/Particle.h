#pragma once

#include "stdafx.h"

struct ParticleVertex
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;
	static const DWORD FVF;
};

struct Attribute
{
	Attribute()
	{
		_lifeTime = 0.0f;
		_age = 0.0f;
		_isAlive = true;
	}

	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float       _lifeTime;
	float       _age;      
	D3DXCOLOR   _color;  
	D3DXCOLOR   _colorFade; 
	bool        _isAlive;
};

class Particle
{
public:
	Particle();
	virtual ~Particle();

	virtual bool init(TCHAR* texFileName);
	virtual void reset();

	virtual void resetParticle(Attribute* attribute) = 0;
	virtual void addParticle();

	virtual void update(float timeDelta) = 0;

	virtual void preRender();
	virtual void render();
	virtual void postRender();

	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticles();

protected:
	D3DXVECTOR3		_origin;
	BoundingBox		_boundingBox;
	float			_emitRate;
	float			_size;
	IDirect3DTexture9*	_tex;
	IDirect3DVertexBuffer9*	_vb;
	std::list<Attribute>	_particles;
	int				_maxParticles;

	DWORD _vbSize;
	DWORD _vbOffset;
	DWORD _vbBatchSize;

};

class Snow : public Particle
{
public:
	Snow(BoundingBox* boundingBox, int numParticles);
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
};

class Firework : public Particle
{
public:
	Firework(D3DXVECTOR3* origin, int numParticles);
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
	void preRender();
	void postRender();
};