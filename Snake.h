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
	void Update(int mTimeLeft, Vec2 PlayerPos, int LockonCount);
	void Draw(Screen& screen);

	static constexpr int kMaxLength = 10;

	//�p�����[�^
	bool mIsActive;
	bool mIsDeath;
	bool mIsAnimation;
	bool mIsClearAnimation;
	int mIsSuper;
	int mDeadFlame;
	void FirstMake();
	void Make(int mTimeLeft, Vec2 PlayerPos);
	//������
	Vec2 mHeadPosition;
	Vec2 mHeadDeadPosition;
	int mHeadRadius;
	float mHeadAngle;
	float mDeadAngle;
	Vec2 mHeadPositionStart;//�X�^�[�g�n�_
	Vec2 mTargetPoint;//�ڕW�n�_
	Vec2 mDirectionPoint;
	Vec2 mVelocity;//���x
	void Move(int LockonCount);
	void Angle();

	//���x
	const float mSpeed = 6;
	const float mSuperSpeed = 8;
	const float mLockonSpeed = 11;
	const float mLockonSuperSpeed = 13;

	//�ǔ�
	bool IsPlayerLockon = false;//�v���C���[��⑫���Ă��邩�ǂ���
	int mLockonRadius = 600;//�G�̎��E�͈̔�
	void LockOn(Vec2 playerposition, float radius);

	int LockOnMoveTimer;
	static constexpr int kMaxLockOnMoveTimer = 300;

	static constexpr int kEnemyMapLimit = 300;
	bool mMapLimitFlag;

	//�G���j��ɐk����^�C�}�[
	static constexpr int kMaxShakeTimer = 8;
	int mShakeTimer = -1;

	//�̕�������
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

	//�p�[�e�B�N��
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

	//�摜�֌W
	bool mIsLoadTexture = false;
	int head;
	int body;
	int superhead;
	int superbody;
	int fov;
	int hiteffect;
};
