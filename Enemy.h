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
	void Update(int timeLeft, Vec2 playerPos, int lockOnCount, bool isReady);
	void Draw(Screen& screen, int hitStop);

	enum WAVE
	{
		WAVE1,
		WAVE2,
		WAVE3,
		WAVEMAX
	};
	WAVE wave;


	Snake snake[kEnemyMax];
	Tsuchinoko tsuchinoko[kEnemyMax];


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
};