#pragma once
#include "entity.h"

class CExplosion :
	public CEntity
{
public:
	long TimeStarted;
	CExplosion(float x, float y);
	~CExplosion(void);
};
