#pragma once
#include "Vec2.h"
#include "Enemy.h"
#include "Tsuchinoko.h"
class Fever
{
public:
	int mSnakeDefeat = 0;//ヘビを倒した数
	int mTsuchinokoDefeat = 0;//ツチノコを倒した数
	int mFeverGauge = 0;//フィーバーゲージ
	bool mIsFever = 0;//フィーバーしているかどうか
	void Update();//アップデート
	void Draw(Screen& screen);//描画
	static constexpr int kMaxParticle = 30;//パーティクルの最大数
	static constexpr int kMaxEnemy = Snake::kMaxSnake + Tsuchinoko::kMaxTsuchinoko;//パーティクルの最大数
	const float mParticleRadius = 200;//パーティクルを出す半径
	
	typedef struct Particle {
		Vec2 Pos;
		int IsUse = 0;
		Vec2 StartPos;
		Vec2 EndPos;
		float T = 0;
	};

	typedef struct ParticleCreat {
		Vec2 Pos;
		int IsUse = 0;
		Particle particle[kMaxParticle];
	};
	ParticleCreat particlecreat[kMaxEnemy];

};

