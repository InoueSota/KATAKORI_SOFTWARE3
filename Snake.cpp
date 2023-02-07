#include "Snake.h"
#include "Key.h"
#include "Ingame.h"
#include "Collision.h"
#include <Easing.h>



void Snake::Init() {
	//生成
	mIsActive = false;
	mIsDeath = false;
	mIsClearAnimation = false;
	mHeadRadius = 80;
	mBodyRadius = 60;
	mFollowFrame = 0;
	mDeadFlame = 0;
	mIsSuper = 0;
	mShakeTimer = -1;
}


void Snake::Update(int mTimeLeft, Vec2 PlayerPos, int LockonCount) {

	//速度の初期化
	mVelocity.setZero();

	//生成処理
	Make(mTimeLeft, PlayerPos);

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
		//向きを変える処理
		Angle();

		//速度をポジションに代入する
		mHeadPosition += mVelocity;

		//体部分が追う処理
		Follow();
	}
}
void Snake::FirstMake() {

	if (!mIsActive || mIsDeath)
	{
		mIsClearAnimation = false;

		mHeadPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mHeadPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);

		while ((mHeadPosition.x >= -1280) && (mHeadPosition.x <= 1280)) {
			mHeadPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mHeadPosition.y >= -720) && (mHeadPosition.y <= 720)) {
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
		if (mTimeLeft < 30 && mTimeLeft > 0 && SuperRand <= 30) {
			mIsSuper = 1;
		}
	}

}
void Snake::Move(int LockonCount) {

	//プレイヤーを追いかけて"ない"
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
			//徐々に向きを変える
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
	//プレイヤーを追いかけて"いる"
	else {

		mDirectionPoint += (mTargetPoint - mHeadPosition) * 0.001f;

		mDirectionPoint = mDirectionPoint.Normalized();

		if (mIsSuper) {
			mVelocity += mDirectionPoint * (mLockonSuperSpeed + (LockonCount * 1));
		} else {
			mVelocity += mDirectionPoint * (mLockonSpeed + (LockonCount * 1));
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

	//移動している時のみ角度を設定する
	if (mVelocity.x != 0.0f || mVelocity.y != 0.0f)
	{
		//基準線と向きベクトル
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mVelocity;

		//なす角を求める
		float dp = tmpDirection.Dot(base);
		float cp = tmpDirection.Cross(base);
		mHeadAngle = atan2(cp, dp);
	}
}
void Snake::LockOn(Vec2 playerposition, float radius) {

	Vec2 toPlayer = playerposition - mHeadPosition;

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
void Snake::Follow() {

	//頭が動いているとき
	if (mOldHeadPosition[0].x != mHeadPosition.x || mOldHeadPosition[0].y != mHeadPosition.y) {

		//加算しているフレームが最大値以下のとき
		if (mFollowFrame < kMaxFrame) {

			//１フレーム分更新する
			for (int i = mFollowFrame - 1; i > -1; i--) {
				if (i != 0) {
					mOldHeadPosition[i] = mOldHeadPosition[i - 1];
					mOldHeadAngle[i] = mOldHeadAngle[i - 1];
				}
			}

			//０フレーム目は頭
			mOldHeadPosition[0] = mHeadPosition;
			mOldHeadAngle[0] = mHeadAngle;

			//フレームを加算する
			mFollowFrame++;
		}
		else {

			//１フレーム分更新する
			for (int i = kMaxFrame - 1; i > -1; i--) {
				if (i != 0) {
					mOldHeadPosition[i] = mOldHeadPosition[i - 1];
					mOldHeadAngle[i] = mOldHeadAngle[i - 1];
				}
			}
			//０番目は頭に更新する
			mOldHeadPosition[0] = mHeadPosition;
			mOldHeadAngle[0] = mHeadAngle;
		}
	}
	
	//任意のOldPositionを代入する
	//体（１つ目）
	mBodyPosition[0] = mOldHeadPosition[19];
	mBodyAngle[0] = mOldHeadAngle[19];
	//体（２つ目）
	mBodyPosition[1] = mOldHeadPosition[39];
	mBodyAngle[1] = mOldHeadAngle[39];
	//体（３つ目）
	mBodyPosition[2] = mOldHeadPosition[59];
	mBodyAngle[2] = mOldHeadAngle[59];
	//体（４つ目）
	mBodyPosition[3] = mOldHeadPosition[79];
	mBodyAngle[3] = mOldHeadAngle[79];
	//体（５つ目）
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

	//生成されていて死んでいない時
	if (mIsActive && !mIsDeath)
	{
		//体描画
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

		//頭描画
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
		

		//視界描画
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

	//スポーンパーティクル
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
	//描画
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
