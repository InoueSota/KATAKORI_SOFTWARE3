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
	void Draw(Screen& screen);


	//パラメータ
	Vec2 mPosition;
	Vec2 mOldPosition;
	Vec2 mVelocity;
	float mSizeValue;
	float mSize;
	float mRadius;


	//通常移動 = NormalMove
	Vec2 mNormalVelocity;
	float mNormalMag;
	void NormalMove();


	//ダッシュ = Dush
	Vec2 mDushVelocity;
	float mDushMag;
	void Dush();


	//マーキング = Mark
	bool mIsMarkActive;
	Vec2 mMarkPosition;
	static constexpr int kMarkTimeLimit = 240;
	int mMarkFrame;
	void Mark();


	//ストライク = Strike
	enum StrikeMode {
		STRAIGHT,
		SPIRAL
	};
	StrikeMode strikeMode = STRAIGHT;
	bool mIsStrikeActive;
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
	//フィーバーストライク
	float mStrikeEasingt;
	float mStrikeEasingtIncrementValue;
	float mStrikeTheta;
	float mStrikeThetaStartValue;
	float mStrikeRadius;
	float mStrikeRadiusStartValue;
	void Strike(bool isFever, bool isOldFever);


	//ストライク演出
	static constexpr int kStrikeLineMax = 200;
	bool mIsStrikeLineActive[kStrikeLineMax];
	Vec2 mStrikeLinePosition[kStrikeLineMax];
	int mStrikeLineWidth[kStrikeLineMax];
	int mStrikeLineHeight[kStrikeLineMax];
	float mStrikeLineAngle;
	float mStrikeLineAlphat[kStrikeLineMax];
	unsigned int mStrikeLineColor[kStrikeLineMax];
	void StrikeLine(Screen& screen);


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
	void Shadow(bool isHitStop);


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


	//画像
	int flame;
	int lastflame;
	int circle;
	int straight;
	int spiral;
	int b;
	int lb;
	int rb;
	int nopower;
	bool mIsLoadTexture = false;
	void LoadTexture();

};


