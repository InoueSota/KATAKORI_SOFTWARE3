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

	int mSnakeDefeat = 0;			//�w�r��|������
	int mTsuchinokoDefeat = 0;		//�c�`�m�R��|������
	int mFeverGauge = 0;			//�t�B�[�o�[�Q�[�W
	bool mIsFever = 0;				//�t�B�[�o�[���Ă��邩�ǂ���
	bool mIsOldFever = 0;			//�t�B�[�o�[���Ă������ǂ���
	void Init();					//������
	void Update(Screen& screen);	//�A�b�v�f�[�g
	void Draw(Screen& screen);		//�`��
	static constexpr int kMaxParticle = 30;//�p�[�e�B�N���̍ő吔
	static constexpr int kMaxEnemy = Snake::kMaxSnake + Tsuchinoko::kMaxTsuchinoko;//�p�[�e�B�N���̍ő吔
	const float mParticleRadius = 200;//�p�[�e�B�N�����o�����a
	static constexpr int kfeverGaugeDelayTime = 50;//�t�B�[�o�[�Q�[�W�x���t���[����

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

	typedef struct FeverGauge {
		Vec2 StartPos;
		Vec2 EndPos;
		Vec2 Pos;
		float T = 0;
		int Flag;
		int Timer = kfeverGaugeDelayTime;
	};
	FeverGauge feverGauge;

};

