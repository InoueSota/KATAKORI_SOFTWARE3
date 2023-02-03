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
	void Update(Vec2 playerposition, int mTimeLeft, int LockonCount);
	void Draw(Screen& screen);

	Vec2 mCenterPositionStart;//�X�^�[�g�n�_
	Vec2 mTargetPoint;//�ڕW�n�_
	Vec2 mDirectionPoint;
	int mIsSuper;

	//�ǔ�
	bool IsPlayerLockon = false;//�v���C���[��⑫���Ă��邩�ǂ���
	int mLockonRadius = 600;//�G�̎��E�͈̔�
	void LockOn(Vec2 playerposition, float radius);


	int LockOnMoveTimer;
	static constexpr int kMaxLockOnMoveTimer = 300;

	static constexpr int kEnemyMapLimit = 300;
	bool mMapLimitFlag;


	//���S���W
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mCenterSpeed;
	float mSuperCenterSpeed;
	float mLockonCenterSpeed = 10;
	float mLockonSuperCenterSpeed = 12;


	//��������
	bool mIsActive;
	bool mIsDeath;
	void Make(Vec2 playerPosition, int mTimeLeft);


	//�ړ�����
	void Move(Vec2 playerPosition, int LockonCount);


	//�p�x����
	float mCenterAngle;
	float mDeadAngle;
	void SetAngle();

	//�G���j��ɐk����^�C�}�[
	static constexpr int kMaxShakeTimer = 8;
	int mShakeTimer = -1;


	//���Ɣ�
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	Vec2 mHeadDeadPosition;
	Vec2 mTailDeadPosition;
	float mSize;
	float mRadius;

	//��
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	Vec2 mBodyDeadPosition[kBodyMax];
	float mBodySize;
	float mBodyRadius;
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


	//�摜
	bool mIsLoadTexture = false;
	int tsuchinoko;
	int tsuchinokobody;
	int supertsuchinoko;
	int supertsuchinokobody;
	int fov;
	int hiteffect;

	bool mIsAnimation;
	bool mIsClearAnimation;
	int mDeadFrame;
};

