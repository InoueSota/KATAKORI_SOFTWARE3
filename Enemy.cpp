#include "Enemy.h"
#include "Key.h"
#include <stdio.h>



void Enemy::Init() {
	wave = WAVE1;
	mSnakeDeathCount = 0;
	mTsuchinokoDeathCount = 0;

	kSnakeDeathCountMax[FEVER] = kEnemyMax;
	kTsuchinokoDeathCountMax[FEVER] = kEnemyMax;
	kSnakeDeathCountMax[WAVE1] = 2;
	kTsuchinokoDeathCountMax[WAVE1] = 0;
	kSnakeDeathCountMax[WAVE2] = 6;
	kTsuchinokoDeathCountMax[WAVE2] = 0;
	kSnakeDeathCountMax[WAVE3] = 6;
	kTsuchinokoDeathCountMax[WAVE3] = 0;
	kSnakeDeathCountMax[WAVE4] = 0;
	kTsuchinokoDeathCountMax[WAVE4] = 2;
	kSnakeDeathCountMax[WAVE5] = 0;
	kTsuchinokoDeathCountMax[WAVE5] = 6;
	kSnakeDeathCountMax[WAVE6] = 0;
	kTsuchinokoDeathCountMax[WAVE6] = 5;
	kSnakeDeathCountMax[WAVE7] = 3;
	kTsuchinokoDeathCountMax[WAVE7] = 2;
	kSnakeDeathCountMax[WAVE8] = 8;
	kTsuchinokoDeathCountMax[WAVE8] = 4;
	kSnakeDeathCountMax[WAVE9] = 0;
	kTsuchinokoDeathCountMax[WAVE9] = 20;
	kSnakeDeathCountMax[WAVE10] = 16;
	kTsuchinokoDeathCountMax[WAVE10] = 16;
	mWave10Frame = 0;
	mDeadCount = 0;

	for (int i = 0; i < WAVEMAX; i++) {
		mIsMakeSnake[i] = false;
		mIsMakeTsuchinoko[i] = false;
	}
	for (int i = 0; i < kEnemyMax; i++) {
		snake[i].Init();
		tsuchinoko[i].Init();
	}
}


