#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Key.h"



void Tsuchinoko::Init() {

	mCenterPosition.x = 100;
	mCenterPosition.y = 0;
	mSpeed = 5.0f;

	//���Ɣ��̔��a
	mRadius = 75.0f;

	//�̂̔��a
	mBodyRadius = 50.0f;

}



void Tsuchinoko::Update() {

	//���x��0�ɏ���������
	mVelocity.setZero();

	//�ړ�����
	Move();

	//�p�x����
	SetAngle();

	//�|�W�V�����ɑ��x��������
	mCenterPosition += mVelocity;

	//�{��
	Follow();

}

void Tsuchinoko::Move() {

	//�ړ�����
	if (Key::IsPress(DIK_UP)) {
		mVelocity.y += mSpeed;
	}
	if (Key::IsPress(DIK_DOWN)) {
		mVelocity.y -= mSpeed;
	}
	if (Key::IsPress(DIK_LEFT)) {
		mVelocity.x -= mSpeed;
	}
	if (Key::IsPress(DIK_RIGHT)) {
		mVelocity.x += mSpeed;
	}

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
		mCenterPosition.x += cosf(rad) * mSpeed;
	} else {
		mCenterPositionStart.x = mCenterPosition.x;
		mTargetPoint.x = rand() % 6400 - (6400 / 2);
	}

	if ((mTargetPoint.y > mCenterPositionStart.y && mTargetPoint.y > mHeadPosition.y) || (mTargetPoint.y < mCenterPositionStart.y && mTargetPoint.y < mHeadPosition.y)) {
		mCenterPosition.y += sinf(rad) * mSpeed;
	} else {
		mCenterPositionStart.y = mCenterPosition.y;
		mTargetPoint.y = rand() % 3200 - (3200 / 2);
	}

	//�R���g���[���[�œ���������
	/*
	//�X�e�B�b�N�̕������擾����
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//int�^ �̒l�� float�^ �ɓ����iint�^�̂܂܂��ƌv�Z���O���P�ɂȂ�j
	mCenterVelocity.x = tmpX;
	mCenterVelocity.y = tmpY;

	//�ő�l���P�ɂ���
	mCenterVelocity = mCenterVelocity.Normalized();

	//���{����
	mCenterVelocity *= mCenterSpeed;
	*/

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

	mHeadPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (150.0f + (mRadius - mBodyRadius)));
	mHeadPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (150.0f + (mRadius - mBodyRadius)));
	mTailPosition.x = mCenterPosition.x + ( cosf(mCenterAngle + Degree(90)) * (-150.0f - (mRadius - mBodyRadius)));
	mTailPosition.y = mCenterPosition.y + (-sinf(mCenterAngle + Degree(90)) * (-150.0f - (mRadius - mBodyRadius)));

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

	//���Ɣ��̕`��
	screen.DrawPicture(mHeadPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);
	screen.DrawPicture(mTailPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);

	//�̂̕`��
	for (int i = 0; i < kBodyMax; i++)
	{
		screen.DrawPicture(mBodyPosition[i], mBodyRadius, mCenterAngle, 100, 100, tsuchinoko);
	}

	//�����蔻��`��

	//�����蔻��̃f�o�b�O
	if (IsCollision[0]) {
		screen.DrawCircle(mHeadPosition, mRadius / 2, 0xFF000080, kFillModeSolid);
	}
	if (IsCollision[1]) {
		screen.DrawCircle(mBodyPosition[0], mBodyRadius / 2, 0xFF000080, kFillModeSolid);
	}
	if (IsCollision[2]) {
		screen.DrawCircle(mBodyPosition[1], mBodyRadius / 2, 0xFF000080, kFillModeSolid);
	}
	if (IsCollision[3]) {
		screen.DrawCircle(mBodyPosition[2], mBodyRadius / 2, 0xFF000080, kFillModeSolid);
	}
	if (IsCollision[4]) {
		screen.DrawCircle(mTailPosition, mRadius / 2, 0xFF000080, kFillModeSolid);
	}
	screen.DrawCircle(mCenterPosition, mLockonRadius, 0x0000FF80, kFillModeSolid);
}