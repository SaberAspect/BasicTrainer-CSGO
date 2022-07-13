#pragma once
#include "vec3.h"



class Enemy
{
private:
	Enemy();
public:
	static Enemy* getEnemy(int index);
	vec3* getOrigin();
	int* getHealth();
	int* getTeam();
	bool isValidEnt();
	bool isAlive();
	bool isDefusing();
	vec3* getBonePos(int boneID);
};