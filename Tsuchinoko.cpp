#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Ingame.h"
#include "Function.h"



void Tsuchinoko::Init() {

	mCenterSpeed = 5.0f;
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



void Tsuchinoko::Update(Vec2 playerposition) {

	//‘¬“x‚ð‰Šú‰»‚·‚é
	mVelocity.setZero();

	//¶¬ˆ—
	Make(playerposition);

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

	//ˆê‰ž³‹K‰»‚·‚é
	Vec2 tmpTarget = mTargetPoint.Normalized();

	//™X‚ÉŒü‚«‚ð•Ï‚¦‚é
	mDirectionPoint += (tmpTarget - mDirectionPoint) * 0.01f;

	if ((mTargetPoint.x > mCenterPositionStart.x && mTargetPoint.x > mCenterPosition.x) || (mTargetPoint.x < mCenterPositionStart.x && mTargetPoint.x < mCenterPosition.x)) {
		mVelocity = mDirectionPoint * mCenterSpeed;
	} else {
		mCenterPositionStart.x = mCenterPosition.x;
		mTargetPoint.x = rand() % 6400 - (6400 / 2);
	}

	if ((mTargetPoint.y > mCenterPositionStart.y && mTargetPoint.y > mCenterPosition.y) || (mTargetPoint.y < mCenterPositionStart.y && mTargetPoint.y < mCenterPosition.y)) {
		mVelocity = mDirectionPoint * mCenterSpeed;
	} else {
		mCenterPositionStart.y = mCenterPosition.y;
		mTargetPoint.y = rand() % 3200 - (3200 / 2);
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

	//¶¬‚³‚ê‚Ä‚¢‚ÄŽ€‚ñ‚Å‚¢‚È‚¢Žž
	if (mIsActive && !mIsDeath)
	{
		//“ª‚Æ”ö‚Ì•`‰æ
		screen.DrawPicture(mHeadPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);
		screen.DrawPicture(mTailPosition, mSize, mCenterAngle, 100, 100, tsuchinoko);

		//‘Ì‚Ì•`‰æ
		for (int i = 0; i < kBodyMax; i++)
		{
			screen.DrawPicture(mBodyPosition[i], mBodySize, mCenterAngle, 100, 100, tsuchinokobody);
		}

		//“–‚½‚è”»’è•`‰æ

		//“–‚½‚è”»’è‚ÌƒfƒoƒbƒO
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