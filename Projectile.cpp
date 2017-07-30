#include "Projectile.h"

CProjectile::CProjectile(float x, float y)
{
	this->PosX = x;
	this->PosY = y;
	this->Speed = 25;
	this->Height = 5;
	this->Width = 4;
}

void CProjectile::Move(void)
{
	this->PosY -= Speed;
}

CProjectile::~CProjectile(void)
{
}
