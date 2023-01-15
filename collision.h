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

	A = player.mPosition.x - snake.mHeadPosition.x;
	B = player.mPosition.y - snake.mHeadPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mLockonRadius) {
		snake.IsPlayerLockon = true;
		snake.mTargetPoint = player.mPosition;
	} else {
		snake.IsPlayerLockon = false;
	}
}

void collisionTsuchinoko(Player& player, Tsuchinoko& tsuchinoko) {
	float A = player.mPosition.x - tsuchinoko.mHeadPosition.x;
	float B = player.mPosition.y - tsuchinoko.mHeadPosition.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mRadius / 2) {
		tsuchinoko.IsCollision[0] = true;
	} else {
		tsuchinoko.IsCollision[0] = false;
	}

	A = player.mPosition.x - tsuchinoko.mBodyPosition[0].x;
	B = player.mPosition.y - tsuchinoko.mBodyPosition[0].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mBodyRadius / 2) {
		tsuchinoko.IsCollision[1] = true;
	} else {
		tsuchinoko.IsCollision[1] = false;
	}

	A = player.mPosition.x - tsuchinoko.mBodyPosition[1].x;
	B = player.mPosition.y - tsuchinoko.mBodyPosition[1].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mBodyRadius / 2) {
		tsuchinoko.IsCollision[2] = true;
	} else {
		tsuchinoko.IsCollision[2] = false;
	}

	A = player.mPosition.x - tsuchinoko.mBodyPosition[2].x;
	B = player.mPosition.y - tsuchinoko.mBodyPosition[2].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mBodyRadius / 2) {
		tsuchinoko.IsCollision[3] = true;
	} else {
		tsuchinoko.IsCollision[3] = false;
	}

	A = player.mPosition.x - tsuchinoko.mTailPosition.x;
	B = player.mPosition.y - tsuchinoko.mTailPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mRadius / 2) {
		tsuchinoko.IsCollision[4] = true;
	} else {
		tsuchinoko.IsCollision[4] = false;
	}

	A = player.mPosition.x - tsuchinoko.mCenterPosition.x;
	B = player.mPosition.y - tsuchinoko.mCenterPosition.y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + tsuchinoko.mLockonRadius) {
		tsuchinoko.IsPlayerLockon = true;
		tsuchinoko.mTargetPoint = player.mPosition;
	} else {
		tsuchinoko.IsPlayerLockon = false;
	}
}
