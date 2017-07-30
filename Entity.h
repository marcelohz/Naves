#pragma once

class CEntity
{
public:
	long Height, Width;
	float PosX, PosY;
	long Speed;
	virtual void Move(void);
	CEntity(void);
	~CEntity(void);
};
