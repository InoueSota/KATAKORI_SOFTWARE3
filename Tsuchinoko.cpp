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

	if (!mIsActive && !mIsDeath)
	{
		mCenterPosition.x = RAND(Map::kMapLeft, Map::kMapRight);
		mCenterPosition.y = RAND(Map::kMapBottom, Map::kMapTop);
		mIsActive = true;
	}

}

void Tsuchinoko::Move(Vec2 playerPosition) {

	//�����_���Ɉړ��n�_�����߂Ĉړ�
	float rad = atan2(mTargetPoint.y - mCenterPosition.y, mTargetPoint.x - mCenterPosition.x);
	float rad_2 = 0;

	/*if (rad > mHeadAngle + 3.14159) {
		rad_2 = mHeadAngle + mAngleSpeed;
		if (rad < mHeadAngle + 3.14159) {
			rad_2 = rad;
		}
	} else {
		rad_2 = mHeadAngle - mAngleSpeed;
		if (rad > mHeadAngle + 3.14159) {
			rad_2 = rad;
		}
	}*/

	if ((mTargetPoint.x > mCenterPositionStart.x && mTargetPoint.x > mHeadPosition.x) || (mTargetPoint.x < mCenterPositionStart.x && mTargetPoint.x < mHeadPosition.x)) {
		mCenterPosition.x += cosf(rad) * mCenterSpeed;
	} else {
		mCenterPositionStart.x = mCenterPosition.x;
		mTargetPoint.x = rand() % 6400 - (6400 / 2);
	}

	if ((mTargetPoint.y > mCenterPositionStart.y && mTargetPoint.y > mHeadPosition.y) || (mTargetPoint.y < mCenterPositionStart.y && mTargetPoint.y < mHeadPosition.y)) {
		mCenterPosition.y += sinf(rad) * mCenterSpeed;
	} else {
		mCenterPositionStart.y = mCenterPosition.y;
		mTargetPoint.y = rand() % 3200 - (3200 / 2);
	}



	////�v���C���[�ւ̌����x�N�g��
	//Vec2 toPlayer = { playerPosition.x - mCenterPosition.x, playerPosition.y - mCenterPosition.y };

	////�����x�N�g���Ƒ��x�𐳋K������
	//toPlayer = toPlayer.Normalized();
	//mVelocity = mVelocity.Normalized();

	////������ݒ肷��
	//mVelocity += (toPlayer - mVelocity) * 0.08f;

	////���x��ݒ肷��
	//mVelocity *= mCenterSpeed;

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
			screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinoko);
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
		screen.DrawCircle(mCenterPosition, mLockonRadius, 0x0000FF80, kFillModeSolid);
	}


}