#include "Particle.h"
#include "Device.h"

const DWORD ParticleVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

Particle::Particle() : _vb(NULL), _tex(NULL)
{
}

Particle::~Particle()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_tex);
}

bool Particle::init(TCHAR * texFileName)
{
	HRESULT hr = 0;

	hr = D3DDevice->CreateVertexBuffer(_vbSize * sizeof(ParticleVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ParticleVertex::FVF,
		D3DPOOL_DEFAULT,
		&_vb,
		0);

	if (FAILED(hr))
	{
		DebugError(hr);
		return false;	
	}

	hr = D3DXCreateTextureFromFile(D3DDevice, texFileName, &_tex);

	if (FAILED(hr))
	{
		DebugError(hr);
		return false;
	}

	return true;
}

void Particle::reset()
{
	std::list<Attribute>::iterator it;

	for (it = _particles.begin(); it != _particles.end(); ++it)
	{
		resetParticle(&(*it));
	}
}

void Particle::addParticle()
{
	Attribute attribute;
	resetParticle(&attribute);
	_particles.push_back(attribute);
}

void Particle::preRender()
{
	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	D3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(_size));
	D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	

	// use alpha from texture
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Particle::postRender()
{
	D3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void Particle::render()
{
	if (!_particles.empty())
	{
		preRender();

		D3DDevice->SetTexture(0, _tex);
		D3DDevice->SetFVF(ParticleVertex::FVF);
		D3DDevice->SetStreamSource(0, _vb, 0, sizeof(ParticleVertex));
		
		if (_vbOffset >= _vbSize) _vbOffset = 0;

		ParticleVertex* v = 0;

		_vb->Lock(_vbOffset * sizeof(ParticleVertex), _vbBatchSize * sizeof(ParticleVertex),
			(void**)&v, _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		std::list<Attribute>::iterator it;
		for (it = _particles.begin(); it != _particles.end(); ++it)
		{
			if (it->_isAlive)
			{
				v->_position = it->_position;
				v->_color = (D3DCOLOR)it->_color;
				v++;

				numParticlesInBatch++;

				if (numParticlesInBatch == _vbBatchSize)
				{
					_vb->Unlock();
					D3DDevice->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, _vbBatchSize);

					_vbOffset += _vbBatchSize;

					if (_vbOffset >= _vbSize) _vbOffset = 0;

					_vb->Lock(_vbOffset * sizeof(ParticleVertex), _vbBatchSize * sizeof(ParticleVertex),
						(void**)&v, _vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0;
				}
			}			
		}

		_vb->Unlock();

		if (numParticlesInBatch)
		{
			D3DDevice->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, numParticlesInBatch);
		}

		_vbOffset += _vbBatchSize;

		postRender();
	}
}

bool Particle::isEmpty()
{
	return _particles.empty();
}

bool Particle::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{		
		if (i->_isAlive) return false;
	}

	return true;
}


void Particle::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = _particles.begin();

	while (i != _particles.end())
	{
		if (i->_isAlive == false)		
			i = _particles.erase(i);		
		else		
			i++;		
	}
}

Snow::Snow(BoundingBox* boundingBox, int numParticles)
{
	_boundingBox = *boundingBox;
	_size = 0.25f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Snow::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	GetRandomVector(&attribute->_position, &_boundingBox._min, &_boundingBox._max);

	attribute->_position.y = _boundingBox._max.y;
	attribute->_velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->_velocity.z = 0.0f;
	attribute->_color = D3DCOLOR_XRGB(255, 255, 255);
}

void Snow::update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;

		if (_boundingBox.isPointInside(i->_position) == false)
		{
			resetParticle(&(*i));
		}
	}
}

//*****************************************************************************
// Explosion System
//********************

Firework::Firework(D3DXVECTOR3* origin, int numParticles)
{
	_origin = *origin;
	_size = 0.9f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void Firework::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;
	attribute->_position = _origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= 100.0f;

	attribute->_color = D3DXCOLOR(
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		1.0f);

	attribute->_age = 0.0f;
	attribute->_lifeTime = 2.0f;
}

void Firework::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = _particles.begin(); i != _particles.end(); i++)
	{		
		if (i->_isAlive)
		{
			i->_position += i->_velocity * timeDelta;

			i->_age += timeDelta;

			if (i->_age > i->_lifeTime) 
				i->_isAlive = false;
		}
	}
}

void Firework::preRender()
{
	Particle::preRender();

	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Firework::postRender()
{
	Particle::postRender();

	D3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
