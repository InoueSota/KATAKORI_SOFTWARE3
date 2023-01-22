#include "Screen.h"
#include "MatVec.h"
#include "Quad.h"
#include "Function.h"
#include "Easing.h"
#include "ControllerInput.h"
#include "Key.h"
#include "Player.h"
#include "Ingame.h"



void Screen::Init() {
	mWorldCenter = { kWindowWidth / 2, kWindowHeight / 2 };
	mScroll.setZero();
	mZoom = 0.4f;

	mScreenShake.setZero();
	mShakeMag = 1.0f;
	mShakeEasingt = 0.0f;
	mIsShakeActive = false;

	mMiniMapCenter = { kWindowWidth - kMiniMapSize - 10.0f, kWindowHeight - kMiniMapSize - 10.0f };
	mMiniMapZoom = (float)kMiniMapSize / (float)Map::kMapRadius;
};



void Screen::SetShake(bool condition) {

	if (condition)
	{
		mShakeEasingt = 0.0f;
		mShakeMag = 1.0f;
		mIsShakeActive = true;
	}

	if (mIsShakeActive) {

		mShakeEasingt = EasingClamp(0.033f, mShakeEasingt);
		mShakeMag = EasingMove(1.0f, 0.0f, easeOutSine(mShakeEasingt));

		mScreenShake.x = RAND(-5.0f, 5.0f) * mShakeMag;
		mScreenShake.y = RAND(-5.0f, 5.0f) * mShakeMag;

		if (mShakeEasingt == 1.0f) {
			mIsShakeActive = false;
		}
	}
}

void Screen::SetShake(int min, int max, bool condition) {

	if (condition == true)
	{
		mScreenShake.x = RAND(min, max);
		mScreenShake.y = RAND(min, max);
	}
	else
	{
		mScreenShake = { 0.0f, 0.0f };
	}

}

void Screen::SetShake(int minX, int maxX, int minY, int maxY, bool condition) {

	if (condition == true)
	{
		mScreenShake.x = RAND(minX, maxX);
		mScreenShake.y = RAND(minY, maxY);
	}
	else
	{
		mScreenShake = { 0.0f, 0.0f };
	}

}

void Screen::SetScroll(Player& player) {

	if (!player.mIsStrikeActive)
	{
		mScroll.x += (( player.mPosition.x * mZoom) - mScroll.x) * 0.15f;
		mScroll.y += ((-player.mPosition.y * mZoom) - mScroll.y) * 0.15f;
	}
	else 
	{
		mScroll.x += (( player.mPosition.x * mZoom) - mScroll.x) * 0.15f;
		mScroll.y += ((-player.mPosition.y * mZoom) - mScroll.y) * 0.15f;
	}

}

void Screen::SetZoom() {

	if (Key::IsPress(DIK_UP) || Controller::IsPressedButton(0,Controller::bUP))
	{
		mZoom += 0.01f;
	}
	else if (Key::IsPress(DIK_DOWN) || Controller::IsPressedButton(0, Controller::bDOWN))
	{
		mZoom -= 0.01f;
	}

	mZoom = Clamp(mZoom, 0.1f, 5.0f);

}


//--------------------------------------------------------------------------------------------//


void Screen::DrawLine(Vec2 startposition, Vec2 endposition, unsigned int color) {
	startposition = ScreenTransform(startposition);
	endposition = ScreenTransform(endposition);
	Novice::DrawLine((int)startposition.x, (int)startposition.y, (int)endposition.x, (int)endposition.y, color);
}


void Screen::DrawBox(Vec2 Position, float w, float h, float angle, unsigned int color, FillMode fillMode, bool isScroll) {
	float tmpZoom;
	if (isScroll) {
		tmpZoom = mZoom;
		Position = ScreenTransform(Position);
	}
	else {
		tmpZoom = 1.0f;
	}
	Novice::DrawBox((int)Position.x, (int)Position.y, w * tmpZoom, h * tmpZoom, angle, color, fillMode);
}


void Screen::DrawEllipse(Vec2 Position, float radiusX, float radiusY, float angle, unsigned int color, FillMode fillMode) {
	Position = ScreenTransform(Position);
	Novice::DrawEllipse((int)Position.x, (int)Position.y, radiusX * mZoom, radiusY * mZoom, angle, color, fillMode);
}


