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
	void Update(int mStrikeTutorial, int mIsStrikeActive);
	void Draw(Screen& screen, bool mIsReady);

	//�n�߂邩�t���O
	const int kToStart = 3;
	bool mIsReady;
	bool mIsOldReady;
	bool mIsStart;
	bool mIsOldStart;
	int mCountDownTime;
	Vec2 mStartPosition;
	int mTutorialScene;
	int mTutorialControl;
	bool mSlowTutorialFlag;


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


	//�G��|�������i�X�R�A�̉��Z�ɉe������j
	bool mIsKillCountScaleAnimation;
	bool mIsDarkKillCount;
	Vec2 mKillCountPosition[3];
	Vec2 mKillCountScale;
	unsigned int mKillCountColor[3];
	int mKillCount;
	void KillCount();
	void AddKillCount();


	//�X�R�A
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
	void SnakeScore(bool isStrikeActive, Vec2 enemyPosition);
	void TsuchinokoScore(bool isStrikeActive, Vec2 enemyPosition);
	float mStartScore;
	float mEndScore;
	float mScoreAnimationEasingt;
	void ScoreAnimation();


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


	//�摜
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
	int Tutorial1;
	int Tutorial2;
	int Tutorial3;
	int Tutorial4;
	int Tutorial5;
	int areyouready;
	int TutorialSkip;
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
	void Update(bool isFever, bool isOldFever, unsigned int feverColor);
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

	//�w�i�Ɏl�p�`�������_����
	static constexpr int kBackBoxMax = 200;
	bool mIsBackBoxActive[kBackBoxMax];
	bool mIsBackBoxEasingClear[kBackBoxMax];
	Vec2 mBackBoxPosition[kBackBoxMax];
	float mBackBoxAngle[kBackBoxMax];
	float mBackBoxEasingt[kBackBoxMax];
	int mBackBoxFrame;
	int mBackBoxSize[kBackBoxMax];
	int mBackBoxEndSize[kBackBoxMax];
	unsigned int mBackBoxColor;

	//�f�o�b�O�p
	static constexpr int kAxisLength = 33;
	static constexpr int kLineInterval = 300;
	unsigned int mBackLineColor;
	Vec2 mXaxisStartPosition[kAxisLength];
	Vec2 mXaxisEndPosition[kAxisLength];
	Vec2 mYaxisStartPosition[kAxisLength];
	Vec2 mYaxisEndPosition[kAxisLength];
	bool XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	bool YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);

};