void Enemy::Update(int timeLeft, Vec2 playerPos, int lockOnCount, bool isReady, bool isFever) {

	if (isReady)
	{
		switch (wave)
		{
		case FEVER:

			MakeSnake("./csv/snakeFever.csv");
			MakeTsuchinoko("./csv/tsuchinokoFever.csv");

			for (int i = 0; i < kEnemyMax; i++) {
				snake[i].Make(timeLeft, playerPos);
				tsuchinoko[i].Make(playerPos, timeLeft);
			}

			if (!isFever) {
				mIsMakeSnake[nowWave] = false;
				mIsMakeTsuchinoko[nowWave] = false;
				wave = (WAVE)(nowWave + 1);
			}

			break;
		case WAVE1:

			nowWave = wave;

			MakeSnake("./csv/snake1.csv");
			MakeTsuchinoko("./csv/tsuchinoko1.csv");

			NextWave(WAVE2);

			break;
		case WAVE2:

			nowWave = wave;

			MakeSnake("./csv/snake2.csv");
			MakeTsuchinoko("./csv/tsuchinoko2.csv");

			NextWave(WAVE3);

			break;
		case WAVE3:

			nowWave = wave;

			MakeSnake("./csv/snake3.csv");
			MakeTsuchinoko("./csv/tsuchinoko3.csv");

			NextWave(WAVE4);

			break;
		case WAVE4:

			nowWave = wave;

			MakeSnake("./csv/snake4.csv");
			MakeTsuchinoko("./csv/tsuchinoko4.csv");

			NextWave(WAVE5);

			break;
		case WAVE5:

			nowWave = wave;

			MakeSnake("./csv/snake5.csv");
			MakeTsuchinoko("./csv/tsuchinoko5.csv");

			NextWave(WAVE6);

			break;
		case WAVE6:

			nowWave = wave;

			MakeSnake("./csv/snake6.csv");
			MakeTsuchinoko("./csv/tsuchinoko6.csv");

			NextWave(WAVE7);

			break;
		case WAVE7:

			nowWave = wave;

			MakeSnake("./csv/snake7.csv");
			MakeTsuchinoko("./csv/tsuchinoko7.csv");

			NextWave(WAVE8);

			break;
		case WAVE8:

			nowWave = wave;

			MakeSnake("./csv/snake8.csv");
			MakeTsuchinoko("./csv/tsuchinoko8.csv");

			//“G“¯Žm’Ç‚¢‡‚¤ˆ—
			for (int i = 0; i < kSnakeDeathCountMax[WAVE8]; i++) {
				if (i != kSnakeDeathCountMax[WAVE8] - 1) {
					snake[i].mTargetPoint = snake[i + 1].mHeadPosition;
				} else {
					snake[kSnakeDeathCountMax[WAVE8] - 1].mTargetPoint = snake[0].mHeadPosition;
				}
			}
			for (int i = 0; i < kTsuchinokoDeathCountMax[WAVE8]; i++) {
				if (i != kTsuchinokoDeathCountMax[WAVE8] - 1) {
					tsuchinoko[i].mTargetPoint = tsuchinoko[i + 1].mCenterPosition;
				} else {
					tsuchinoko[kTsuchinokoDeathCountMax[WAVE8] - 1].mTargetPoint = tsuchinoko[0].mCenterPosition;
				}
			}

			NextWave(WAVE9);

			break;
		case WAVE9:

			nowWave = wave;

			MakeSnake("./csv/snake9.csv");
			MakeTsuchinoko("./csv/tsuchinoko9.csv");

			NextWave(WAVE10);

			break;
		case WAVE10:

			nowWave = wave;

			MakeSnake("./csv/snake10.csv");
			MakeTsuchinoko("./csv/tsuchinoko10.csv");

			mWave10Frame++;

			//“G“¯Žm’Ç‚¢‡‚¤ˆ—
				for (int i = 0; i < kSnakeDeathCountMax[WAVE10]; i++) {
					if (i != kSnakeDeathCountMax[WAVE10] - 1) {
						snake[i].mTargetPoint = snake[i + 1].mHeadPosition;
					} else {
						snake[kSnakeDeathCountMax[WAVE10] - 1].mTargetPoint = snake[0].mHeadPosition;
					}
				}
				for (int i = 0; i < kTsuchinokoDeathCountMax[WAVE10]; i++) {
					if (i != kTsuchinokoDeathCountMax[WAVE10] - 1) {
						tsuchinoko[i].mTargetPoint = tsuchinoko[i + 1].mCenterPosition;
					} else {
						tsuchinoko[kTsuchinokoDeathCountMax[WAVE10] - 1].mTargetPoint = tsuchinoko[0].mCenterPosition;
					}
				}

			NextWave(WAVE1);

			break;
		}

		//ƒtƒB[ƒo[Žž‚Í–³ŒÀ”­¶‚É‚·‚é
		if (isFever && wave != FEVER) {
			mIsMakeSnake[FEVER] = false;
			mIsMakeTsuchinoko[FEVER] = false;
			wave = FEVER;
		}

		if (Key::IsTrigger(DIK_1)) {
			mIsMakeSnake[WAVE1] = false;
			mIsMakeTsuchinoko[WAVE1] = false;
			wave = WAVE1;
		}
		if (Key::IsTrigger(DIK_2)) {
			mIsMakeSnake[WAVE2] = false;
			mIsMakeTsuchinoko[WAVE2] = false;
			wave = WAVE2;
		}
		if (Key::IsTrigger(DIK_3)) {
			mIsMakeSnake[WAVE3] = false;
			mIsMakeTsuchinoko[WAVE3] = false;
			wave = WAVE3;
		}
		if (Key::IsTrigger(DIK_4)) {
			mIsMakeSnake[WAVE4] = false;
			mIsMakeTsuchinoko[WAVE4] = false;
			wave = WAVE4;
		}
		if (Key::IsTrigger(DIK_5)) {
			mIsMakeSnake[WAVE5] = false;
			mIsMakeTsuchinoko[WAVE5] = false;
			wave = WAVE5;
		}
		if (Key::IsTrigger(DIK_6)) {
			mIsMakeSnake[WAVE6] = false;
			mIsMakeTsuchinoko[WAVE6] = false;
			wave = WAVE6;
		}
		if (Key::IsTrigger(DIK_7)) {
			mIsMakeSnake[WAVE7] = false;
			mIsMakeTsuchinoko[WAVE7] = false;
			wave = WAVE7;
		}
		if (Key::IsTrigger(DIK_8)) {
			mIsMakeSnake[WAVE8] = false;
			mIsMakeTsuchinoko[WAVE8] = false;
			wave = WAVE8;
		}
		if (Key::IsTrigger(DIK_9)) {
			mIsMakeSnake[WAVE9] = false;
			mIsMakeTsuchinoko[WAVE9] = false;
			wave = WAVE9;
		}
		if (Key::IsTrigger(DIK_0)) {
			mIsMakeSnake[WAVE10] = false;
			mIsMakeTsuchinoko[WAVE10] = false;
			wave = WAVE10;
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
				snake[i].mDirectionPoint = snake[i].mTargetPoint - snake[i].mHeadPosition;
				snake[i].mVelocity = snake[i].mDirectionPoint.Normalized();

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
				tsuchinoko[i].mDirectionPoint = tsuchinoko[i].mTargetPoint - tsuchinoko[i].mCenterPosition;
				tsuchinoko[i].mVelocity = tsuchinoko[i].mDirectionPoint.Normalized();

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

	if ((kSnakeDeathCountMax[wave] == mSnakeDeathCount && kTsuchinokoDeathCountMax[wave] == mTsuchinokoDeathCount) || Key::IsTrigger(DIK_N)) {
		mIsMakeSnake[nextWave] = false;
		mIsMakeTsuchinoko[nextWave] = false;
		if (nextWave == WAVE10) {
			mWave10Frame = 0;
		}
		wave = nextWave;
	}
}

void Enemy::Draw(Screen& screen, int hitStop) {

	for (int i = 0; i < kEnemyMax; i++) {
		snake[i].Draw(screen, hitStop);
		tsuchinoko[i].Draw(screen, hitStop);
	}
}
void Enemy::HitSound() {

	if (!mIsPlayHitSound) {

		int handle = -1;
		if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
			handle = Novice::PlayAudio(enemyhit, 0, 1);
		}

		mIsPlayHitSound = true;
	}

}
void Enemy::StrikeHitSound() {

	if (!mIsPlayStrikeHitSound) {

		int handle = -1;
		if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
			handle = Novice::PlayAudio(strikeenemyhit[mDeadCount], 0, 1);
		}
		mDeadCount++;
		mDeadCount = Clamp(mDeadCount, 0, hiC);

		mIsPlayStrikeHitSound = true;
	}
}
void Enemy::LoadTexture() {
	enemyhit = Novice::LoadAudio("./Sounds/enemyhit.wav");
	strikeenemyhit[C] = Novice::LoadAudio("./Sounds/strikeC.wav");
	strikeenemyhit[D] = Novice::LoadAudio("./Sounds/strikeD.wav");
	strikeenemyhit[E] = Novice::LoadAudio("./Sounds/strikeE.wav");
	strikeenemyhit[F] = Novice::LoadAudio("./Sounds/strikeF.wav");
	strikeenemyhit[G] = Novice::LoadAudio("./Sounds/strikeG.wav");
	strikeenemyhit[A] = Novice::LoadAudio("./Sounds/strikeA.wav");
	strikeenemyhit[B] = Novice::LoadAudio("./Sounds/strikeB.wav");
	strikeenemyhit[hiC] = Novice::LoadAudio("./Sounds/strikehiC.wav");
}