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
	
	//生成
	mIsActive = false;
	mIsDeath = false;

	//頭と尾の半径
	mSize = 75.0f;
	mRadius = mSize / 2.0f;

	//体の半径
	mBodySize = 50.0f;
	mBodyRadius = mBodySize / 2.0f;

}



void Tsuchinoko::Update(Vec2 playerposition, int mTimeLeft) {

	//速度を初期化する
	mVelocity.setZero();

	//生成処理
	Make(playerposition, mTimeLeft);

	if (mIsActive && !mIsDeath)
	{
		//移動処理
		Move(playerposition);

		//角度処理
		SetAngle();

		//ポジションに速度を代入する
		mCenterPosition += mVelocity;

		//本体
		Follow();
	}
}

void Tsuchinoko::Make(Vec2 playerPosition, int mTimeLeft) {

	if (!mIsActive || mIsDeath)
	{
		mCenterPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mCenterPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		mIsDeath = false;
		mIsActive = true;
		if (mTimeLeft < 30) {
			mIsSuper = 1;
		}
	}

}

void Tsuchinoko::Move(Vec2 playerPosition) {

	//プレイヤーを追いかけて"ない"
	if (!IsPlayerLockon) {

		//徐々に向きを変える
		mDirectionPoint += (mTargetPoint - mCenterPosition) * 0.00001f;

		if (Collision(mCenterPosition, mRadius, mTargetPoint, 50.0f)) {
			mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
			mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		}
		else {
			mDirectionPoint = mDirectionPoint.Normalized();
			mVelocity += mDirectionPoint * mCenterSpeed;
		}
	}
	//プレイヤーを追いかけて"いる"
	else {

		mDirectionPoint = mTargetPoint - mCenterPosition;
		mDirectionPoint = mDirectionPoint.Normalized();
		mVelocity += mDirectionPoint * mCenterSpeed;
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
		mIsLoadTexture = true;
	}

	//生成されていて死んでいない時
	if (mIsActive && !mIsDeath)
	{
		//頭と尾の描画
		if (mIsSuper) {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
		} else {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		}
		

		//体の描画
		for (int i = 0; i < kBodyMax; i++)
		{
			if (mIsSuper) {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, supertsuchinokobody);
			} else {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
			}
		}

		//視界描画
		screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0x0000FF80);
		
	}


}