#pragma once
#include "Vec2.h"
#include "Snake.h"
#include "Tsuchinoko.h"
class Fever
{
public:
	Fever() {
		Init();
	}

	bool mIsLoadTexture;
	int PlayerFeverParticleTexture;

	int mSnakeDefeat = 0;			//ヘビを倒した数
	int mTsuchinokoDefeat = 0;		//ツチノコを倒した数
	int mFeverGauge = 0;			//フィーバーゲージ
	bool mIsFever = 0;				//フィーバーしているかどうか
	bool mIsOldFever = 0;			//フィーバーしていたかどうか
	void Init();					//初期化
	void Update(Screen& screen, Vec2 PlayerPos);	//アップデート
	void Draw(Screen& screen);		//描画
	static constexpr int kMaxParticle = 30;//パーティクルの最大数
	static constexpr int kMaxPlayerFeverParticle = 100;//パーティクルの最大数
	static constexpr int kMaxEnemy = Snake::kMaxSnake + Tsuchinoko::kMaxTsuchinoko;//パーティクルの最大数
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
		double color = 0xFFFFFF00;
	};
	FeverGauge feverGauge;

};

