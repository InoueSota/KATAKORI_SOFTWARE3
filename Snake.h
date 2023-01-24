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
	int mIsSuper = 0;
	void Make(int mTimeLeft, Vec2 PlayerPos);
	//������
	Vec2 mHeadPosition;
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
	float mBodyAngle[kBodyMax];
	Vec2 mOldHeadPosition[kMaxFrame];
	float mOldHeadAngle[kMaxFrame];
	int mFollowFrame;
	int mBodyRadius;
	void Follow();

	//�摜�֌W
	bool mIsLoadTexture = false;
	int head;
	int body;
	int superhead;
	int superbody;
	int fov;

};
