#include "Ingame.h"
#include "Player.h"
#include "Function.h"
#include "Easing.h"
#include "ControllerInput.h"



//ゲーム中のＵＩ
void UI::Init() {

	//始めるかフラグ
	mIsReady = false;
	mIsStart = false;
	mIsOldStart = true;
	mStartPosition = { -Screen::kWindowWidth / 2.0 + 200,Screen::kWindowHeight / 2.0 };
	mTutorialScene = 0;
	mTutorialControl = 0;

	//画面の中心
	mCenterPosition.x = Screen::kWindowWidth / 2.0;
	mCenterPosition.y = Screen::kWindowHeight / 2.0;

	//制限時間
	mIsExtendTime = false;
	mTimeScale = { 1.0f, 1.0f };
	mTimeUISize = 48;
	mTimeLastUISize = 256;
	mTimePosition[0].x = Screen::kWindowWidth / 2.0 + mTimeUISize / 2.0 + 5.0f;
	mTimePosition[0].y = mTimeUISize / 2.0 + 45.0f;
	mTimePosition[1].x = Screen::kWindowWidth / 2.0 - mTimeUISize / 2.0 - 5.0f;
	mTimePosition[1].y = mTimePosition[0].y;
	mTimeLeft = kTimeLimit;
	mTimeElapsed = 0;
	mTimeFrame = 0;
	mTimeLastColor = 0xFFFFFFFF;
	mTimeEasingt = 0.0f;
	mIsTimeUpFinish = false;
	mTimeUpAlphat = 0.0f;
	mTimeUpColor = 0xFFFFFF00;
	
	//コンボ
	mIsKillCountScaleAnimation = false;
	mKillCountPosition[0].x = Screen::kWindowWidth - mTimeUISize;
	mKillCountPosition[0].y = 300.0f;
	mKillCountPosition[1].x = Screen::kWindowWidth - mTimeUISize * 2;
	mKillCountPosition[1].y = mKillCountPosition[0].y;
	mKillCountPosition[2].x = Screen::kWindowWidth - mTimeUISize * 3;
	mKillCountPosition[2].y = mKillCountPosition[0].y;
	mKillCountScale.x = 1.0f;
	mKillCountScale.y = 1.0f;
	mKillCount = 0;

	//スコア
	mScore = 0;
	for (int i = 0; i < 8; i++) {
		mScorePosition[i].x = Screen::kWindowWidth - mTimeUISize * (i + 1);
		mScorePosition[i].y = 100.0f;
	}
	for (int i = 0; i < kEnemyScoreMax; i++) {
		mIsEnemyScoreActive[i] = false;
	}
	mScoreAnimationEasingt = 0.0f;
	mStartScore = 0.0f;
	mEndScore = 0.0f;

	//頭か尾に当たってしまったとき
	mIsWarning = false;
	mIsWarningRed = false;
	mWarningAlphat = 0.0f;
	mWarningColor = 0xFFFFFF00;

	//操作説明
	mXScale = { 1.0f, 1.0f };
	mXScaleActive = false;
	mAScale = { 1.0f, 1.0f };
	mAScaleActive = false;
}
void UI::Update(int mStrikeTutorial, int mIsStrikeActive) {

	//前回準備完了フラグの保存
	mIsOldReady = mIsReady;

	//操作確認フェーズを終了するか
	if (!mIsReady) {
		switch (mTutorialScene) {
			case 0:
				if (Controller::IsTriggerButton(0, Controller::bA)) {
					mTutorialControl++;
					if (mTutorialControl >= 10) {
						mTutorialScene = 1;
						mTutorialControl = 0;
					}
				}
				break;
			case 1:
				if (Controller::IsTriggerButton(0, Controller::bX)) {
					mTutorialControl++;
					if (mTutorialControl >= 3) {
						mTutorialScene = 2;
						mTutorialControl = 0;
					}
				}
				break;
			case 2:
				if (mStrikeTutorial == 1) {
					mTutorialControl++;
				}
				if (mTutorialControl >= 3) {
					mTutorialScene = 3;
					mTutorialControl = 0;
				}
				break;
			case 3:
				if (Controller::IsTriggerButton(0, Controller::bA) && mIsStrikeActive) {
					if (!mSlowTutorialFlag) {
						mSlowTutorialFlag = true;
						mTutorialControl++;
					}
				}
				if (mStrikeTutorial == 1 && mSlowTutorialFlag == true) {
					mSlowTutorialFlag = false;
				}
				if (mTutorialControl >= 3) {
					mTutorialScene = 4;
					mTutorialControl = 0;
				}
				
				break;
		}
		if (Controller::IsTriggerButton(0, Controller::bY)) {
			mIsReady = true;
		}
	}

	//コンボ
	KillCount();

	//スコアのアニメーション
	ScoreAnimation();

	//頭か尾に当たってしまったとき
	Warning();

	//操作説明の色を押してるときは光らせる
	if (Controller::IsPressedButton(0, Controller::bX)) {
		mXColor = WHITE;

		if (Controller::IsTriggerButton(0, Controller::bX)) {
			mXEasingt = 0.0f;
			mXScaleActive = true;
		}
	} else {
		mXColor = 0xFFFFFF50;
	}
	if (Controller::IsPressedButton(0, Controller::bA)) {
		mAColor = WHITE;

		if (Controller::IsTriggerButton(0, Controller::bA)) {
			mAEasingt = 0.0f;
			mAScaleActive = true;
		}
	}
	else {
		mAColor = 0xFFFFFF50;
	}
	if (LeftStickVelocity(15.0f).x != 0.0f) {
		mLStickColor = WHITE;
		mLStickBackColor = 0x606060FF;
	} else {
		mLStickColor = 0xFFFFFF40;
		mLStickBackColor = 0x60606040;
	}

	//スケールイージング
	if (mXScaleActive) {

		mXEasingt = EasingClamp(0.2f, mXEasingt);
		mXScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mXEasingt));

		if (mXEasingt == 1.0f) {
			mXScaleActive = false;
		}
	}
	if (mAScaleActive) {

		mAEasingt = EasingClamp(0.2f, mAEasingt);
		mAScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mAEasingt));

		if (mAEasingt == 1.0f) {
			mAScaleActive = false;
		}
	}
}
void UI::ExtendTime(bool isFever, bool isOldFever) {

	//フィーバーに入ったら時間を延ばす
	if (isFever && !isOldFever) {
		mExtendTimeEasingt = 0.0f;
		mIsExtendTime = true;
		mTimeElapsed -= 5;
	}

	//フィーバーで時間が延びたよイージング
	if (mIsExtendTime) {
		mExtendTimeEasingt = EasingClamp(0.01f, mExtendTimeEasingt);
		mTimeScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mExtendTimeEasingt));

		if (mExtendTimeEasingt == 1.0f) {
			mIsExtendTime = false;
		}
	}
}
void UI::TimeLimit() {

	//敵の発生をさせるため、１フレームでやらせる
	mIsOldStart = mIsStart;

	if (mIsReady) {

		//フレームが60になったら一秒経過
		if (mTimeFrame == 60 && mTimeLeft != 0) {

			mTimeElapsed++;
			mTimeFrame = 0;

			//カウントダウンの初期化
			if (mTimeLeft < 10 || !mIsStart) {
				mTimeEasingt = 0.0f;
				mTimeLastColor = 0xFFFFFF70;
				mTimeLastScale = { 1.0f, 1.0f };

			}
		}

		//時間のフレームを加算する
		mTimeFrame++;

		//残り時間 = 制限時間 - 経過時間
		if (!mIsStart) {
			mCountDownTime = kToStart - mTimeElapsed;
		}
		else {
			mTimeLeft = kTimeLimit - mTimeElapsed;
		}

		//残り時間の限界値の設定
		mTimeLeft = Clamp(mTimeLeft, 0, kTimeLimit);
		mTimeElapsed = Clamp(mTimeElapsed, 0, kTimeLimit);
		mCountDownTime = Clamp(mCountDownTime, 0, kToStart);

		//カウントダウンの透明度と大きさをイージング処理する
		if (mTimeLeft < 10 || !mIsStart) {
			if (mTimeEasingt == 0.0f) {
				if ((!mIsStart && 0 < mCountDownTime) || (mIsStart && 0 < mTimeLeft)) {
					int handle = -1;
					if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
						handle = Novice::PlayAudio(countdown, 0, 1);
					}
				}
				if (mTimeLeft == 0) {
					int handle = -1;
					if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
						handle = Novice::PlayAudio(finish, 0, 1);
					}
				}
			}
			mTimeEasingt = EasingClamp(1.0f / 60.0f, mTimeEasingt);
			mTimeLastColor = ColorEasingMove(0xFFFFFF70, 0xFFFFFF00, easeOutSine(mTimeEasingt));
			mTimeLastScale = EasingMove({ 1.0f, 1.0f }, { 2.0f,2.0f }, easeOutSine(mTimeEasingt));
		}

		//始めるフラグをtrueにする
		if (!mIsStart && mCountDownTime == 0) {
			mTimeElapsed = 0;
			mIsStart = true;
		}

		//スタート処理
		if (mIsStart) {
			mStartPosition.x += 30.0f;
		}

		//タイムアップ処理
		if (mTimeLeft == 0) {

			//タイムアップの文字を透明度イージングさせる
			mTimeUpAlphat = EasingClamp(0.1f, mTimeUpAlphat);
			mTimeUpColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutSine(mTimeUpAlphat));

			//シーンをスコア画面に遷移させるフラグをtrueにする
			if (240 < mTimeFrame) {
				mIsTimeUpFinish = true;
			}
		}
	}


}
void UI::KillCount() {

	//敵の位置に表示しているスコアを消す
	for (int i = 0; i < kEnemyScoreMax; i++) {
		if (mIsEnemyScoreActive[i]) {
			mEnemyScoreLife[i]--;
			mEnemyScoreLife[i] = Clamp(mEnemyScoreLife[i], 0, 180);
			if (mEnemyScoreLife[i] == 0) {
				mIsEnemyScoreActive[i] = false;
			}
		}
	}

	//拡縮アニメーション
	if (mIsKillCountScaleAnimation) {
		mKillCountScale.x -= 0.1f;
		mKillCountScale.y -= 0.1f;
		mKillCountScale.x = Clamp(mKillCountScale.x, 1.0f, 1.8f);
		mKillCountScale.y = Clamp(mKillCountScale.y, 1.0f, 1.8f);
		if (mKillCountScale.x == 1.0f) {
			mIsKillCountScaleAnimation = false;
		}
	}
}
void UI::AddKillCount() {

	//コンボを足す
	mKillCount++;
	//拡縮アニメーション
	mKillCountScale.x = 1.8f;
	mKillCountScale.y = 1.8f;
	mIsKillCountScaleAnimation = true;
}
void UI::SnakeScore(bool isStrikeActive, Vec2 enemyPosition) {

	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kSnakeScore * 2.5) * (1.0f + (float)mKillCount / 10);
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = kSnakeScore * (1.0f + (float)mKillCount / 10);;
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::TsuchinokoScore(bool isStrikeActive, Vec2 enemyPosition) {
	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kTsuchinokoScore * 2.5) * (1.0f + (float)mKillCount / 10);
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = kTsuchinokoScore * (1.0f + (float)mKillCount / 10);;
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::StrikeEndScore(bool isStrikeActive, int mTsuchinokoDefeatStrike, int mSnakeDefeatStrike) {

	int DefeatStrike = mTsuchinokoDefeatStrike + mSnakeDefeatStrike;

	if (DefeatStrike >= 4) {
		//スーパーキル
		mEndScore += 8000;
		for (int i = 0; i < 10; i++) {
			if (!strikeEasingEffect[i].StrikeEasingFlag) {
				strikeEasingEffect[i].StrikeEasingFlag = 3;
				strikeEasingEffect[i].StrikeEasingStart = { -200, 550 };
				strikeEasingEffect[i].StrikeEasingEnd = { 600,550 };
				strikeEasingEffect[i].StrikeEasingPos = { -200,550 };
				strikeEasingEffect[i].StrikeEasingFrame = 0;
				strikeEasingEffect[i].StrikeEasingT = 0;
			}
		}
	} else if (DefeatStrike == 3) {
		//トリプルキル
		mEndScore += 4000;
		for (int i = 0; i < 10; i++) {
			if (!strikeEasingEffect[i].StrikeEasingFlag) {
				strikeEasingEffect[i].StrikeEasingFlag = 2;
				strikeEasingEffect[i].StrikeEasingStart = { -200, 550 };
				strikeEasingEffect[i].StrikeEasingEnd = { 600,550 };
				strikeEasingEffect[i].StrikeEasingPos = { -200,550 };
				strikeEasingEffect[i].StrikeEasingFrame = 0;
				strikeEasingEffect[i].StrikeEasingT = 0;
			}
		}
	} else if (DefeatStrike == 2) {
		//ダブルキル
		mEndScore += 2000;
		for (int i = 0; i < 10; i++) {
			if (!strikeEasingEffect[i].StrikeEasingFlag) {
				strikeEasingEffect[i].StrikeEasingFlag = 1;
				strikeEasingEffect[i].StrikeEasingStart = { -200, 550 };
				strikeEasingEffect[i].StrikeEasingEnd = { 600,550 };
				strikeEasingEffect[i].StrikeEasingPos = { -200,550 };
				strikeEasingEffect[i].StrikeEasingFrame = 0;
				strikeEasingEffect[i].StrikeEasingT = 0;
			}
		}
	}

	mStartScore = mScore;
	mScoreAnimationEasingt = 0.0f;

}
void UI::StrikeEasing() {

	for (int i = 0; i < 10; i++) {
		if (strikeEasingEffect[i].StrikeEasingFlag) {
			if (strikeEasingEffect[i].StrikeEasingT < 1) {
				strikeEasingEffect[i].StrikeEasingT += 0.033;
				if (strikeEasingEffect[i].StrikeEasingT > 1) {
					strikeEasingEffect[i].StrikeEasingT = 1;
				}
				float easedT = easeOutCirc(strikeEasingEffect[i].StrikeEasingT);

				strikeEasingEffect[i].StrikeEasingPos.x = ((1 - easedT) * strikeEasingEffect[i].StrikeEasingStart.x) + (easedT * strikeEasingEffect[i].StrikeEasingEnd.x);

			} else {
				
				strikeEasingEffect[i].StrikeEasingFrame++;
				strikeEasingEffect[i].StrikeEasingPos.x++;
				if (80 <= strikeEasingEffect[i].StrikeEasingFrame) {
					strikeEasingEffect[i].StrikeEasingFrame = 0;
					strikeEasingEffect[i].StrikeEasingFlag = 0;
					strikeEasingEffect[i].StrikeEasingT = 0;
				}

			}
		}
	}
}
void UI::ScoreAnimation() {

	mScoreAnimationEasingt = EasingClamp(0.1f, mScoreAnimationEasingt);
	mScore = EasingMove(mStartScore, mEndScore, easeInSine(mScoreAnimationEasingt));

}
void UI::Warning() {

	if (mIsWarning) {
		if (!mIsWarningRed) {
			mWarningAlphat = EasingClamp(0.5f, mWarningAlphat);
			mWarningColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutCirc(mWarningAlphat));
			if (mWarningAlphat == 1.0f) {
				mWarningAlphat = 0.0f;
				mIsWarningRed = true;
			}
		}
		else {
			mWarningAlphat = EasingClamp(0.1f, mWarningAlphat);
			mWarningColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeInSine(mWarningAlphat));
			if (mWarningAlphat == 1.0f) {
				mIsWarning = false;
			}
		}
	}
	else {
		mIsWarningRed = false;
		mWarningAlphat = 0.0f;
		mWarningColor = 0xFFFFFF00;
	}
}
void UI::DrawBackTimeLimit(Screen& screen) {

	//カウントダウンの描画
	//開始
	if (!mIsStart && mIsReady) {
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mCountDownTime % 10), 288, 288, mTimeLimitNumber, 0xFFFFFF70);
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mCountDownTime % 10), 288, 288, mTimeLimitNumber, mTimeLastColor, mTimeLastScale);
	} else if (mStartPosition.x < (Screen::kWindowWidth + Screen::kWindowWidth / 2.0)) {
		screen.DrawUI(mStartPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mStart, 0xFFFFFFB0);
	}
	if (0 < mTimeLeft && mTimeLeft < 10) {
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, 0xFFFFFF70);
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, mTimeLastColor, mTimeLastScale);
	}
}
void UI::Draw(Screen& screen, bool mIsReady) {

	//頭か尾に当たってしまったとき
	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mWarningRed, mWarningColor);

	//制限時間
	if (10 <= mTimeLeft && mIsStart) {
		screen.DrawUI(mTimePosition[1], mTimeUISize, 288 * (mTimeLeft / 10), 288, 288, mTimeLimitNumber, WHITE, mTimeScale);
		screen.DrawUI(mTimePosition[0], mTimeUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, WHITE, mTimeScale);
	}
	//タイムアップ
	if (mTimeLeft == 0) {
		screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mTimeUp, mTimeUpColor);
	}

	//キルカウント
	if (mIsReady) {

		//スコアの桁より大きい数字は色を薄くする
		mIsDarkKillCount = false;
		for (int i = 0; i < 3; i++) {
			mKillCountColor[i] = 0xFFFFFF50;
		}

		mKillCount = Clamp(mKillCount, 0, 1000);
		float Result[3];
		float tmpScore = mKillCount;
		for (int i = 2; i > -1; i--) {
			Result[i] = tmpScore / powf(10, i);
			tmpScore = (int)tmpScore % (int)powf(10, i);
			if ((int)Result[i] != 0) {
				mIsDarkKillCount = true;
			}
			if (mIsDarkKillCount) {
				mKillCountColor[i] = WHITE;
			} else {
				mKillCountColor[i] = 0xFFFFFF50;
			}
			screen.DrawUI(mKillCountPosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mKillCountColor[i], mKillCountScale);
		}
		screen.DrawUI({ mScorePosition[0].x - 50.0f, mKillCountPosition[0].y + 50.0f }, 200, 50, 0, 400, 100, mKillCountLetter, WHITE);
	}

	//敵スコア
	for (int j = 0; j < kEnemyScoreMax; j++) {
		if (mIsEnemyScoreActive[j]) {
			float Result[8];
			float tmpScore = mEnemyScore[j];
			for (int i = 7; i > -1; i--) {
				Result[i] = tmpScore / powf(10, i);
				tmpScore = (int)tmpScore % (int)powf(10, i);
				if (powf(10, i) <= mEnemyScore[j]) {
					screen.DrawUI({ mEnemyScorePosition[j].x - (i * 24) / screen.GetZoom(), mEnemyScorePosition[j].y }, 24 / screen.GetZoom(), 32 * (int)Result[i], 32, 32, mNumber, WHITE, { 1.0f,1.0f }, true);
				}
			}
		}

	}
	//スコア
	if (mIsReady) {

		//スコアの桁より大きい数字は色を薄くする
		mIsDarkScore = false;
		for (int i = 0; i < 8; i++) {
			mScoreColor[i] = 0xFFFFFF50;
		}

		mScore = Clamp(mScore, 0, 100000000);
		float Result[8];
		float tmpScore = mScore;
		for (int i = 7; i > -1; i--) {
			Result[i] = tmpScore / powf(10, i);
			tmpScore = (int)tmpScore % (int)powf(10, i);
			if ((int)Result[i] != 0) {
				mIsDarkScore = true;
			}
			if (mIsDarkScore) {
				mScoreColor[i] = WHITE;
			} else {
				mScoreColor[i] = 0xFFFFFF50;
			}
			screen.DrawUI(mScorePosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mScoreColor[i], mKillCountScale);
		}
		screen.DrawUI({ mScorePosition[0].x, mScorePosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mScoreLetter, WHITE);
	}

	//ミニマップ
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0x00000080, kFillModeSolid, false);
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0xFFFF00FF, kFillModeWireFrame, false);
	screen.DrawUI(mRadarPosition, 150, 75, 0, 200, 100, mRadar, WHITE);

	//操作説明
	screen.DrawUI(mStickPosition, 50, 0, 100, 100, mStick, mLStickBackColor);
	screen.DrawUI({ mStickPosition.x + LeftStickVelocity(15.0f).x, mStickPosition.y - LeftStickVelocity(15.0f).y}, 50, 0, 160, 160, mLStick, mLStickColor);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 55 }, 50, 0, 160, 160, mX, mXColor, mXScale);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 110 }, 50, 0, 160, 160, mA, mAColor, mAScale);

	for (int i = 0; i < 10; i++) {
		switch(strikeEasingEffect[i].StrikeEasingFlag) {
			case 1:
				Novice::DrawSprite(strikeEasingEffect[i].StrikeEasingPos.x, strikeEasingEffect[i].StrikeEasingPos.y, doublekill, 1, 1, 0, WHITE);
				break;
			case 2:
				Novice::DrawSprite(strikeEasingEffect[i].StrikeEasingPos.x, strikeEasingEffect[i].StrikeEasingPos.y, triplekill, 1, 1, 0, WHITE);
				break;
			case 3:
				Novice::DrawSprite(strikeEasingEffect[i].StrikeEasingPos.x, strikeEasingEffect[i].StrikeEasingPos.y, superkill, 1, 1, 0, WHITE);
				break;
		}
	}

	if (!mIsReady) {
		switch (mTutorialScene) {
			case 0:
				Novice::DrawSprite(880, 64, Tutorial2, 1, 1, 0, WHITE);
				Novice::DrawSprite(920, 296, TutorialSkip, 1, 1, 0, WHITE);
				screen.DrawBox({ 904,252 }, 32.8 * 10 + 2, 21, 0.0f, BLACK, kFillModeWireFrame, false);
				screen.DrawBox({ 904,252 }, 32.8 * mTutorialControl, 20, 0.0f, BLUE, kFillModeSolid, false);
				break;
			case 1:
				Novice::DrawSprite(880, 64, Tutorial4, 1, 1, 0, WHITE);
				Novice::DrawSprite(920, 296, TutorialSkip, 1, 1, 0, WHITE);
				screen.DrawBox({ 904,252 }, 109 * 3 + 2, 21, 0.0f, BLACK, kFillModeWireFrame, false);
				screen.DrawBox({ 904,252 }, 109 * mTutorialControl, 20, 0.0f, BLUE, kFillModeSolid, false);
				break;
			case 2:
				Novice::DrawSprite(880, 64, Tutorial3, 1, 1, 0, WHITE);
				Novice::DrawSprite(920, 296, TutorialSkip, 1, 1, 0, WHITE);
				screen.DrawBox({ 904,252 }, 109 * 3 + 2, 21, 0.0f, BLACK, kFillModeWireFrame, false);
				screen.DrawBox({ 904,252 }, 109 * mTutorialControl, 20, 0.0f, BLUE, kFillModeSolid, false);
				break;
			case 3:
				Novice::DrawSprite(880, 64, Tutorial5, 1, 1, 0, WHITE);
				Novice::DrawSprite(920, 296, TutorialSkip, 1, 1, 0, WHITE);
				screen.DrawBox({ 904,252 }, 109 * 3 + 2, 21, 0.0f, BLACK, kFillModeWireFrame, false);
				screen.DrawBox({ 904,252 }, 109 * mTutorialControl, 20, 0.0f, BLUE, kFillModeSolid, false);
				break;
			case 4:
				Novice::DrawSprite(880, 64, Tutorial1, 1, 1, 0, WHITE);
				screen.DrawUI({ Screen::kWindowWidth / 2.0, 100 }, 400, 100, 0.0f, 400, 100, areyouready, WHITE);
				break;
		}
		
	}
}
void UI::LoadTexture() {

	mNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
	mTimeLimitNumber = Novice::LoadTexture("./Resources/UI/Time/timelimit.png");
	mStart = Novice::LoadTexture("./Resources/UI/Time/start.png");
	mTimeUp = Novice::LoadTexture("./Resources/UI/Time/timeup.png");
	mKillCountLetter = Novice::LoadTexture("./Resources/UI/Killcount/killcount.png");
	mScoreLetter = Novice::LoadTexture("./Resources/UI/Score/score.png");
	mWarningRed = Novice::LoadTexture("./Resources/Player/warningred.png");
	mRadar = Novice::LoadTexture("./Resources/UI/Minimap/radar.png");
	mStick = Novice::LoadTexture("./Resources/UI/Explanation/stick.png");
	mLStick = Novice::LoadTexture("./Resources/UI/Explanation/lstick.png");
	mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
	mA = Novice::LoadTexture("./Resources/UI/Explanation/a.png");
	Tutorial1 = Novice::LoadTexture("./Resources/UI/Explanation/1.png");
	Tutorial2 = Novice::LoadTexture("./Resources/UI/Explanation/2.png");
	Tutorial3 = Novice::LoadTexture("./Resources/UI/Explanation/3.png");
	Tutorial4 = Novice::LoadTexture("./Resources/UI/Explanation/4.png");
	Tutorial5 = Novice::LoadTexture("./Resources/UI/Explanation/5.png");
	TutorialSkip = Novice::LoadTexture("./Resources/UI/Explanation/tutorialskip.png");
	areyouready = Novice::LoadTexture("./Resources/UI/Explanation/areyouready.png");
	doublekill = Novice::LoadTexture("./Resources/UI/Explanation/doublekill.png");
	triplekill = Novice::LoadTexture("./Resources/UI/Explanation/triplekill.png");
	superkill = Novice::LoadTexture("./Resources/UI/Explanation/superkill.png");
	countdown = Novice::LoadAudio("./Sounds/countdown.mp3");
	finish = Novice::LoadAudio("./Sounds/finish.mp3");
}


