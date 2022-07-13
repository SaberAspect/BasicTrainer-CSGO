#include "Headers/Aimbot.h"
#include "../Headers/Player.h"
#include <Windows.h>

// closestEnemy
Enemy* getClosestEnemy()
{
	Player* p = Player::getPlayer();

	double closestDistance = 9999999;
	int index = -1;
	//Looping through Entity Array 
	for (int i = 1; i < *p->getMaxPlayers(); i++) {
		//Getting the I entity in the array
		Enemy* currentEnemy = Enemy::getEnemy(i);
		//Checking to see if the current entity is valid
		if (!currentEnemy->isValidEnt())
			continue;
		//More checks
		if (!currentEnemy->isAlive() || *currentEnemy->getTeam() == *p->getTeam())
			continue;

		//calc the distance of the current enemy and player
		double dist = p->getDistance(*currentEnemy->getOrigin());
		if (dist < closestDistance)
		{
			closestDistance = dist;
			index = i;
		}
	}
	if (index == -1)
		return NULL;
	return Enemy::getEnemy(index);

}
Enemy* getLowestHealthEnemy() {

	Player* p = Player::getPlayer();

	double maxHealth = 99999;
	int index = -1;
	//Looping through Entity Array 
	for (int i = 1; i < *p->getMaxPlayers(); i++) {
		//Getting the I entity in the array
		Enemy* currentEnemy = Enemy::getEnemy(i);
		//Checking to see if the current entity is valid
		if (!currentEnemy->isValidEnt())
			continue;
		if ((uintptr_t)currentEnemy == (uintptr_t)p)
		{
			//If is error skip all the code in the loop
			continue;
		}
		//More checks
		if (!currentEnemy->isAlive() || *currentEnemy->getTeam() == *p->getTeam())
			continue;
		//calc the distance of the current enemy and player
		double health = *currentEnemy->getHealth();
		if (health < maxHealth)
		{
			maxHealth = health;
			index = i;
		}
	}
	if (index == -1 || maxHealth == 99999)
		return NULL;
	return Enemy::getEnemy(index);
}
bool IsGoodTarget(Enemy* entity)
{
	//Trace Ray

	//Checks
	if (entity != NULL && entity->isAlive() && entity->isValidEnt())
		return true;
	return false;
}
void aimBot(bool smooth, float smoothness, bool fov, int fovRange, int bone, bool key, int keyCode, bool sort, int sortingAlgo)
{
	Player* p = Player::getPlayer();

	if (sort)
	{
		Enemy* closestEnemy = getClosestEnemy();
		Enemy* lowestHealth = getLowestHealthEnemy();
		if (sortingAlgo == 0) {
			if (IsGoodTarget(closestEnemy))
			{
				if (key)
				{
					if (GetKeyState(keyCode) & 0x8000)
						p->aimAt(closestEnemy->getBonePos(bone), smooth, smoothness, fov, fovRange);
				}
				else
					p->aimAt(closestEnemy->getBonePos(bone), smooth, smoothness, fov, fovRange);
			}
		}
		if (sortingAlgo == 1)
		{
			if (IsGoodTarget(lowestHealth))
			{
				if (key)
				{
					if (GetKeyState(keyCode) & 0x8000)
						p->aimAt(lowestHealth->getBonePos(bone), smooth, smoothness, fov, fovRange);
				}
				else
					p->aimAt(lowestHealth->getBonePos(bone), smooth, smoothness, fov, fovRange);
			}
		}
	}
	else
	{
		for (int i = 1; i < *p->getMaxPlayers(); i++) {
			//Getting the I entity in the array
			Enemy* currentEnemy = Enemy::getEnemy(i);
			//Checking to see if the current entity is valid
			if (!currentEnemy->isValidEnt())
				continue;
			if ((uintptr_t)currentEnemy == (uintptr_t)p)
			{
				//If is error skip all the code in the loop
				continue;
			}
			//More checks
			if (!currentEnemy->isAlive() || *currentEnemy->getTeam() == *p->getTeam())
				continue;

			if (key)
			{
				if (GetKeyState(keyCode) & 0x8000)
					p->aimAt(currentEnemy->getBonePos(bone), smooth, smoothness, fov, fovRange);
			}
			else
			{
				p->aimAt(currentEnemy->getBonePos(bone), smooth, smoothness, fov, fovRange);
			}
		}
	}
}