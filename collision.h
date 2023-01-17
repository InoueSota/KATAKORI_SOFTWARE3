#pragma once

#include "Vec2.h"
#include "Enemy.h"
#include "Player.h"
#include "Novice.h"
#include "Function.h"



void collision(Player &player, Snake &snake) {
	float A = player.mPosition.x - snake.mHeadPosition.x;
	float B = player.mPosition.y - snake.mHeadPosition.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mHeadRadius / 2) {
		snake.IsCollision[0] = true;
	} else {
		snake.IsCollision[0] = false;
	}

	A = player.mPosition.x - snake.mBodyPosition[0].x;
	B = player.mPosition.y - snake.mBodyPosition[0].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[1] = true;
	} else {
		snake.IsCollision[1] = false;
	}

	A = player.mPosition.x - snake.mBodyPosition[1].x;
	B = player.mPosition.y - snake.mBodyPosition[1].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[2] = true;
	} else {
		snake.IsCollision[2] = false;
	}

	A = player.mPosition.x - snake.mBodyPosition[2].x;
	B = player.mPosition.y - snake.mBodyPosition[2].y;
	Dis = sqrtf(A * A + B * B);
	if (Dis <= player.mSize / 2 + snake.mBodyRadius / 2) {
		snake.IsCollision[3] = true;
	} else {
		snake.IsCollision[3] = false;
	}

	A = player.mPosition.x - snake.mBodyPosition[3].x;
	B = player.mPosition.y - snake.mBodyPosition[3].y;
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

		Vec2 toPlayer = player.mPosition - snake.mHeadPosition;

		//‚È‚·Šp‚ð‹‚ß‚é
		float dp = snake.mVelocity.Dot(toPlayer);
		float cp = snake.mVelocity.Cross(toPlayer);
		if (-Degree(30) < atan2(cp, dp) && atan2(cp, dp) < Degree(30)) {
			snake.IsPlayerLockon = true;
			snake.mTargetPoint = player.mPosition;
		}
		else {
			snake.IsPlayerLockon = false;
		}
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

		Vec2 toPlayer = player.mPosition - tsuchinoko.mCenterPosition;

		//‚È‚·Šp‚ð‹‚ß‚é
		float dp = tsuchinoko.mVelocity.Dot(toPlayer);
		float cp = tsuchinoko.mVelocity.Cross(toPlayer);
		if (-Degree(30) < atan2(cp, dp) && atan2(cp, dp) < Degree(30)) {
			tsuchinoko.IsPlayerLockon = true;
			tsuchinoko.mTargetPoint = player.mPosition;
		}
		else {
			tsuchinoko.IsPlayerLockon = false;
		}
	} else {
		tsuchinoko.IsPlayerLockon = false;
	}
}

bool Collision(Vec2 position1, float radius1, Vec2 position2, float radius2) {

	float A = position1.x - position2.x;
	float B = position1.y - position2.y;
	float Dis = sqrtf(A * A + B * B);
	if (Dis <= radius1 + radius2) {
		return true;
	}
	return false;
}

Vec2 AfterCollision(Vec2 position1, float radius1, Vec2 position2, float radius2) {

	Vec2 direction = (position1 - position2).Normalized();
	float length = (position1 - position2).Length();
	float distance = radius1 + radius2;

	return position1 + (direction * (distance - length));
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

int SightCount(Snake snake[Snake::kMaxSnake], Tsuchinoko tsuchinoko[Tsuchinoko::kMaxTsuchinoko]) {
	int Count = 0;

	for (int i = 0; i < snake[i].kMaxSnake; i++) {
		if (snake[i].IsPlayerLockon) {
			Count++;
		}
	}
	for (int i = 0; i < tsuchinoko[i].kMaxTsuchinoko; i++) {
		if (tsuchinoko[i].IsPlayerLockon) {
			Count++;
		}
	}

	return Count;
}
