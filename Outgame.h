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


	//"KATAKORI SOFTWARE"�̕`��
	bool mIsKatakoriClear = true;
	bool mIsKatakoriAlphaClear = true;
	bool mIsKatakoriScaleStart = true;
	Vec2 mKatakoriPosition{ Screen::kWindowWidth / 2, Screen::kWindowHeight / 2 };
	Vec2 mKatakoriScale{ 0.75f, 0.75f };
	float mKatakoriAlphaT = 0.0f;
	unsigned int mKatakoriColor = 0xFFFFFF00;
	int mKatakoriFrame = 0;
	void Katakori();

	//�^�C�g��
	Vec2 mTitlePosition{ Screen::kWindowWidth / 2, Screen::kWindowHeight / 2 };
	float mTitleAlphaT;
	unsigned int mTitleColor;
	unsigned int mAScaleColor;
	Vec2 mAScale;
	float mAEasingt;

	//�摜
	bool mIsLoadTexture = false;
	int mKatakori;
	int mTitle;
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


	//�X�R�A
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

	//�摜
	int mResult;
	int mNumber;
	int mX;
	void LoadTexture();


};

