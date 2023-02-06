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

		//Aボタンのアニメーション
		mAEasingt = EasingClamp(0.02f, mAEasingt);
		mAScaleColor = ColorEasingMove(0xFFFFFF80, 0xFFFFFF00, easeOutSine(mAEasingt));
		mAScale = EasingMove({ 1.0f,1.0f }, { 1.5f, 1.5f }, easeOutSine(mAEasingt));
		if (mAEasingt == 1.0f) {
			mAEasingt = 0.0f;
		}
	}
}
void Title::Katakori() {

	//"KATAKORI SOFTWARE"の描画を遅らせるためのフレーム
	mKatakoriFrame++;

	//一定フレーム経過時、描画を開始する
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

	//"KATAKORI SOFTWARE"を一定フレーム描画を残したあと
	else if (20 < mKatakoriFrame && mIsKatakoriAlphaClear) {
		mKatakoriAlphaT = EasingClamp(0.01f, mKatakoriAlphaT);
		mKatakoriColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeOutSine(mKatakoriAlphaT));

		if (mKatakoriAlphaT == 1.0f) {
			mIsKatakoriClear = true;
		}
	}

	//"KATAKORI SOFTWARE"を少し大きくする
	if (mIsKatakoriScaleStart) {
		mKatakoriScale.x += 0.0008f;
		mKatakoriScale.y += 0.0008f;
	}

}
void Title::Draw(Screen& screen) {

	//"KATAKORI SOFTWARE"の描画
	if (!mIsKatakoriClear) {
		screen.DrawRectAngle(mKatakoriPosition, Screen::kWindowWidth, Screen::kWindowHeight, BLACK, kFillModeSolid, false);
		screen.DrawUI(mKatakoriPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 3440, 1935, mKatakori, mKatakoriColor, mKatakoriScale);
	}

	//タイトル画面の描画
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


void Result::Init() {

	mIsScoreInit = false;
	mScoreEasingt = 0.0f;
	mScoreSize = 96;
	for (int i = 0; i < 7; i++) {
		mScorePosition[i].x = (Screen::kWindowWidth - 15.0f - mScoreSize / 2.0) - mScoreSize * i;
		mScorePosition[i].y = 200.0f;
	}
	mXEasingt = 0.0f;
}
void Result::Update(float score) {

	//スコアのアニメーション
	if (!mIsScoreInit) {
		mStartScore = 0.0f;
		mEndScore = score;
		mScoreEasingt = 0.0f;
		mIsScoreInit = true;
	} else {
		mScoreEasingt = EasingClamp(0.008f, mScoreEasingt);
		mScore = EasingMove(mStartScore, mEndScore, easeOutCirc(mScoreEasingt));
	}

	//Xボタンのアニメーション
	mXEasingt = EasingClamp(0.02f, mXEasingt);
	mXScaleColor = ColorEasingMove(0x00000080, 0x00000000, easeOutSine(mXEasingt));
	mXScale = EasingMove({ 1.0f,1.0f }, { 1.5f, 1.5f }, easeOutSine(mXEasingt));
	if (mXEasingt == 1.0f) {
		mXEasingt = 0.0f;
	}

}
void Result::Draw(Screen& screen) {

	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 1280, 720, mResult, WHITE);
	screen.DrawEllipse({ mCenterPosition.x, Screen::kWindowHeight - 75 }, 100, 100, 0.0f, mXScaleColor, kFillModeSolid, mXScale, false);
	screen.DrawUI({ mCenterPosition.x, Screen::kWindowHeight - 75 }, 75, 75, 0, 160, 160, mX, WHITE);

	//スコア描画
	mIsDarkScore = false;
	for (int i = 0; i < 7; i++) {
		mScoreColor[i] = 0xFFFFFF50;
	}

	int Result[7];
	int tmpScore = mScore;
	for (int i = 6; i > -1; i--) {
		Result[i] = tmpScore / powf(10, i);
		tmpScore = tmpScore % (int)powf(10, i);
		if ((int)Result[i] != 0) {
			mIsDarkScore = true;
		}
		if (mIsDarkScore) {
			mScoreColor[i] = WHITE;
		} else {
			mScoreColor[i] = 0xFFFFFF50;
		}
		screen.DrawUI(mScorePosition[i], mScoreSize, 32 * Result[i], 32, 32, mNumber, mScoreColor[i]);
	}
}
void Result::LoadTexture() {

	mResult = Novice::LoadTexture("./Resources/Outgame/End/end.png");
	mNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
	mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
}