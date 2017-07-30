#pragma once
#include "entity.h"

class CProjectile :
	public CEntity
{
public:
	void Move(void);
	CProjectile(float x, float y);
	~CProjectile(void);
};
