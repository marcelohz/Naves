#include "Explosion.h"
#include <windows.h>

CExplosion::CExplosion(float x, float y)
{
	this->TimeStarted = GetTickCount();
	this->PosX = x;
	this->PosY = y;
	this->Height = 40;
	this->Width = 40;
}

CExplosion::~CExplosion(void)
{
}
