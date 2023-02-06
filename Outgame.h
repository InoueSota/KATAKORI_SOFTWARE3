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
	Vec2 mTitleWordPosition[WORDMAX];
	float mTitleWordAlphat;
	unsigned int mTitleWordColor;
	unsigned int mTitleBackGroundColor;
	unsigned int mAScaleColor;
	Vec2 mAScale;
	float mAEasingt;

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
	int mA;
	void LoadTexture();

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
	Vec2 mScorePosition[7];
	unsigned int mScoreColor[7];
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

