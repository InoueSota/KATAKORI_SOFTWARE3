#include "Player.h"
#include "Key.h"
#include "ControllerInput.h"
#include "Collision.h"
#include "Function.h"
#include "Easing.h"
#include "Ingame.h"



void Player::Init() {
	//�p�����[�^
	mPosition.setZero();
	mVelocity.setZero();
	mSizeValue = 100;
	mSize = mSizeValue;
	mRadius = mSize / 2.0f;

	//�ʏ�ړ�
	mNormalMag = 6.0f;
	mNormalMagLockon = 12.0f;

	//�_�b�V��
	mDushMag = 300.0f;

	//�}�[�L���O
	mIsMarkActive = false;
	mMarkFrame = 0;

	//�X�g���C�N
	mIsStrikeActive = false;
	mStrikePower = 0;
	mStrikeModeScale = { 1.0f, 1.0f };
	mStrikeModeBScale = { 1.0f, 1.0f };
	mStrikeModeScaleActive = false;
	strikeMode = STRAIGHT;

	mDushTimer = 0;

	//�X�g���C�N���o
	for (int i = 0; i < kStrikeLineMax; i++) {
		mIsStrikeLineActive[i] = false;
	}

	//�c��
	mIsDushShadowActive = false;
	for (int i = 0; i < kShadowMax; i++) {
		mIsShadowActive[i] = false;
	}

	//�m�b�N�o�b�N
	mKnockbackSet = false;
}


