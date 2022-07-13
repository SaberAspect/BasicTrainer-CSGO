#include "Headers/Radar.h"
#include "../Headers/Enemy.h"
#include "../Headers/Player.h"
#include "../Headers/Offsets.h"
#include <Windows.h>
//Loop through entity list, make sure they are alive and other checks, set the bSpoted var to 1;

void Radar()
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("client.dll");

	Player* p = Player::getPlayer();
	int pTeam = *p->getTeam();
	for (int i = 1; i < *p->getMaxPlayers(); i++)
	{
		Enemy* currentEnt = Enemy::getEnemy(i);

		if (currentEnt->isAlive() && currentEnt->isValidEnt())
		{
			int enemyTeam = *currentEnt->getTeam();
			if (enemyTeam != pTeam)
			{
				*(bool*)(currentEnt + hazedumper::netvars::m_bSpotted) = true;
			}
		}
	}


}