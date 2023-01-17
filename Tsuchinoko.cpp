#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Ingame.h"
#include "Function.h"



void Tsuchinoko::Init() {

	mCenterSpeed = 5.0f;
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



void Tsuchinoko::Update(Vec2 playerposition) {

	//速度を初期化する
	mVelocity.setZero();

	//生成処理
	Make(playerposition);

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

	//一応正規化する
	Vec2 tmpTarget = mTargetPoint.Normalized();

	//プレイヤーを追いかけて"ない"
	if (!IsPlayerLockon) {

		//徐々に向きを変える
		mDirectionPoint += (tmpTarget - mDirectionPoint) * 0.01f;

		if ((mTargetPoint.x > mCenterPositionStart.x && mTargetPoint.x > mCenterPosition.x) || (mTargetPoint.x < mCenterPositionStart.x && mTargetPoint.x < mCenterPosition.x)) {
			mVelocity = mDirectionPoint * mCenterSpeed;
		}
		else {
			mCenterPositionStart.x = mCenterPosition.x;
			mTargetPoint.x = rand() % 6400 - (6400 / 2);
		}

		if ((mTargetPoint.y > mCenterPositionStart.y && mTargetPoint.y > mCenterPosition.y) || (mTargetPoint.y < mCenterPositionStart.y && mTargetPoint.y < mCenterPosition.y)) {
			mVelocity = mDirectionPoint * mCenterSpeed;
		}
		else {
			mCenterPositionStart.y = mCenterPosition.y;
			mTargetPoint.y = rand() % 3200 - (3200 / 2);
		}
	}
	//プレイヤーを追いかけて"いる"
	else {

		mDirectionPoint = mTargetPoint - mHeadPosition;

		mDirectionPoint = mDirectionPoint.Normalized();

		mVelocity = mDirectionPoint * mCenterSpeed;
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

	//生成されていて死んでいない時
	if (mIsActive && !mIsDeath)
	{
		//頭と尾の描画
		screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);

		//体の描画
		for (int i = 0; i < kBodyMax; i++)
		{
			screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
		}

		//当たり判定描画

		//当たり判定のデバッグ
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