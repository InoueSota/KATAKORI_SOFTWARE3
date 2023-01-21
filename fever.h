#pragma once
#include "Vec2.h"
#include "Enemy.h"
#include "Tsuchinoko.h"
class Fever
{
public:
	int mSnakeDefeat = 0;//�w�r��|������
	int mTsuchinokoDefeat = 0;//�c�`�m�R��|������
	int mFeverGauge = 0;//�t�B�[�o�[�Q�[�W
	bool mIsFever = 0;//�t�B�[�o�[���Ă��邩�ǂ���
	void Update(Screen& screen);//�A�b�v�f�[�g
	void Draw(Screen& screen);//�`��
	static constexpr int kMaxParticle = 30;//�p�[�e�B�N���̍ő吔
	static constexpr int kMaxEnemy = Snake::kMaxSnake + Tsuchinoko::kMaxTsuchinoko;//�p�[�e�B�N���̍ő吔
	const float mParticleRadius = 200;//�p�[�e�B�N�����o�����a
	
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

