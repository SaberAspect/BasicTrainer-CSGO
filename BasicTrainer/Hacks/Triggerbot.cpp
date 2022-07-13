#include "Headers/Triggerbot.h"
#include "../Headers/Player.h"
#include "../Headers/Enemy.h"
#include "../Headers/Offsets.h"
#include <Windows.h>

//CrosshairID
//dwForceAttack

//If current ent in Id -> team is equal to localPlayer team, continue
//else dwForceAttack -> 6

//Have a sleep system to reset the forceAttack val so that it keeps shooting

void Triggerbot()
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("client.dll");
	Player* p = Player::getPlayer();
	int crosshairID = *p->getCrosshairID();
	Enemy* currentEnt = nullptr;
	if (crosshairID > 0 && crosshairID < 64)
		currentEnt = Enemy::getEnemy(crosshairID - 1);

	if (currentEnt->isValidEnt() && currentEnt->isAlive())
	{
		if (*currentEnt->getTeam() != *p->getTeam())
		{
			Sleep(5);
			*(int*)((moduleBase + hazedumper::signatures::dwForceAttack)) = 6;
			Sleep(5);
			*(int*)((moduleBase + hazedumper::signatures::dwForceAttack)) = 4;
		}
	}
}