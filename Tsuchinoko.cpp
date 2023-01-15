#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Key.h"



void Tsuchinoko::Init() {

	mCenterPosition.x = 100;
	mCenterPosition.y = 0;
	mSpeed = 5.0f;

	//頭と尾の半径
	mRadius = 75.0f;

	//体の半径
	mBodyRadius = 50.0f;

}



void Tsuchinoko::Update() {

	//速度を0に初期化する
	mVelocity.setZero();

	//移動処理
	Move();

	//角度処理
	SetAngle();

	//ポジションに速度を代入する
	mCenterPosition += mVelocity;

	//本体
	Follow();

}

void Tsuchinoko::Move() {

	//移動処理
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

	//ランダムに移動地点を決めて移動
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

	//コントローラーで動かす処理
	/*
	//スティックの方向を取得する
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//int型 の値を float型 に入れる（int型のままだと計算が０か１になる）
	mCenterVelocity.x = tmpX;
	mCenterVelocity.y = tmpY;

	//最大値を１にする
	mCenterVelocity = mCenterVelocity.Normalized();

	//ｎ倍する
	mCenterVelocity *= mCenterSpeed;
	*/

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

	//頭と尾の描画
	screen.DrawPicture(mHeadPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);
	screen.DrawPicture(mTailPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);

	//体の描画
	for (int i = 0; i < kBodyMax; i++)
	{
		screen.DrawPicture(mBodyPosition[i], mBodyRadius, mCenterAngle, 100, 100, tsuchinoko);
	}

	//当たり判定描画

	//当たり判定のデバッグ
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