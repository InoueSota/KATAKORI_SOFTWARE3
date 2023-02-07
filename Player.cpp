#include "Player.h"
#include "Key.h"
#include "ControllerInput.h"
#include "Collision.h"
#include "Function.h"
#include "Easing.h"
#include "Ingame.h"



void Player::Init() {
	//パラメータ
	mPosition.setZero();
	mVelocity.setZero();
	mSizeValue = 100;
	mSize = mSizeValue;
	mRadius = mSize / 2.0f;
	mColor = WHITE;

	//通常移動
	mNormalMag = 5.0f;
	mNormalMagLockon = 12.0f;

	//ダッシュ
	mDushMag = 300.0f;
	mIsUseDushOutOfRange = false;
	mUseDushFrame = 0;
	mUseDushCount = 0;

	//ダッシュ演出
	for (int i = 0; i < kDushBoxMax; i++) {
		mIsDushBoxActive[i] = false;
	}

	//マーキング
	mIsMarkActive = false;
	mMarkFrame = 0;
	mMarkLength = 0.0f;
	mMarkScale.setZero();
	mMarkUIPosition = { 50.0f,100.0f };
	mMarkShake.setZero();
	mMarkShakeMag = 1.0f;
	mMarkAnimationEasingt = 0.0f;
	mMarkScaleEasingt = 0.0f;
	mMarkScaleColor = 0xFFFFFF80;
	mIsMarkLimitAnimationActive = false;

	//ストライク
	mIsStrikeActive = false;
	mIsStrikeBoxShakeActive = false;
	mStrikePower = 0;
	mStrikeModeScale = { 1.0f, 1.0f };
	mStrikeModeBScale = { 1.0f, 1.0f };
	mStrikeModeScaleActive = false;
	strikeMode = STRAIGHT;
	mStrikeTutorial = 0;

	//スロー
	mIsSlowActive = false;
	mSlowMag = 1.0f;
	mSlowEasingt = 0.0f;
	mLeftStickMag = 1.0f;

	mDushTimer = 0;

	//ストライク演出
	mIsStrikeBoxActive = false;

	//残像
	mIsDushShadowActive = false;
	for (int i = 0; i < kShadowMax; i++) {
		mIsShadowActive[i] = false;
	}

	//タイトル画面専用
	mIsControll = false;
	mTargetPoint.x = RAND(100, Screen::kWindowWidth - 100);
	mTargetPoint.y = RAND(100, Screen::kWindowHeight - 100);
	mDirectionPoint.setZero();
	mSpeed = 5.0f;
	mControllIntervalFrame = 0;

	//ノックバック
	mKnockbackSet = false;
	mKnockbackActive = false;
}


