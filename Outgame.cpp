#include "Outgame.h"
#include "Function.h"
#include "Easing.h"



void Title::Init() {

	mTitleWordBoundY = 20.0f;
	mTitleWordPosition[TO].x = mCenterPosition.x - 100.0f;
	mTitleWordPosition[TO].y = mCenterPosition.y - 100.0f;
	mTitleWordPosition[GE].x = mCenterPosition.x + 100.0f;
	mTitleWordPosition[GE].y = mTitleWordPosition[TO].y;
	mTitleWordPosition[SU].x = mCenterPosition.x - 250.0f;
	mTitleWordPosition[SU].y = mCenterPosition.y + 100.0f;
	mTitleWordPosition[MA].x = mCenterPosition.x - 125.0f;
	mTitleWordPosition[MA].y = mTitleWordPosition[SU].y;
	mTitleWordPosition[LTU].x = mCenterPosition.x;
	mTitleWordPosition[LTU].y = mTitleWordPosition[SU].y;
	mTitleWordPosition[SI].x = mCenterPosition.x + 125.0f;
	mTitleWordPosition[SI].y = mTitleWordPosition[SU].y;
	mTitleWordPosition[LYU].x = mCenterPosition.x + 250.0f;
	mTitleWordPosition[LYU].y = mTitleWordPosition[SU].y;
	mTitleWordJudgePosition[TO].y = mTitleWordPosition[TO].y;
	mTitleWordJudgePosition[GE].y = mTitleWordPosition[GE].y;
	mTitleWordJudgePosition[SU].y = mTitleWordPosition[SU].y + 40.0f;
	mTitleWordJudgePosition[MA].y = mTitleWordPosition[MA].y + 50.0f;
	mTitleWordJudgePosition[LTU].y = mTitleWordPosition[LTU].y + 80.0f;
	mTitleWordJudgePosition[SI].y = mTitleWordPosition[SI].y + 40.0f;
	mTitleWordJudgePosition[LYU].y = mTitleWordPosition[LYU].y + 80.0f;
	mTitleWordJudgeSize[TO] = 75.0f;
	mTitleWordJudgeSize[GE] = 75.0f;
	mTitleWordJudgeSize[SU] = 60.0f;
	mTitleWordJudgeSize[MA] = 50.0f;
	mTitleWordJudgeSize[LTU] = 30.0f;
	mTitleWordJudgeSize[SI] = 60.0f;
	mTitleWordJudgeSize[LYU] = 30.0f;
	mTitleWordAlphat = 0.0f;
	mTitleWordColor = 0xFFFFFF00;
	for (int i = 0; i < WORDMAX; i++) {
		mIsHitTitleWord[i] = false;
		mTitleWordJudgePosition[i].x = mTitleWordPosition[i].x;
		mTitleWordBasePositionY[i] = mTitleWordPosition[i].y;
		mTitleWordGravityY[i] = 0.0f;
		mTitleWordBoundIntervalFrame[i] = 0;
	}
	mTitleBackGroundColor = 0xDADADA00;
	mXEasingt = 0.0f;

	for (int i = 0; i < kBackBoxMax; i++) {
		mIsBackBoxActive[i] = false;
	}
	mBackBoxFrame = 0;
}
void Title::Update() {

	//"KATAKORI SOFTWARE"
	if (!mIsKatakoriClear) {
		Katakori();
	}
	else {
		mTitleWordAlphat = EasingClamp(0.02f, mTitleWordAlphat);
		mTitleWordColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutSine(mTitleWordAlphat));
		mTitleBackGroundColor = ColorEasingMove(0xFFFFFF00, 0xDADADAFF, easeOutSine(mTitleWordAlphat));

		//タイトル文字が当たった時の処理（跳ねる）
		for (int i = 0; i < WORDMAX; i++) {
			if (mIsHitTitleWord[i]) {
				mTitleWordPosition[i].y -= mTitleWordBoundY - mTitleWordGravityY[i];
				mTitleWordGravityY[i] += 1.5f;
				mTitleWordPosition[i].y = Clamp(mTitleWordPosition[i].y, 0.0f, mTitleWordBasePositionY[i]);
				if (mTitleWordPosition[i].y == mTitleWordBasePositionY[i]) {
					mTitleWordBoundIntervalFrame[i]++;
					if (60 <= mTitleWordBoundIntervalFrame[i]) {
						mTitleWordGravityY[i] = 0.0f;
						mTitleWordBoundIntervalFrame[i] = 0;
						mIsHitTitleWord[i] = false;
					}
				}
			}
		}

		//Xボタンのアニメーション
		mXEasingt = EasingClamp(0.02f, mXEasingt);
		mXScaleColor = ColorEasingMove(0xFFFFFF80, 0xFFFFFF00, easeOutSine(mXEasingt));
		mXScale = EasingMove({ 1.0f,1.0f }, { 1.5f, 1.5f }, easeOutSine(mXEasingt));
		if (mXEasingt == 1.0f) {
			mXEasingt = 0.0f;
		}

		//背景矩形フレームの加算
		mBackBoxFrame++;

		for (int i = 0; i < kBackBoxMax; i++) {

			//背景矩形生成
			if (!mIsBackBoxActive[i] && mBackBoxFrame % 4 == 0) {
				mBackBoxPosition[i].x = RAND(0, Screen::kWindowWidth);
				mBackBoxPosition[i].y = RAND(0, Screen::kWindowHeight);
				mBackBoxAngle[i] = RAND(Degree(0), Degree(360));
				mBackBoxEndSize[i] = RAND(8, 12) * 10;
				mBackBoxEasingt[i] = 0.0f;
				mIsBackBoxEasingClear[i] = false;
				mIsBackBoxActive[i] = true;
				break;
			}

			//背景矩形処理
			if (mIsBackBoxActive[i]) {
				mBackBoxAngle[i] += Degree(2);
				if (!mIsBackBoxEasingClear[i]) {
					mBackBoxEasingt[i] = EasingClamp(0.02f, mBackBoxEasingt[i]);
					mBackBoxSize[i] = EasingMove(0.0f, mBackBoxEndSize[i], easeOutSine(mBackBoxEasingt[i]));
					if (mBackBoxEasingt[i] == 1.0f) {
						mBackBoxEasingt[i] = 0.0f;
						mIsBackBoxEasingClear[i] = true;
					}
				}
				else {
					mBackBoxEasingt[i] = EasingClamp(0.1f, mBackBoxEasingt[i]);
					mBackBoxSize[i] = EasingMove(mBackBoxEndSize[i], 0.0f, easeInSine(mBackBoxEasingt[i]));
					if (mBackBoxEasingt[i] == 1.0f) {
						mIsBackBoxActive[i] = false;
					}
				}
			}
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
		screen.DrawRectAngle(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, BLACK, kFillModeSolid, false);
		screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 3440, 1935, mKatakori, mKatakoriColor, mKatakoriScale);
	}

	//タイトル画面の描画
	if (mIsKatakoriClear) {
		screen.DrawRectAngle(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, BLACK, kFillModeSolid, false);
		screen.DrawRectAngle(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, mTitleBackGroundColor, kFillModeSolid, false);
		for (int i = 0; i < kBackBoxMax; i++) {
			screen.DrawRectAngle(mBackBoxPosition[i], mBackBoxSize[i], mBackBoxSize[i], mBackBoxAngle[i], BLACK, kFillModeWireFrame, false);
		}
	}

	

}
void Title::FrontDraw(Screen& screen) {

	//タイトル画面の描画
	if (mIsKatakoriClear) {
		for (int i = 0; i < WORDMAX; i++) {
			screen.DrawUI(mTitleWordPosition[i], 300, 0, 400, 400, mTitleWord[i], mTitleWordColor);
		}
		screen.DrawUI({ mCenterPosition.x, Screen::kWindowHeight - 50 }, 75, 75, 0, 160, 160, mX, mXScaleColor, mXScale);
		screen.DrawUI({ mCenterPosition.x, Screen::kWindowHeight - 50 }, 75, 75, 0, 160, 160, mX, mTitleWordColor);
	}
}
void Title::LoadTexture() {

	if (!mIsLoadTexture) {
		mKatakori = Novice::LoadTexture("./Resources/Outgame/Title/katakorisoftware.png");
		mTitleWord[TO] = Novice::LoadTexture("./Resources/Outgame/Title/To.png");
		mTitleWord[GE] = Novice::LoadTexture("./Resources/Outgame/Title/Ge.png");
		mTitleWord[SU] = Novice::LoadTexture("./Resources/Outgame/Title/Su.png");
		mTitleWord[MA] = Novice::LoadTexture("./Resources/Outgame/Title/Ma.png");
		mTitleWord[LTU] = Novice::LoadTexture("./Resources/Outgame/Title/Ltu.png");
		mTitleWord[SI] = Novice::LoadTexture("./Resources/Outgame/Title/Si.png");
		mTitleWord[LYU] = Novice::LoadTexture("./Resources/Outgame/Title/Lyu.png");
		mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
		mIsLoadTexture = true;
	}
}