void Player::Update(Screen& screen, bool isFever, bool isOldFever) {

	//�O��|�W�V�������擾����
	mOldPosition = mPosition;

	//�O��X�g���C�N�t���O���擾����
	mIsOldStrikeActive = mIsStrikeActive;

	//���x������������
	mVelocity.setZero();

	//�傫�����Y�[���ɂ��������ĕς���
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

	//�X�g���C�N
	Strike(isFever, isOldFever);
	if (!isFever) {
		StrikeLine(screen);
	}

	//�X�g���C�N�����Ă��Ȃ����ɉ\
	if (!mIsStrikeActive) {

		//�ʏ�ړ�
		if (!mKnockbackActive) {
			NormalMove(screen);
		}
		
		//�_�b�V��
		Dush();

		//�}�[�L���O
		Mark();

		//���x��������
		mPosition += (mVelocity / screen.GetZoom() * 0.4f);

		//�}�[�N�Ƃ̋����ɐ�����������
		MarkLimitLength(isFever);
	}
	if (Controller::IsTriggerButton(0, Controller::bA)) {
		//�_�b�V������̃^�C�}�[�Z�b�g
		mDushTimer = kMaxDushTimer;
	}
	if (mDushTimer > 0) {
		mDushTimer--;
	}
		
}
void Player::NormalMove(Screen& screen) {

	//�X�e�B�b�N�̕������擾����
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//int�^ �̒l�� float�^ �ɓ����iint�^�̂܂܂��ƌv�Z���O���P�ɂȂ�j
	mNormalVelocity.x = tmpX;
	mNormalVelocity.y = tmpY;

	//�ő�l���P�ɂ���
	mNormalVelocity = mNormalVelocity.Normalized();

	//���{����
	if (LockonCount) {
		mNormalVelocity *= (mNormalMagLockon + (LockonCount * 1));
	} else {
		mNormalVelocity *= mNormalMag;
	}

	//���x��������
	mVelocity += mNormalVelocity;

}
void Player::Dush() {

	//�X�e�B�b�N�̕������擾����
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);
	
	//�X�e�B�b�N�𗣂��Ă��_�b�V�����o����悤�ɂ���
	if (tmpX != 0 && tmpY != 0){

		//int�^ �̒l�� float�^ �ɓ����iint�^�̂܂܂��ƌv�Z���O���P�ɂȂ�j
		mDushVelocity.x = tmpX;
		mDushVelocity.y = tmpY;

		//�ő�l���P�ɂ���
		mDushVelocity = mDushVelocity.Normalized();

		//���{����
		mDushVelocity *= mDushMag;
	}

	//A�{�^��������
	if (Controller::IsTriggerButton(0,Controller::bA)){

		//���x��������
		mVelocity += mDushVelocity;
		
	}
}
void Player::Mark() {

	//X�{�^����������Position��ݒ聕�t���O��true�ɂ���
	if (Controller::IsTriggerButton(0, Controller::bX)) {

		mMarkPosition = mPosition;
		mMarkFrame = 0;
		mIsMarkActive = true;

	}

	//�������ԂŃ}�[�L���O������
	if (mIsMarkActive){

		//�t���[���̉��Z
		mMarkFrame++;

		////�������Ԃ𒴂�����t���O��false�ɂ���
		//if (kMarkTimeLimit <= mMarkFrame){
		//	mIsMarkActive = false;
		//}
	}

	//������
	if (!mIsMarkActive){
		mMarkFrame = 0;
	}

}
void Player::MarkLimitLength(bool isFever) {

	if (mIsMarkActive && !isFever) {

		if (!Collision(mPosition, 0, mMarkPosition, kMarkMaxLength)) {
			Vec2 tmpDirection = (mPosition - mMarkPosition).Normalized();
			mPosition = mMarkPosition + tmpDirection * kMarkMaxLength;
		}
	}
}
void Player::Strike(bool isFever, bool isOldFever) {

	//�t�B�[�o�[��Ԃ��n�܂��� || �I�����
	if ((isFever && !isOldFever) || (!isFever && isOldFever)) {
		mIsMarkActive = false;
		mIsStrikeActive = false;
	}

	//�t�B�[�o�[���̓p���[�}�b�N�X
	if (isFever) {
		mStrikePower = kStrikePowerMax;
	}

	/*
	//���[�h�`�F���W
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
	//���[�h�`�F���W�̃X�P�[���C�[�W���O
	if (mStrikeModeScaleActive) {

		mStrikeModeScaleEasingt = EasingClamp(0.1f, mStrikeModeScaleEasingt);
		mStrikeModeScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mStrikeModeScaleEasingt));
		mStrikeModeBScale = EasingMove({ 0.7f, 0.7f }, { 1.0f, 1.0f }, easeOutSine(mStrikeModeScaleEasingt));

		if (mStrikeModeScaleEasingt == 1.0f) {
			mStrikeModeScaleActive = false;
		}
	}
	*/

	if (mIsMarkActive && !mIsStrikeActive) {

		//�}�[�N���������ƃp���[�����鎞�Ƀt���O��true�ɂ���
		if (Controller::IsTriggerButton(0, Controller::rSHOULDER) && 0 < mStrikePower) {

			//�����ɉ����ăC�[�W���O�̑����ʂ�ω�������
			//�}�[�N�Ƃ̋��������߂�
			float tmpDistance = (mMarkPosition - mPosition).Length();

			//������0�������璆�~����
			if (tmpDistance == 0) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
			//������0����Ȃ��Ƃ�
			else {

				//�X�g���C�N�p���[�������
				mStrikePower--;

				//�����ʂ������ɉ����ĕς��邽�߂̌v�Z
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

				//�t���O��true�ɂ���
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
			mStrikeVelocity = mStrikeDirection * mStrikeSpeed + LeftStickVelocity(30.0f);
			mStrikeSpeed += 1.0f;

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

			//�ړ����I��������
			if ((mPosition.x == mMarkPosition.x && mPosition.y == mMarkPosition.y) || mIsStraightStrikeFinish == true) {
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
		}
		else {
			//�C�[�W���O�l���Z
			mStrikeEasingt = EasingClamp(0.015f, mStrikeEasingt);

			mStrikeTheta = EasingMove(mStrikeThetaStartValue, mStrikeThetaStartValue + Degree(360), easeLinear(mStrikeEasingt));
			mStrikeRadius = EasingMove(mStrikeRadiusStartValue, 0.0f, easeLinear(mStrikeEasingt));

			mPosition.x = mStrikeRadius * -cosf(mStrikeTheta) + mMarkPosition.x;
			mPosition.y = mStrikeRadius * sinf(mStrikeTheta) + mMarkPosition.y;

			//�ړ����I��������
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
	//	//����ƃX�g���C�N�̕������擾
	//	Vec2 base = { 1,0 };
	//	Vec2 tmpDirection = mStrikeEndPosition - mStrikeStartPosition;

	//	//�����𐳋K�����Ă��{����
	//	tmpDirection = tmpDirection.Normalized();
	//	tmpDirection = tmpDirection * (100 / screen.GetZoom());

	//	//�Ȃ��p�����߂�
	//	float dp = tmpDirection.Dot(base);
	//	float cp = tmpDirection.Cross(base);
	//	mStrikeLineAngle = atan2(cp, dp);

	//	//�����ʒu�̍ŏ��l�ƍő�l
	//	float randminX = mPosition.x - ((float)Screen::kWindowWidth  / screen.GetZoom());
	//	float randmaxX = mPosition.x + ((float)Screen::kWindowWidth  / screen.GetZoom());
	//	float randminY = mPosition.y - ((float)Screen::kWindowHeight / screen.GetZoom());
	//	float randmaxY = mPosition.y + ((float)Screen::kWindowHeight / screen.GetZoom());

	//	for (int i = 0; i < kStrikeLineMax; i++)
	//	{
	//		//����
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
	//		//�����x��ς���
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

	//�}�b�v���Ɏ��߂�
	mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
	mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));

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

	//A�{�^��������
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
		mKnockBackT = EasingClamp(0.033f, mKnockBackT);
		mPosition = EasingMove(mKnockbackStart, mKnockbackEnd, easeOutSine(mKnockBackT));
		if (mKnockBackT >= 0.7f) {
			mKnockBackT = 0.0f;
			mKnockbackActive = false;
		}
	}
}


void Player::Draw(Screen& screen, bool isReady) {

	//�c���`��
	for (int i = 0; i < kShadowMax; i++) {
		screen.DrawPicture(mShadowPosition[i], mSize, 0, 100, 100, toge, mShadowColor[i]);
	}


	for (int i = 0; i < 4; i++) {
		screen.DrawPicture(mDushShadowPosition[i], mSize, 0, 100, 100, toge, mDushShadowColor);
	}

	//�}�[�N�`��
	if (mIsMarkActive){

		screen.DrawLine(mPosition, mMarkPosition, RED);
		screen.DrawPicture(mMarkPosition, mSize, 0, 100, 100, mark, WHITE);

	}

	//�v���C���[�{�̕`��
	screen.DrawPicture(mPosition, mSize, 0, 100, 100, toge, WHITE);

	if (!isReady) {
		screen.DrawUI({ Screen::kWindowWidth / 2.0, 100 }, 400, 100, 0.0f, 400, 100, areyouready, WHITE);
	}

	//�X�g���C�N����(��)�`��
	if (mIsMarkActive && !mIsStrikeActive) {
		if (0 < mStrikePower) {
			screen.DrawAnime({ mPosition.x, mPosition.y + (40 / screen.GetZoom()) }, 60 / screen.GetZoom(), 30 / screen.GetZoom(), 0, 200, 100, 2, 20, mMarkFrame, rb, 0xFFFFFFE5);
		} else {
			screen.DrawPicture({ mPosition.x, mPosition.y + (40 / screen.GetZoom()) }, 360, 120, 0.0f, 300, 100, nopower, WHITE);
		}
	}

	//�X�g���C�N���̐��`��
	for (int i = 0; i < kStrikeLineMax; i++)
	{
		screen.DrawRectAngle(mStrikeLinePosition[i], mStrikeLineWidth[i] / screen.GetZoom(), mStrikeLineHeight[i] / screen.GetZoom(), mStrikeLineAngle, mStrikeLineColor[i]);
	}

}
void Player::DrawStrikeUI(Screen& screen) {

	//�X�g���C�N�p���[�����߂�
	mStrikePower = Clamp(mStrikePower, 0, kStrikePowerMax);
	screen.DrawBox({ 35.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, WHITE, kFillModeSolid, false);

	for (int i = 0; i < kStrikePowerMax; i++) {
		if (i == kStrikePowerMax - 1) {
			screen.DrawUI({ 10.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, lastflame, WHITE);
		} else {
			screen.DrawUI({ 10.0f + (i + 1) * 50, 75.0f }, 50, 25, 0, 200, 100, flame, WHITE);
		}
	}

	//�}�[�N�̒���
	if (mIsMarkActive) {
		float marklength = (mPosition - mMarkPosition).Length();
		marklength = Clamp(marklength, 0, kMarkMaxLength);
		screen.DrawBox({ 35.0f, 100.0f }, marklength * (250.0f / kMarkMaxLength), 25, 0.0f, WHITE, kFillModeSolid, false);
	}
	screen.DrawUI({ 160.0f, 112.5f }, 250, 25, 0, 1000, 100, lengthflame, WHITE);

	//screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, circle, WHITE);
	//if (strikeMode == STRAIGHT) {
	//	screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, straight, WHITE, mStrikeModeScale);
	//} else {
	//	screen.DrawUI(mStrikeModePosition, 170, 0, 500, 500, spiral, WHITE, mStrikeModeScale);
	//}
	//screen.DrawUI({ mStrikeModePosition.x + 50, mStrikeModePosition.y - 50 }, 60, 0, 160, 160, b, WHITE, mStrikeModeBScale);


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
	mark = Novice::LoadTexture("./Resources/Player/mark.png");
	areyouready = Novice::LoadTexture("./Resources/UI/Explanation/areyouready.png");
}
