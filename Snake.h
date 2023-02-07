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
	void Update(int mTimeLeft, Vec2 PlayerPos, int LockonCount, bool isReady);
	void Draw(Screen& screen, int HitStop);

	static constexpr int kMaxLength = 10;

	//パラメータ
	bool mIsActive;
	bool mIsDeath;
	bool mIsOldDeath;
	bool mIsAnimation;
	bool mIsClearAnimation;
	int mIsSuper;
	int mDeadFlame;
	void TutorialMake(int mTimeLeft, Vec2 PlayerPos);
	//頭部分
	Vec2 mHeadPosition;
	Vec2 mHeadDeadPosition;
	int mHeadRadius;
	float mHeadAngle;
	float mDeadAngle;
	Vec2 mHeadPositionStart;//スタート地点
	Vec2 mTargetPoint;//目標地点
	Vec2 mTargetPlayer;//目標地点（プレイヤー版）
	Vec2 mDirectionPoint;
	Vec2 mVelocity;//速度
	void Move(int LockonCount);
	void Angle();

	//速度
	const float mSpeed = 6;
	const float mSuperSpeed = 8;
	const float mLockonSpeed = 11;
	const float mLockonSuperSpeed = 13;

	//追尾
	bool IsPlayerLockon = false;//プレイヤーを補足しているかどうか
	int mLockonRadius = 600;//敵の視界の範囲
	void LockOn(Vec2 playerposition, float radius);

	int LockOnMoveTimer;
	static constexpr int kMaxLockOnMoveTimer = 300;

	static constexpr int kEnemyMapLimit = 300;
	bool mMapLimitFlag;

	//敵撃破後に震えるタイマー
	static constexpr int kMaxShakeTimer = 8;
	int mShakeTimer = -1;

	//体部分共通
	static constexpr int kMaxFrame = 120;
	static constexpr int kBodyMax = 5;
	Vec2 mBodyPosition[kBodyMax];
	Vec2 mBodyDeadPosition[kBodyMax];
	float mBodyAngle[kBodyMax];
	float mBodyDeadAngle[kBodyMax];
	Vec2 mOldHeadPosition[kMaxFrame];
	float mOldHeadAngle[kMaxFrame];
	int mFollowFrame;
	int mBodyRadius;
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

	//画像関係
	bool mIsLoadTexture = false;
	int head;
	int body;
	int superhead;
	int superbody;
	int fov;
	int hiteffect;
};
