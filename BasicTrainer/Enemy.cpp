#include "Headers/Enemy.h"
#include <Windows.h>
#include "Headers/Offsets.h"


Enemy* Enemy::getEnemy(int index)
{
	uintptr_t moduleBase = (uintptr_t)(GetModuleHandle("client.dll"));
	Enemy* currentEnemy = (Enemy*)(moduleBase + hazedumper::signatures::dwEntityList + (0x10 * index));
	if (currentEnemy == NULL)
		while (currentEnemy == NULL)
			currentEnemy = (Enemy*)(moduleBase + hazedumper::signatures::dwEntityList + (0x10 * index));
	return currentEnemy;
}
vec3* Enemy::getOrigin()
{
	return (vec3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}
int* Enemy::getHealth()
{
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iHealth);
}
int* Enemy::getTeam()
{
	int* team;
	if (this->isValidEnt())
		team = (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum);
	else {
		team = nullptr;
	}
	return team;

}
vec3* Enemy::getBonePos(int boneID)
{
	uintptr_t boneMatrix = *(uintptr_t*)(*(uintptr_t*)this + hazedumper::netvars::m_dwBoneMatrix);
	static vec3 bonePos = { 0,0,0 };
	if (boneMatrix) {
		bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
		bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
		bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
	}
	return &bonePos;
}
bool Enemy::isValidEnt()
{
	if (!this || !*(uintptr_t*)this || this == nullptr)
	{
		return false;
	}
	return true;
}
bool Enemy::isAlive()
{
	if (*this->getHealth() <= 0 || *this->getHealth() > 100)
		return false;
	return true;
}
bool Enemy::isDefusing()
{
	bool* defusing = (bool*)(*(uintptr_t*)this + hazedumper::netvars::m_bIsDefusing);
	if (*defusing)
		return true;
	else
		return false;
	return false;
}

