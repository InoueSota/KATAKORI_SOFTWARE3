#include "Outgame.h"
#include "Function.h"
#include "Easing.h"



void Title::Init() {

	mTitleAlphaT = 0.0f;
	mTitleColor = 0xFFFFFF00;
	mAEasingt = 0.0f;
}
void Title::Update() {

	//"KATAKORI SOFTWARE"
	if (!mIsKatakoriClear) {
		Katakori();
	}
	else {
		mTitleAlphaT = EasingClamp(0.02f, mTitleAlphaT);
		mTitleColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutSine(mTitleAlphaT));

		//A�{�^���̃A�j���[�V����
		mAEasingt = EasingClamp(0.02f, mAEasingt);
		mAScaleColor = ColorEasingMove(0xFFFFFF80, 0xFFFFFF00, easeOutSine(mAEasingt));
		mAScale = EasingMove({ 1.0f,1.0f }, { 1.5f, 1.5f }, easeOutSine(mAEasingt));
		if (mAEasingt == 1.0f) {
			mAEasingt = 0.0f;
		}
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
		screen.DrawUI(mTitlePosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 1920, 1080, mTitle, mTitleColor);
		screen.DrawUI({ mTitlePosition.x, Screen::kWindowHeight - 75 }, 75, 75, 0, 160, 160, mA, mAScaleColor, mAScale);
		screen.DrawUI({ mTitlePosition.x, Screen::kWindowHeight - 75 }, 75, 75, 0, 160, 160, mA, mTitleColor);
	}

}
void Title::LoadTexture() {

	if (!mIsLoadTexture) {
		mKatakori = Novice::LoadTexture("./Resources/Outgame/Title/katakorisoftware.png");
		mTitle = Novice::LoadTexture("./Resources/Outgame/Title/title.png");
		mA = Novice::LoadTexture("./Resources/UI/Explanation/a.png");
		mIsLoadTexture = true;
	}
}