#include "naves.h"
#include "Enemy.h"
#include <stdlib.h>
#include <windows.h>

CEnemy::CEnemy(void)
{
	srand ( GetTickCount() );
	this->Height = 30;
	this->Width = 30;
	this->Speed = 1 + rand()%7;
	this->PosX = (float)(this->Width + rand() % (SCREENX - this->Width*2));
	this->PosY = 1;

}

void CEnemy::Move(void)
{
	this->PosY += this->Speed;
}

CEnemy::~CEnemy(void)
{
}
