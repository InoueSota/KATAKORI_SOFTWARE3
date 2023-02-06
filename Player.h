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


	//パラメータ
	Vec2 mPosition;
	Vec2 mOldPosition;
	Vec2 mVelocity;
	float mSizeValue;
	float mSize;
	float mRadius;
	unsigned int mColor;


	//通常移動 = NormalMove
	Vec2 mNormalVelocity;
	float mNormalMag;
	float mNormalMagLockon;
	void NormalMove();


	//ダッシュ = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	int mDushTimer = 0;
	static constexpr int kMaxDushTimer = 30;
	bool mIsUseDushOutOfRange;
	bool mIsOldUseDushOutOfRange;
	int mUseDushFrame;
	int mUseDushCount;
	void Dush(float mag);


	//マーキング = Mark
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


	//ストライク = Strike
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
	//フィーバーストライク
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	float mStrikeTheta;
	float mStrikeThetaStartValue;
	float mStrikeRadius;
	float mStrikeRadiusStartValue;
	void Strike(bool isFever, bool isOldFever, Screen& screen);


	//スロー
	bool mIsSlowActive;
	float mSlowMag;
	float mSlowEasingt;
	float mLeftStickMag;

	//ストライク演出
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


	//残像
	int mShadowFrame;
	static constexpr int kShadowMax = 10;
	bool mIsShadowActive[kShadowMax];
	Vec2 mShadowPosition[kShadowMax];
	float mShadowAlphat[kShadowMax];
	unsigned int mShadowColor[kShadowMax];
	//ダッシュ残像
	bool mIsDushShadowActive;
	Vec2 mDushShadowPosition[4];
	float mDushShadowAlphat;
	unsigned int mDushShadowColor;
	void Shadow(bool isHitStop, bool isStart, bool isReady);


	//ノックバック
	int LockonCount;//何体の敵に補足されているか
	bool mKnockbackSet;
	bool mKnockbackActive;
	Vec2 mKnockbackEnemyPos;
	Vec2 mKnockbackStart;
	Vec2 mKnockbackEnd;
	float mKnockBackT;
	void Knockback();
	void SetKnockbackPosition(Vec2 enemyPosition, float enemyRadius);


	//タイトル画面専用
	void TitleUpdate();
	void TitleShadow();
	void TitleDraw(Screen& screen);


	//画像
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


