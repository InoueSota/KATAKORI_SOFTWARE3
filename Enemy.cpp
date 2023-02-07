#include "Enemy.h"
#include "Key.h"
#include <stdio.h>



void Enemy::Init() {
	wave = WAVE1;
	mSnakeDeathCount = 0;
	mTsuchinokoDeathCount = 0;

	kSnakeDeathCountMax[WAVE1] = 2;
	kTsuchinokoDeathCountMax[WAVE1] = 0;
	kSnakeDeathCountMax[WAVE2] = 0;
	kTsuchinokoDeathCountMax[WAVE2] = 2;
	kSnakeDeathCountMax[WAVE3] = 2;
	kTsuchinokoDeathCountMax[WAVE3] = 2;

	for (int i = 0; i < WAVEMAX; i++) {
		mIsMakeSnake[i] = false;
		mIsMakeTsuchinoko[i] = false;
	}
	for (int i = 0; i < kEnemyMax; i++) {
		snake[i].Init();
		tsuchinoko[i].Init();
	}
}


void Enemy::Update(int timeLeft, Vec2 playerPos, int lockOnCount, bool isReady) {

	if (isReady)
	{
		switch (wave)
		{
		case WAVE1:

			MakeSnake("./csv/snake1.csv");
			MakeTsuchinoko("./csv/tsuchinoko1.csv");

			NextWave(WAVE2);

			break;
		case WAVE2:

			MakeSnake("./csv/snake2.csv");
			MakeTsuchinoko("./csv/tsuchinoko2.csv");

			NextWave(WAVE3);

			break;
		case WAVE3:

			MakeSnake("./csv/snake3.csv");
			MakeTsuchinoko("./csv/tsuchinoko3.csv");

			NextWave(WAVE2);

			break;
		}
	}


	for (int i = 0; i < kEnemyMax; i++) {
		snake[i].Update(timeLeft, playerPos, lockOnCount, isReady);
		tsuchinoko[i].Update(timeLeft, playerPos, lockOnCount, isReady);
	}
}

bool Enemy::MakeSnake(const char* csv) {

	if (Key::IsTrigger(DIK_SPACE) || !mIsMakeSnake[wave]) {

		FILE* fp = NULL;
		fopen_s(&fp, csv, "rt");
		if (fp == NULL) {
			return 0;
		}

		mSnakeDeathCount = 0;

		for (int i = 0; i < kEnemyMax; i++)
		{
			if (i < kSnakeDeathCountMax[wave]) {

				fscanf_s(fp, "%f, %f, %f, %f\n", &snake[i].mHeadPosition.x, &snake[i].mHeadPosition.y, &snake[i].mTargetPoint.x, &snake[i].mTargetPoint.y);
				snake[i].mDirectionPoint = snake[i].mTargetPoint;

				for (int j = 0; j < snake[i].kMaxSpawnParticle; j++) {
					snake[i].spawnParticle[j].Pos = snake[i].mHeadPosition;
					snake[i].spawnParticle[j].EndPos = snake[i].mHeadPosition;
					float angle = Degree(RAND(1, 360));
					snake[i].spawnParticle[j].StartPos.x = snake[i].spawnParticle[j].Pos.x + cosf(angle) * 200;
					snake[i].spawnParticle[j].StartPos.y = snake[i].spawnParticle[j].Pos.y + sinf(angle) * 200;
					snake[i].spawnParticle[j].IsUse = 1;
				}

				snake[i].mHeadPositionStart = snake[i].mHeadPosition;
				for (int j = 0; j < Snake::kMaxFrame; j++) {
					snake[i].mOldHeadPosition[j] = snake[i].mHeadPosition;
				}

				snake[i].LockOnMoveTimer = 0;
				snake[i].IsPlayerLockon = false;
				snake[i].mIsClearAnimation = false;
				snake[i].mIsDeath = false;
				snake[i].mIsOldDeath = false;
				snake[i].mIsActive = true;
				snake[i].mShakeTimer = -1;
			}
			else {
				snake[i].mIsClearAnimation = true;
				snake[i].mIsDeath = true;
				snake[i].mIsOldDeath = true;
				snake[i].mIsActive = false;

			}


		}
		mIsMakeSnake[wave] = true;
		fclose(fp);
	}
}
bool Enemy::MakeTsuchinoko(const char* csv) {

	if (Key::IsTrigger(DIK_SPACE) || !mIsMakeTsuchinoko[wave]) {

		FILE* fp = NULL;
		fopen_s(&fp, csv, "rt");
		if (fp == NULL) {
			return 0;
		}

		mTsuchinokoDeathCount = 0;

		for (int i = 0; i < kEnemyMax; i++)
		{
			if (i < kTsuchinokoDeathCountMax[wave]) {

				fscanf_s(fp, "%f, %f, %f, %f\n", &tsuchinoko[i].mCenterPosition.x, &tsuchinoko[i].mCenterPosition.y, &tsuchinoko[i].mTargetPoint.x, &tsuchinoko[i].mTargetPoint.y);
				tsuchinoko[i].mDirectionPoint = tsuchinoko[i].mTargetPoint;

				tsuchinoko[i].mIsClearAnimation = false;

				for (int j = 0; j < tsuchinoko[i].kMaxSpawnParticle; j++) {
					tsuchinoko[i].spawnParticle[j].Pos = tsuchinoko[i].mCenterPosition;
					tsuchinoko[i].spawnParticle[j].EndPos = tsuchinoko[i].mCenterPosition;
					float angle = Degree(RAND(1, 360));
					tsuchinoko[i].spawnParticle[j].StartPos.x = tsuchinoko[i].spawnParticle[j].Pos.x + cosf(angle) * 200;
					tsuchinoko[i].spawnParticle[j].StartPos.y = tsuchinoko[i].spawnParticle[j].Pos.y + sinf(angle) * 200;
					tsuchinoko[i].spawnParticle[j].IsUse = 1;
				}

				tsuchinoko[i].LockOnMoveTimer = 0;
				tsuchinoko[i].IsPlayerLockon = false;
				tsuchinoko[i].mCenterPositionStart = tsuchinoko[i].mCenterPosition;
				tsuchinoko[i].mIsDeath = false;
				tsuchinoko[i].mIsOldDeath = false;
				tsuchinoko[i].mIsActive = true;
				tsuchinoko[i].mShakeTimer = -1;
			}
			else {
				tsuchinoko[i].mIsClearAnimation = true;
				tsuchinoko[i].mIsDeath = true;
				tsuchinoko[i].mIsOldDeath = true;
				tsuchinoko[i].mIsActive = false;

			}
		}
		mIsMakeTsuchinoko[wave] = true;
		fclose(fp);
	}
}

void Enemy::NextWave(WAVE nextWave) {

	if (kSnakeDeathCountMax[wave] == mSnakeDeathCount && kTsuchinokoDeathCountMax[wave] == mTsuchinokoDeathCount) {
		mIsMakeSnake[nextWave] = false;
		mIsMakeTsuchinoko[nextWave] = false;
		wave = nextWave;
	}
}

void Enemy::Draw(Screen& screen, int hitStop) {

	for (int i = 0; i < kEnemyMax; i++) {
		snake[i].Draw(screen, hitStop);
		tsuchinoko[i].Draw(screen, hitStop);
	}
}