#include "Headers/Glow.h"
#include "../Headers/Offsets.h"
#include "../Headers/Player.h"
#include "../Headers/Enemy.h"
#include <iostream>



struct GlowStruct
{
	BYTE buff[8];
	float red; //0x8
	float green; //0xc
	float blue; //0x10
	float alpha; //0x14
	BYTE buff1[16];
	bool renderWhenOccluded; //0x28
	bool renderWhenUnOccluded; //0x29
	bool fullBloom;
	BYTE buff2[5];
	int glowStyle;
};
struct ClrRender
{
	BYTE red, green, blue;
};
GlowStruct SetGlowColor(GlowStruct Glow, Enemy* entity)
{

	if (entity->isDefusing())
	{
		Glow.red = 1.0f;
		Glow.green = 1.0f;
		Glow.blue = 1.0f;
		Glow.alpha = 1.0f;
	}
	else
	{
		Glow.red = *entity->getHealth() * -0.01f + 1;
		Glow.green = *entity->getHealth() * 0.01f;
		Glow.alpha = 1.f;
	}
	Glow.renderWhenOccluded = true;
	Glow.renderWhenUnOccluded = false;
	return Glow;
}
void setTeamGlow(int glowIndex, uintptr_t glowObj)
{
	GlowStruct TGlow;
	TGlow = *(GlowStruct*)((glowObj + glowIndex * 0x38));

	TGlow.blue = 1.f;
	TGlow.alpha = 1.f;
	TGlow.renderWhenOccluded = true;
	TGlow.renderWhenUnOccluded = false;
	*(GlowStruct*)((glowObj + glowIndex * 0x38)) = TGlow;

}
void setEnemyGlow(Enemy* entity, int glowIndex, uintptr_t glowObj)
{
	GlowStruct EGlow;
	EGlow = *(GlowStruct*)((glowObj + glowIndex * 0x38));
	EGlow = SetGlowColor(EGlow, entity);
	*(GlowStruct*)((glowObj + glowIndex * 0x38)) = EGlow;

}
void Glow()
{
	uintptr_t baseAddr = (uintptr_t)GetModuleHandle("client.dll");
	Player* p = Player::getPlayer();

	uintptr_t glowObj = *(uintptr_t*)(baseAddr + hazedumper::signatures::dwGlowObjectManager);
	int playerTeam = *p->getTeam();

	for (int i = 1; i < *p->getMaxPlayers(); i++)
	{
		Enemy* currentEnt = Enemy::getEnemy(i);
		int entityTeam;

		//Error checking
		if (currentEnt->isValidEnt())
		{
			//Do Stuff
			entityTeam = *currentEnt->getTeam();
			int glowIndex = *(int*)(*(uintptr_t*)currentEnt + hazedumper::netvars::m_iGlowIndex);

			if (playerTeam == entityTeam)
				setTeamGlow(glowIndex, glowObj);
			else
				setEnemyGlow(currentEnt, glowIndex, glowObj);
		}
	}
}

