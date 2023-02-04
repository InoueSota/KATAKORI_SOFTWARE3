#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Ingame.h"
#include "Function.h"
#include "Collision.h"
#include "Easing.h"



void Tsuchinoko::Init() {

	mCenterSpeed = 5.0f;
	mSuperCenterSpeed = 7.0f;
	mVelocity.setZero();
	
	//生成
	mIsActive = false;
	mIsDeath = false;
	mIsSuper = 0;

	//頭と尾の半径
	mSize = 75.0f;
	mRadius = mSize / 2.0f;

	//体の半径
	mBodySize = 50.0f;
	mBodyRadius = mBodySize / 2.0f;

	//ヒットエフェクト
	mDeadFrame = 0;
	mIsAnimation = false;
	mIsClearAnimation = false;

	mShakeTimer = -1;
	
}


void Tsuchinoko::Update(Vec2 playerposition, int mTimeLeft, int LockonCount) {

	//速度を初期化する
	mVelocity.setZero();

	//生成処理
	Make(playerposition, mTimeLeft);

	//死亡エフェクトが完了したらフラグを初期化する
	if (mIsClearAnimation) {
		mIsClearAnimation = false;
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
		//移動処理
		if (mShakeTimer == -1) {
			Move(playerposition, LockonCount);
		}
		

		//角度処理
		SetAngle();

		//ポジションに速度を代入する
		mCenterPosition += mVelocity;

		//本体
		Follow();
	}
}
void Tsuchinoko::Make(Vec2 PlayerPos, int mTimeLeft) {

	int SuperRand = RAND(1, 100);

	if (!mIsActive || mIsDeath)
	{
		mIsClearAnimation = false;

		mCenterPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mCenterPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);

		while ((mCenterPosition.x >= PlayerPos.x - 1280) && (mCenterPosition.x <= PlayerPos.x + 1280)) {
			mCenterPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mCenterPosition.y >= PlayerPos.y - 720) && (mCenterPosition.y <= PlayerPos.y + 720)) {
			mCenterPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		}
		for (int i = 0; i < kMaxSpawnParticle; i++) {
			spawnParticle[i].Pos = mCenterPosition;
			spawnParticle[i].EndPos = mCenterPosition;
			float angle = Degree(RAND(1, 360));
			spawnParticle[i].StartPos.x = spawnParticle[i].Pos.x + cosf(angle) * 200;
			spawnParticle[i].StartPos.y = spawnParticle[i].Pos.y + sinf(angle) * 200;
			spawnParticle[i].IsUse = 1;
		}


		mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		mIsDeath = false;
		mIsActive = true;
		mShakeTimer = -1;
		if (mTimeLeft < 30 && mTimeLeft > 0 && SuperRand <= 30) {
			mIsSuper = 1;
		}
	}

}
void Tsuchinoko::Move(Vec2 playerPosition, int LockonCount) {

	//プレイヤーを追いかけて"ない"
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

			//徐々に向きを変える
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
	//プレイヤーを追いかけて"いる"
	else {
		mDirectionPoint += (mTargetPoint - mCenterPosition) * 0.001f;
		mDirectionPoint = mDirectionPoint.Normalized();
		if (mIsSuper) {
			mVelocity += mDirectionPoint * (mLockonSuperCenterSpeed + (LockonCount * 1));
		} else {
			mVelocity += mDirectionPoint * (mLockonCenterSpeed + (LockonCount * 1));
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

	//移動している時のみ角度を設定する
	if (mVelocity.x != 0.0f || mVelocity.y != 0.0f)
	{
		//基準線と向きベクトル
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mVelocity;

		//なす角を求める
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

	//なす角を求める
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
		hiteffect = Novice::LoadTexture("./Resources/Enemy/hiteffect.png");
		mIsLoadTexture = true;
	}

	Vec2 ShakeRand = {};
	ShakeRand.x = RAND(-10, 10);
	ShakeRand.y = RAND(-10, 10);

	//生成されていて死んでいない時
	if (mIsActive && !mIsDeath)
	{
		//頭と尾の描画
		if (mShakeTimer > 0) {
			if (mIsSuper) {
				screen.DrawPicture(mHeadPosition + ShakeRand, mSize, mCenterAngle, 100, 100, supertsuchinoko, WHITE);
				screen.DrawPicture(mTailPosition + ShakeRand, mSize, mCenterAngle, 100, 100, supertsuchinoko, WHITE);
			} else {
				screen.DrawPicture(mHeadPosition + ShakeRand, mSize, mCenterAngle, 100, 100, tsuchinoko, WHITE);
				screen.DrawPicture(mTailPosition + ShakeRand, mSize, mCenterAngle, 100, 100, tsuchinoko, WHITE);
			}
		} else {
			if (mIsSuper) {
				screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko, WHITE);
				screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko, WHITE);
			} else {
				screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko, WHITE);
				screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko, WHITE);
			}
		}
		
		

		//体の描画
		for (int i = 0; i < kBodyMax; i++){
			if (mShakeTimer > 0) {
				if (mIsSuper) {
					screen.DrawPicture(mBodyPosition[i] + ShakeRand, mBodySize, mCenterAngle, 100, 100, supertsuchinokobody, WHITE);
				} else {
					screen.DrawPicture(mBodyPosition[i] + ShakeRand, mBodySize, mCenterAngle, 100, 100, tsuchinokobody, WHITE);
				}
			} else {
				if (mIsSuper) {
					screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, supertsuchinokobody, WHITE);
				} else {
					screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody, WHITE);
				}
			}
		}

		//視界描画
		if (IsPlayerLockon) {
			screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0xFF000080);
		} else {
			screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0x0000FF80);
		}
		
	}

	//スポーンパーティクル
	for (int i = 0; i < kMaxSpawnParticle; i++) {
		if (spawnParticle[i].IsUse) {
			screen.DrawBox(spawnParticle[i].Pos, 50, 50, 0, WHITE, kFillModeSolid);
		}
	}

	if (mIsDeath && !mIsClearAnimation) {
		for (int i = 0; i < kBodyMax; i++) {
			mBodyDeadPosition[i] = mBodyPosition[i];
		}
		mHeadDeadPosition = mHeadPosition;
		mTailDeadPosition = mTailPosition;
		mDeadAngle = mCenterAngle;
		mIsAnimation = true;
	}
	//描画
	if (mIsAnimation) {
		mDeadFrame++;
		for (int i = 0; i < kBodyMax; i++) {
			screen.DrawAnime(mBodyDeadPosition[i], mBodySize * 3, mDeadAngle, 0, 100, 100, 11, 3, mDeadFrame, hiteffect, WHITE);
		}
		screen.DrawAnime(mHeadDeadPosition, mSize * 3, mDeadAngle, 0, 100, 100, 11, 3, mDeadFrame, hiteffect, WHITE);
		screen.DrawAnime(mTailDeadPosition, mSize * 3, mDeadAngle, 0, 100, 100, 11, 3, mDeadFrame, hiteffect, WHITE);
	}
	if (mDeadFrame == 0 && mIsAnimation) {
		mIsClearAnimation = true;
		mIsAnimation = false;
	}

}