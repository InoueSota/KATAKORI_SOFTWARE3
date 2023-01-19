#include "Outgame.h"
#include "Function.h"
#include "Easing.h"



void Title::Init() {

	mTitleAlphaT = 0.0f;
	mTitleColor = 0xFFFFFF00;
}
void Title::Update() {

	//"KATAKORI SOFTWARE"
	if (!mIsKatakoriClear) {
		Katakori();
	}
	else {
		mTitleAlphaT = EasingClamp(0.01f, mTitleAlphaT);
		mTitleColor = ColorEasingMove(0xFFFFFF00, WHITE, easeInSine(mTitleAlphaT));
	}
}
void Title::Katakori() {

	//"KATAKORI SOFTWARE"�̕`���x�点�邽�߂̃t���[��
	mKatakoriFrame++;

	//���t���[���o�ߎ��A�`����J�n����
	if (30 < mKatakoriFrame && !mIsKatakoriAlphaClear) {
		mIsKatakoriScaleStart = true;
		mKatakoriAlphaT = EasingClamp(0.01f, mKatakoriAlphaT);
		mKatakoriColor = ColorEasingMove(0xFFFFFF00, WHITE, easeInSine(mKatakoriAlphaT));

		if (mKatakoriAlphaT == 1.0f) {
			mKatakoriFrame = 0;
			mKatakoriAlphaT = 0.0f;
			mIsKatakoriAlphaClear = true;
		}
	}

	//"KATAKORI SOFTWARE"�����t���[���`����c��������
	else if (20 < mKatakoriFrame && mIsKatakoriAlphaClear) {
		mKatakoriAlphaT = EasingClamp(0.01f, mKatakoriAlphaT);
		mKatakoriColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeOutSine(mKatakoriAlphaT));

		if (mKatakoriAlphaT == 1.0f) {
			mIsKatakoriClear = true;
		}
	}

	//"KATAKORI SOFTWARE"�������傫������
	if (mIsKatakoriScaleStart) {
		mKatakoriScale.x += 0.0008f;
		mKatakoriScale.y += 0.0008f;
	}

}
void Title::Draw(Screen& screen) {

	//"KATAKORI SOFTWARE"�̕`��
	if (!mIsKatakoriClear) {
		screen.DrawRectAngle(mKatakoriPosition, Screen::kWindowWidth, Screen::kWindowHeight, BLACK, kFillModeSolid, false);
		screen.DrawUI(mKatakoriPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 3440, 1935, mKatakori, mKatakoriColor, mKatakoriScale);
	}

	//�^�C�g����ʂ̕`��
	if (mIsKatakoriClear) {
		screen.DrawRectAngle(mKatakoriPosition, Screen::kWindowWidth, Screen::kWindowHeight, BLACK, kFillModeSolid, false);
		screen.DrawUI(mTitlePosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mTitle, mTitleColor);
	}

}
void Title::LoadTexture() {

	if (!mIsLoadTexture) {
		mKatakori = Novice::LoadTexture("./Resources/Outgame/Title/katakorisoftware.png");
		mTitle = Novice::LoadTexture("./Resources/Outgame/Title/title.png");
		mIsLoadTexture = true;
	}
}