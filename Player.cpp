#include "Player.h"
#include "Key.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Easing.h"
#include "Ingame.h"


void Player::Init() {
	//パラメータ
	mPosition.setZero();
	mVelocity.setZero();
	mSize = 64;
	mRadius = mSize / 2.0f;

	//通常移動
	mNormalMag = 6.0f;

	//ダッシュ
	mDushMag = 300.0f;

	//マーキング
	mIsMarkActive = false;
	mMarkFrame = 0;

	//ストライク
	mIsStrikeActive = false;
	mStrikeEasingt = 0.0f;

	//ストライク演出
	for (int i = 0; i < kStrikeLineMax; i++) {
		mIsStrikeLineActive[i] = false;
	}

	//残像
	mIsShadowActive = false;
}



void Player::Update(Screen& screen) {

	//前回ポジションを取得する
	mOldPosition = mPosition;

	//速度を初期化する
	mVelocity.setZero();

	//ストライクをしていない時に可能
	if (!mIsStrikeActive) {

		//通常移動
		if (!mKnockbackFlag) {
			NormalMove();
		}

		//ダッシュ
		Dush();

		//マーキング
		Mark();

		//速度を代入する
		mPosition += mVelocity;
	}

	//ストライク
	Strike();
	StrikeLine(screen);

	//マップ内に収める
	mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
	mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));

	//残像処理
	Shadow();

	Knockback();

		
}
void Player::NormalMove() {

	//スティックの方向を取得する
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//int型 の値を float型 に入れる（int型のままだと計算が０か１になる）
	mNormalVelocity.x = tmpX;
	mNormalVelocity.y = tmpY;

	//最大値を１にする
	mNormalVelocity = mNormalVelocity.Normalized();

	//ｎ倍する
	mNormalVelocity *= mNormalMag;

	//速度を代入する
	mVelocity += mNormalVelocity;

}
void Player::Dush() {

	//スティックの方向を取得する
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);
	
	//スティックを離してもダッシュが出来るようにする
	if (tmpX != 0 && tmpY != 0){

		//int型 の値を float型 に入れる（int型のままだと計算が０か１になる）
		mDushVelocity.x = tmpX;
		mDushVelocity.y = tmpY;

		//最大値を１にする
		mDushVelocity = mDushVelocity.Normalized();

		//ｎ倍する
		mDushVelocity *= mDushMag;
	}

	//Aボタン押下時
	if (Controller::IsTriggerButton(0,Controller::bA)){

		//速度を代入する
		mVelocity += mDushVelocity;
	}
}
void Player::Mark() {

	//Xボタン押下時にPositionを設定＆フラグをtrueにする
	if (Controller::IsTriggerButton(0,Controller::bX)){

		mMarkPosition = mPosition;
		mMarkFrame = 0;
		mIsMarkActive = true;

	}

	//制限時間でマーキングを消す
	if (mIsMarkActive){

		//フレームの加算
		mMarkFrame++;

		//制限時間を超えたらフラグをfalseにする
		if (kMarkTimeLimit <= mMarkFrame){
			mIsMarkActive = false;
		}
	}

	//初期化
	if (!mIsMarkActive){
		mMarkFrame = 0;
	}

}
void Player::Strike() {

	if (mIsMarkActive && !mIsStrikeActive){

		//LTボタン＆RTボタン同時押下時にフラグをtrueにする
		if (Controller::IsPressedButton(0, Controller::lTrigger) && Controller::IsPressedButton(0, Controller::rTrigger)) {

			//距離に応じてイージングの増加量を変化させる
			//マークとの距離を求める
			float tmpDistance = (mMarkPosition - mPosition).Length();

			//距離が0だったら中止する
			if (tmpDistance == 0){
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
			//距離が0じゃないとき
			else {
				//増加量を距離に応じて変えるための計算
				float tmpValue = tmpDistance / 100;

				// n / tmpValue のとき、nは距離が100のときのEasingtの増加量になる
				mStrikeEasingtIncrementValue = 0.25f / tmpValue;

				//イージング時の始点と終点の設定
				mStrikeStartPosition = mPosition;
				mStrikeEndPosition = mMarkPosition;

				//フラグをtrueにする
				mIsStrikeActive = true;
			}
		}
	}

	if (mIsStrikeActive){

		//イージング移動
		mStrikeEasingt = EasingClamp(mStrikeEasingtIncrementValue, mStrikeEasingt);
		mPosition = EasingMove(mStrikeStartPosition, mStrikeEndPosition, easeInSine(mStrikeEasingt));

		//移動が終了したら
		if (mStrikeEasingt == 1.0f){
			mIsMarkActive = false;
			mIsStrikeActive = false;
		}

	}

	//初期化
	if (!mIsStrikeActive){

		mStrikeEasingt = 0.0f;

	}

}
void Player::StrikeLine(Screen& screen) {

	if (mIsStrikeActive && 0.2f <= mStrikeEasingt && mStrikeEasingt <= 0.85f)
	{
		//基準線とストライクの方向を取得
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mStrikeEndPosition - mStrikeStartPosition;

		//方向を正規化してｎ倍する
		tmpDirection = tmpDirection.Normalized();
		tmpDirection = tmpDirection * (100 / screen.GetZoom());

		//なす角を求める
		float dp = tmpDirection.Dot(base);
		float cp = tmpDirection.Cross(base);
		mStrikeLineAngle = atan2(cp, dp);

		//生成位置の最少値と最大値
		float randminX = mPosition.x - ((float)Screen::kWindowWidth  / screen.GetZoom());
		float randmaxX = mPosition.x + ((float)Screen::kWindowWidth  / screen.GetZoom());
		float randminY = mPosition.y - ((float)Screen::kWindowHeight / screen.GetZoom());
		float randmaxY = mPosition.y + ((float)Screen::kWindowHeight / screen.GetZoom());

		for (int i = 0; i < kStrikeLineMax; i++)
		{
			//生成
			if (!mIsStrikeLineActive[i])
			{
				mStrikeLinePosition[i].x = RAND(randminX, randmaxX);
				mStrikeLinePosition[i].y = RAND(randminY, randmaxY);
				mStrikeLinePosition[i] += tmpDirection;
				mStrikeLineWidth[i] = RAND(200, 400);
				mStrikeLineHeight[i] = RAND(2.5f, 3.5f);
				mStrikeLineAlphat[i] = 0.0f;
				mStrikeLineColor[i] = 0x50505000;
				mIsStrikeLineActive[i] = true;
				break;
			}
		}
	}
	for (int i = 0; i < kStrikeLineMax; i++) 
	{
		if (mIsStrikeLineActive[i])
		{
			//透明度を変える
			mStrikeLineAlphat[i] = EasingClamp(0.015f, mStrikeLineAlphat[i]);
			if (mStrikeLineAlphat[i] < 0.5f) {
				mStrikeLineColor[i] = ColorEasingMove(0x50505000, 0x505050BB, easeLinear(mStrikeLineAlphat[i] * 2));
			}
			else {
				mStrikeLineColor[i] = ColorEasingMove(0x505050BB, 0x50505000, easeLinear(mStrikeLineAlphat[i] * 2 - 0.5f));
			}
			if (mStrikeLineAlphat[i] == 1.0f) {
				mIsStrikeLineActive[i] = false;
			}
		}
	}

}
void Player::Shadow() {

	//Aボタン押下時
	if (Controller::IsTriggerButton(0, Controller::bA)) {

		float distanceX = mPosition.x - mOldPosition.x;
		float distanceY = mPosition.y - mOldPosition.y;

		float intervalX = distanceX / kShadowMax;
		float intervalY = distanceY / kShadowMax;

		for (int i = 0; i < kShadowMax; i++) {

			mShadowPosition[i].x = mOldPosition.x + (intervalX * i);
			mShadowPosition[i].y = mOldPosition.y + (intervalY * i);

		}

		mShadowAlphat = 0.0f;
		mShadowColor = 0x60606060;
		mIsShadowActive = true;

	}

	if (mIsShadowActive){

		mShadowAlphat = EasingClamp(0.08f, mShadowAlphat);
		mShadowColor = ColorEasingMove(0x60606060, 0x60606000, easeLinear(mShadowAlphat));

		if (mShadowAlphat == 1.0f) {
			mIsShadowActive = false;
		}

	}

}

void Player::Knockback() {

	if (mKnockbackFlag == 1) {
		int A = mPosition.x - mKnockbackEnemyPos.x;
		int B = mPosition.y - mKnockbackEnemyPos.y;

		float length = sqrtf(A * A + B * B);

		float newA = A;
		float newB = B;

		if (length != 0) {
			newA = A / length;
			newB = B / length;
		}

		mKnockbackStart.x = mPosition.x;
		mKnockbackStart.y = mPosition.y;
		mKnockbackEnd.x = mPosition.x + newA * 300;
		mKnockbackEnd.y = mPosition.y + newB * 300;
		mKnockbackFlag = 2;

	} else if (mKnockbackFlag == 2) {
		if (mKnockBackT < 1) {
			mKnockBackT += 0.033;
			if (mKnockBackT > 1) {
				mKnockBackT = 1;
			}
			float easedT = easeOutSine(mKnockBackT);

			mPosition.x = ((1 - easedT) * mKnockbackStart.x) + (easedT * mKnockbackEnd.x);
			mPosition.y = ((1 - easedT) * mKnockbackStart.y) + (easedT * mKnockbackEnd.y);
		} else {
			mKnockbackFlag = 0;
			mKnockBackT = 0;
		}
	}
}

void Player::Draw(Screen& screen) {

	//残像描画
	for (int i = 0; i < kShadowMax; i++){

		screen.DrawSquare(mShadowPosition[i], mSize, mShadowColor);

	}

	//マーク描画
	if (mIsMarkActive){

		screen.DrawLine(mPosition, mMarkPosition, WHITE);
		screen.DrawSquare(mMarkPosition, mSize, WHITE);

	}

	//プレイヤー本体描画
	screen.DrawSquare(mPosition, mSize, 0x606060FF);

	//ストライク中の線描画
	for (int i = 0; i < kStrikeLineMax; i++)
	{
		screen.DrawRectAngle(mStrikeLinePosition[i], mStrikeLineWidth[i], mStrikeLineHeight[i], mStrikeLineAngle, mStrikeLineColor[i]);
	}

}
