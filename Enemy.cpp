#include "Enemy.h"
#include "Key.h"
#include "Ingame.h"




void Snake::Init() {
	//����
	mIsActive = false;
	mIsDeath = false;
	mHeadRadius = 50;
	mBodyRadius = 30;
	mFollowFrame = 0;
}

void Snake::Update() {

	//���x�̏�����
	mVelocity.setZero();

	//��������
	Make();

	if (mIsActive && !mIsDeath)
	{
		Move();

		//������ς��鏈��
		Angle();

		//���x���|�W�V�����ɑ������
		mHeadPosition += mVelocity;

		//�̕������ǂ�����
		Follow();
	}
}

void Snake::Make() {

	if (!mIsActive || mIsDeath)
	{
		mHeadPosition.x = RAND(Map::kMapLeft, Map::kMapRight);
		mHeadPosition.y = RAND(Map::kMapBottom, Map::kMapTop);
		mHeadPositionStart = mHeadPosition;
		mTargetPoint.x = rand() % 6400 - (6400 / 2);
		mTargetPoint.y = rand() % 3200 - (3200 / 2);
		for (int i = 0; i < kMaxFrame; i++) {
			mOldHeadPosition[i] = mHeadPosition;
		}
		mIsDeath = false;
		mIsActive = true;
	}

}

void Snake::Move() {

	//�����_���Ɉړ��n�_�����߂Ĉړ�
	float rad = atan2(mTargetPoint.y - mHeadPosition.y, mTargetPoint.x - mHeadPosition.x);
	float rad_2 = 0;

	if (rad > mHeadAngle + 3.14159) {
		rad_2 = mHeadAngle + mAngleSpeed;
		if (rad < mHeadAngle + 3.14159) {
			rad_2 = rad;
		}
	} else {
		rad_2 = mHeadAngle - mAngleSpeed;
		if (rad > mHeadAngle + 3.14159) {
			rad_2 = rad;
		}
	}

	if ((mTargetPoint.x > mHeadPositionStart.x && mTargetPoint.x > mHeadPosition.x) || (mTargetPoint.x < mHeadPositionStart.x && mTargetPoint.x < mHeadPosition.x)) {
		mVelocity.x += cosf(rad) * mSpeed;
	} else {
		mHeadPositionStart.x = mHeadPosition.x;
		mTargetPoint.x = rand() % 6400 - (6400 / 2);
	}

	if ((mTargetPoint.y > mHeadPositionStart.y && mTargetPoint.y > mHeadPosition.y) || (mTargetPoint.y < mHeadPositionStart.y && mTargetPoint.y < mHeadPosition.y)) {
		mVelocity.y += sinf(rad) * mSpeed;
	} else {
		mHeadPositionStart.y = mHeadPosition.y;
		mTargetPoint.y = rand() % 3200 - (3200 / 2);
	}
}

void Snake::Angle() {

	//�ړ����Ă��鎞�̂݊p�x��ݒ肷��
	if (mVelocity.x != 0.0f || mVelocity.y != 0.0f)
	{
		//����ƌ����x�N�g��
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mVelocity;

		//�Ȃ��p�����߂�
		float dp = tmpDirection.Dot(base);
		float cp = tmpDirection.Cross(base);
		mHeadAngle = atan2(cp, dp);
	}
}

void Snake::Follow() {

	//���������Ă���Ƃ�
	if (mOldHeadPosition[0].x != mHeadPosition.x || mOldHeadPosition[0].y != mHeadPosition.y) {

		//���Z���Ă���t���[�����ő�l�ȉ��̂Ƃ�
		if (mFollowFrame < kMaxFrame) {

			//�P�t���[�����X�V����
			for (int i = mFollowFrame - 1; i > -1; i--) {
				if (i != 0) {
					mOldHeadPosition[i] = mOldHeadPosition[i - 1];
					mOldHeadAngle[i] = mOldHeadAngle[i - 1];
				}
			}

			//�O�t���[���ڂ͓�
			mOldHeadPosition[0] = mHeadPosition;
			mOldHeadAngle[0] = mHeadAngle;

			//�t���[�������Z����
			mFollowFrame++;
		}
		else {

			//�P�t���[�����X�V����
			for (int i = kMaxFrame - 1; i > -1; i--) {
				if (i != 0) {
					mOldHeadPosition[i] = mOldHeadPosition[i - 1];
					mOldHeadAngle[i] = mOldHeadAngle[i - 1];
				}
			}
			//�O�Ԗڂ͓��ɍX�V����
			mOldHeadPosition[0] = mHeadPosition;
			mOldHeadAngle[0] = mHeadAngle;
		}
	}
	
	//�C�ӂ�OldPosition��������
	//�́i�P�ځj
	mBodyPosition[0] = mOldHeadPosition[19];
	mBodyAngle[0] = mOldHeadAngle[19];
	//�́i�Q�ځj
	mBodyPosition[1] = mOldHeadPosition[39];
	mBodyAngle[1] = mOldHeadAngle[39];
	//�́i�R�ځj
	mBodyPosition[2] = mOldHeadPosition[59];
	mBodyAngle[2] = mOldHeadAngle[59];
	//�́i�S�ځj
	mBodyPosition[3] = mOldHeadPosition[79];
	mBodyAngle[3] = mOldHeadAngle[79];

}



void Snake::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		head = Novice::LoadTexture("./Resources/Debugs/head.png");
		body = Novice::LoadTexture("./Resources/Debugs/body.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		mIsLoadTexture = true;
	}

	//��������Ă��Ď���ł��Ȃ���
	if (mIsActive && !mIsDeath)
	{
		//�̕`��
		for (int i = 0; i < kBodyMax; i++)
		{
			screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, body);
		}

		//���`��
		screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, head);

		//�����蔻��`��

		//�����蔻��̃f�o�b�O
		if (IsCollision[0]) {
			screen.DrawCircle(mHeadPosition, mHeadRadius / 2, 0xFF000080, kFillModeSolid);
		}
		for (int i = 0; i < kBodyMax; i++)
		{
			if (IsCollision[i]) {
				screen.DrawCircle(mBodyPosition[i], mBodyRadius / 2, 0xFF000080);
			}
		}
		screen.DrawPicture({ mHeadPosition.x + mLockonRadius / 2 * cosf(mHeadAngle), mHeadPosition.y + mLockonRadius / 2 * -sinf(mHeadAngle) }, mLockonRadius, mHeadAngle, 500, 500, fov, 0x0000FF80);
	}
}
