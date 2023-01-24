#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Ingame.h"
#include "Function.h"
#include "Collision.h"



void Tsuchinoko::Init() {

	mCenterSpeed = 5.0f;
	mSuperCenterSpeed = 7.0f;
	mVelocity.setZero();
	
	//����
	mIsActive = false;
	mIsDeath = false;

	//���Ɣ��̔��a
	mSize = 75.0f;
	mRadius = mSize / 2.0f;

	//�̂̔��a
	mBodySize = 50.0f;
	mBodyRadius = mBodySize / 2.0f;

}



void Tsuchinoko::Update(Vec2 playerposition, int mTimeLeft) {

	//���x������������
	mVelocity.setZero();

	//��������
	Make(playerposition, mTimeLeft);

	if (mIsActive && !mIsDeath)
	{
		//�ړ�����
		Move(playerposition);

		//�p�x����
		SetAngle();

		//�|�W�V�����ɑ��x��������
		mCenterPosition += mVelocity;

		//�{��
		Follow();
	}
}

void Tsuchinoko::Make(Vec2 PlayerPos, int mTimeLeft) {

	int SuperRand = RAND(1, 100);

	if (!mIsActive || mIsDeath)
	{
		while ((mCenterPosition.x >= PlayerPos.x - 640) && (mCenterPosition.x <= PlayerPos.x + 640)) {
			mCenterPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mCenterPosition.y >= PlayerPos.y - 360) && (mCenterPosition.y <= PlayerPos.y + 360)) {
			mCenterPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		}


		mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		mIsDeath = false;
		mIsActive = true;
		if (mTimeLeft < 30 && mTimeLeft > 0 && SuperRand <= 30) {
			mIsSuper = 1;
		}
	}

}

void Tsuchinoko::Move(Vec2 playerPosition) {

	//�v���C���[��ǂ�������"�Ȃ�"
	if (!IsPlayerLockon) {
		if (LockOnMoveTimer) {
			if (mIsSuper) {
				mVelocity += mDirectionPoint * mSuperCenterSpeed;
			} else {
				mVelocity += mDirectionPoint * mCenterSpeed;
			}
			LockOnMoveTimer--;
			if (!LockOnMoveTimer) {
				mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
				mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
			}
		} else {

			//���X�Ɍ�����ς���
			mDirectionPoint += (mTargetPoint - mCenterPosition) * 0.00001f;

			if (Collision(mCenterPosition, mRadius, mTargetPoint, 30.0f)) {
				mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
				mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
			} else {
				mDirectionPoint = mDirectionPoint.Normalized();
				if (mIsSuper) {
					mVelocity += mDirectionPoint * mSuperCenterSpeed;
				} else {
					mVelocity += mDirectionPoint * mCenterSpeed;
				}
			}
		}
	}
	//�v���C���[��ǂ�������"����"
	else {
		mDirectionPoint += (mTargetPoint - mCenterPosition) * 0.001f;
		mDirectionPoint = mDirectionPoint.Normalized();
		if (mIsSuper) {
			mVelocity += mDirectionPoint * mSuperCenterSpeed;
		} else {
			mVelocity += mDirectionPoint * mCenterSpeed;
		}
		LockOnMoveTimer = kMaxLockOnMoveTimer;
	}

	if (!(mCenterPosition.x >= Map::kMapLeft + kEnemyMapLimit && mCenterPosition.x <= Map::kMapRight - kEnemyMapLimit && mCenterPosition.y <= Map::kMapTop - kEnemyMapLimit && mCenterPosition.y >= Map::kMapBottom + kEnemyMapLimit)) {
		if (!mMapLimitFlag && !IsPlayerLockon) {
			LockOnMoveTimer = 0;
			mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
			mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
			mMapLimitFlag = true;
		} 
	} else {
		mMapLimitFlag = false;
	}
}


void Tsuchinoko::SetAngle() {

	//�ړ����Ă��鎞�̂݊p�x��ݒ肷��
	if (mVelocity.x != 0.0f || mVelocity.y != 0.0f)
	{
		//����ƌ����x�N�g��
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mVelocity;

		//�Ȃ��p�����߂�
		float dp = tmpDirection.Dot(base);
		float cp = tmpDirection.Cross(base);
		mCenterAngle = atan2(cp, dp);
	}
}

void Tsuchinoko::Follow() {

	mHeadPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * ( 150.0f + (mSize - mBodySize)));
	mHeadPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * ( 150.0f + (mSize - mBodySize)));
	mTailPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (-150.0f - (mSize - mBodySize)));
	mTailPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (-150.0f - (mSize - mBodySize)));

	for (int i = 0; i < kBodyMax; i++)
	{
		mBodyPosition[i].x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (-75.0f + (75.0f * i)));
		mBodyPosition[i].y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (-75.0f + (75.0f * i)));
	}

}

void Tsuchinoko::LockOn(Vec2 playerposition, float radius) {

	Vec2 toPlayer = playerposition - mCenterPosition;

	//�Ȃ��p�����߂�
	float dp = mVelocity.Dot(toPlayer);
	float cp = mVelocity.Cross(toPlayer);
	if (-Degree(30) < atan2(cp, dp) && atan2(cp, dp) < Degree(30)) {
		IsPlayerLockon = true;
		mTargetPoint = playerposition;
	}
	else {
		IsPlayerLockon = false;
	}
}



void Tsuchinoko::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		tsuchinoko = Novice::LoadTexture("./Resources/Debugs/tsuchinoko.png");
		tsuchinokobody = Novice::LoadTexture("./Resources/Debugs/tsuchinokobody.png");
		supertsuchinoko = Novice::LoadTexture("./Resources/Debugs/supertsuchinoko.png");
		supertsuchinokobody = Novice::LoadTexture("./Resources/Debugs/supertsuchinokobody.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		mIsLoadTexture = true;
	}

	//��������Ă��Ď���ł��Ȃ���
	if (mIsActive && !mIsDeath)
	{
		//���Ɣ��̕`��
		if (mIsSuper) {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
		} else {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		}
		

		//�̂̕`��
		for (int i = 0; i < kBodyMax; i++)
		{
			if (mIsSuper) {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, supertsuchinokobody);
			} else {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
			}
		}

		//���E�`��
		screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0x0000FF80);
		
	}


}