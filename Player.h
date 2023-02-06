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
	void Draw(Screen& screen, bool isReady, bool isFever, unsigned int feverGaugeColor, unsigned int backLineColor);


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
	void NormalMove();


	//�_�b�V�� = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	int mDushTimer = 0;
	static constexpr int kMaxDushTimer = 30;
	bool mIsUseDushOutOfRange;
	bool mIsOldUseDushOutOfRange;
	int mUseDushFrame;
	int mUseDushCount;
	void Dush(float mag);


	//�}�[�L���O = Mark
	static constexpr int kMarkMaxLength = 1600;
	bool mIsMarkActive;
	bool mIsMarkLimitAnimationActive;
	Vec2 mMarkPosition;
	Vec2 mMarkUIPosition;
	Vec2 mMarkShake;
	Vec2 mMarkScale;
	int mMarkFrame;
	unsigned int mMarkColor;
	unsigned int mMarkScaleColor;
	float mMarkLength;
	float mMarkAnimationEasingt;
	float mMarkScaleEasingt;
	float mMarkShakeMag;
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
	void DrawStrikeUI(Screen& screen, bool isFever, unsigned int feverGaugeColor);
	//�t�B�[�o�[�X�g���C�N
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	float mStrikeTheta;
	float mStrikeThetaStartValue;
	float mStrikeRadius;
	float mStrikeRadiusStartValue;
	void Strike(bool isFever, bool isOldFever, Screen& screen);


	//�X���[
	bool mIsSlowActive;
	float mSlowMag;
	float mSlowEasingt;
	float mLeftStickMag;

	//�X�g���C�N���o
	static constexpr int kStrikeBoxMax = 10;
	bool mIsStrikeBoxActive;
	bool mIsStrikeBoxShakeActive;
	Vec2 mStrikeBoxPosition[kStrikeBoxMax];
	Vec2 mStrikeBoxStartPosition[kStrikeBoxMax];
	Vec2 mStrikeBoxEndPosition[kStrikeBoxMax];
	float mStrikeBoxAngle[kStrikeBoxMax];
	float mStrikeBoxEasingt;
	int mStrikeBoxSize;
	int mStrikeBoxStartSize;
	void StrikeBoxInit(Screen& screen);
	void StrikeBox();


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
	void Shadow(bool isHitStop, bool isStart, bool isReady);


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


	//�^�C�g����ʐ�p
	bool mIsControll;
	Vec2 mTargetPoint;
	Vec2 mDirectionPoint;
	float mSpeed;
	int mControllIntervalFrame;
	void AutoMove();
	void TitleUpdate();
	void TitleShadow();
	void TitleDraw(Screen& screen);


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


