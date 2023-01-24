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
	
	//¶¬
	mIsActive = false;
	mIsDeath = false;

	//“ª‚Æ”ö‚Ì”¼Œa
	mSize = 75.0f;
	mRadius = mSize / 2.0f;

	//‘Ì‚Ì”¼Œa
	mBodySize = 50.0f;
	mBodyRadius = mBodySize / 2.0f;

}



void Tsuchinoko::Update(Vec2 playerposition, int mTimeLeft) {

	//‘¬“x‚ð‰Šú‰»‚·‚é
	mVelocity.setZero();

	//¶¬ˆ—
	Make(playerposition, mTimeLeft);

	if (mIsActive && !mIsDeath)
	{
		//ˆÚ“®ˆ—
		Move(playerposition);

		//Šp“xˆ—
		SetAngle();

		//ƒ|ƒWƒVƒ‡ƒ“‚É‘¬“x‚ð‘ã“ü‚·‚é
		mCenterPosition += mVelocity;

		//–{‘Ì
		Follow();
	}
}

void Tsuchinoko::Make(Vec2 PlayerPos, int mTimeLeft) {

	int SuperRand = RAND(1, 100);

	if (!mIsActive || mIsDeath)
	{
		while ((mCenterPosition.x >= PlayerPos.x - 640) && (mCenterPosition.x <= PlayerPos.x + 640)) {
			mCenterPosition.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		}
		while ((mCenterPosition.y >= PlayerPos.y - 360) && (mCenterPosition.y <= PlayerPos.y + 360)) {
			mCenterPosition.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		}


		mTargetPoint.x = RAND(Map::kMapLeft + 100.0f, Map::kMapRight - 100.0f);
		mTargetPoint.y = RAND(Map::kMapBottom + 100.0f, Map::kMapTop - 100.0f);
		mIsDeath = false;
		mIsActive = true;
		if (mTimeLeft < 30 && SuperRand <= 30) {
			mIsSuper = 1;
		}
	}

}

void Tsuchinoko::Move(Vec2 playerPosition) {

	//ƒvƒŒƒCƒ„[‚ð’Ç‚¢‚©‚¯‚Ä"‚È‚¢"
	if (!IsPlayerLockon) {
		if (LockOnMoveTimer) {
			if (mIsSuper) {
				mVelocity += mDirectionPoint * mSuperCenterSpeed;
			} else {
				mVelocity += mDirectionPoint * mCenterSpeed;
			}
			LockOnMoveTimer--;
		} else {

			//™X‚ÉŒü‚«‚ð•Ï‚¦‚é
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
	//ƒvƒŒƒCƒ„[‚ð’Ç‚¢‚©‚¯‚Ä"‚¢‚é"
	else {
		mDirectionPoint += (mTargetPoint - mCenterPosition) * 0.001f;
		mDirectionPoint = mDirectionPoint.Normalized();
		if (mIsSuper) {
			mVelocity += mDirectionPoint * mSuperCenterSpeed;
		} else {
			mVelocity += mDirectionPoint * mCenterSpeed;
		}
		LockOnMoveTimer = kMaxLockOnMoveTimer;
	}


}


void Tsuchinoko::SetAngle() {

	//ˆÚ“®‚µ‚Ä‚¢‚éŽž‚Ì‚ÝŠp“x‚ðÝ’è‚·‚é
	if (mVelocity.x != 0.0f || mVelocity.y != 0.0f)
	{
		//Šî€ü‚ÆŒü‚«ƒxƒNƒgƒ‹
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mVelocity;

		//‚È‚·Šp‚ð‹‚ß‚é
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

	//‚È‚·Šp‚ð‹‚ß‚é
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

	//¶¬‚³‚ê‚Ä‚¢‚ÄŽ€‚ñ‚Å‚¢‚È‚¢Žž
	if (mIsActive && !mIsDeath)
	{
		//“ª‚Æ”ö‚Ì•`‰æ
		if (mIsSuper) {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, supertsuchinoko);
		} else {
			screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
			screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		}
		

		//‘Ì‚Ì•`‰æ
		for (int i = 0; i < kBodyMax; i++)
		{
			if (mIsSuper) {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, supertsuchinokobody);
			} else {
				screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
			}
		}

		//Ž‹ŠE•`‰æ
		screen.DrawPicture({ mCenterPosition.x + mLockonRadius / 2 * cosf(mCenterAngle), mCenterPosition.y + mLockonRadius / 2 * -sinf(mCenterAngle) }, mLockonRadius, mCenterAngle, 500, 500, fov, 0x0000FF80);
		
	}


}