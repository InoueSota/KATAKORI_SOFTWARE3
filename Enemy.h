#pragma once
#include "Screen.h"
#include "Vec2.h"
#include <time.h>
#include <conio.h>
#include "Function.h"



class Snake
{
public:
	Snake() {
		SRAND();
		Init();
	}

	void Init();
	void Update();
	void Draw(Screen& screen);

	static constexpr int kMaxLength = 10;

	//パラメータ
	bool mIsActive;
	bool mIsDeath;
	void Make();
	//頭部分
	Vec2 mHeadPosition;
	int mHeadRadius;
	float mHeadAngle;
	Vec2 mHeadPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	Vec2 mDirectionPoint;
	Vec2 mVelocity;//速度
	void Move();
	void Angle();
	float mSpeed = 5;
	const float mAngleSpeed = 0.1;//回転の速さ
	bool IsCollision[5] = {};
	bool IsPlayerLockon = false;//プレイヤーを補足しているかどうか
	int mLockonRadius = 600;//敵の視界の範囲

	static constexpr int kMaxSnake = 10;//敵の最大数

	//体部分共通
	static constexpr int kMaxFrame = 120;
	static constexpr int kBodyMax = 4;
	Vec2 mBodyPosition[kBodyMax];
	float mBodyAngle[kBodyMax];
	Vec2 mOldHeadPosition[kMaxFrame];
	float mOldHeadAngle[kMaxFrame];
	int mFollowFrame;
	int mBodyRadius;
	void Follow();

	//画像関係
	bool mIsLoadTexture = false;
	int head;
	int body;
	int fov;

};
