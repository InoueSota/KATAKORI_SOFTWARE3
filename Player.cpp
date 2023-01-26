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
	mSizeValue = 80;
	mSize = mSizeValue;
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
	mStrikePower = 0;
	mStrikeModeScale = { 1.0f, 1.0f };
	mStrikeModeBScale = { 1.0f, 1.0f };
	mStrikeModeScaleActive = false;
	strikeMode = STRAIGHT;

	//ストライク演出
	for (int i = 0; i < kStrikeLineMax; i++) {
		mIsStrikeLineActive[i] = false;
	}

	//残像
	mIsDushShadowActive = false;
	for (int i = 0; i < kShadowMax; i++) {
		mIsShadowActive[i] = false;
	}
}



void Player::Update(Screen& screen, bool isFever, bool isOldFever) {

	//前回ポジションを取得する
	mOldPosition = mPosition;

	//速度を初期化する
	mVelocity.setZero();

	//大きさをズームにしたがって変える
	if (!isFever && isOldFever) {
		mSizeValue = 80;
	}
	mSizeValue = Clamp(mSizeValue, 0.0f, 200.0f);
	if (!isFever) {
		mSize += (mSizeValue - mSize) * 0.5f;
	}
	else if (isFever) {
		mSize = 80 / screen.GetZoom() * 0.4f;
	}
	mRadius = mSize / 2.0f;

	//ストライクをしていない時に可能
	if (!mIsStrikeActive) {

		//通常移動
		if (!mKnockbackActive) {
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
	Strike(isFever, isOldFever);
	if (!isFever) {
		StrikeLine(screen);
	}

	//ノックバック
	Knockback();

	//マップ内に収める
	mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
	mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));
		
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
	if (Controller::IsTriggerButton(0, Controller::bX)) {

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
void Player::Strike(bool isFever, bool isOldFever) {

	//ストライクパワーを収める
	mStrikePower = Clamp(mStrikePower, 0, kStrikePowerMax);

	//フィーバー状態が始まった || 終わった
	if ((isFever && !isOldFever) || (!isFever && isOldFever)) {
		mIsMarkActive = false;
		mIsStrikeActive = false;
	}

	//フィーバー中はパワーマックス
	if (isFever) {
		mStrikePower = kStrikePowerMax;
	}

	//モードチェンジ
	if (Controller::IsTriggerButton(0, Controller::bB) && !mIsStrikeActive) {

		if (strikeMode == STRAIGHT) {
			strikeMode = SPIRAL;
		}
		else {
			strikeMode = STRAIGHT;
		}
		mStrikeModeScaleEasingt = 0.0f;
		mStrikeModeScaleActive = true;
	}

	//モードチェンジのスケールイージング
	if (mStrikeModeScaleActive) {

		mStrikeModeScaleEasingt = EasingClamp(0.1f, mStrikeModeScaleEasingt);
		mStrikeModeScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mStrikeModeScaleEasingt));
		mStrikeModeBScale = EasingMove({ 0.7f, 0.7f }, { 1.0f, 1.0f }, easeOutSine(mStrikeModeScaleEasingt));

		if (mStrikeModeScaleEasingt == 1.0f) {
			mStrikeModeScaleActive = false;
		}
	}

	if (mIsMarkActive && !mIsStrikeActive) {

		//LBボタン＆RBボタン同時押下時でパワーがある時にフラグをtrueにする
		if ((Controller::IsPressedButton(0, Controller::lSHOULDER) && Controller::IsPressedButton(0, Controller::rSHOULDER)) && 0 < mStrikePower) {

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
					mStrikeSpeed = 10.0f;
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

	if (mIsStrikeActive) {

		if (mIsStraightStrikeActive)
		{
			mStraightStrikeOldTheta = mStraightStrikeTheta;

			mStrikeDirection = (mMarkPosition - mPosition).Normalized();

			mStrikeVelocity.setZero();
			mStrikeVelocity = mStrikeDirection * mStrikeSpeed + LeftStickVelocity(15.0f);
			mStrikeSpeed += 0.5f;

			mPosition += mStrikeVelocity;

			mStraightStrikeTheta = BetweenTheta(mMarkPosition - mPosition);

			if (Radian(mStraightStrikeTheta) < 10 && 350 < Radian(mStraightStrikeOldTheta)) {
				mStraightStrikeOldTheta -= Degree(360);
			}
			else if (Radian(mStraightStrikeOldTheta) < 10 && 350 < Radian(mStraightStrikeTheta)) {
				mStraightStrikeOldTheta += Degree(360);
			}

			if (90 > Radian(abs(mStraightStrikeTheta - mStraightStrikeOldTheta))) {
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
			if ((mPosition.x == mMarkPosition.x && mPosition.y == mMarkPosition.y) || mIsStraightStrikeFinish == true) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
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
void Player::StrikeLine(Screen& screen) {

	//if (mIsStrikeActive && 0.2f <= mStrikeEasingt && mStrikeEasingt <= 0.85f)
	//{
	//	//基準線とストライクの方向を取得
	//	Vec2 base = { 1,0 };
	//	Vec2 tmpDirection = mStrikeEndPosition - mStrikeStartPosition;

	//	//方向を正規化してｎ倍する
	//	tmpDirection = tmpDirection.Normalized();
	//	tmpDirection = tmpDirection * (100 / screen.GetZoom());

	//	//なす角を求める
	//	float dp = tmpDirection.Dot(base);
	//	float cp = tmpDirection.Cross(base);
	//	mStrikeLineAngle = atan2(cp, dp);

	//	//生成位置の最少値と最大値
	//	float randminX = mPosition.x - ((float)Screen::kWindowWidth  / screen.GetZoom());
	//	float randmaxX = mPosition.x + ((float)Screen::kWindowWidth  / screen.GetZoom());
	//	float randminY = mPosition.y - ((float)Screen::kWindowHeight / screen.GetZoom());
	//	float randmaxY = mPosition.y + ((float)Screen::kWindowHeight / screen.GetZoom());

	//	for (int i = 0; i < kStrikeLineMax; i++)
	//	{
	//		//生成
	//		if (!mIsStrikeLineActive[i])
	//		{
	//			mStrikeLinePosition[i].x = RAND(randminX, randmaxX);
	//			mStrikeLinePosition[i].y = RAND(randminY, randmaxY);
	//			mStrikeLinePosition[i] += tmpDirection;
	//			mStrikeLineWidth[i] = RAND(200, 400);
	//			mStrikeLineHeight[i] = RAND(2.5f, 3.5f);
	//			mStrikeLineAlphat[i] = 0.0f;
	//			mStrikeLineColor[i] = 0x50505000;
	//			mIsStrikeLineActive[i] = true;
	//			break;
	//		}
	//	}
	//}
	//for (int i = 0; i < kStrikeLineMax; i++) 
	//{
	//	if (mIsStrikeLineActive[i])
	//	{
	//		//透明度を変える
	//		mStrikeLineAlphat[i] = EasingClamp(0.015f, mStrikeLineAlphat[i]);
	//		if (mStrikeLineAlphat[i] < 0.5f) {
	//			mStrikeLineColor[i] = ColorEasingMove(0x50505000, 0x505050BB, easeLinear(mStrikeLineAlphat[i] * 2));
	//		}
	//		else {
	//			mStrikeLineColor[i] = ColorEasingMove(0x505050BB, 0x50505000, easeLinear(mStrikeLineAlphat[i] * 2 - 0.5f));
	//		}
	//		if (mStrikeLineAlphat[i] == 1.0f) {
	//			mIsStrikeLineActive[i] = false;
	//		}
	//	}
	//}
	//if (!mIsStrikeActive) 
	//{
	//	for (int i = 0; i < kStrikeLineMax; i++)
	//	{
	//		mIsStrikeLineActive[i] = false;
	//	}
	//}

}
void Player::Shadow(bool isHitStop) {

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
	if (Controller::IsTriggerButton(0, Controller::bA) && !mKnockbackActive) {

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

void Player::SetKnockbackPosition(Vec2 enemyPosition, float enemyRadius) {

	if (!mKnockbackActive) {
		Vec2 tmpDirection = (mOldPosition - enemyPosition).Normalized();
		mPosition = enemyPosition + tmpDirection * enemyRadius;
		mKnockbackEnemyPos = enemyPosition;
		mKnockbackSet = true;
	}
}
void Player::Knockback() {

	if (mKnockbackSet) {

		//パワーペナルティ
		mStrikePower--;

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
		mKnockbackActive = true;
		mKnockbackSet = false;
		mIsStrikeActive = false;
		mIsMarkActive = false;
	}
	if (mKnockbackActive) {
		mKnockBackT = EasingClamp(0.033f, mKnockBackT);
		mPosition = EasingMove(mKnockbackStart, mKnockbackEnd, easeOutSine(mKnockBackT));
		if (mKnockBackT == 1.0f) {
			mKnockbackActive = false;
		}
	}
	else {
		mKnockBackT = 0.0f;
		mKnockbackSet = false;
	}
}

void Player::Draw(Screen& screen) {

	//残像描画
	for (int i = 0; i < kShadowMax; i++) {
		screen.DrawSquare(mShadowPosition[i], mSize, mShadowColor[i]);
	}


	for (int i = 0; i < 4; i++) {
		screen.DrawSquare(mDushShadowPosition[i], mSize, mDushShadowColor);
	}

	//マーク描画
	if (mIsMarkActive){

		screen.DrawLine(mPosition, mMarkPosition, WHITE);
		screen.DrawSquare(mMarkPosition, mSize, WHITE);

	}

	//プレイヤー本体描画
	screen.DrawSquare(mPosition, mSize, 0x606060FF);
	screen.DrawPicture(mPosition, mSize, 0, 100, 100, toge, WHITE);

	//ストライクしろ(圧)描画
	if (mIsMarkActive && !mIsStrikeActive) {
		if (0 < mStrikePower) {
			screen.DrawAnime({ mPosition.x - (30 / screen.GetZoom()), mPosition.y + (40 / screen.GetZoom()) }, 60 / screen.GetZoom(), 30 / screen.GetZoom(), 0, 200, 100, 2, 10, mMarkFrame, lb, 0xFFFFFFE5);
			screen.DrawAnime({ mPosition.x + (30 / screen.GetZoom()), mPosition.y + (40 / screen.GetZoom()) }, 60 / screen.GetZoom(), 30 / screen.GetZoom(), 0, 200, 100, 2, 10, mMarkFrame, rb, 0xFFFFFFE5);
		} else {
			screen.DrawPicture({ mPosition.x, mPosition.y + (40 / screen.GetZoom()) }, 360, 120, 0.0f, 300, 100, nopower, WHITE);
		}
	}

	//ストライク中の線描画
	for (int i = 0; i < kStrikeLineMax; i++)
	{
		screen.DrawRectAngle(mStrikeLinePosition[i], mStrikeLineWidth[i] / screen.GetZoom(), mStrikeLineHeight[i] / screen.GetZoom(), mStrikeLineAngle, mStrikeLineColor[i]);
	}

}
void Player::DrawStrikeUI(Screen& screen) {

	screen.DrawBox({ 35.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, WHITE, kFillModeSolid, false);
	for (int i = 0; i < kStrikePowerMax; i++) {
		if (i == kStrikePowerMax - 1) {
			screen.DrawUI({ 10.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, lastflame, WHITE);
		} else {
			screen.DrawUI({ 10.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, flame, WHITE);
		}
	}

	screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, circle, WHITE);
	if (strikeMode == STRAIGHT) {
		screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, straight, WHITE, mStrikeModeScale);
	} else {
		screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, spiral, WHITE, mStrikeModeScale);
	}
	screen.DrawUI({ mStrikeModePosition.x + 50, mStrikeModePosition.y - 50 }, 60, 0, 160, 160, b, WHITE, mStrikeModeBScale);

}

void Player::LoadTexture() {

	if (!mIsLoadTexture) {
		flame = Novice::LoadTexture("./Resources/Player/flame.png");
		lastflame = Novice::LoadTexture("./Resources/Player/lastflame.png");
		circle = Novice::LoadTexture("./Resources/Player/circle.png");
		straight = Novice::LoadTexture("./Resources/Player/straight.png");
		spiral = Novice::LoadTexture("./Resources/Player/spiral.png");
		b = Novice::LoadTexture("./Resources/Player/b.png");
		lb = Novice::LoadTexture("./Resources/Player/lb.png");
		rb = Novice::LoadTexture("./Resources/Player/rb.png");
		nopower = Novice::LoadTexture("./Resources/Player/nopower.png");
		toge = Novice::LoadTexture("./Resources/Player/toge.png");
	}
}
