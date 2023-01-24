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
	bool mIsKatakoriClear = false;
	bool mIsKatakoriAlphaClear = true;
	bool mIsKatakoriScaleStart = false;
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

	//�摜
	bool mIsLoadTexture = false;
	int mKatakori;
	int mTitle;
	void LoadTexture();

};

