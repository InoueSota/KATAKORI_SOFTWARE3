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

	bool mIsLoadTexture;
	int PlayerFeverParticleTexture;

	int mSnakeDefeat = 0;			//�w�r��|������
	int mTsuchinokoDefeat = 0;		//�c�`�m�R��|������
	int mSnakeDefeatStrike = 0;		//�X�g���C�N���Ƀw�r��|������
	int mTsuchinokoDefeatStrike = 0;//�X�g���C�N���Ƀc�`�m�R��|������
	int mFeverGaugeStrikeEndFlag = 2;//�t�B�[�o�[�Q�[�W���Z�t���O
	int mFeverGauge = 0;			//�t�B�[�o�[�Q�[�W
	bool mIsFever = 0;				//�t�B�[�o�[���Ă��邩�ǂ���
	bool mIsOldFever = 0;			//�t�B�[�o�[���Ă������ǂ���
	bool mFeverStop = 0;
	void Init();					//������
	void Update(Screen& screen, Vec2 PlayerPos, bool isStrikeActive);	//�A�b�v�f�[�g
	void Draw(Screen& screen);		//�`��
	static constexpr int kMaxParticle = 30;//�p�[�e�B�N���̍ő吔
	static constexpr int kMaxPlayerFeverParticle = 100;//�p�[�e�B�N���̍ő吔
	static constexpr int kMaxEnemy = Enemy::kEnemyMax + Enemy::kEnemyMax;//�p�[�e�B�N���̍ő吔
	const float mParticleRadius = 200;//�p�[�e�B�N�����o�����a
	static constexpr int kfeverGaugeDelayTime = 50;//�t�B�[�o�[�Q�[�W�x���t���[����
	static constexpr int kPlayerFeverParticleTime = 60;//�v���C���[�t�B�[�o�[�p�[�e�B�N���̃^�C�}�[�K��l

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

};

