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
	void Draw(Screen& screen);


	//制限時間
	const int kTimeLimit = 60;
	Vec2 mTimePosition[2];
	int mTimeFrame;
	int mTimeElapsed;
	int mTimeLeft;
	void TimeLimit();


	//コンボ
	const int kComboCoolTime = 180;
	bool mIsComboScaleAnime;
	Vec2 mComboPosition[2];
	Vec2 mComboScale;
	int mCombo;
	int mComboCoolTime;
	void Combo();
	void AddCombo();


	//スコア
	const int kSnakeScore = 1000;
	const int kTsuchinokoScore = 1500;
	Vec2 mScorePosition[6];
	int mScore;
	void SnakeScore(bool playermIsStrikeActive);
	void MissSnakeScore(bool playermIsStrikeActive);
	void TsuchinokoScore(bool playermIsStrikeActive);
	void MissTsuchinokoScore(bool playermIsStrikeActive);


	//画像
	bool mIsLoadTexture = false;
	int mTimeUISize;
	int mTimeNumber;

};



class Player;
class Map
{
public:

	Map() {
		Init();
	}

	void Init();
	void Update(Player& player);
	void Draw(Screen& screen);

	//マップ端定数
	static constexpr int kMapRadius = 5000;
	static constexpr int kMapTop = kMapRadius;
	static constexpr int kMapBottom = -kMapRadius;
	static constexpr int kMapLeft = -kMapRadius;
	static constexpr int kMapRight = kMapRadius;

	//デバッグ用
	static constexpr int kAxisLength = 66;
	static constexpr int kLineInterval = 150;
	Vec2 mXaxisStartPosition[kAxisLength];
	Vec2 mXaxisEndPosition[kAxisLength];
	Vec2 mYaxisStartPosition[kAxisLength];
	Vec2 mYaxisEndPosition[kAxisLength];
	bool XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	bool YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);

};

