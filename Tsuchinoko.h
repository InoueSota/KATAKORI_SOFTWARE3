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
	void Update(int mTimeLeft, Vec2 playerposition, int LockonCount, bool isReady);
	void Draw(Screen& screen, int HitStop);

	Vec2 mCenterPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	Vec2 mTargetPlayer;//目標地点（プレイヤー版）
	Vec2 mDirectionPoint;
	int mIsSuper;

	//追尾
	bool IsPlayerLockon = false;//プレイヤーを補足しているかどうか
	int mLockonRadius = 600;//敵の視界の範囲
	void LockOn(Vec2 playerposition, float radius);


	int LockOnMoveTimer;
	static constexpr int kMaxLockOnMoveTimer = 300;

	static constexpr int kEnemyMapLimit = 300;
	bool mMapLimitFlag;


	//中心座標
	Vec2 mCenterPosition;
	Vec2 mVelocity;

	const float mCenterSpeed = 5;
	const float mSuperCenterSpeed = 7;
	const float mLockonCenterSpeed = 10;
	const float mLockonSuperCenterSpeed = 12;


	//生成処理
	bool mIsActive;
	bool mIsDeath;
	bool mIsOldDeath;
	void Make(Vec2 playerPosition, int mTimeLeft);


	//移動処理
	void Move(Vec2 playerPosition, int LockonCount);


	//角度処理
	float mCenterAngle;
	float mDeadAngle;
	void SetAngle();

	//敵撃破後に震えるタイマー
	static constexpr int kMaxShakeTimer = 8;
	int mShakeTimer = -1;

	//ビックリマーク出すタイマー
	static constexpr int kMaxBikkuriTimer = 60;
	int mBikkuriTimer;
	bool mBikkuriFlag = false;


	//頭と尾
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	Vec2 mHeadDeadPosition;
	Vec2 mTailDeadPosition;
	float mSize;
	float mRadius;

	//体
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	Vec2 mBodyDeadPosition[kBodyMax];
	float mBodySize;
	float mBodyRadius;
	void Follow();


	//パーティクル
	static constexpr int kMaxSpawnParticle = 30;

	typedef struct Particle {
		Vec2 Pos;
		int IsUse = 0;
		Vec2 StartPos;
		Vec2 EndPos;
		float T = 0;
		int Timer = 120;
		float velocity;
		double R = 255;
		double B = 255;
		double G = 255;
		double color = 0xFFFFFF00;
		int alpha = 200;
	};
	Particle DefaultSpawnParticle;
	Particle spawnParticle[kMaxSpawnParticle];


	//画像
	bool mIsLoadTexture = false;
	int tsuchinoko;
	int tsuchinokobody;
	int supertsuchinoko;
	int supertsuchinokobody;
	int fov;
	int hiteffect;
	int enemyspawnparticleTexture;
	int enemybikkuritexture;
	int speedTexture;

	bool mIsAnimation;
	bool mIsClearAnimation;
	int mDeadFrame;
};

