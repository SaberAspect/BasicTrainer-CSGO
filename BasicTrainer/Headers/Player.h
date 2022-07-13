#pragma once
#include "vec3.h"

class Player
{
private:
	Player();

public:
	static Player* getPlayer();
	vec3* getAngles();
	vec3* getOrigin();
	int* getHealth();
	int* getMaxPlayers();
	int* getTeam();
	int* getShotsFired();
	double getDistance(vec3 ent);
	vec3 calcAngle(vec3* entPos);
	vec3* getViewOffset();
	int* getCrosshairID();
	void aimAt(vec3*, bool, float, bool, int);
	vec3* getPunchAngles();
};
