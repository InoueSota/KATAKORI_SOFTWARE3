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
	void Update(int mTimeLeft, Vec2 PlayerPos);
	void Draw(Screen& screen);

	static constexpr int kMaxLength = 10;

	//�p�����[�^
	bool mIsActive;
	bool mIsDeath;
	bool mIsAnimation;
	int mIsSuper;
	int mDeadFlame;
	void Make(int mTimeLeft, Vec2 PlayerPos);
	//������
	Vec2 mHeadPosition;
	Vec2 mHeadDeadPosition;
	int mHeadRadius;
	float mHeadAngle;
	Vec2 mHeadPositionStart;//�X�^�[�g�n�_
	Vec2 mTargetPoint;//�ڕW�n�_
	Vec2 mDirectionPoint;
	Vec2 mVelocity;//���x
	void Move();
	void Angle();

	//���x
	float mSpeed = 5;
	float mSuperSpeed = 7;
	float mLockonSpeed = 10;
	float mLockonSuperSpeed = 12;

	//�ǔ�
	bool IsPlayerLockon = false;//�v���C���[��⑫���Ă��邩�ǂ���
	int mLockonRadius = 600;//�G�̎��E�͈̔�
	void LockOn(Vec2 playerposition, float radius);

	int LockOnMoveTimer;
	static constexpr int kMaxLockOnMoveTimer = 300;

	static constexpr int kEnemyMapLimit = 300;
	bool mMapLimitFlag;

	static constexpr int kMaxSnake = 10;//�G�̍ő吔

	//�̕�������
	static constexpr int kMaxFrame = 120;
	static constexpr int kBodyMax = 4;
	Vec2 mBodyPosition[kBodyMax];
	Vec2 mBodyDeadPosition[kBodyMax];
	float mBodyAngle[kBodyMax];
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
