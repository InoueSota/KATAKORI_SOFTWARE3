#pragma once
#include "Screen.h"
#include "Snake.h"
#include "Tsuchinoko.h"



class Enemy
{
public:
	//�G�̍ő吔
	static constexpr int kEnemyMax = 20;

	void Init();
	void Update(int timeLeft, Vec2 playerPos, int lockOnCount, bool isReady, bool isFever);
	void Draw(Screen& screen, int hitStop);

	enum WAVE
	{
		FEVER,
		WAVE1,
		WAVE2,
		WAVE3,
		WAVE4,
		WAVE5,
		WAVE6,
		WAVE7,
		WAVE8,
		WAVE9,
		WAVE10,
		WAVEMAX
	};
	WAVE wave;
	WAVE nowWave;


	Snake snake[kEnemyMax];
	Tsuchinoko tsuchinoko[kEnemyMax];

	//�E�F�[�u�P�O��p�t���[��
	int mWave10Frame;

	//�E�F�[�u��i�߂邩
	int kSnakeDeathCountMax[WAVEMAX];
	int kTsuchinokoDeathCountMax[WAVEMAX];
	int mSnakeDeathCount;
	int mTsuchinokoDeathCount;
	bool mIsMakeSnake[WAVEMAX];
	bool mIsMakeTsuchinoko[WAVEMAX];
	bool MakeSnake(const char* csv);
	bool MakeTsuchinoko(const char* csv);
	void NextWave(WAVE nextWave);

	//��
	bool mIsPlayHitSound;
	bool mIsPlayStrikeHitSound;
	int enemyhit;
	enum MusicalScale
	{
		C,
		D,
		E,
		F,
		G,
		A,
		B,
		hiC,
		MusicalScaleMax
	};
	int strikeenemyhit[MusicalScaleMax];
	int mDeadCount;
	void HitSound();
	void StrikeHitSound();
	void LoadTexture();
};