//マップ
void Map::Init() {

	for (int i = 0; i < kAxisLength; i++)
	{
		mXaxisStartPosition[i].x = -5000;
		mXaxisStartPosition[i].y = (i - kAxisLength / 2) * kLineInterval;
		mXaxisEndPosition[i].x = -mXaxisStartPosition[i].x;
		mXaxisEndPosition[i].y =  mXaxisStartPosition[i].y;
		mYaxisStartPosition[i].x = (i - kAxisLength / 2) * kLineInterval;
		mYaxisStartPosition[i].y = -5000;
		mYaxisEndPosition[i].x =  mYaxisStartPosition[i].x;
		mYaxisEndPosition[i].y = -mYaxisStartPosition[i].y;
	}

	mIsBackGroundColorChange = false;
	mBackGroundColor = 0xDADADAFF;
	mBackLineColor = 0x202020FF;

	for (int i = 0; i < kBackBoxMax; i++) {
		mIsBackBoxActive[i] = false;
	}
	mBackBoxFrame = 0;
}
void Map::Update(bool isFever, bool isOldFever, unsigned int feverColor) {

	if (isFever && !isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0xDADADAFF;
		mBackGroundEndColor = 0x202020FF;
		mIsBackGroundColorChange = true;
	}
	else if (!isFever && isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x202020FF;
		mBackGroundEndColor = 0xDADADAFF;
		mIsBackGroundColorChange = true;
	}

	if (mIsBackGroundColorChange) {
		mBackGroundEasingt = EasingClamp(0.01f, mBackGroundEasingt);
		mBackGroundColor = ColorEasingMove(mBackGroundStartColor, mBackGroundEndColor, easeOutSine(mBackGroundEasingt));

		if (mBackGroundEasingt == 1.0f) {
			mIsBackGroundColorChange = false;
		}
	}

	//背景矩形フレームの加算
	mBackBoxFrame++;

	for (int i = 0; i < kBackBoxMax; i++) {

		//背景矩形生成
		if (!mIsBackBoxActive[i] && (mBackBoxFrame % 3 == 0 || mBackBoxFrame % 3 == 1)) {
			mBackBoxPosition[i].x = RAND(kMapLeft + 200, kMapRight - 200);
			mBackBoxPosition[i].y = RAND(kMapBottom + 200, kMapTop - 200);
			mBackBoxAngle[i] = RAND(Degree(0), Degree(360));
			mBackBoxEndSize[i] = RAND(8, 12) * 20;
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
			} else {
				mBackBoxEasingt[i] = EasingClamp(0.1f, mBackBoxEasingt[i]);
				mBackBoxSize[i] = EasingMove(mBackBoxEndSize[i], 0.0f, easeInSine(mBackBoxEasingt[i]));
				if (mBackBoxEasingt[i] == 1.0f) {
					mIsBackBoxActive[i] = false;
				}
			}
		}
	}

	//背景矩形の色を変える
	if (isFever) {
		mBackLineColor = feverColor;
		for (int i = 0; i < kBackBoxMax; i++) {
			mBackBoxColor = feverColor;
		}
	} else {
		mBackLineColor = BLACK;
		for (int i = 0; i < kBackBoxMax; i++) {
			mBackBoxColor = BLACK;
		}
	}

}
bool Map::XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player) {

	float left  = player.mPosition.x - (Screen::kWindowWidth / 2);
	float right = player.mPosition.x + (Screen::kWindowWidth / 2);
	if (left < startposition.x && endposition.x < right) {
		return true;
	}
	return false;
}
bool Map::YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player) {

	float top    = player.mPosition.y + (Screen::kWindowHeight / 2);
	float bottom = player.mPosition.y - (Screen::kWindowHeight / 2);
	if (bottom < startposition.y && endposition.y < top) {
		return true;
	}
	return false;
}
void Map::Draw(Screen& screen) {

	//背景描画
	Novice::DrawBox(0, 0, Screen::kWindowWidth, Screen::kWindowHeight, 0.0, mBackGroundColor, kFillModeSolid);

	for (int i = 0; i < kBackBoxMax; i++) {
		screen.DrawRectAngle(mBackBoxPosition[i], mBackBoxSize[i], mBackBoxSize[i], mBackBoxAngle[i], mBackBoxColor, kFillModeWireFrame, true);
	}

	screen.DrawRectAngle({ kMapLeft, 0 }, 10, kMapTop * 2, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ kMapRight, 0 }, 10, kMapTop * 2, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ 0, kMapTop }, kMapLeft * 2, 10, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ 0, kMapBottom }, kMapLeft * 2, 10, 0.0f, mBackLineColor);

}