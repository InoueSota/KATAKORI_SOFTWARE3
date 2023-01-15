#pragma once

#include "Vec2.h"
#include "Enemy.h"
#include "Player.h"
#include "Novice.h"



void collision(Player &player, Snake &snake) {
	float A = player.mPosition.x - snake.mHeadPosition.x;
	float B = player.mPosition.y - snake.mHeadPosition.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mHeadRadius / 2) {
		snake.IsCollision[0] = true;
	} else {
		snake.IsCollision[0] = false;
	}

	A = player.mPosition.x - snake.mFirstBodyPosition.x;
	B = player.mPosition.y - snake.mFirstBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[1] = true;
	} else {
		snake.IsCollision[1] = false;
	}

	A = player.mPosition.x - snake.mSecondBodyPosition.x;
	B = player.mPosition.y - snake.mSecondBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[2] = true;
	} else {
		snake.IsCollision[2] = false;
	}

	A = player.mPosition.x - snake.mThirdBodyPosition.x;
	B = player.mPosition.y - snake.mThirdBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[3] = true;
	} else {
		snake.IsCollision[3] = false;
	}

	A = player.mPosition.x - snake.mFourthBodyPosition.x;
	B = player.mPosition.y - snake.mFourthBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[4] = true;
	} else {
		snake.IsCollision[4] = false;
	}
}

bool Collision(Vec2 playerPosition, float playerRadius, Vec2 enemyPosition, float enemyRadius) {

	float A = playerPosition.x - enemyPosition.x;
	float B = playerPosition.y - enemyPosition.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= playerRadius + enemyRadius) {
		return true;
	}
	return false;
}

float PlayerEnemyDist(Player& player, Vec2 enemyPosition) {

	Vec2 playerSegment = player.mPosition - player.mOldPosition;

	float t = 0.0f;
	float dvv = playerSegment.Length() * playerSegment.Length();

	if (dvv > 0.0f) {
		t = playerSegment.Dot(enemyPosition - player.mOldPosition) / dvv;
	}

	Vec2 distPoint = player.mOldPosition + playerSegment * t;

	if (t < 0.0f) {
		distPoint = player.mOldPosition;
		return (enemyPosition - distPoint).Length();
	}
	if (t > 1.0f) {
		distPoint = player.mPosition;
		return (enemyPosition - distPoint).Length();
	}
	return (enemyPosition - distPoint).Length();
}

bool CircleCapsuleCollsion(Player& player, Vec2 enemyPosition, float enemyRadius) {

	float dist = PlayerEnemyDist(player, enemyPosition);

	if (dist < (player.mRadius + enemyRadius)) {
		return true;
	}
	return false;
}