void Change::Init() {

	mIsChangeActive = false;
	mMakePosition = 0.0f;
	for (int i = 0; i < kChangeBoxMax; i++) {
		mIsChangeBoxActive[i] = false;
	}
}
void Change::Update() {

	if (mIsChangeActive) {
		mMakePosition += 10.0f;
	}

	for (int i = 0; i < kChangeBoxMax; i++) {

		if (!mIsChangeBoxActive[i] && mIsChangeActive) {
			mChangeBoxPosition[i].x = mMakePosition;
			mChangeBoxPosition[i].y = RAND(0, Screen::kWindowHeight);
			mChangeBoxEasingt[i] = 0.0f;
			mIsChangeBoxActive[i] = true;
			break;
		}

		if (mIsChangeBoxActive[i]) {
			mChangeBoxEasingt[i] = EasingClamp(0.01f, mChangeBoxEasingt[i]);
			mChangeBoxSize[i] = EasingMove(200.0f, 0.0f, easeOutSine(mChangeBoxEasingt[i]));
			if (mChangeBoxEasingt[i] == 1.0f) {
				mIsChangeBoxActive[i] = false;
			}
		}
	}

	if (Screen::kWindowWidth <= mMakePosition) {
		mIsChangeActive = false;
	}
}
void Change::DrawTitle(Screen& screen, bool isStart) {
	if (!isStart) {
		screen.DrawRectAngle({ mMakePosition / 2.0f, Screen::kWindowHeight / 2.0 }, mMakePosition, Screen::kWindowHeight, 0xDADADAFF, kFillModeSolid, false);
	} else {
		screen.DrawRectAngle({ mMakePosition / 2.0f, Screen::kWindowHeight / 2.0 }, mMakePosition, Screen::kWindowHeight, 0x383838FF, kFillModeSolid, false);
	}
}
void Change::Draw(Screen& screen, bool isStart) {

	for (int i = 0; i < kChangeBoxMax; i++) {

		if (mIsChangeBoxActive[i]) {
			if (!isStart) {
				screen.DrawSquare(mChangeBoxPosition[i], mChangeBoxSize[i], 0xDADADAFF, kFillModeSolid, false);
				screen.DrawSquare(mChangeBoxPosition[i], mChangeBoxSize[i], BLACK, kFillModeWireFrame, false);
			} else {
				screen.DrawSquare(mChangeBoxPosition[i], mChangeBoxSize[i], 0x383838FF, kFillModeSolid, false);
				screen.DrawSquare(mChangeBoxPosition[i], mChangeBoxSize[i], BLACK, kFillModeWireFrame, false);
			}
		}
	}
}


