#include "Player.h"
#include "Key.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Easing.h"


void Player::Init() {
	//パラメータ
	mPosition.setZero();
	mVelocity.setZero();
	mSize = 64;

	//通常移動
	mNormalMag = 3.0f;

	//ダッシュ
	mDushMag = 200.0f;

	//マーキング
	mIsMarkActive = false;
	mMarkFrame = 0;

	//ストライク
	mIsStrikeActive = false;
	mStrikeEasingt = 0.0f;
}



void Player::Update() {

	//速度を初期化する
	mVelocity.setZero();

	//ストライクをしていない時に可能
	if (!mIsStrikeActive) {

		//通常移動
		NormalMove();

		//ダッシュ
		Dush();

		//マーキング
		Mark();
	}

	//ストライク
	Strike();

	//速度を代入する
	mPosition += mVelocity;

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
	if (!mIsMarkActive && Controller::IsTriggerButton(0,Controller::bX)){

		mMarkPosition = mPosition;
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
				mStrikeEasingtIncrementValue = 0.15f / tmpValue;

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



void Player::Draw(Screen& screen) {

	if (mIsMarkActive){

		screen.DrawLine(mPosition, mMarkPosition, WHITE);
		screen.DrawSquare(mMarkPosition, mSize, WHITE);

	}

	screen.DrawSquare(mPosition, mSize, 0x606060FF);

}
