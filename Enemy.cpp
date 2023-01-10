#include "Enemy.h"
#include "Key.h"
#include "Function.h"



void Snake::Init() {
	mHeadRadius = 50;
	mBodyRadius = 30;
	mFollowFrame = 0;
	mHeadPosition = { 0, 0 };
	for (int i = 0; i < kMaxFrame; i++){
		mOldHeadPosition[i] = mHeadPosition;
	}
}



void Snake::Update() {
	Move();
}

void Snake::Move() {

	//移動処理
	if (Key::IsPress(DIK_UP)){
		mHeadPosition.y += 3;
	}
	if (Key::IsPress(DIK_DOWN)) {
		mHeadPosition.y -= 3;
	}
	if (Key::IsPress(DIK_LEFT)) {
		mHeadPosition.x -= 3;
	}
	if (Key::IsPress(DIK_RIGHT)) {
		mHeadPosition.x += 3;
	}

	//向きを変える処理
	Angle();

	//体部分が追う処理
	Follow();
}

void Snake::Angle() {

	//基準線と向きベクトル
	Vec2 base = { 1,0 };
	Vec2 tmpDirection = mHeadPosition - mOldHeadPosition[1];

	//向きベクトルにする
	tmpDirection = tmpDirection.Normalized();

	//なす角を求める
	float dp = tmpDirection.Dot(base);
	float cp = tmpDirection.Cross(base);
	mHeadAngle = atan2(cp, dp);

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
	mFirstBodyPosition = mOldHeadPosition[19];
	mFirstBodyAngle = mOldHeadAngle[19];
	//体（２つ目）
	mSecondBodyPosition = mOldHeadPosition[39];
	mSecondBodyAngle = mOldHeadAngle[39];
	//体（３つ目）
	mThirdBodyPosition = mOldHeadPosition[59];
	mThirdBodyAngle = mOldHeadAngle[59];
	//体（４つ目）
	mFourthBodyPosition = mOldHeadPosition[79];
	mFourthBodyAngle = mOldHeadAngle[79];

}



void Snake::Draw(Screen& screen) {

	if (!mIsLoadTexture){
		head = Novice::LoadTexture("./Resources/Debugs/head.png");
		mIsLoadTexture = true;
	}

	//体描画
	screen.DrawPicture(mFourthBodyPosition, mBodyRadius, mFourthBodyAngle, 100, 100, head);
	screen.DrawPicture(mThirdBodyPosition, mBodyRadius, mThirdBodyAngle, 100, 100, head);
	screen.DrawPicture(mSecondBodyPosition, mBodyRadius, mSecondBodyAngle, 100, 100, head);
	screen.DrawPicture(mFirstBodyPosition, mBodyRadius, mFirstBodyAngle, 100, 100, head);

	//頭描画
	screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, head);

}
