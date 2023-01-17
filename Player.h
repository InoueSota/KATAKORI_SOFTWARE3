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
	void Update(Screen& screen);
	void Draw(Screen& screen);


	//パラメータ
	Vec2 mPosition;
	Vec2 mVelocity;
	float mSize;
	float mRadius;


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

	//ストライク演出
	static constexpr int kStrikeLineMax = 200;
	bool mIsStrikeLineActive[kStrikeLineMax];
	Vec2 mStrikeLinePosition[kStrikeLineMax];
	int mStrikeLineWidth[kStrikeLineMax];
	int mStrikeLineHeight[kStrikeLineMax];
	float mStrikeLineAngle;
	float mStrikeLineAlphat[kStrikeLineMax];
	unsigned int mStrikeLineColor[kStrikeLineMax];
	void StrikeLine(Screen& screen);


	//残像
	static constexpr int kShadowMax = 4;
	Vec2 mOldPosition;
	Vec2 mShadowPosition[kShadowMax];
	bool mIsShadowActive;
	float mShadowAlphat;
	unsigned int mShadowColor;
	void Shadow();

	int LockonCount;//何体の敵に補足されているか
	int mKnockbackFlag;
	Vec2 mKnockbackEnemyPos;
	Vec2 mKnockbackStart;
	Vec2 mKnockbackEnd;
	float mKnockBackT;
	void Knockback();

};


