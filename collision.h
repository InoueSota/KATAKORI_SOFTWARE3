#pragma once

#include "Vec2.h"
#include "Enemy.h"
#include "Player.h"
#include "Novice.h"

bool collisiontest[5] = {};

void collision(Player &player, Snake &snake) {
	float A = player.mPosition.x - snake.mHeadPosition.x;
	float B = player.mPosition.y - snake.mHeadPosition.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mHeadRadius / 2) {
		collisiontest[0] = true;
	} else {
		collisiontest[0] = false;
	}

	A = player.mPosition.x - snake.mFirstBodyPosition.x;
	B = player.mPosition.y - snake.mFirstBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		collisiontest[1] = true;
	} else {
		collisiontest[1] = false;
	}

	A = player.mPosition.x - snake.mSecondBodyPosition.x;
	B = player.mPosition.y - snake.mSecondBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		collisiontest[2] = true;
	} else {
		collisiontest[2] = false;
	}

	A = player.mPosition.x - snake.mThirdBodyPosition.x;
	B = player.mPosition.y - snake.mThirdBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		collisiontest[3] = true;
	} else {
		collisiontest[3] = false;
	}

	A = player.mPosition.x - snake.mFourthBodyPosition.x;
	B = player.mPosition.y - snake.mFourthBodyPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		collisiontest[4] = true;
	} else {
		collisiontest[4] = false;
	}
}
