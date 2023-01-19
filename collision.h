#pragma once
#include "Vec2.h"
#include "Novice.h"



class Player;

bool Collision(Vec2 position1, float radius1, Vec2 position2, float radius2);
Vec2 AfterCollision(Vec2 position1, float radius1, Vec2 position2, float radius2);
float PlayerEnemyDist(Player& player, Vec2 enemyPosition);
bool CircleCapsuleCollsion(Player& player, Vec2 enemyPosition, float enemyRadius);