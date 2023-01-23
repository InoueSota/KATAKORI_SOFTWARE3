#pragma once
#include "Screen.h"
#include "Vec2.h"

class Player
{
public:

	Player() {
		Init();
	}

	void Init();
	void Update(Screen& screen);
	void Draw(Screen& screen);


	//�p�����[�^
	Vec2 mPosition;
	Vec2 mOldPosition;
	Vec2 mVelocity;
	float mSize;
	float mRadius;


	//�ʏ�ړ� = NormalMove
	Vec2 mNormalVelocity;
	float mNormalMag;
	void NormalMove();


	//�_�b�V�� = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	void Dush();


	//�}�[�L���O = Mark
	bool mIsMarkActive;
	Vec2 mMarkPosition;
	static constexpr int kMarkTimeLimit = 300;
	int mMarkFrame;
	void Mark();


	//�X�g���C�N = Strike
	bool mIsStrikeActive;
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	Vec2 mStrikeStartPosition;
	Vec2 mStrikeEndPosition;
	void Strike();

	//�X�g���C�N���o
	static constexpr int kStrikeLineMax = 200;
	bool mIsStrikeLineActive[kStrikeLineMax];
	Vec2 mStrikeLinePosition[kStrikeLineMax];
	int mStrikeLineWidth[kStrikeLineMax];
	int mStrikeLineHeight[kStrikeLineMax];
	float mStrikeLineAngle;
	float mStrikeLineAlphat[kStrikeLineMax];
	unsigned int mStrikeLineColor[kStrikeLineMax];
	void StrikeLine(Screen& screen);


	//�c��
	int mShadowFrame;
	static constexpr int kShadowMax = 10;
	bool mIsShadowActive[kShadowMax];
	Vec2 mShadowPosition[kShadowMax];
	float mShadowAlphat[kShadowMax];
	unsigned int mShadowColor[kShadowMax];
	//�_�b�V���c��
	bool mIsDushShadowActive;
	Vec2 mDushShadowPosition[4];
	float mDushShadowAlphat;
	unsigned int mDushShadowColor;
	void Shadow(bool isHitStop);


	//�m�b�N�o�b�N
	int LockonCount;//���̂̓G�ɕ⑫����Ă��邩
	bool mKnockbackSet;
	bool mKnockbackActive;
	Vec2 mKnockbackEnemyPos;
	Vec2 mKnockbackStart;
	Vec2 mKnockbackEnd;
	float mKnockBackT;
	void Knockback();
	void SetKnockbackPosition(Vec2 enemyPosition, float enemyRadius);

};