void Result::Init() {

	mIsScoreInit = false;
	mScoreEasingt = 0.0f;
	mScoreSize = 96;
	for (int i = 0; i < 8; i++) {
		mScorePosition[i].x = (Screen::kWindowWidth - 15.0f - mScoreSize / 2.0) - mScoreSize * i;
		mScorePosition[i].y = 200.0f;
	}
	mXEasingt = 0.0f;
	mSnakeKillCount = 0;
	mTsuchinokoKillCount = 0;
	mDoubleKillCount = 0;
	mTripleKillCount = 0;
	mSuperKillCount = 0;
	mIsBlackActive = false;
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

	//画面遷移用黒
	if (mIsBlackActive) {
		mBlackAlpht = EasingClamp(0.02f, mBlackAlpht);
		mBlackColor = ColorEasingMove(0x00000000, BLACK, easeOutSine(mBlackAlpht));
	}
	else {
		mBlackAlpht = 0.0f;
	}

}
void Result::Draw(Screen& screen) {

	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 1280, 720, mResult, WHITE);
	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, 1280, 720, mResultUI, WHITE);
	screen.DrawEllipse({ mCenterPosition.x, Screen::kWindowHeight - 75 }, 100, 100, 0.0f, mXScaleColor, kFillModeSolid, mXScale, false);
	screen.DrawUI({ mCenterPosition.x, Screen::kWindowHeight - 75 }, 75, 75, 0, 160, 160, mX, WHITE);

	//スコア描画
	mIsDarkScore = false;
	for (int i = 0; i < 8; i++) {
		mScoreColor[i] = 0xFFFFFF50;
	}

	int DoubleKillCountViewNum = mDoubleKillCount;
	int DoubleKillCountView[2] = {};

	DoubleKillCountView[0] = DoubleKillCountViewNum / 10;
	DoubleKillCountViewNum = DoubleKillCountViewNum % 10;

	DoubleKillCountView[1] = DoubleKillCountViewNum;

	for (int i = 0; i < 2; i++) {
		Novice::DrawSpriteRect(400 + (i * 64), 288, DoubleKillCountView[i] * 32, 0, 32, 32, mNumber, 0.2, 2, 0.0f, WHITE);
	}

	int TripleKillCountViewNum = mTripleKillCount;
	int TripleKillCountView[2] = {};

	TripleKillCountView[0] = TripleKillCountViewNum / 10;
	TripleKillCountViewNum = TripleKillCountViewNum % 10;

	TripleKillCountView[1] = TripleKillCountViewNum;

	for (int i = 0; i < 2; i++) {
		Novice::DrawSpriteRect(400 + (i * 64), 385, TripleKillCountView[i] * 32, 0, 32, 32, mNumber, 0.2, 2, 0.0f, WHITE);
	}

	int SuperKillCountViewNum = mSuperKillCount;
	int SuperKillCountView[2] = {};

	SuperKillCountView[0] = SuperKillCountViewNum / 10;
	SuperKillCountViewNum = SuperKillCountViewNum % 10;

	SuperKillCountView[1] = SuperKillCountViewNum;

	for (int i = 0; i < 2; i++) {
		Novice::DrawSpriteRect(400 + (i * 64), 482, SuperKillCountView[i] * 32, 0, 32, 32, mNumber, 0.2, 2, 0.0f, WHITE);
	}

	int SnakeKillCountViewNum = mSnakeKillCount;
	int SnakeKillCountView[3] = {};

	SnakeKillCountView[0] = SnakeKillCountViewNum / 100;
	SnakeKillCountViewNum = SnakeKillCountViewNum % 100;

	SnakeKillCountView[1] = SnakeKillCountViewNum / 10;
	SnakeKillCountViewNum = SnakeKillCountViewNum % 10;

	SnakeKillCountView[2] = SnakeKillCountViewNum;

	for (int i = 0; i < 3; i++) {
		Novice::DrawSpriteRect(1008 + (i * 64), 330, SnakeKillCountView[i] * 32, 0, 32, 32, mNumber, 0.2, 2, 0.0f, WHITE);
	}

	int TsuchinokoKillCountViewNum = mTsuchinokoKillCount;
	int TsuchinokoKillCountView[3] = {};

	TsuchinokoKillCountView[0] = TsuchinokoKillCountViewNum / 100;
	TsuchinokoKillCountViewNum = TsuchinokoKillCountViewNum % 100;

	TsuchinokoKillCountView[1] = TsuchinokoKillCountViewNum / 10;
	TsuchinokoKillCountViewNum = TsuchinokoKillCountViewNum % 10;

	TsuchinokoKillCountView[2] = TsuchinokoKillCountViewNum;

	for (int i = 0; i < 3; i++) {
		Novice::DrawSpriteRect(1008 + (i * 64), 458, TsuchinokoKillCountView[i] * 32, 0, 32, 32, mNumber, 0.2, 2, 0.0f, WHITE);
	}

	int Result[8];
	int tmpScore = mScore;
	for (int i = 7; i > -1; i--) {
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

	if (mScore >= 9000000) {
		Novice::DrawSpriteRect(33, 40, 0, 736, 264, 184, Rank, 1, 0.2, 0, WHITE);
	} else if (mScore >= 5000000) {
		Novice::DrawSpriteRect(33, 40, 0, 0, 264, 184, Rank, 1, 0.2, 0, WHITE);
	} else if (mScore >= 1000000) {
		Novice::DrawSpriteRect(33, 40, 0, 184, 264, 184, Rank, 1, 0.2, 0, WHITE);
	} else if (mScore >= 500000) {
		Novice::DrawSpriteRect(33, 40, 0, 368, 264, 184, Rank, 1, 0.2, 0, WHITE);
	} else if (mScore >= 300000) {
		Novice::DrawSpriteRect(33, 40, 0, 552, 264, 184, Rank, 1, 0.2, 0, WHITE);
	}

	screen.DrawRectAngle(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, mBlackColor, kFillModeSolid, false);
}
void Result::LoadTexture() {

	mResult = Novice::LoadTexture("./Resources/Outgame/End/end.png");
	mResultUI = Novice::LoadTexture("./Resources/Outgame/End/result.png");
	mNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
	mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
	Rank = Novice::LoadTexture("./Resources/UI/Explanation/Rank.png");
}