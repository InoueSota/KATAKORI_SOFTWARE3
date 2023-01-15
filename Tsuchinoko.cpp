#include "Tsuchinoko.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Key.h"



void Tsuchinoko::Init() {

	mCenterPosition.x = 100;
	mCenterPosition.y = 0;
	mSpeed = 5.0f;

	//“ª‚Æ”ö‚Ì”¼Œa
	mRadius = 75.0f;

	//‘Ì‚Ì”¼Œa
	mBodyRadius = 50.0f;

}



void Tsuchinoko::Update() {

	//‘¬“x‚ð0‚É‰Šú‰»‚·‚é
	mVelocity.setZero();

	//ˆÚ“®ˆ—
	Move();

	//Šp“xˆ—
	SetAngle();

	//ƒ|ƒWƒVƒ‡ƒ“‚É‘¬“x‚ð‘ã“ü‚·‚é
	mCenterPosition += mVelocity;

	//–{‘Ì
	Follow();

}

void Tsuchinoko::Move() {

	//ˆÚ“®ˆ—
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

	//ƒ‰ƒ“ƒ_ƒ€‚ÉˆÚ“®’n“_‚ðŒˆ‚ß‚ÄˆÚ“®
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

	//ƒRƒ“ƒgƒ[ƒ‰[‚Å“®‚©‚·ˆ—
	/*
	//ƒXƒeƒBƒbƒN‚Ì•ûŒü‚ðŽæ“¾‚·‚é
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//intŒ^ ‚Ì’l‚ð floatŒ^ ‚É“ü‚ê‚éiintŒ^‚Ì‚Ü‚Ü‚¾‚ÆŒvŽZ‚ª‚O‚©‚P‚É‚È‚éj
	mCenterVelocity.x = tmpX;
	mCenterVelocity.y = tmpY;

	//Å‘å’l‚ð‚P‚É‚·‚é
	mCenterVelocity = mCenterVelocity.Normalized();

	//‚Ž”{‚·‚é
	mCenterVelocity *= mCenterSpeed;
	*/

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

	//“ª‚Æ”ö‚Ì•`‰æ
	screen.DrawPicture(mHeadPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);
	screen.DrawPicture(mTailPosition, mRadius, mCenterAngle, 100, 100, tsuchinoko);

	//‘Ì‚Ì•`‰æ
	for (int i = 0; i < kBodyMax; i++)
	{
		screen.DrawPicture(mBodyPosition[i], mBodyRadius, mCenterAngle, 100, 100, tsuchinoko);
	}

	//“–‚½‚è”»’è•`‰æ

	//“–‚½‚è”»’è‚ÌƒfƒoƒbƒO
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