#pragma once
#include "Entity.h"

class CEnemy :
	public CEntity
{
public:
	void Move(void);
	CEnemy(void);
	~CEnemy(void);
};
