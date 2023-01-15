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

	static constexpr int kMaxTsuchinoko = 10;//�G�̍ő吔
	Vec2 mCenterPositionStart;//�X�^�[�g�n�_
	Vec2 mTargetPoint;//�ڕW�n�_
	bool IsPlayerLockon = false;//�v���C���[��⑫���Ă��邩�ǂ���
	int mLockonRadius = 300;//�G�̎��E�͈̔�

	bool IsCollision[5] = {};

	//���S���W
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mSpeed;
	void Move();


	//�p�x����
	float mCenterAngle;
	void SetAngle();


	//���Ɣ�
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	int mRadius;

	//��
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	int mBodyRadius;
	void Follow();


	//�摜
	bool mIsLoadTexture = false;
	int tsuchinoko;
};

