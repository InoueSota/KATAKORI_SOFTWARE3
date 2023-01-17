#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Ingame.h"
#include "Function.h"



void Tsuchinoko::Init() {

	mCenterSpeed = 5.0f;
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



void Tsuchinoko::Update(Vec2 playerposition) {

	//���x������������
	mVelocity.setZero();

	//��������
	Make(playerposition);

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

void Tsuchinoko::Make(Vec2 playerPosition) {

	if (!mIsActive || mIsDeath)
	{
		mCenterPosition.x = RAND(Map::kMapLeft, Map::kMapRight);
		mCenterPosition.y = RAND(Map::kMapBottom, Map::kMapTop);
		mIsDeath = false;
		mIsActive = true;
	}

}

void Tsuchinoko::Move(Vec2 playerPosition) {

	//�ꉞ���K������
	Vec2 tmpTarget = mTargetPoint.Normalized();

	//�v���C���[��ǂ�������"�Ȃ�"
	if (!IsPlayerLockon) {

		//���X�Ɍ�����ς���
		mDirectionPoint += (tmpTarget - mDirectionPoint) * 0.01f;

		if ((mTargetPoint.x > mCenterPositionStart.x && mTargetPoint.x > mCenterPosition.x) || (mTargetPoint.x < mCenterPositionStart.x && mTargetPoint.x < mCenterPosition.x)) {
			mVelocity = mDirectionPoint * mCenterSpeed;
		}
		else {
			mCenterPositionStart.x = mCenterPosition.x;
			mTargetPoint.x = RAND(Map::kMapLeft, Map::kMapRight);
		}

		if ((mTargetPoint.y > mCenterPositionStart.y && mTargetPoint.y > mCenterPosition.y) || (mTargetPoint.y < mCenterPositionStart.y && mTargetPoint.y < mCenterPosition.y)) {
			mVelocity = mDirectionPoint * mCenterSpeed;
		}
		else {
			mCenterPositionStart.y = mCenterPosition.y;
			mTargetPoint.y = RAND(Map::kMapBottom, Map::kMapTop);
		}
	}
	//�v���C���[��ǂ�������"����"
	else {

		mDirectionPoint = mTargetPoint - mCenterPosition;

		mDirectionPoint = mDirectionPoint.Normalized();

		mVelocity = mDirectionPoint * mCenterSpeed;
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

	mHeadPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (150.0f + (mSize - mBodySize)));
	mHeadPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (150.0f + (mSize - mBodySize)));
	mTailPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (-150.0f - (mSize - mBodySize)));
	mTailPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (-150.0f - (mSize - mBodySize)));

	for (int i = 0; i < kBodyMax; i++)
	{
		mBodyPosition[i].x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (-75.0f + (75.0f * i)));
		mBodyPosition[i].y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (-75.0f + (75.0f * i)));
	}

}



void Tsuchinoko::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		tsuchinoko = Novice::LoadTexture("./Resources/Debugs/tsuchinoko.png");
		tsuchinokobody = Novice::LoadTexture("./Resources/Debugs/tsuchinokobody.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		mIsLoadTexture = true;
	}

	//��������Ă��Ď���ł��Ȃ���
	if (mIsActive && !mIsDeath)
	{
		//���Ɣ��̕`��
		screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);

		//�̂̕`��
		for (int i = 0; i < kBodyMax; i++)
		{
			screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
		}

		//�����蔻��`��

		//�����蔻��̃f�o�b�O
		if (IsCollision[0]) {
			screen.DrawCircle(mHeadPosition, mRadius, 0xFF000080, kFillModeSolid);
		}
		if (IsCollision[1]) {
			screen.DrawCircle(mBodyPosition[0], mBodyRadius, 0xFF000080, kFillModeSolid);
		}
		if (IsCollision[2]) {
			screen.DrawCircle(mBodyPosition[1], mBodyRadius, 0xFF000080, kFillModeSolid);
		}
		if (IsCollision[3]) {
			screen.DrawCircle(mBodyPosition[2], mBodyRadius, 0xFF000080, kFillModeSolid);
		}
		if (IsCollision[4]) {
			screen.DrawCircle(mTailPosition, mRadius, 0xFF000080, kFillModeSolid);
		}
		screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0x0000FF80);
	}


}