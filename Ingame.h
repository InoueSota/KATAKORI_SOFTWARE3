#pragma once
#include "Screen.h"
#include "Vec2.h"



class UI
{
public:
	UI() {
		Init();
	}

	void Init();
	void Update();
	void Draw(Screen& screen, bool mIsReady);

	//始めるかフラグ
	const int kToStart = 3;
	bool mIsReady;
	bool mIsOldReady;
	bool mIsStart;
	bool mIsOldStart;
	int mCountDownTime;
	Vec2 mStartPosition;


	//画面の中心
	Vec2 mCenterPosition;


	//制限時間
	bool mIsExtendTime;
	const int kTimeLimit = 60;
	Vec2 mTimePosition[2];
	Vec2 mTimeScale;
	Vec2 mTimeLastScale;
	float mExtendTimeEasingt;
	int mTimeFrame;
	int mTimeElapsed;
	int mTimeLeft;
	int mTimeUISize;
	//カウントダウン
	int mTimeLastUISize;
	unsigned int mTimeLastColor;
	float mTimeEasingt;
	//タイムアップ
	bool mIsTimeUpFinish;
	float mTimeUpAlphat;
	unsigned int mTimeUpColor;
	void ExtendTime(bool isFever, bool isOldFever);
	void TimeLimit();
	void DrawBackTimeLimit(Screen& screen);


	//敵を倒した数（スコアの加算に影響する）
	bool mIsKillCountScaleAnimation;
	bool mIsDarkKillCount;
	Vec2 mKillCountPosition[3];
	Vec2 mKillCountScale;
	unsigned int mKillCountColor[3];
	int mKillCount;
	void KillCount();
	void AddKillCount();


	//スコア
	const int kSnakeScore = 1000;
	const int kTsuchinokoScore = 1000;
	bool mIsDarkScore;
	Vec2 mScorePosition[7];
	float mScore;
	unsigned int mScoreColor[7];
	static constexpr int kEnemyScoreMax = 20;
	bool mIsEnemyScoreActive[kEnemyScoreMax];
	Vec2 mEnemyScorePosition[kEnemyScoreMax];
	int mEnemyScoreLife[kEnemyScoreMax];
	float mEnemyScore[kEnemyScoreMax];
	void SnakeScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition);
	void MissSnakeScore();
	void TsuchinokoScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition);
	void MissTsuchinokoScore();


	//頭か尾に当たってしまったとき
	bool mIsWarning;
	bool mIsWarningRed;
	float mWarningAlphat;
	unsigned int mWarningColor;
	void Warning();


	//ミニマップ
	Vec2 mMiniMapPosition{ Screen::kWindowWidth - Screen::kMiniMapSize - 10.0f, Screen::kWindowHeight - Screen::kMiniMapSize - 10.0f };
	Vec2 mRadarPosition{ mMiniMapPosition.x ,mMiniMapPosition.y - Screen::kMiniMapSize - 10.0f };


	//操作説明
	Vec2 mStickPosition{ 60, 685 };
	unsigned int mLStickColor;
	unsigned int mLStickBackColor;
	unsigned int mXColor;
	bool mXScaleActive;
	Vec2 mXScale;
	float mXEasingt;
	unsigned int mAColor;
	bool mAScaleActive;
	Vec2 mAScale;
	float mAEasingt;


	//画像
	int mNumber;
	int mTimeLimitNumber;
	int mStart;
	int mTimeUp;
	int mKillCountLetter;
	int mScoreLetter;
	int mWarningRed;
	int mRadar;
	int mStick;
	int mLStick;
	int mX;
	int mA;
	void LoadTexture();

};



class Player;
class Map
{
public:

	Map() {
		Init();
	}

	void Init();
	void Update(bool isFever, bool isOldFever);
	void Draw(Screen& screen);

	//マップ端定数
	static constexpr int kMapRadius = 5000;
	static constexpr int kMapTop = kMapRadius;
	static constexpr int kMapBottom = -kMapRadius;
	static constexpr int kMapLeft = -kMapRadius;
	static constexpr int kMapRight = kMapRadius;

	//通常時とフィーバー時で背景色を変える
	bool mIsBackGroundColorChange;
	unsigned int mBackGroundStartColor;
	unsigned int mBackGroundEndColor;
	unsigned int mBackGroundColor;
	float mBackGroundEasingt;

	//デバッグ用
	static constexpr int kAxisLength = 66;
	static constexpr int kLineInterval = 150;
	unsigned int mBackLineColor;
	Vec2 mXaxisStartPosition[kAxisLength];
	Vec2 mXaxisEndPosition[kAxisLength];
	Vec2 mYaxisStartPosition[kAxisLength];
	Vec2 mYaxisEndPosition[kAxisLength];
	bool XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	bool YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);

};

