#include "Player.h"
#include "Key.h"
#include "ControllerInput.h"
#include "Function.h"
#include "Easing.h"
#include "Ingame.h"



void Player::Init() {
	//�p�����[�^
	mPosition.setZero();
	mVelocity.setZero();
	mSizeMax = 70;
	mSize = mSizeMax;
	mRadius = mSize / 2.0f;

	//�ʏ�ړ�
	mNormalMag = 6.0f;

	//�_�b�V��
	mDushMag = 300.0f;

	//�}�[�L���O
	mIsMarkActive = false;
	mMarkFrame = 0;

	//�X�g���C�N
	mIsStrikeActive = false;
	mStrikeEasingt = 0.0f;
	mStrikePower = 0;

	//�X�g���C�N���o
	for (int i = 0; i < kStrikeLineMax; i++) {
		mIsStrikeLineActive[i] = false;
	}

	//�c��
	mIsDushShadowActive = false;
	for (int i = 0; i < kShadowMax; i++) {
		mIsShadowActive[i] = false;
	}
}



void Player::Update(Screen& screen, bool isFever, bool isOldFever) {

	//�O��|�W�V�������擾����
	mOldPosition = mPosition;

	//���x������������
	mVelocity.setZero();

	//�傫�����Y�[���ɂ��������ĕς���
	mSize = mSizeMax / screen.GetZoom() * 0.4f;
	mRadius = mSize / 2.0f;
	//�T�C�Y�̑���l�����߂�
	mSizeMax = Clamp(mSizeMax, 0.0f, 80.0f);

	//�X�g���C�N�����Ă��Ȃ����ɉ\
	if (!mIsStrikeActive) {

		//�ʏ�ړ�
		if (!mKnockbackActive) {
			NormalMove();
		}

		//�_�b�V��
		Dush();

		//�}�[�L���O
		Mark();

		//���x��������
		mPosition += mVelocity;
	}

	//�X�g���C�N
	Strike(isFever, isOldFever);
	if (!isFever) {
		StrikeLine(screen);
	}

	//�m�b�N�o�b�N
	Knockback();

	//�}�b�v���Ɏ��߂�
	mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
	mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));
		
}
void Player::NormalMove() {

	//�X�e�B�b�N�̕������擾����
	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);

	//int�^ �̒l�� float�^ �ɓ����iint�^�̂܂܂��ƌv�Z���O���P�ɂȂ�j
	mNormalVelocity.x = tmpX;
	mNormalVelocity.y = tmpY;

	//�ő�l���P�ɂ���
	mNormalVelocity = mNormalVelocity.Normalized();

	//���{����
	mNormalVelocity *= mNormalMag;

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

		//�������Ԃ𒴂�����t���O��false�ɂ���
		if (kMarkTimeLimit <= mMarkFrame){
			mIsMarkActive = false;
		}
	}

	//������
	if (!mIsMarkActive){
		mMarkFrame = 0;
	}

}
void Player::Strike(bool isFever, bool isOldFever) {

	//�t�B�[�o�[��Ԃ��n�܂��� || �I�����
	if ((isFever && !isOldFever) || (!isFever && isOldFever)) {
		mIsStrikeActive = false;
	}

	if (mIsMarkActive && !mIsStrikeActive) {

		//LT�{�^����RT�{�^�������������Ƀt���O��true�ɂ���
		if (Controller::IsPressedButton(0, Controller::lTrigger) && Controller::IsPressedButton(0, Controller::rTrigger)) {

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
				//�����ʂ������ɉ����ĕς��邽�߂̌v�Z
				float tmpValue = tmpDistance / 100;

				// n / tmpValue �̂Ƃ��An�͋�����100�̂Ƃ���Easingt�̑����ʂɂȂ�
				mStrikeEasingtIncrementValue = 0.25f / tmpValue;

				if (!isFever) {
					//�C�[�W���O���̎n�_�ƏI�_�̐ݒ�
					mStrikeStartPosition = mPosition;
					mStrikeEndPosition = mMarkPosition;
				}
				else {
					//����ƌ����x�N�g��
					Vec2 base = { 1,0 };
					Vec2 tmpDirection = mMarkPosition - mPosition;

					//�Ȃ��p�����߂�
					float dp = tmpDirection.Dot(base);
					float cp = tmpDirection.Cross(base);
					float tmpTheta = atan2(cp, dp);
					if (tmpTheta < 0) {
						tmpTheta = Degree(360) + tmpTheta;
					}
					mStrikeThetaStartValue = tmpTheta;
					mStrikeRadiusStartValue = tmpDistance;
				}

				//�X�g���C�N�p���[�������
				mStrikePower = 0;

				//�t���O��true�ɂ���
				mIsStrikeActive = true;
			}
		}
	}

	if (mIsStrikeActive) {

		//�C�[�W���O�l���Z
		mStrikeEasingt = EasingClamp(mStrikeEasingtIncrementValue, mStrikeEasingt);

		if (!isFever) {
			mPosition = EasingMove(mStrikeStartPosition, mStrikeEndPosition, easeInSine(mStrikeEasingt));
		}
		else {
			mStrikeTheta = EasingMove(mStrikeThetaStartValue, mStrikeThetaStartValue + Degree(360), easeLinear(mStrikeEasingt));
			mStrikeRadius = EasingMove(mStrikeRadiusStartValue, 0.0f, easeLinear(mStrikeEasingt));

			mPosition.x = mStrikeRadius * -cosf(mStrikeTheta) + mMarkPosition.x;
			mPosition.y = mStrikeRadius * sinf(mStrikeTheta) + mMarkPosition.y;
		}

		//�ړ����I��������
		if (mStrikeEasingt == 1.0f) {
			mIsMarkActive = false;
			mIsStrikeActive = false;
		}


	}

	//������
	if (!mIsStrikeActive) {
		mStrikeEasingt = 0.0f;
	}
	if (mIsMarkActive && !mIsStrikeActive) {

		//LT�{�^����RT�{�^�������������Ƀt���O��true�ɂ���
		if (Controller::IsPressedButton(0, Controller::lTrigger) && Controller::IsPressedButton(0, Controller::rTrigger)) {

		}
	}


}
void Player::StrikeLine(Screen& screen) {

	if (mIsStrikeActive && 0.2f <= mStrikeEasingt && mStrikeEasingt <= 0.85f)
	{
		//����ƃX�g���C�N�̕������擾
		Vec2 base = { 1,0 };
		Vec2 tmpDirection = mStrikeEndPosition - mStrikeStartPosition;

		//�����𐳋K�����Ă��{����
		tmpDirection = tmpDirection.Normalized();
		tmpDirection = tmpDirection * (100 / screen.GetZoom());

		//�Ȃ��p�����߂�
		float dp = tmpDirection.Dot(base);
		float cp = tmpDirection.Cross(base);
		mStrikeLineAngle = atan2(cp, dp);

		//�����ʒu�̍ŏ��l�ƍő�l
		float randminX = mPosition.x - ((float)Screen::kWindowWidth  / screen.GetZoom());
		float randmaxX = mPosition.x + ((float)Screen::kWindowWidth  / screen.GetZoom());
		float randminY = mPosition.y - ((float)Screen::kWindowHeight / screen.GetZoom());
		float randmaxY = mPosition.y + ((float)Screen::kWindowHeight / screen.GetZoom());

		for (int i = 0; i < kStrikeLineMax; i++)
		{
			//����
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
			//�����x��ς���
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

	if (!mKnockbackActive) {
		Vec2 tmpDirection = (mOldPosition - enemyPosition).Normalized();
		mPosition = enemyPosition + tmpDirection * enemyRadius;
		mKnockbackEnemyPos = enemyPosition;
		mKnockbackSet = true;
	}
}
void Player::Knockback() {

	if (mKnockbackSet) {
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

	//�c���`��
	for (int i = 0; i < kShadowMax; i++) {
		screen.DrawSquare(mShadowPosition[i], mSize, mShadowColor[i]);
	}


	for (int i = 0; i < 4; i++) {
		screen.DrawSquare(mDushShadowPosition[i], mSize, mDushShadowColor);
	}

	//�}�[�N�`��
	if (mIsMarkActive){

		screen.DrawLine(mPosition, mMarkPosition, WHITE);
		screen.DrawSquare(mMarkPosition, mSize, WHITE);

	}

	//�v���C���[�{�̕`��
	screen.DrawSquare(mPosition, mSize, 0x606060FF);

	//�X�g���C�N���̐��`��
	for (int i = 0; i < kStrikeLineMax; i++)
	{
		screen.DrawRectAngle(mStrikeLinePosition[i], mStrikeLineWidth[i] / screen.GetZoom(), mStrikeLineHeight[i] / screen.GetZoom(), mStrikeLineAngle, mStrikeLineColor[i]);
	}

}

void Player::DrawStrikePower(Screen& screen) {

	screen.DrawBox({ Screen::kWindowWidth - (Screen::kMiniMapSize * 4),599 }, 5 * 20, 22, 0.0f, BLACK, kFillModeWireFrame, false);
	screen.DrawBox({ Screen::kWindowWidth - (Screen::kMiniMapSize * 4),600 }, 5 * mStrikePower, 20, 0.0f, WHITE, kFillModeSolid, false);
}
