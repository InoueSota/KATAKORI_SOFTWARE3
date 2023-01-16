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

	static constexpr int kMaxTsuchinoko = 10;//敵の最大数
	Vec2 mCenterPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	bool IsPlayerLockon = false;//プレイヤーを補足しているかどうか
	int mLockonRadius = 600;//敵の視界の範囲

	const float mAngleSpeed = 0.1;//回転の速さ
	bool IsCollision[5] = {};


	//中心座標
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mCenterSpeed;


	//生成処理
	bool mIsActive;
	bool mIsDeath;
	void Make(Vec2 playerPosition);


	//移動処理
	void Move(Vec2 playerPosition);


	//角度処理
	float mCenterAngle;
	void SetAngle();


	//頭と尾
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	float mSize;
	float mRadius;

	//体
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	float mBodySize;
	float mBodyRadius;
	void Follow();


	//画像
	bool mIsLoadTexture = false;
	int tsuchinoko;
	int fov;
};

