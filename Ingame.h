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

	//�n�߂邩�t���O
	const int kToStart = 3;
	bool mIsReady;
	bool mIsOldReady;
	bool mIsStart;
	bool mIsOldStart;
	int mCountDownTime;
	Vec2 mStartPosition;


	//��ʂ̒��S
	Vec2 mCenterPosition;


	//��������
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
	//�J�E���g�_�E��
	int mTimeLastUISize;
	unsigned int mTimeLastColor;
	float mTimeEasingt;
	//�^�C���A�b�v
	bool mIsTimeUpFinish;
	float mTimeUpAlphat;
	unsigned int mTimeUpColor;
	void ExtendTime(bool isFever, bool isOldFever);
	void TimeLimit();
	void DrawBackTimeLimit(Screen& screen);


	//�R���{
	bool mIsComboScaleAnime;
	Vec2 mComboPosition[2];
	Vec2 mComboScale;
	int mCombo;
	void Combo();
	void AddCombo();


	//�X�R�A
	const int kSnakeScore = 1200;
	const int kTsuchinokoScore = 1000;
	Vec2 mScorePosition[7];
	float mScore;
	static constexpr int kEnemyScoreMax = 20;
	bool mIsEnemyScoreActive[kEnemyScoreMax];
	Vec2 mEnemyScorePosition[kEnemyScoreMax];
	int mEnemyScoreLife[kEnemyScoreMax];
	float mEnemyScore[kEnemyScoreMax];
	void SnakeScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition);
	void MissSnakeScore();
	void TsuchinokoScore(bool isStrikeActive, float playerSizeMax, Vec2 enemyPosition);
	void MissTsuchinokoScore();


	//�������ɓ������Ă��܂����Ƃ�
	bool mIsWarning;
	bool mIsWarningRed;
	float mWarningAlphat;
	unsigned int mWarningColor;
	void Warning();


	//�~�j�}�b�v
	Vec2 mMiniMapPosition{ Screen::kWindowWidth - Screen::kMiniMapSize - 10.0f, Screen::kWindowHeight - Screen::kMiniMapSize - 10.0f };
	Vec2 mRadarPosition{ mMiniMapPosition.x ,mMiniMapPosition.y - Screen::kMiniMapSize - 10.0f };


	//�������
	Vec2 mStickPosition{ 60, 685 };
	unsigned int mXColor;
	bool mXScaleActive;
	Vec2 mXScale;
	float mXEasingt;
	unsigned int mAColor;
	bool mAScaleActive;
	Vec2 mAScale;
	float mAEasingt;


	//�摜
	bool mIsLoadTexture = false;
	int mTimeNumber;
	int mTimeLimitNumber;
	int mStart;
	int mTimeUp;
	int mComboLetter;
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

	//�}�b�v�[�萔
	static constexpr int kMapRadius = 5000;
	static constexpr int kMapTop = kMapRadius;
	static constexpr int kMapBottom = -kMapRadius;
	static constexpr int kMapLeft = -kMapRadius;
	static constexpr int kMapRight = kMapRadius;

	//�ʏ펞�ƃt�B�[�o�[���Ŕw�i�F��ς���
	bool mIsBackGroundColorChange;
	unsigned int mBackGroundStartColor;
	unsigned int mBackGroundEndColor;
	unsigned int mBackGroundColor;
	float mBackGroundEasingt;

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

