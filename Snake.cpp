#include "Snake.h"
#include "Key.h"
#include "Ingame.h"
#include "Collision.h"
#include <Easing.h>
#include <stdio.h>



void Snake::Init() {
	//����
	mHeadPosition.setZero();
	mIsActive = false;
	mIsDeath = false;
	mIsOldDeath = false;
	mIsClearAnimation = false;
	mHeadRadius = 80;
	mBodyRadius = 60;
	mFollowFrame = 0;
	mDeadFlame = 0;
	mIsSuper = 0;
	mShakeTimer = -1;
}


void Snake::Update(int mTimeLeft, Vec2 PlayerPos, int LockonCount, bool isReady) {

	//���x�̏�����
	mVelocity.setZero();

	//��������
	if (!isReady) {
		Make(mTimeLeft, PlayerPos);
	}

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
		if (mShakeTimer == -1) {
			Move(LockonCount);
		}

		if (IsPlayerLockon && !mBikkuriFlag) {
			mBikkuriTimer = kMaxBikkuriTimer;
			mBikkuriFlag = true;
		}
		if (!IsPlayerLockon) {
			mBikkuriFlag = false;
		}
		if (mBikkuriTimer) {
			mBikkuriTimer--;
		}
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
		mIsClearAnimation = false;

		mHeadPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mHeadPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);

		while ((mHeadPosition.x >= PlayerPos.x - 1780) && (mHeadPosition.x <= PlayerPos.x + 1780)) {
			mHeadPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mHeadPosition.y >= PlayerPos.y - 1320) && (mHeadPosition.y <= PlayerPos.y + 1320)) {
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
		mShakeTimer = -1;
		//if (mTimeLeft < 30 && mTimeLeft > 0 && SuperRand <= 30) {
		//	mIsSuper = 1;
		//}
	}

}
void Snake::Move(int LockonCount) {

	//�v���C���[��ǂ�������"�Ȃ�"
	if (!IsPlayerLockon) {
		if (LockOnMoveTimer) {
			if (mIsSuper) {
				mVelocity += mDirectionPoint * mSuperSpeed;
			} else {
				mVelocity += mDirectionPoint * mSpeed;
			}
			LockOnMoveTimer--;
		} else {

			//���X�Ɍ�����ς���
			mDirectionPoint += (mTargetPoint - mHeadPosition) * 0.00001f;

			mDirectionPoint = mDirectionPoint.Normalized();
			if (mIsSuper) {
				mVelocity += mDirectionPoint * mSuperSpeed;
			} else {
				mVelocity += mDirectionPoint * mSpeed;
			}
		}
	}
	//�v���C���[��ǂ�������"����"
	else {
		mDirectionPoint += (mTargetPlayer - mHeadPosition) * 0.001f;
		mDirectionPoint = mDirectionPoint.Normalized();
		if (mIsSuper) {
			mVelocity += mDirectionPoint * (mLockonSuperSpeed + (LockonCount * 1));
		} else {
			mVelocity += mDirectionPoint * (mLockonSpeed + (LockonCount * 1));
		}

		LockOnMoveTimer = kMaxLockOnMoveTimer;
	}

	if (!(mHeadPosition.x >= Map::kMapLeft && mHeadPosition.x <= Map::kMapRight && mHeadPosition.y <= Map::kMapTop && mHeadPosition.y >= Map::kMapBottom)) {
		mIsDeath = true;
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
		mTargetPlayer = playerposition;
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
	//�́i�T�ځj
	mBodyPosition[4] = mOldHeadPosition[99];
	mBodyAngle[4] = mOldHeadAngle[99];

}


void Snake::Draw(Screen& screen, int HitStop) {

	if (!mIsLoadTexture) {
		head = Novice::LoadTexture("./Resources/Debugs/head.png");
		body = Novice::LoadTexture("./Resources/Debugs/body.png");
		superhead = Novice::LoadTexture("./Resources/Debugs/superhead.png");
		superbody = Novice::LoadTexture("./Resources/Debugs/superbody.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		hiteffect = Novice::LoadTexture("./Resources/Enemy/hiteffect.png");
		enemyspawnparticleTexture = Novice::LoadTexture("./Resources/Enemy/enemyspawnparticle.png");
		enemybikkuritexture = Novice::LoadTexture("./Resources/Enemy/bikkuri.png");
		speedTexture = Novice::LoadTexture("./Resources/Enemy/speed.png");
		mIsLoadTexture = true;
	}

	Vec2 ShakeRand = {};
	ShakeRand.x = RAND(-10, 10);
	ShakeRand.y = RAND(-10, 10);

	//��������Ă��Ď���ł��Ȃ���
	if (mIsActive && !mIsDeath)
	{
		//�̕`��
		for (int i = 0; i < kBodyMax; i++){
			if (mShakeTimer > 0 && !HitStop) {
				if (mIsSuper) {
					screen.DrawPicture(mBodyPosition[i] + ShakeRand, mBodyRadius, mBodyAngle[i], 100, 100, superbody, WHITE);
				} else {
					screen.DrawPicture(mBodyPosition[i] + ShakeRand, mBodyRadius, mBodyAngle[i], 100, 100, body, WHITE);
				}
			} else {
				if (mIsSuper) {
					screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, superbody, WHITE);
				} else {
					screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, body, WHITE);
				}
			}
			
		}

		//���`��
		if (mShakeTimer > 0) {
			if (mIsSuper) {
				screen.DrawPicture(mHeadPosition + ShakeRand, mHeadRadius, mHeadAngle, 100, 100, superhead, WHITE);
			} else {
				screen.DrawPicture(mHeadPosition + ShakeRand, mHeadRadius, mHeadAngle, 100, 100, head, WHITE);
			}
		} else {
			if (mIsSuper) {
				screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, superhead, WHITE);
			} else {
				screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, head, WHITE);
			}
		}
		

		//���E�`��
		if (IsPlayerLockon) {
			screen.DrawPicture({ mHeadPosition.x + mLockonRadius / 2 * cosf(mHeadAngle), mHeadPosition.y + mLockonRadius / 2 * -sinf(mHeadAngle) }, mLockonRadius, mHeadAngle, 500, 500, fov, 0xFF000080);
		} else {
			screen.DrawPicture({ mHeadPosition.x + mLockonRadius / 2 * cosf(mHeadAngle), mHeadPosition.y + mLockonRadius / 2 * -sinf(mHeadAngle) }, mLockonRadius, mHeadAngle, 500, 500, fov, 0x0000FF80);
		}
		
		if (mBikkuriTimer) {
			screen.DrawPicture({ mHeadPosition.x, mHeadPosition.y + 100 }, 100, 0, 50, 50, enemybikkuritexture, WHITE);
			screen.DrawPicture({ mHeadPosition.x + 100, mHeadPosition.y + 30 }, 120, 0, 50, 50, speedTexture, WHITE);
		}
		
	}

	//�X�|�[���p�[�e�B�N��
	for (int i = 0; i < kMaxSpawnParticle; i++) {
		if (spawnParticle[i].IsUse) {
			Novice::SetBlendMode(kBlendModeAdd);
			screen.DrawPicture(spawnParticle[i].Pos, 100, 0, 100, 100, enemyspawnparticleTexture, WHITE);
			Novice::SetBlendMode(kBlendModeNormal);
			//screen.DrawBox(spawnParticle[i].Pos, 50, 50, 0, WHITE, kFillModeSolid);
		}
	}

	if (mIsDeath && !mIsClearAnimation) {
		for (int i = 0; i < kBodyMax; i++) {
			mBodyDeadPosition[i] = mBodyPosition[i];
			mBodyDeadAngle[i] = mBodyAngle[i];
		}
		mHeadDeadPosition = mHeadPosition;
		mDeadAngle = mHeadAngle;
		mIsAnimation = true;
	}
	//�`��
	if (mIsAnimation) {
		mDeadFlame++;
		for (int i = 0; i < kBodyMax; i++) {
			screen.DrawAnime(mBodyDeadPosition[i], mBodyRadius * 3, mBodyDeadAngle[i], 0, 100, 100, 11, 3, mDeadFlame, hiteffect, WHITE);
		}
		screen.DrawAnime(mHeadDeadPosition, mHeadRadius * 3, mDeadAngle, 0, 100, 100, 11, 3, mDeadFlame, hiteffect, WHITE);
	}
	if (mDeadFlame == 0 && mIsAnimation) {
		mIsAnimation = false;
		mIsClearAnimation = true;
	}
}
