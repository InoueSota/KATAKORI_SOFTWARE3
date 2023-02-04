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
	mIsComboScaleAnime = false;
	mComboPosition[0].x = Screen::kWindowWidth - mTimeUISize;
	mComboPosition[0].y = 300.0f;
	mComboPosition[1].x = Screen::kWindowWidth - mTimeUISize * 2;
	mComboPosition[1].y = mComboPosition[0].y;
	mComboPosition[2].x = Screen::kWindowWidth - mTimeUISize * 3;
	mComboPosition[2].y = mComboPosition[0].y;
	mComboScale.x = 1.0f;
	mComboScale.y = 1.0f;
	mDefeatedEnemyCount = 0;

	//スコア
	mScore = 0;
	for (int i = 0; i < 7; i++) {
		mScorePosition[i].x = Screen::kWindowWidth - mTimeUISize * (i + 1);
		mScorePosition[i].y = 100.0f;
	}
	for (int i = 0; i < kEnemyScoreMax; i++) {
		mIsEnemyScoreActive[i] = false;
	}

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
void UI::Update() {

	//前回準備完了フラグの保存
	mIsOldReady = mIsReady;

	//操作確認フェーズを終了するか
	if (!mIsReady) {
		if (Controller::IsTriggerButton(0, Controller::bY)) {
			mIsReady = true;
		}
	}

	//コンボ
	Combo();

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
void UI::Combo() {

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
	if (mIsComboScaleAnime) {
		mComboScale.x -= 0.1f;
		mComboScale.y -= 0.1f;
		mComboScale.x = Clamp(mComboScale.x, 1.0f, 1.8f);
		mComboScale.y = Clamp(mComboScale.y, 1.0f, 1.8f);
		if (mComboScale.x == 1.0f) {
			mIsComboScaleAnime = false;
		}
	}
}
void UI::AddCombo() {

	//コンボを足す
	mDefeatedEnemyCount++;
	//拡縮アニメーション
	mComboScale.x = 1.8f;
	mComboScale.y = 1.8f;
	mIsComboScaleAnime = true;
}
void UI::SnakeScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition) {

	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kSnakeScore * 2.5) * (1.0f + (float)mDefeatedEnemyCount / 10);
				mScore += mEnemyScore[i];
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
				mEnemyScore[i] = kSnakeScore * (1.0f + (float)mDefeatedEnemyCount / 10);;
				mScore += mEnemyScore[i];
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::MissSnakeScore() {
		mScore -= 100;
}
void UI::TsuchinokoScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition) {
	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kTsuchinokoScore * 2.5) * (1.0f + (float)mDefeatedEnemyCount / 10);
				mScore += mEnemyScore[i];
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
				mEnemyScore[i] = kTsuchinokoScore * (1.0f + (float)mDefeatedEnemyCount / 10);;
				mScore += mEnemyScore[i];
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::MissTsuchinokoScore() {
	mScore -= 100;
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

	//コンボ
	if (mIsReady) {

		//スコアの桁より大きい数字は色を薄くする
		mIsDarkDefeatedEnemyCount = false;
		for (int i = 0; i < 3; i++) {
			mDefeatedEnemyCountColor[i] = 0xFFFFFF50;
		}

		mDefeatedEnemyCount = Clamp(mDefeatedEnemyCount, 0, 1000);
		float Result[3];
		float tmpScore = mDefeatedEnemyCount;
		for (int i = 2; i > -1; i--) {
			Result[i] = tmpScore / powf(10, i);
			tmpScore = (int)tmpScore % (int)powf(10, i);
			if ((int)Result[i] != 0) {
				mIsDarkDefeatedEnemyCount = true;
			}
			if (mIsDarkDefeatedEnemyCount) {
				mDefeatedEnemyCountColor[i] = WHITE;
			} else {
				mDefeatedEnemyCountColor[i] = 0xFFFFFF50;
			}
			screen.DrawUI(mComboPosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mDefeatedEnemyCountColor[i], mComboScale);
		}
		screen.DrawUI({ mScorePosition[0].x, mComboPosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mComboLetter, WHITE);
	}

	//敵スコア
	for (int j = 0; j < kEnemyScoreMax; j++) {
		if (mIsEnemyScoreActive[j]) {
			float Result[7];
			float tmpScore = mEnemyScore[j];
			for (int i = 6; i > -1; i--) {
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
		for (int i = 0; i < 7; i++) {
			mScoreColor[i] = 0xFFFFFF50;
		}

		mScore = Clamp(mScore, 0, 10000000);
		float Result[7];
		float tmpScore = mScore;
		for (int i = 6; i > -1; i--) {
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
			screen.DrawUI(mScorePosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mScoreColor[i], mComboScale);
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
}
void UI::LoadTexture() {

	mNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
	mTimeLimitNumber = Novice::LoadTexture("./Resources/UI/Time/timelimit.png");
	mStart = Novice::LoadTexture("./Resources/UI/Time/start.png");
	mTimeUp = Novice::LoadTexture("./Resources/UI/Time/timeup.png");
	mComboLetter = Novice::LoadTexture("./Resources/UI/Combo/combo.png");
	mScoreLetter = Novice::LoadTexture("./Resources/UI/Score/score.png");
	mWarningRed = Novice::LoadTexture("./Resources/Player/warningred.png");
	mRadar = Novice::LoadTexture("./Resources/UI/Minimap/radar.png");
	mStick = Novice::LoadTexture("./Resources/UI/Explanation/stick.png");
	mLStick = Novice::LoadTexture("./Resources/UI/Explanation/lstick.png");
	mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
	mA = Novice::LoadTexture("./Resources/UI/Explanation/a.png");
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
	mBackGroundColor = 0x589E35FF;

}
void Map::Update(bool isFever, bool isOldFever) {

	if (isFever && !isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x589E35FF;
		mBackGroundEndColor = 0x202020FF;
		mIsBackGroundColorChange = true;
	}
	else if (!isFever && isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x202020FF;
		mBackGroundEndColor = 0x589E35FF;
		mIsBackGroundColorChange = true;
	}

	if (mIsBackGroundColorChange) {
		mBackGroundEasingt = EasingClamp(0.01f, mBackGroundEasingt);
		mBackGroundColor = ColorEasingMove(mBackGroundStartColor, mBackGroundEndColor, easeOutSine(mBackGroundEasingt));

		if (mBackGroundEasingt == 1.0f) {
			mIsBackGroundColorChange = false;
		}
	}

	//for (int i = 0; i < kAxisLength; i++)
	//{
	//	if (XaxisNearPlayer(mYaxisStartPosition[i], mYaxisEndPosition[i], player))
	//	{
	//		mYaxisStartPosition[i].y = player.mPosition.y + (Screen::kWindowHeight / 2);
	//		mYaxisEndPosition[i].y   = player.mPosition.y - (Screen::kWindowHeight / 2);
	//	}
	//	if (YaxisNearPlayer(mXaxisStartPosition[i], mXaxisEndPosition[i], player))
	//	{
	//		mXaxisStartPosition[i].x = player.mPosition.x + (Screen::kWindowWidth / 2);
	//		mXaxisEndPosition[i].x   = player.mPosition.x - (Screen::kWindowWidth / 2);
	//	}
	//}

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

	for (int i = 0; i < kAxisLength; i++)
	{
		screen.DrawLine(mXaxisStartPosition[i], mXaxisEndPosition[i]);
		screen.DrawLine(mYaxisStartPosition[i], mYaxisEndPosition[i]);
	}

	screen.DrawLine({ kMapLeft, kMapTop }, { kMapRight, kMapTop }, BLACK);
	screen.DrawLine({ kMapLeft, kMapTop }, { kMapLeft, kMapBottom }, BLACK);
	screen.DrawLine({ kMapLeft, kMapBottom }, { kMapRight, kMapBottom }, BLACK);
	screen.DrawLine({ kMapRight, kMapTop }, { kMapRight, kMapBottom }, BLACK);
}