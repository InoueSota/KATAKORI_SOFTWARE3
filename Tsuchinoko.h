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


	//中心座標
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mSpeed;
	void Move();


	//角度処理
	float mCenterAngle;
	void SetAngle();


	//頭と尾
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	int mRadius;

	//体
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	int mBodyRadius;
	void Follow();


	//画像
	bool mIsLoadTexture = false;
	int tsuchinoko;
};

