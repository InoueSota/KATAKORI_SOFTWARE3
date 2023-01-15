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

	static constexpr int kMaxTsuchinoko = 10;//敵の最大数
	Vec2 mCenterPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	bool IsPlayerLockon = false;//プレイヤーを補足しているかどうか
	int mLockonRadius = 300;//敵の視界の範囲

	bool IsCollision[5] = {};

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