void Player::Update(Screen& screen, bool isFever, bool isOldFever, unsigned int BackBoxColor) {

	//前回ポジションを取得する
	mOldPosition = mPosition;

	//前回ストライクフラグを取得する
	mIsOldStrikeActive = mIsStrikeActive;

	//シェイクフラグを初期化する
	mIsStrikeBoxShakeActive = false;

	//速度を初期化する
	mVelocity.setZero();

	//大きさをズームにしたがって変える
	if (!isFever && isOldFever) {
		mSizeValue = 100;
	}
	mSizeValue = Clamp(mSizeValue, 0.0f, 200.0f);
	if (!isFever) {
		mSize += (mSizeValue - mSize) * 0.5f;
	}
	else if (isFever) {
		mSize = 100 / screen.GetZoom() * 0.4f;
	}
	mRadius = mSize / 2.0f;

	//ストライク
	Strike(isFever, isOldFever, screen);
	StrikeBox();

	//ストライクをしていない時に可能
	if (!mIsStrikeActive) {

		//通常移動
		if (!mKnockbackActive) {
			NormalMove();
		}
		
		//ダッシュ
		Dush(1.0f);

		//マーキング
		Mark();

		//速度を代入する
		mPosition += (mVelocity / screen.GetZoom() * 0.4f);

		//マップ内に収める
		mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
		mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));

		//ダッシュ演出
		DushBox(BackBoxColor);

		//マークとの距離に制限をかける
		MarkLimitLength(isFever);
	}
	//if (Controller::IsTriggerButton(0, Controller::bA)) {
	//	//ダッシュ判定のタイマーセット
	//	mDushTimer = kMaxDushTimer;
	//}
	if (mDushTimer > 0) {
		mDushTimer--;
	}
		
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
	if (LockonCount) {
		mNormalVelocity *= (mNormalMagLockon + (LockonCount * 1));
	} else {
		mNormalVelocity *= mNormalMag;
	}

	//速度を代入する
	mVelocity += mNormalVelocity;

}
void Player::Dush(float mag) {

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
		mDushVelocity *= mDushMag * mag;
	}

	//Aボタン押下時
	if (Controller::IsTriggerButton(0,Controller::bA)){

		//速度を代入する
		mVelocity += mDushVelocity;
		
	}
}
void Player::DushBox(unsigned int BackBoxColor) {

	//Aボタン押下時
	if (Controller::IsTriggerButton(0, Controller::bA)) {

		//ダッシュ演出フラグ
		for (int i = 0; i < kDushBoxMax; i++) {
			if (!mIsDushBoxActive[i]) {
				mDushBoxPosition[i] = mPosition;
				mDushBoxEasingt[i] = 0.0f;
				mIsDushBoxActive[i] = true;
				break;
			}
		}
	}

	//ダッシュ演出処理
	for (int i = 0; i < kDushBoxMax; i++) {
		if (mIsDushBoxActive[i]) {
			mDushBoxEasingt[i] = EasingClamp(0.02f, mDushBoxEasingt[i]);
			mDushBoxAngle[i] = EasingMove(Degree(0), Degree(135), easeLinear(mDushBoxEasingt[i]));
			mDushBoxSize[i] = EasingMove(0, 400, easeOutSine(mDushBoxEasingt[i]));
			mDushBoxColor[i] = ColorEasingMove(BackBoxColor, BackBoxColor ^ 0x000000FF, easeInSine(mDushBoxEasingt[i]));
			if (mDushBoxEasingt[i] == 1.0f) {
				mIsDushBoxActive[i] = false;
			}
		}
	}
}
void Player::Mark() {

	//Xボタン押下時にPositionを設定＆フラグをtrueにする
	if (Controller::IsTriggerButton(0, Controller::bX)) {

		mMarkPosition = mPosition;
		mMarkFrame = 0;
		mMarkScaleEasingt = 0.0f;
		mIsMarkActive = true;

	}
	//RBのアニメーションに使うフレームを更新する
	if (mIsMarkActive){
		mMarkFrame++;

		//マークのアニメーション
		mMarkScaleEasingt = EasingClamp(0.02f, mMarkScaleEasingt);
		mMarkScaleColor = ColorEasingMove(0xFFFFFFA0, 0xFFFFFF00, easeOutCirc(mMarkScaleEasingt));
		mMarkScale = EasingMove({ 1.0f,1.0f }, { 5.0f, 5.0f }, easeOutSine(mMarkScaleEasingt));
		if (mMarkScaleEasingt == 1.0f) {
			mMarkScaleEasingt = 0.0f;
		}
	}
	//初期化
	if (!mIsMarkActive){
		mMarkFrame = 0;
	}

}
void Player::MarkLimitLength(bool isFever) {

	mIsOldUseDushOutOfRange = mIsUseDushOutOfRange;

	if (mIsMarkActive && !isFever) {

		if (!Collision(mPosition, 0, mMarkPosition, kMarkMaxLength)) {
			if (Controller::IsTriggerButton(0, Controller::bA)) {
				mUseDushCount++;
				mIsUseDushOutOfRange = true;
			}
			Vec2 tmpDirection = (mPosition - mMarkPosition).Normalized();
			mPosition = mMarkPosition + tmpDirection * kMarkMaxLength;
		}
	}

	//範囲外にダッシュを続けたらマークを切る
	if (mIsUseDushOutOfRange) {
		mUseDushFrame++;
	}
	//ダッシュを利用して範囲外に出ようとした瞬間、フレームを初期化する
	if (!mIsOldUseDushOutOfRange && mIsUseDushOutOfRange) {
		mUseDushFrame = 0;
	}
	//３回以上ダッシュで抜けようとしたらマークを切る
	if (3 <= mUseDushCount) {
		mUseDushCount = 0;
		mUseDushFrame = 0;
		mIsMarkActive = false;
		mIsUseDushOutOfRange = false;
		mIsOldUseDushOutOfRange = false;
	}
	//抜けるのに６０フレームかかったらカウントを初期化する
	if (60 <= mUseDushFrame) {
		mUseDushCount = 0;
		mIsUseDushOutOfRange = false;
		mIsOldUseDushOutOfRange = false;
	}

	//距離を求める
	mMarkLength = (mPosition - mMarkPosition).Length();
	mMarkLength = Clamp(mMarkLength, 0, kMarkMaxLength);

	//マークの限界距離が最大に達したら赤色にする
	if (kMarkMaxLength - 1 <= mMarkLength) {

		if (!mIsMarkLimitAnimationActive) {
			mMarkAnimationEasingt = 0.0f;
			mMarkShakeMag = 1.0f;
			mIsMarkLimitAnimationActive = true;
		}
		mMarkColor = RED;

	} else {

		mMarkColor = WHITE;

		if (mMarkAnimationEasingt == 1.0f && mIsMarkLimitAnimationActive) {
			mIsMarkLimitAnimationActive = false;
		}
	}
	//マークが限界距離に達したら、UIをシェイクする
	if (mIsMarkLimitAnimationActive) {

		mMarkAnimationEasingt = EasingClamp(0.033f, mMarkAnimationEasingt);
		mMarkShakeMag = EasingMove(1.0f, 0.0f, easeOutSine(mMarkAnimationEasingt));

		mMarkShake.x = RAND(-3.0f, 5.0f) * mMarkShakeMag;
		mMarkShake.y = RAND(-3.0f, 5.0f) * mMarkShakeMag;
	}

}
void Player::Strike(bool isFever, bool isOldFever, Screen& screen) {

	//フィーバー状態が始まった || 終わった
	if ((isFever && !isOldFever) || (!isFever && isOldFever)) {
		mIsMarkActive = false;
		mIsStrikeActive = false;
	}

	//フィーバー中はパワーマックス
	if (isFever) {
		mStrikePower = kStrikePowerMax;
	}

	if (mIsMarkActive && !mIsStrikeActive) {

		//マークをつけたあとパワーがある時にフラグをtrueにする
		if (Controller::IsTriggerButton(0, Controller::rSHOULDER) && 0 < mStrikePower) {

			//距離に応じてイージングの増加量を変化させる
			//マークとの距離を求める
			float tmpDistance = (mMarkPosition - mPosition).Length();

			//距離が0だったら中止する
			if (tmpDistance == 0) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
			//距離が0じゃないとき
			else {

				//ストライクパワーを消費する
				mStrikePower--;

				//増加量を距離に応じて変えるための計算
				float tmpValue = tmpDistance / 100;

				float tmpTheta = BetweenTheta(mMarkPosition - mPosition);

				if (strikeMode == STRAIGHT)
				{
					mStrikeSpeed = 15.0f;
					mStraightStrikeTheta = tmpTheta;
					mIsStraightStrikeFinish = false;
					mIsStraightStrikeActive = true;
				}
				else
				{
					mStrikeEasingt = 0.0f;
					mStrikeThetaStartValue = tmpTheta;
					mStrikeRadiusStartValue = tmpDistance;
					mIsStraightStrikeActive = false;
				}

				//フラグをtrueにする
				mIsStrikeActive = true;
			}
		}
	}

	if (mStrikeTutorial) {
		mStrikeTutorial = 0;
	}

	if (mIsStrikeActive) {

		if (mIsStraightStrikeActive)
		{
			mStraightStrikeOldTheta = mStraightStrikeTheta;

			mStrikeDirection = (mMarkPosition - mPosition).Normalized();

			//Aを押したらスローかける
			if (Controller::IsTriggerButton(0,Controller::bA) && !mIsSlowActive) {
				mLeftStickMag = 0.5f;
				mSlowEasingt = 0.0f;
				mStrikeSpeed = 0.0f;
				mIsSlowActive = true;
			}

			if (mIsSlowActive) {
				mSlowEasingt = EasingClamp(0.006f, mSlowEasingt);
				mSlowMag = EasingMove(0.2f, 1.0f, easeInSine(mSlowEasingt));
				mLeftStickMag = EasingMove(0.6f, 1.0f, easeInSine(mSlowEasingt));
			}

			mStrikeVelocity.setZero();
			mStrikeVelocity = mStrikeDirection * (mStrikeSpeed * mSlowMag) + LeftStickVelocity(30.0f * mLeftStickMag);
			mStrikeSpeed += 0.8f;

			mPosition += mStrikeVelocity;

			mStraightStrikeTheta = BetweenTheta(mMarkPosition - mPosition);

			if (Radian(mStraightStrikeTheta) < 10 && 350 < Radian(mStraightStrikeOldTheta)) {
				mStraightStrikeOldTheta -= Degree(360);
			}
			else if (Radian(mStraightStrikeOldTheta) < 10 && 350 < Radian(mStraightStrikeTheta)) {
				mStraightStrikeOldTheta += Degree(360);
			}

			if (170.0f > Radian(abs(mStraightStrikeTheta - mStraightStrikeOldTheta))) {
				if (0 <= Radian(mStraightStrikeTheta) && Radian(mStraightStrikeTheta) <= 90) {
					mStrikeClampMin.x = Map::kMapLeft;
					mStrikeClampMin.y = mMarkPosition.y;
					mStrikeClampMax.x = mMarkPosition.x;
					mStrikeClampMax.y = Map::kMapTop;
				}
				else if (90 < Radian(mStraightStrikeTheta) && Radian(mStraightStrikeTheta) <= 180) {
					mStrikeClampMin.x = mMarkPosition.x;
					mStrikeClampMin.y = mMarkPosition.y;
					mStrikeClampMax.x = Map::kMapRight;
					mStrikeClampMax.y = Map::kMapTop;
				}
				else if (180 < Radian(mStraightStrikeTheta) && Radian(mStraightStrikeTheta) <= 270) {
					mStrikeClampMin.x = mMarkPosition.x;
					mStrikeClampMin.y = Map::kMapBottom;
					mStrikeClampMax.x = Map::kMapRight;
					mStrikeClampMax.y = mMarkPosition.y;
				}
				else if (270 < Radian(mStraightStrikeTheta)) {
					mStrikeClampMin.x = Map::kMapLeft;
					mStrikeClampMin.y = Map::kMapBottom;
					mStrikeClampMax.x = mMarkPosition.x;
					mStrikeClampMax.y = mMarkPosition.y;
				}
			}
			else {
				mIsStraightStrikeFinish = true;
			}

			mPosition.x = Clamp(mPosition.x, mStrikeClampMin.x, mStrikeClampMax.x);
			mPosition.y = Clamp(mPosition.y, mStrikeClampMin.y, mStrikeClampMax.y);

			//移動が終了したら
			if ((mPosition.x == mMarkPosition.x && mPosition.y == mMarkPosition.y) || mIsStraightStrikeFinish) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
				mSlowMag = 1.0f;
				mLeftStickMag = 1.0f;
				mSlowEasingt = 0.0f;
				mIsSlowActive = false;
				StrikeBoxInit(screen);
				mStrikeTutorial = 1;
			}
		}
		else {
			//イージング値加算
			mStrikeEasingt = EasingClamp(0.015f, mStrikeEasingt);

			mStrikeTheta = EasingMove(mStrikeThetaStartValue, mStrikeThetaStartValue + Degree(360), easeLinear(mStrikeEasingt));
			mStrikeRadius = EasingMove(mStrikeRadiusStartValue, 0.0f, easeLinear(mStrikeEasingt));

			mPosition.x = mStrikeRadius * -cosf(mStrikeTheta) + mMarkPosition.x;
			mPosition.y = mStrikeRadius * sinf(mStrikeTheta) + mMarkPosition.y;

			//移動が終了したら
			if (mStrikeEasingt == 1.0f) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
		}

	}
}
void Player::StrikeBoxInit(Screen& screen) {

	//初期化
	for (int i = 0; i < kStrikeBoxMax; i++) {

		float tmpLength = 150 - RAND(0, 2) * 20;
		mStrikeBoxAngle[i] = RAND(Degree(0), Degree(360));
		mStrikeBoxStartPosition[i].x = mMarkPosition.x;
		mStrikeBoxStartPosition[i].y = mMarkPosition.y;
		mStrikeBoxEndPosition[i].x = cosf(mStrikeBoxAngle[i]) * (tmpLength / screen.GetZoom()) + mStrikeBoxStartPosition[i].x;
		mStrikeBoxEndPosition[i].y = sinf(mStrikeBoxAngle[i]) * (tmpLength / screen.GetZoom()) + mStrikeBoxStartPosition[i].y;
		mStrikeBoxStartSize = 40 / screen.GetZoom();
	}
	mStrikeBoxEasingt = 0.0f;
	mIsStrikeBoxActive = true;
	mIsStrikeBoxShakeActive = true;
}
void Player::StrikeBox() {

	//ストライクが終わった時のアニメーション
	if (mIsStrikeBoxActive) {

		mStrikeBoxEasingt = EasingClamp(0.02f, mStrikeBoxEasingt);
		for (int i = 0; i < kStrikeBoxMax; i++) {
			mStrikeBoxPosition[i] = EasingMove(mStrikeBoxStartPosition[i], mStrikeBoxEndPosition[i], easeOutSine(mStrikeBoxEasingt));
		}
		mStrikeBoxSize = EasingMove(mStrikeBoxStartSize, 0.0, easeOutSine(mStrikeBoxEasingt));

		if (mStrikeBoxEasingt == 1.0f) {
			mIsStrikeBoxActive = false;
		}
	}

}
void Player::Shadow(bool isHitStop, bool isStart, bool isReady) {

	mShadowFrame++;

	for (int i = 0; i < kShadowMax; i++)
	{
		if (!mIsShadowActive[i] && mShadowFrame % 3 == 0 && !isHitStop) {
			mShadowPosition[i] = mPosition;
			mShadowAlphat[i] = 0.0f;
			mShadowColor[i] = 0x606060A0;
			mIsShadowActive[i] = true;
			break;
		}

		if (mIsShadowActive[i]) {
			mShadowAlphat[i] = EasingClamp(0.05f, mShadowAlphat[i]);
			mShadowColor[i] = ColorEasingMove(0x606060A0, 0x60606000, easeLinear(mShadowAlphat[i]));

			if (mShadowAlphat[i] == 1.0f) {
				mIsShadowActive[i] = false;
			}
		}
	}

	//Aボタン押下時
	if (Controller::IsTriggerButton(0, Controller::bA) && !mKnockbackActive && (isStart || !isReady)) {

		float distanceX = mPosition.x - mOldPosition.x;
		float distanceY = mPosition.y - mOldPosition.y;

		float intervalX = distanceX / 4;
		float intervalY = distanceY / 4;

		for (int i = 0; i < 4; i++) {

			mDushShadowPosition[i].x = mOldPosition.x + (intervalX * i);
			mDushShadowPosition[i].y = mOldPosition.y + (intervalY * i);
		}

		mDushShadowAlphat = 0.0f;
		mDushShadowColor = 0x60606060;
		mIsDushShadowActive = true;

		for (int i = 0; i < kShadowMax; i++) {

			mShadowAlphat[i] = 0.0f;
			mShadowColor[i] = 0x60606000;
			mIsShadowActive[i] = false;
		}
	}

	if (mIsDushShadowActive) {

		mDushShadowAlphat = EasingClamp(0.05f, mDushShadowAlphat);
		mDushShadowColor = ColorEasingMove(0x60606060, 0x60606000, easeLinear(mDushShadowAlphat));

		if (mDushShadowAlphat == 1.0f) {
			mIsDushShadowActive = false;
		}
	}

	//ゲームに入ったら影を初期化する
	if (isReady && !isStart) {
		mIsDushShadowActive = false;
		for (int i = 0; i < kShadowMax; i++) {
			mIsShadowActive[i] = false;
		}
	}

}
void Player::SetKnockbackPosition(Vec2 enemyPosition, float enemyRadius) {

	if (!mKnockbackActive && !mKnockbackSet) {
		Vec2 tmpDirection = (mOldPosition - enemyPosition).Normalized();
		mPosition = enemyPosition + tmpDirection * enemyRadius;
		mKnockbackEnemyPos = enemyPosition;
		mKnockbackSet = true;
	}
}
void Player::Knockback() {

	if (mKnockbackSet && !mKnockbackActive) {

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
		mKnockbackEnd.x = mPosition.x + newA * 500;
		mKnockbackEnd.y = mPosition.y + newB * 500;
		mKnockbackEnd.x = mPosition.x + newA * 600;
		mKnockbackEnd.y = mPosition.y + newB * 600;
		mKnockbackActive = true;
		mKnockbackSet = false;
		mIsStrikeActive = false;
		mIsMarkActive = false;
	}
	if (mKnockbackActive) {
		mColor = 0xFFFFFF50;
		mKnockBackT = EasingClamp(0.033f, mKnockBackT);
		mPosition = EasingMove(mKnockbackStart, mKnockbackEnd, easeOutSine(mKnockBackT));
		if (mKnockBackT >= 0.7f) {
			mKnockBackT = 0.0f;
			mColor = WHITE;
			mKnockbackActive = false;
		}
	}
}


