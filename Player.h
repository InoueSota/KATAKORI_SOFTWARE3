#pragma once
#include "Screen.h"
#include "Vec2.h"

class Player
{
public:

	Player() {
		Init();
	}

	void Init();
	void Update();
	void Draw(Screen& screen);

	//パラメータ
	Vec2 mPosition;
	Vec2 mVelocity;
	float mSize;

	//通常移動 = NormalMove
	Vec2 mNormalVelocity;
	float mNormalMag;
	void NormalMove();

	//ダッシュ = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	void Dush();

	//マーキング = Mark
	bool mIsMarkActive;
	Vec2 mMarkPosition;
	static constexpr int kMarkTimeLimit = 300;
	int mMarkFrame;
	void Mark();

	//ストライク = Strike
	bool mIsStrikeActive;
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	Vec2 mStrikeStartPosition;
	Vec2 mStrikeEndPosition;
	void Strike();

};


