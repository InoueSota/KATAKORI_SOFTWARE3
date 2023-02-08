#pragma once
#include "Vec2.h"
#include "Screen.h"



class Title
{
public:
	Title() {
		Init();
	}

	void Init();
	void Update();
	void Draw(Screen& screen);
	void FrontDraw(Screen& screen);


	//中心座標
	Vec2 mCenterPosition{ Screen::kWindowWidth / 2, Screen::kWindowHeight / 2 };

	//"KATAKORI SOFTWARE"の描画
	bool mIsKatakoriClear = true;
	bool mIsKatakoriAlphaClear = true;
	bool mIsKatakoriScaleStart = true;
	Vec2 mKatakoriScale{ 0.75f, 0.75f };
	float mKatakoriAlphaT = 0.0f;
	unsigned int mKatakoriColor = 0xFFFFFF00;
	int mKatakoriFrame = 0;
	void Katakori();

	//タイトル
	enum TitleWord
	{
		TO,
		GE,
		SU,
		MA,
		LTU,
		SI,
		LYU,
		WORDMAX
	};
	bool mIsHitTitleWord[WORDMAX];
	Vec2 mTitleWordPosition[WORDMAX];
	Vec2 mTitleWordJudgePosition[WORDMAX];
	float mTitleWordJudgeSize[WORDMAX];
	float mTitleWordBasePositionY[WORDMAX];
	float mTitleWordBoundY;
	float mTitleWordGravityY[WORDMAX];
	float mTitleWordAlphat;
	int mTitleWordBoundIntervalFrame[WORDMAX];
	unsigned int mTitleWordColor;
	unsigned int mTitleBackGroundColor;
	unsigned int mXScaleColor;
	Vec2 mXScale;
	float mXEasingt;

	//背景に四角形をランダムに
	static constexpr int kBackBoxMax = 50;
	bool mIsBackBoxActive[kBackBoxMax];
	bool mIsBackBoxEasingClear[kBackBoxMax];
	Vec2 mBackBoxPosition[kBackBoxMax];
	float mBackBoxAngle[kBackBoxMax];
	float mBackBoxEasingt[kBackBoxMax];
	int mBackBoxFrame;
	int mBackBoxSize[kBackBoxMax];
	int mBackBoxEndSize[kBackBoxMax];

	//画像
	bool mIsLoadTexture = false;
	int mKatakori;
	int mTitleWord[WORDMAX];
	int mX;
	void LoadTexture();

};

class Change
{
public:
	Change() {
		Init();
	}

	void Init();
	void Update();
	void DrawTitle(Screen& screen);
	void Draw(Screen& screen);

	//画面遷移
	static constexpr int kChangeBoxMax = 200;
	bool mIsChangeActive;
	float mMakePosition;
	bool mIsChangeBoxActive[kChangeBoxMax];
	Vec2 mChangeBoxPosition[kChangeBoxMax];
	float mChangeBoxEasingt[kChangeBoxMax];
	int mChangeBoxSize[kChangeBoxMax];
};

class Result
{
public:
	Result() {
		Init();
	}

	void Init();
	void Update(float score);
	void Draw(Screen& screen);


	//スコア
	bool mIsScoreInit;
	bool mIsDarkScore;
	Vec2 mScorePosition[8];
	unsigned int mScoreColor[8];
	int mScoreSize;
	float mStartScore;
	float mEndScore;
	float mScore;
	float mScoreEasingt;

	Vec2 mCenterPosition{ Screen::kWindowWidth / 2, Screen::kWindowHeight / 2 };
	unsigned int mXScaleColor;
	Vec2 mXScale;
	float mXEasingt;

	//画像
	int mResult;
	int mNumber;
	int mX;
	void LoadTexture();


};