void Player::Draw(Screen& screen, bool isReady, bool isFever, unsigned int feverGaugeColor, unsigned int backLineColor) {

	//残像描画
	for (int i = 0; i < kShadowMax; i++) {
		if (mIsShadowActive[i]) {
			screen.DrawPicture(mShadowPosition[i], mSize, 0, 100, 100, toge, mShadowColor[i]);
		}
	}

	if (mIsDushShadowActive) {
		for (int i = 0; i < 4; i++) {
			screen.DrawPicture(mDushShadowPosition[i], mSize, 0, 100, 100, toge, mDushShadowColor);
		}
	}

	//ストライク演出
	if (mIsStrikeBoxActive) {

		for (int i = 0; i < kStrikeBoxMax; i++) {
			screen.DrawRectAngle(mStrikeBoxPosition[i], mStrikeBoxSize, mStrikeBoxSize, mStrikeBoxAngle[i], backLineColor, kFillModeWireFrame, true);
		}
	}

	//ダッシュ演出
	for (int i = 0; i < kDushBoxMax; i++) {
		if (mIsDushBoxActive[i]) {
			screen.DrawRectAngle(mDushBoxPosition[i], mDushBoxSize[i], mDushBoxSize[i], mDushBoxAngle[i], mDushBoxColor[i], kFillModeWireFrame, true);
		}
	}
	
	//マーク描画
	if (mIsMarkActive){

		if (!isFever) {
			screen.DrawPicture((mPosition - mMarkPosition) / 2.0f + mMarkPosition, (mPosition - mMarkPosition).Length(), 10, BetweenTheta(mMarkPosition - mPosition), 0, 0, 192, mMarkColor);
		} else {
			screen.DrawPicture((mPosition - mMarkPosition) / 2.0f + mMarkPosition, (mPosition - mMarkPosition).Length(), 10, BetweenTheta(mMarkPosition - mPosition), 0, 0, 192, feverGaugeColor);
		}
		screen.DrawPicture(mMarkPosition, mSize, 0, 100, 100, mark, mMarkScaleColor, mMarkScale);
		screen.DrawPicture(mMarkPosition, mSize, 0, 100, 100, mark, WHITE);
	}

	//プレイヤー本体描画
	screen.DrawPicture(mPosition, mSize, 0, 100, 100, toge, mColor);

	//ストライクしろ(圧)描画
	if (mIsMarkActive && !mIsStrikeActive) {
		if (0 < mStrikePower) {
			screen.DrawAnime({ mPosition.x, mPosition.y + (40 / screen.GetZoom()) }, 60 / screen.GetZoom(), 30 / screen.GetZoom(), 0.0f, 0, 200, 100, 2, 20, mMarkFrame, rb, 0xFFFFFFE5);
		} else {
			screen.DrawPicture({ mPosition.x, mPosition.y + (40 / screen.GetZoom()) }, 360, 120, 0.0f, 300, 100, nopower, WHITE);
		}
	}

}
void Player::DrawStrikeUI(Screen& screen, bool isFever, unsigned int feverGaugeColor) {

	//ストライクパワーを収める
	mStrikePower = Clamp(mStrikePower, 0, kStrikePowerMax);
	if (!isFever) {
		screen.DrawBox({ 50.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, WHITE, kFillModeSolid, false);
	} else {
		screen.DrawBox({ 50.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, feverGaugeColor, kFillModeSolid, false);
	}

	for (int i = 0; i < kStrikePowerMax; i++) {
		if (i == kStrikePowerMax - 1) {
			screen.DrawUI({ 25.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, lastflame, WHITE);
		} else {
			screen.DrawUI({ 25.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, flame, WHITE);
		}
	}

	//マークの長さ
	if (!isFever && mIsMarkActive) {
		screen.DrawBox(mMarkUIPosition + mMarkShake, mMarkLength * (250.0f / kMarkMaxLength), 25, 0.0f, mMarkColor, kFillModeSolid, false);
	} else if (isFever) {
		screen.DrawBox(mMarkUIPosition + mMarkShake, 250.0f, 25, 0.0f, feverGaugeColor, kFillModeSolid, false);
	}
	screen.DrawUI({ 175.0f + mMarkShake.x, 112.5f + mMarkShake.y }, 250, 25, 0, 1000, 100, lengthflame, WHITE);

}
void Player::LoadTexture() {

	flame = Novice::LoadTexture("./Resources/Player/flame.png");
	lastflame = Novice::LoadTexture("./Resources/Player/lastflame.png");
	lengthflame = Novice::LoadTexture("./Resources/Player/lengthflame.png");
	circle = Novice::LoadTexture("./Resources/Player/circle.png");
	straight = Novice::LoadTexture("./Resources/Player/straight.png");
	spiral = Novice::LoadTexture("./Resources/Player/spiral.png");
	b = Novice::LoadTexture("./Resources/Player/b.png");
	rb = Novice::LoadTexture("./Resources/Player/rb.png");
	nopower = Novice::LoadTexture("./Resources/Player/nopower.png");
	toge = Novice::LoadTexture("./Resources/Player/toge.png");
	mark = Novice::LoadTexture("./Resources/Player/mark.png");}


void Player::TitleUpdate(unsigned int BackBoxColor) {

	//前回ポジションを取得する
	mOldPosition = mPosition;

	//速度を初期化する
	mVelocity.setZero();

	if (Controller::IsTriggerButton(0,Controller::bA) || 0.0f < LeftStickVelocity(3.0f).x || 0.0f < LeftStickVelocity(3.0f).y) {
		mControllIntervalFrame = 0;
		mIsControll = true;
	} else if (mIsControll) {
		mControllIntervalFrame++;
		if (120 <= mControllIntervalFrame) {
			mControllIntervalFrame = 0;
			mIsControll = false;
		}
	}

	if (mIsControll) {
		//通常移動
		NormalMove();

		//ダッシュ
		Dush(0.5f);

	} else {

		AutoMove();
	}


	//速度を代入する
	mPosition.x += mVelocity.x;
	mPosition.y -= mVelocity.y;

	//画面外に出さない
	mPosition.x = Clamp(mPosition.x, 0, Screen::kWindowWidth);
	mPosition.y = Clamp(mPosition.y, 0, Screen::kWindowHeight);

	//ダッシュ演出
	DushBox(BackBoxColor);

	//残像
	TitleShadow();
}
void Player::AutoMove() {

	//徐々に向きを変える
	mDirectionPoint += (mTargetPoint - mPosition) * 0.0002f;

	if (Collision(mPosition, mSize / 2.0, mTargetPoint, 50.0f)) {
		mTargetPoint.x = RAND(100, Screen::kWindowWidth - 100);
		mTargetPoint.y = RAND(100, Screen::kWindowHeight - 100);
	}
	else {
		mDirectionPoint = mDirectionPoint.Normalized();
		mVelocity.x += mDirectionPoint.x * mSpeed;
		mVelocity.y -= mDirectionPoint.y * mSpeed;
	}

}
void Player::TitleShadow() {

	mShadowFrame++;

	for (int i = 0; i < kShadowMax; i++)
	{
		if (!mIsShadowActive[i] && mShadowFrame % 3 == 0) {
			mShadowPosition[i] = mPosition;
			mShadowAlphat[i] = 0.0f;
			mShadowColor[i] = 0x606060A0;
			mIsShadowActive[i] = true;
			break;
		}

		if (mIsShadowActive[i]) {
			mShadowAlphat[i] = EasingClamp(0.05f, mShadowAlphat[i]);
			mShadowColor[i] = ColorEasingMove(0x606060A0, 0x60606000, easeLinear(mShadowAlphat[i]));

			if (mShadowAlphat[i] == 1.0f) {
				mIsShadowActive[i] = false;
			}
		}
	}

	//Aボタン押下時
	if (Controller::IsTriggerButton(0, Controller::bA)) {

		float distanceX = mPosition.x - mOldPosition.x;
		float distanceY = mPosition.y - mOldPosition.y;

		float intervalX = distanceX / 4;
		float intervalY = distanceY / 4;

		for (int i = 0; i < 4; i++) {

			mDushShadowPosition[i].x = mOldPosition.x + (intervalX * i);
			mDushShadowPosition[i].y = mOldPosition.y + (intervalY * i);
		}

		mDushShadowAlphat = 0.0f;
		mDushShadowColor = 0x60606060;
		mIsDushShadowActive = true;

		for (int i = 0; i < kShadowMax; i++) {

			mShadowAlphat[i] = 0.0f;
			mShadowColor[i] = 0x60606000;
			mIsShadowActive[i] = false;
		}
	}

	if (mIsDushShadowActive) {

		mDushShadowAlphat = EasingClamp(0.05f, mDushShadowAlphat);
		mDushShadowColor = ColorEasingMove(0x60606060, 0x60606000, easeLinear(mDushShadowAlphat));

		if (mDushShadowAlphat == 1.0f) {
			mIsDushShadowActive = false;
		}
	}
}
void Player::TitleDraw(Screen& screen) {

	//残像描画
	for (int i = 0; i < kShadowMax; i++) {
		if (mIsShadowActive[i]) {
			screen.DrawPicture(mShadowPosition[i], mSize / 2.0, 0, 100, 100, toge, mShadowColor[i], { 1.0f, 1.0f }, false);
		}
	}
	if (mIsDushShadowActive) {
		for (int i = 0; i < 4; i++) {
			screen.DrawPicture(mDushShadowPosition[i], mSize / 2.0, 0, 100, 100, toge, mDushShadowColor, { 1.0f, 1.0f }, false);
		}
	}

	//ダッシュ演出
	for (int i = 0; i < kDushBoxMax; i++) {
		if (mIsDushBoxActive[i]) {
			screen.DrawRectAngle(mDushBoxPosition[i], mDushBoxSize[i] / 2.0, mDushBoxSize[i] / 2.0, mDushBoxAngle[i], mDushBoxColor[i], kFillModeWireFrame, false);
		}
	}

	//プレイヤー本体描画
	screen.DrawPicture(mPosition, mSize / 2.0, 0, 100, 100, toge, mColor, { 1.0f, 1.0f }, false);

}
