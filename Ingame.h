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


	//��������
	const int kTimeLimit = 60;
	Vec2 mTimePosition[2];
	int mTimeFrame;
	int mTimeElapsed;
	int mTimeLeft;
	void TimeLimit();


	//�R���{
	const int kComboCoolTime = 180;
	bool mIsComboScaleAnime;
	Vec2 mComboPosition[2];
	Vec2 mComboScale;
	int mCombo;
	int mComboCoolTime;
	void Combo();
	void AddCombo();


	//�X�R�A
	const int kSnakeScore = 1000;
	const int kTsuchinokoScore = 1500;
	Vec2 mScorePosition[6];
	float mScore;
	void SnakeScore(bool playermIsStrikeActive);
	void MissSnakeScore(bool playermIsStrikeActive);
	void TsuchinokoScore(bool playermIsStrikeActive);
	void MissTsuchinokoScore(bool playermIsStrikeActive);


	//�摜
	bool mIsLoadTexture = false;
	int mTimeUISize;
	int mTimeNumber;
	int mComboLetter;
	int mScoreLetter;

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

	//�}�b�v�[�萔
	static constexpr int kMapRadius = 5000;
	static constexpr int kMapTop = kMapRadius;
	static constexpr int kMapBottom = -kMapRadius;
	static constexpr int kMapLeft = -kMapRadius;
	static constexpr int kMapRight = kMapRadius;

	//�f�o�b�O�p
	static constexpr int kAxisLength = 66;
	static constexpr int kLineInterval = 150;
	Vec2 mXaxisStartPosition[kAxisLength];
	Vec2 mXaxisEndPosition[kAxisLength];
	Vec2 mYaxisStartPosition[kAxisLength];
	Vec2 mYaxisEndPosition[kAxisLength];
	bool XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	bool YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);

};

