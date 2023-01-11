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

}