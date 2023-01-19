#include "collision.h"
#include "Player.h"



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
