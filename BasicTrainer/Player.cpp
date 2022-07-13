#include "Headers/Player.h"
#include "Headers/Offsets.h"
#include <Windows.h>
#include <iostream>

#define pi 3.14159653589

Player* Player::getPlayer()
{
	static uintptr_t moduleBase = (uintptr_t)(GetModuleHandle("client.dll"));
	static Player* localPlayer = (Player*)(moduleBase + hazedumper::signatures::dwLocalPlayer);
	if (localPlayer == NULL)
		while (localPlayer == NULL)
			localPlayer = (Player*)(moduleBase + hazedumper::signatures::dwLocalPlayer);
	return localPlayer;
}
vec3* Player::getAngles()
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("engine.dll");
	uintptr_t* clientState = (uintptr_t*)(moduleBase + hazedumper::signatures::dwClientState);
	return (vec3*)(*(uintptr_t*)clientState + hazedumper::signatures::dwClientState_ViewAngles);
}
vec3* Player::getPunchAngles()
{
	return(vec3*)(*(uintptr_t*)this + hazedumper::netvars::m_aimPunchAngle);
}
vec3* Player::getOrigin()
{
	return (vec3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}
int* Player::getHealth()
{
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iHealth);
}
int* Player::getTeam()
{
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum);
}
int* Player::getMaxPlayers()
{
	uintptr_t EngineBase = (uintptr_t)(GetModuleHandle("engine.dll"));
	uintptr_t* clientState = (uintptr_t*)(EngineBase + hazedumper::signatures::dwClientState);
	return(int*)(*clientState + hazedumper::signatures::dwClientState_MaxPlayer);

}
int* Player::getShotsFired()
{
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iShotsFired);
}
int* Player::getCrosshairID()
{
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iCrosshairId);
}
double Player::getDistance(vec3 ent)
{
	vec3 currPos = *getOrigin();
	vec3 origin = currPos - ent;

	double distance = sqrt((pow(origin.x, 2) + pow(origin.y, 2) + pow(origin.z, 2)));

	return distance;
}
vec3* Player::getViewOffset()
{
	return (vec3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset);
}
vec3 Player::calcAngle(vec3* entPos)
{
	vec3 origin = *getOrigin();

	vec3 viewOffset = *getViewOffset();

	vec3* myPos = &(origin + viewOffset);

	vec3 deltaVec = { entPos->x - myPos->x, entPos->y - myPos->y, entPos->z - myPos->z };

	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	vec3 angles;
	angles.x = -asin(deltaVec.z / deltaVecLength) * (180.0f / pi);
	angles.y = atan2(deltaVec.y, deltaVec.x) * (180.0f / pi);
	angles.z = 0;

	return angles;
}
//Aimat function
void Player::aimAt(vec3* ent, bool smooth, float smoothness, bool fov, int fovRange)
{
	static uintptr_t moduleBase = (uintptr_t)GetModuleHandle("engine.dll");
	static vec3* viewAngles = (vec3*)(*(uintptr_t*)(moduleBase + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_ViewAngles);

	vec3 bestAngles = calcAngle(ent);

	bestAngles -= *getPunchAngles() * 2.0f;
	//For smooth
	vec3 diff = bestAngles - *viewAngles;
	float newX = getAngles()->x;
	float newY = getAngles()->y;

	if (smooth && fov)
	{
		newX += diff.x / smoothness;

		if (diff.y > 180)
		{
			diff.y -= 360;
		}
		if (diff.y < -180)
		{
			diff.y += 360;
		}

		newY += diff.y / smoothness;

		if (newY < -180)
		{
			newY = 179.99999f;
		}
		if (newY > 180)
		{
			newY = -179.99999f;
		}

		//  aim
		if (diff.y < fovRange && diff.y > -fovRange && diff.x < fovRange / 2 && diff.x > -(fovRange / 2)) {
			viewAngles->x = newX;
			viewAngles->y = newY;
		}
	}
	else if(smooth && !fov)
	{
		newX += diff.x / smoothness;

		if (diff.y > 180)
		{
			diff.y -= 360;
		}
		if (diff.y < -180)
		{
			diff.y += 360;
		}

		newY += diff.y / smoothness;

		if (newY < -180)
		{
			newY = 179.99999f;
		}
		if (newY > 180)
		{
			newY = -179.99999f;
		}
		viewAngles->x = newX;
		viewAngles->y = newY;
	}
	else if(fov && !smooth)
	{
		if (diff.y > 180)
		{
			diff.y -= 360;
		}
		if (diff.y < -180)
		{
			diff.y += 360;
		}

		if (diff.y < fovRange && diff.y > -fovRange && diff.x < fovRange /2 && diff.x > -(fovRange/2)) {
			*viewAngles = bestAngles;
		}
	}
	else if (bestAngles.x >= -89 && bestAngles.x <= 89 && bestAngles.y >= -180 && bestAngles.y <= 180)
			*viewAngles = bestAngles;

}