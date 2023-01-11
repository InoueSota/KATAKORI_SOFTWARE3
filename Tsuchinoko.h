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
	void Update();
	void Draw(Screen& screen);


	//’†SÀ•W
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mSpeed;
	void Move();


	//Šp“xˆ—
	float mCenterAngle;
	void SetAngle();


	//“ª‚Æ”ö
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	int mRadius;

	//‘Ì
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	int mBodyRadius;
	void Follow();


	//‰æ‘œ
	bool mIsLoadTexture = false;
	int tsuchinoko;
};

