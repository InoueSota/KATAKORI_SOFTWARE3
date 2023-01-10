#pragma once
#include "Screen.h"
#include "Vec2.h"
#include <time.h>
#include <conio.h>



class Snake
{
public:
	Snake() {
		Init();
	}

	void Init();
	void Update();
	void Draw(Screen& screen);

	static constexpr int kMaxLength = 10;

	//パラメータ
	//頭部分
	Vec2 mHeadPosition;
	int mHeadRadius;
	float mHeadAngle;
	Vec2 mHeadPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	void Move();
	void Angle();
	int mSpeed = 3;

	//体部分共通
	static constexpr int kMaxFrame = 120;
	Vec2 mOldHeadPosition[kMaxFrame];
	float mOldHeadAngle[kMaxFrame];
	int mFollowFrame;
	int mBodyRadius;
	void Follow();
	//体（１つ目）
	Vec2 mFirstBodyPosition;
	float mFirstBodyAngle;
	//体（２つ目）
	Vec2 mSecondBodyPosition;
	float mSecondBodyAngle;
	//体（３つ目）
	Vec2 mThirdBodyPosition;
	float mThirdBodyAngle;
	//体（４つ目）
	Vec2 mFourthBodyPosition;
	float mFourthBodyAngle;

	//画像関係
	bool mIsLoadTexture = false;
	int head;

};
