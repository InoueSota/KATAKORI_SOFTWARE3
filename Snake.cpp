#include "Snake.h"
#include "Key.h"
#include "Ingame.h"
#include "Collision.h"
#include <Easing.h>



void Snake::Init() {
	//����
	mIsActive = false;
	mIsDeath = false;
	mHeadRadius = 50;
	mBodyRadius = 30;
	mFollowFrame = 0;
}

void Snake::Update(int mTimeLeft, Vec2 PlayerPos) {

	//���x�̏�����
	mVelocity.setZero();

	//��������
	Make(mTimeLeft, PlayerPos);

	for (int i = 0; i < kMaxSpawnParticle; i++) {
		if (spawnParticle[i].IsUse) {
			if (spawnParticle[i].T < 1) {
				spawnParticle[i].T += 0.033;
				if (spawnParticle[i].T > 1) {
					spawnParticle[i].T = 1;
				}
				float easedT = easeOutSine(spawnParticle[i].T);

				spawnParticle[i].Pos.x = ((1 - easedT) * spawnParticle[i].StartPos.x) + (easedT * spawnParticle[i].EndPos.x);
				spawnParticle[i].Pos.y = ((1 - easedT) * spawnParticle[i].StartPos.y) + (easedT * spawnParticle[i].EndPos.y);
			} else {
				spawnParticle[i] = DefaultSpawnParticle;
			}
		}
	}

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

void Snake::Make(int mTimeLeft, Vec2 PlayerPos) {

	int SuperRand = RAND(1, 100);

	if (!mIsActive || mIsDeath)
	{
		while ((mHeadPosition.x >= PlayerPos.x - 640) && (mHeadPosition.x <= PlayerPos.x + 640)) {
			mHeadPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mHeadPosition.y >= PlayerPos.y - 360) && (mHeadPosition.y <= PlayerPos.y + 360)) {
			mHeadPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		}
		for (int i = 0; i < kMaxSpawnParticle; i++) {
			spawnParticle[i].Pos = mHeadPosition;
			spawnParticle[i].EndPos = mHeadPosition;
			float angle = Degree(RAND(1, 360));
			spawnParticle[i].StartPos.x = spawnParticle[i].Pos.x + cosf(angle) * 200;
			spawnParticle[i].StartPos.y = spawnParticle[i].Pos.y + sinf(angle) * 200;
			spawnParticle[i].IsUse = 1;
		}

		mHeadPositionStart = mHeadPosition;
		mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		for (int i = 0; i < kMaxFrame; i++) {
			mOldHeadPosition[i] = mHeadPosition;
		}
		mIsDeath = false;
		mIsActive = true;
		if (mTimeLeft < 30 && mTimeLeft > 0 && SuperRand <= 30) {
			mIsSuper = 1;
		}
	}

}

void Snake::Move() {

	//�v���C���[��ǂ�������"�Ȃ�"
	if (!IsPlayerLockon) {
		if (LockOnMoveTimer) {
			if (mIsSuper) {
				mVelocity += mDirectionPoint * mSuperSpeed;
			} else {
				mVelocity += mDirectionPoint * mSpeed;
			}
			LockOnMoveTimer--;
			if (!LockOnMoveTimer) {
				mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
				mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
			}
		} else {
			//���X�Ɍ�����ς���
			mDirectionPoint += (mTargetPoint - mHeadPosition) * 0.00001f;

			if (Collision(mHeadPosition, mHeadRadius, mTargetPoint, 50.0f)) {
				mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
				mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
			} else {
				mDirectionPoint = mDirectionPoint.Normalized();
				if (mIsSuper) {
					mVelocity += mDirectionPoint * mSuperSpeed;
				} else {
					mVelocity += mDirectionPoint * mSpeed;
				}
			}
		}
	}
	//�v���C���[��ǂ�������"����"
	else {

		mDirectionPoint += (mTargetPoint - mHeadPosition) * 0.001f;

		mDirectionPoint = mDirectionPoint.Normalized();

		if (mIsSuper) {
			mVelocity += mDirectionPoint * mSuperSpeed;
		} else {
			mVelocity += mDirectionPoint * mSpeed;
		}

		LockOnMoveTimer = kMaxLockOnMoveTimer;
	}

	if (!(mHeadPosition.x >= Map::kMapLeft + kEnemyMapLimit && mHeadPosition.x <= Map::kMapRight - kEnemyMapLimit && mHeadPosition.y <= Map::kMapTop - kEnemyMapLimit && mHeadPosition.y >= Map::kMapBottom + kEnemyMapLimit)) {
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

void Snake::LockOn(Vec2 playerposition, float radius) {

	Vec2 toPlayer = playerposition - mHeadPosition;

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
		superhead = Novice::LoadTexture("./Resources/Debugs/superhead.png");
		superbody = Novice::LoadTexture("./Resources/Debugs/superbody.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		mIsLoadTexture = true;
	}

	//��������Ă��Ď���ł��Ȃ���
	if (mIsActive && !mIsDeath)
	{
		//�̕`��
		for (int i = 0; i < kBodyMax; i++){
			if (mIsSuper) {
				screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, superbody);
			} else{
				screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, body);
			}
		}

		//���`��
		if (mIsSuper) {
			screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, superhead);
		} else {
			screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, head);
		}

		//���E�`��
		screen.DrawPicture({ mHeadPosition.x + mLockonRadius / 2 * cosf(mHeadAngle), mHeadPosition.y + mLockonRadius / 2 * -sinf(mHeadAngle) }, mLockonRadius, mHeadAngle, 500, 500, fov, 0x0000FF80);
		
	}

	//�X�|�[���p�[�e�B�N��
	for (int i = 0; i < kMaxSpawnParticle; i++) {
		if (spawnParticle[i].IsUse) {
			screen.DrawBox(spawnParticle[i].Pos, 50, 50, 0, WHITE, kFillModeSolid);
		}
	}

}