void Screen::DrawCircle(Vec2 Position, float radius, unsigned int color, FillMode fillMode) {
	DrawEllipse(Position, radius, radius, 0.0f, color, fillMode);
}


void Screen::DrawRectAngle(Vec2 position, float width, float height, float angle, unsigned int color, FillMode fillMode, bool isScroll) {
	Quad OriginalPosition;
	Quad Rect;
	if (isScroll) {
		OriginalPosition = RectAssign(width, height);
		Rect = Transform(OriginalPosition, MakeAffineMatrix({ mZoom, mZoom }, angle, ScreenTransform(position)));
	}
	else {
		OriginalPosition = RectAssign(width, height);
		Rect = Transform(OriginalPosition, MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, position));
	}
	if (fillMode == kFillModeSolid) {
		Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, 0, 0, 0, 0, 192, color);
	}
	else {
		Novice::DrawLine((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, color);
		Novice::DrawLine((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, color);
		Novice::DrawLine((int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, color);
		Novice::DrawLine((int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, color);
	}
}


void Screen::DrawRectAngle(Vec2 Position, float Width, float Height, unsigned int color, FillMode fillMode, bool isScroll) {
	DrawRectAngle(Position, Width, Height, 0.0f, color, fillMode, isScroll);
}


void Screen::DrawSquare(Vec2 Position, float size, unsigned int color, FillMode fillMode, bool isScroll) {
	if (isScroll) {
		DrawRectAngle(Position, size, size, color, fillMode);
	}
	else {
		Quad OriginalPosition = RectAssign(size, size);
		Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, Position));
		if (fillMode == kFillModeSolid) {
			Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, 0, 0, 0, 0, 192, color);
		}
		else {
			Novice::DrawLine((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, color);
			Novice::DrawLine((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, color);
			Novice::DrawLine((int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, color);
			Novice::DrawLine((int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, color);
		}
	}
}


void Screen::DrawPicture(Vec2 Position, float size, float angle, float srcW, float srcH, int textureHandle, unsigned int color) {
	Quad OriginalPosition = RectAssign(size, size);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ mZoom, mZoom }, angle, ScreenTransform(Position)));
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, 0, 0, srcW, srcH, textureHandle, color);
}


void Screen::DrawAnime(Vec2 Position, float size, int srcX, int srcW, int srcH, int sheets, int frame, int& frameVariable, int textureHandle, unsigned int color) {
	Quad OriginalPosition = RectAssign(size, size);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ mZoom,mZoom }, 0.0f, ScreenTransform(Position)));
	srcX = srcW * (frameVariable / frame);
	if (srcX >= srcW * sheets) {
		frameVariable = 0;
		srcX = 0;
	}
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, 0, srcW, srcH, textureHandle, color);
}


void Screen::DrawUI(Vec2 Position, float width, float height, int srcX, int srcW, int srcH, int textureHandle, unsigned int color, Vec2 scale) {
	Quad OriginalPosition = RectAssign(width, height);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix(scale, 0.0f, Position));
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, 0, srcW, srcH, textureHandle, color);
}


void Screen::DrawUI(Vec2 Position, float size, int srcX, int srcW, int srcH, int textureHandle, unsigned int color, Vec2 scale) {
	DrawUI(Position, size, size, srcX, srcW, srcH, textureHandle, color, scale);
}


void Screen::DrawMiniMap(Vec2 position, unsigned int color, FillMode fillMode) {
	position = MiniMapTransform(position);
	Novice::DrawEllipse((int)position.x, (int)position.y, 2.0f, 2.0f, 0.0f, color, fillMode);
}


//--------------------------------------------------------------------------------------------//


Vec2 Screen::ScreenTransform(Vec2 position) {

	return{
		position.x * mZoom - mScroll.x + mWorldCenter.x + mScreenShake.x,
		position.y * mZoom * -1 - mScroll.y + mWorldCenter.y + mScreenShake.y
	};
}


Vec2 Screen::MiniMapTransform(Vec2 position) {

	return {
		position.x * mMiniMapZoom + mMiniMapCenter.x,
		position.y * mMiniMapZoom * -1 + mMiniMapCenter.y
	};
}


Vec2 Screen::WorldTransform(Vec2 position) {

	return {
		position.x * mZoom - mScroll.x + mWorldCenter.x,
		position.y * mZoom * -1 - mScroll.y + mWorldCenter.y
	};
}
