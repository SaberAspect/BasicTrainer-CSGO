#pragma once
#include "../../Headers/Enemy.h"



// boneId to aim at
//traceline func and bDormant
// closestEnemy
Enemy* getClosestEnemy();
//Lowest Health
Enemy* getLowestHealthEnemy();
//run
void aimBot(bool, float, bool, int, int, bool, int);