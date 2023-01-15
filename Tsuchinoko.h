#pragma once
#include "Vec2.h"
#include "Screen.h"


class Tsuchinoko
{
public:
	Tsuchinoko() {
		Init();
	}

	void Init();
	void Update(Vec2 playerposition);
	void Draw(Screen& screen);

	static constexpr int kMaxTsuchinoko = 10;


	//’†SÀ•W
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mCenterSpeed;


	//¶¬ˆ—
	bool mIsActive;
	bool mIsDeath;
	void Make(Vec2 playerPosition);


	//ˆÚ“®ˆ—
	void Move(Vec2 playerPosition);


	//Šp“xˆ—
	float mCenterAngle;
	void SetAngle();


	//“ª‚Æ”ö
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	float mSize;
	float mRadius;

	//‘Ì
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	float mBodySize;
	float mBodyRadius;
	void Follow();


	//‰æ‘œ
	bool mIsLoadTexture = false;
	int tsuchinoko;
};

