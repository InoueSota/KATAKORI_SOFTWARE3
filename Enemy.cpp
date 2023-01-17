#include "Enemy.h"
#include "Key.h"
#include "Ingame.h"




void Snake::Init() {
	//生成
	mIsActive = false;
	mIsDeath = false;
	mHeadRadius = 50;
	mBodyRadius = 30;
	mFollowFrame = 0;
}

void Snake::Update() {

	//速度の初期化
	mVelocity.setZero();

	//生成処理
	Make();

	if (mIsActive && !mIsDeath)
	{
		Move();

		//向きを変える処理
		Angle();

		//速度をポジションに代入する
		mHeadPosition += mVelocity;

		//体部分が追う処理
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

	//ランダムに移動地点を決めて移動
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

}



void Snake::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		head = Novice::LoadTexture("./Resources/Debugs/head.png");
		body = Novice::LoadTexture("./Resources/Debugs/body.png");
		fov = Novice::LoadTexture("./Resources/Enemy/fov.png");
		mIsLoadTexture = true;
	}

	//生成されていて死んでいない時
	if (mIsActive && !mIsDeath)
	{
		//体描画
		for (int i = 0; i < kBodyMax; i++)
		{
			screen.DrawPicture(mBodyPosition[i], mBodyRadius, mBodyAngle[i], 100, 100, body);
		}

		//頭描画
		screen.DrawPicture(mHeadPosition, mHeadRadius, mHeadAngle, 100, 100, head);

		//当たり判定描画

		//当たり判定のデバッグ
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
