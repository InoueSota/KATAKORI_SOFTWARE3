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
	void Update(Screen& screen, bool isFever, bool isOldFever);
	void Draw(Screen& screen, bool isReady);


	//�p�����[�^
	Vec2 mPosition;
	Vec2 mOldPosition;
	Vec2 mVelocity;
	float mSizeValue;
	float mSize;
	float mRadius;
	unsigned int mColor;


	//�ʏ�ړ� = NormalMove
	Vec2 mNormalVelocity;
	float mNormalMag;
	float mNormalMagLockon;
	void NormalMove(Screen& screen);


	//�_�b�V�� = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	int mDushTimer = 0;
	static constexpr int kMaxDushTimer = 30;
	void Dush();


	//�}�[�L���O = Mark
	const int kMarkMaxLength = 1600;
	static constexpr int kMarkTimeLimit = 240;
	bool mIsMarkActive;
	Vec2 mMarkPosition;
	int mMarkFrame;
	void Mark();
	void MarkLimitLength(bool isFever);


	//�X�g���C�N = Strike
	enum StrikeMode {
		STRAIGHT,
		SPIRAL
	};
	StrikeMode strikeMode = STRAIGHT;
	bool mIsStrikeActive;
	bool mIsOldStrikeActive;
	bool mIsStraightStrikeActive;
	bool mIsStraightStrikeFinish;
	static constexpr int kToHeadMinusValue = 4;
	static constexpr int kToBodyMinusValue = 2;
	static constexpr int kToBodyPlusValue = 6;
	static constexpr int kStrikePowerMax = 5;
	Vec2 mStrikeVelocity;
	Vec2 mStrikeDirection;
	Vec2 mStrikeClampMin;
	Vec2 mStrikeClampMax;
	Vec2 mStrikeStartPosition;
	Vec2 mStrikeEndPosition;
	float mStrikeSpeed;
	float mStraightStrikeTheta;
	float mStraightStrikeOldTheta;
	Vec2 mStrikePowerPosition[kStrikePowerMax];
	Vec2 mStrikeModePosition{ 100,630 };
	Vec2 mStrikeModeScale;
	Vec2 mStrikeModeBScale;
	bool mStrikeModeScaleActive;
	float mStrikeModeScaleEasingt;
	int mStrikePower;
	void DrawStrikeUI(Screen& screen);
	//�t�B�[�o�[�X�g���C�N
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	float mStrikeTheta;
	float mStrikeThetaStartValue;
	float mStrikeRadius;
	float mStrikeRadiusStartValue;
	void Strike(bool isFever, bool isOldFever);


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


	//�摜
	int flame;
	int lastflame;
	int lengthflame;
	int circle;
	int straight;
	int spiral;
	int b;
	int rb;
	int nopower;
	int toge;
	int mark;
	int areyouready;
	void LoadTexture();

};


