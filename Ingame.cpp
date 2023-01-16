#include "Ingame.h"
#include "Player.h"
#include "Function.h"



//ゲーム中のＵＩ
void UI::Init() {

	//制限時間
	mTimeUISize = 48;
	mTimePosition[0].x = Screen::kWindowWidth / 2.0 + mTimeUISize / 2.0 + 5.0f;
	mTimePosition[0].y = mTimeUISize / 2.0 + 5.0f;
	mTimePosition[1].x = Screen::kWindowWidth / 2.0 - mTimeUISize / 2.0 - 5.0f;
	mTimePosition[1].y = mTimePosition[0].y;
	mTimeLeft = kTimeLimit;
	mTimeElapsed = 0;
	mTimeFrame = 0;
	
	//コンボ
	mIsComboScaleAnime = false;
	mComboPosition[0].x = Screen::kWindowWidth - mTimeUISize;
	mComboPosition[0].y = 300.0f;
	mComboPosition[1].x = Screen::kWindowWidth - mTimeUISize * 2;
	mComboPosition[1].y = mComboPosition[0].y;
	mComboScale.x = 1.0f;
	mComboScale.y = 1.0f;
	mCombo = 0;
	mComboCoolTime = 0;

	//スコア
	mScore = 0;
	for (int i = 0; i < 6; i++) {
		mScorePosition[i].x = Screen::kWindowWidth - mTimeUISize * (i + 1);
		mScorePosition[i].y = 100.0f;
	}

}
void UI::Update() {

	//制限時間
	TimeLimit();

	//コンボ
	Combo();
}
void UI::TimeLimit() {

	//フレームが60になったら一秒経過
	if (mTimeFrame == 60)
	{
		mTimeElapsed++;
		mTimeFrame = 0;
	}

	//時間のフレームを加算する
	mTimeFrame++;

	//残り時間 = 制限時間 - 経過時間
	mTimeLeft = kTimeLimit - mTimeElapsed;

	//残り時間の限界値の設定
	mTimeLeft = Clamp(mTimeLeft, 0, kTimeLimit);
}
void UI::Combo() {

	//クールタイムを加算する
	mComboCoolTime++;

	//コンボの初期化
	if (mComboCoolTime > kComboCoolTime) {
		mCombo = 0;
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
	mCombo++;
	mComboCoolTime = 0;
	//拡縮アニメーション
	mComboScale.x = 1.8f;
	mComboScale.y = 1.8f;
	mIsComboScaleAnime = true;
}
void UI::SnakeScore(bool playermIsStrikeActive) {
	if (playermIsStrikeActive) {
		mScore += (kSnakeScore * 2) * (1.0f + (float)mCombo / 10);
	}
	else {
		mScore += kSnakeScore * (1.0f + (float)mCombo / 10);
	}
}
void UI::MissSnakeScore(bool playermIsStrikeActive) {
	if (playermIsStrikeActive) {
		mScore -= kSnakeScore;
	}
	else {
		mScore -= kSnakeScore / 2.0;
	}
}
void UI::TsuchinokoScore(bool playermIsStrikeActive) {
	if (playermIsStrikeActive) {
		mScore += kTsuchinokoScore * 2 * (1.0f + (float)mCombo / 10);
	}
	else {
		mScore += kTsuchinokoScore * (1.0f + (float)mCombo / 10);
	}
}
void UI::MissTsuchinokoScore(bool playermIsStrikeActive) {
	if (playermIsStrikeActive) {
		mScore -= kTsuchinokoScore;
	}
	else {
		mScore -= kTsuchinokoScore / 2.0;
	}
}
void UI::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		mTimeNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
		mComboLetter = Novice::LoadTexture("./Resources/UI/Combo/combo.png");
		mScoreLetter = Novice::LoadTexture("./Resources/UI/Score/score.png");
		mIsLoadTexture = true;
	}

	//制限時間
	if (10 <= mTimeLeft) {
		screen.DrawUI(mTimePosition[1], mTimeUISize, 32 * (mTimeLeft / 10), 32, 32, mTimeNumber, WHITE);
	}
	if (0 <= mTimeLeft) {
		screen.DrawUI(mTimePosition[0], mTimeUISize, 32 * (mTimeLeft % 10), 32, 32, mTimeNumber, WHITE);
	}

	//コンボ
	if (10 <= mCombo) {
		screen.DrawUI(mComboPosition[1], mTimeUISize, 32 * (mCombo / 10), 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	if (1 <= mCombo) {
		screen.DrawUI(mComboPosition[0], mTimeUISize, 32 * (mCombo % 10), 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	screen.DrawUI({ mScorePosition[0].x, mComboPosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mComboLetter, WHITE);

	//スコア
	mScore = Clamp(mScore, 0, 1000000);
	float Result[6];
	float tmpScore = mScore;
	for (int i = 5; i > -1; i--) {
		Result[i] = tmpScore / powf(10, i);
		tmpScore = (int)tmpScore % (int)powf(10, i);
		screen.DrawUI(mScorePosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	screen.DrawUI({ mScorePosition[0].x, mScorePosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mScoreLetter, WHITE);
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

}
void Map::Update(Player& player) {

	for (int i = 0; i < kAxisLength; i++)
	{
		if (XaxisNearPlayer(mYaxisStartPosition[i], mYaxisEndPosition[i], player))
		{
			mYaxisStartPosition[i].y = player.mPosition.y + (Screen::kWindowHeight / 2);
			mYaxisEndPosition[i].y   = player.mPosition.y - (Screen::kWindowHeight / 2);
		}
		if (YaxisNearPlayer(mXaxisStartPosition[i], mXaxisEndPosition[i], player))
		{
			mXaxisStartPosition[i].x = player.mPosition.x + (Screen::kWindowWidth / 2);
			mXaxisEndPosition[i].x   = player.mPosition.x - (Screen::kWindowWidth / 2);
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

	Novice::DrawBox(0, 0, Screen::kWindowWidth, Screen::kWindowHeight, 0.0, 0x589E35FF, kFillModeSolid);

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