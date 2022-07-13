#include "Headers/Bunnyhop.h"
#include <Windows.h>
#include "../Headers/Offsets.h"
#include "../Headers/Player.h"


void BHop() {
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("client.dll");
	Player* localPlayer = Player::getPlayer();

	int* forceJump = (int*)(moduleBase + hazedumper::signatures::dwForceJump);
	int* flags = (int*)(*(uintptr_t*)localPlayer + hazedumper::netvars::m_fFlags);

	if (GetAsyncKeyState(VK_SPACE) && (*flags == 257))
		*forceJump = 6;
}