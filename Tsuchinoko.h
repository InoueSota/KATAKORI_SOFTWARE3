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

	static constexpr int kMaxTsuchinoko = 10;//�G�̍ő吔
	Vec2 mCenterPositionStart;//�X�^�[�g�n�_
	Vec2 mTargetPoint;//�ڕW�n�_
	bool IsPlayerLockon = false;//�v���C���[��⑫���Ă��邩�ǂ���
	int mLockonRadius = 600;//�G�̎��E�͈̔�

	const float mAngleSpeed = 0.1;//��]�̑���
	bool IsCollision[5] = {};


	//���S���W
	Vec2 mCenterPosition;
	Vec2 mVelocity;
	float mCenterSpeed;


	//��������
	bool mIsActive;
	bool mIsDeath;
	void Make(Vec2 playerPosition);


	//�ړ�����
	void Move(Vec2 playerPosition);


	//�p�x����
	float mCenterAngle;
	void SetAngle();


	//���Ɣ�
	Vec2 mHeadPosition;
	Vec2 mTailPosition;
	float mSize;
	float mRadius;

	//��
	static constexpr int kBodyMax = 3;
	Vec2 mBodyPosition[kBodyMax];
	float mBodySize;
	float mBodyRadius;
	void Follow();


	//�摜
	bool mIsLoadTexture = false;
	int tsuchinoko;
	int tsuchinokobody;
	int fov;
};

