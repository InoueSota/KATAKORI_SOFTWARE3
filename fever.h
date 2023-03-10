#pragma once
#include "Screen.h"
#include "Vec2.h"
#include "Enemy.h"



class Fever
{
public:
	Fever() {
		Init();
	}

	int mSnakeDefeat = 0;			//ヘビを倒した数
	int mTsuchinokoDefeat = 0;		//ツチノコを倒した数
	int mSnakeDefeatStrike = 0;		//ストライク中にヘビを倒した数
	int mTsuchinokoDefeatStrike = 0;//ストライク中にツチノコを倒した数
	int mFeverGaugeStrikeEndFlag = 2;//フィーバーゲージ加算フラグ
	int mFeverGauge = 0;			//フィーバーゲージ
	bool mIsFever = 0;				//フィーバーしているかどうか
	bool mIsOldFever = 0;			//フィーバーしていたかどうか
	bool mFeverStop = 0;
	void Init();					//初期化
	void Update(Screen& screen, Vec2 PlayerPos, bool isStrikeActive);	//アップデート
	void Draw(Screen& screen);		//描画
	static constexpr int kMaxParticle = 30;//パーティクルの最大数
	static constexpr int kMaxPlayerFeverParticle = 100;//パーティクルの最大数
	static constexpr int kMaxEnemy = Enemy::kEnemyMax + Enemy::kEnemyMax;//パーティクルの最大数
	const float mParticleRadius = 200;//パーティクルを出す半径
	static constexpr int kfeverGaugeDelayTime = 50;//フィーバーゲージ遅延フレーム数
	static constexpr int kPlayerFeverParticleTime = 60;//プレイヤーフィーバーパーティクルのタイマー規定値

	typedef struct Particle {
		Vec2 Pos;
		int IsUse = 0;
		Vec2 StartPos;
		Vec2 EndPos;
		float T = 0;
		int Timer = kPlayerFeverParticleTime;
		float velocity;
		double R = 255;
		double B = 255;
		double G = 255;
		double color = 0xFFFFFF00;
		int alpha = 200;
	};
	Particle DefaultParticle;
	Particle playerFeverParticle[kMaxPlayerFeverParticle];

	typedef struct ParticleCreat {
		Vec2 Pos;
		int IsUse = 0;
		Particle particle[kMaxParticle];
	};
	ParticleCreat particlecreat[kMaxEnemy];
	ParticleCreat particleEnemySpawnCreat[kMaxEnemy];

	typedef struct FeverGauge {
		Vec2 StartPos;
		Vec2 EndPos;
		Vec2 Pos;
		float T = 0;
		int Flag;
		int Timer = kfeverGaugeDelayTime;
		double R = 255;
		double B = 155;
		double G = 55;
		int Rnum = 1;
		int Bnum = 1;
		int Gnum = 1;
		int alpha = 255;
		unsigned int color = 0xFFFFFF00;
	};
	FeverGauge feverGauge;

	//画像変数
	bool mIsLoadTexture;
	int PlayerFeverParticleTexture;
	int FeverUITexture;;
	int FeverEndWarning;

	//UIイージング
	int mWarningAlphat;
	int mWarningColor = 0xFFFFFF00;
	bool mIsWarningRed;

};

