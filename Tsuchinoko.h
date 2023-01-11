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

