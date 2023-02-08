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
	mColor = WHITE;

	//�ʏ�ړ�
	mNormalMag = 5.0f;
	mNormalMagLockon = 12.0f;

	//�_�b�V��
	mDushMag = 300.0f;
	mIsUseDushOutOfRange = false;
	mUseDushFrame = 0;
	mUseDushCount = 0;

	//�_�b�V�����o
	for (int i = 0; i < kDushBoxMax; i++) {
		mIsDushBoxActive[i] = false;
	}

	//�p���[�I�[��
	for (int i = 0; i < kPowerParticleMax; i++) {
		mIsPowerParticleActive[i] = false;
	}

	//�}�[�L���O
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

	//�X�g���C�N
	mIsStrikeActive = false;
	mIsStrikeBoxShakeActive = false;
	mStrikePower = 0;
	mStrikeModeScale = { 1.0f, 1.0f };
	mStrikeModeBScale = { 1.0f, 1.0f };
	mStrikeModeScaleActive = false;
	strikeMode = STRAIGHT;
	mStrikeTutorial = 0;

	//�X���[
	mIsSlowActive = false;
	mSlowMag = 1.0f;
	mSlowEasingt = 0.0f;
	mLeftStickMag = 1.0f;

	mDushTimer = 0;

	//�X�g���C�N���o
	mIsStrikeBoxActive = false;

	//�c��
	mIsDushShadowActive = false;
	for (int i = 0; i < kShadowMax; i++) {
		mIsShadowActive[i] = false;
	}

	//�^�C�g����ʐ�p
	mIsControll = false;
	mTargetPoint.x = RAND(100, Screen::kWindowWidth - 100);
	mTargetPoint.y = RAND(100, Screen::kWindowHeight - 100);
	mDirectionPoint.setZero();
	mSpeed = 5.0f;
	mControllIntervalFrame = 0;

	//�m�b�N�o�b�N
	mKnockbackSet = false;
	mKnockbackActive = false;
}


void Player::Update(Screen& screen, bool isFever, bool isOldFever, unsigned int BackBoxColor) {

	//�O��|�W�V�������擾����
	mOldPosition = mPosition;

	//�O��X�g���C�N�t���O���擾����
	mIsOldStrikeActive = mIsStrikeActive;

	//�V�F�C�N�t���O������������
	mIsStrikeBoxShakeActive = false;

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
	Strike(isFever, isOldFever, screen);
	StrikeBox();

	//�X�g���C�N�����Ă��Ȃ����ɉ\
	if (!mIsStrikeActive) {

		//�ʏ�ړ�
		if (!mKnockbackActive) {
			NormalMove();
		}
		
		//�_�b�V��
		Dush(1.0f);

		//�p���[�����鎞�ɃI�[��
		PowerParticle();

		//�}�[�L���O
		Mark();

		//���x��������
		mPosition += (mVelocity / screen.GetZoom() * 0.4f);

		//�}�b�v���Ɏ��߂�
		mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
		mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));

		//�}�[�N�Ƃ̋����ɐ�����������
		MarkLimitLength(isFever);

		//�_�b�V�����o
		DushBox(BackBoxColor);
	}
	//if (Controller::IsTriggerButton(0, Controller::bA)) {
	//	//�_�b�V������̃^�C�}�[�Z�b�g
	//	mDushTimer = kMaxDushTimer;
	//}
	if (mDushTimer > 0) {
		mDushTimer--;
	}
		
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
	if (LockonCount) {
		mNormalVelocity *= (mNormalMagLockon + (LockonCount * 1));
	} else {
		mNormalVelocity *= mNormalMag;
	}

	//���x��������
	mVelocity += mNormalVelocity;

}
void Player::Dush(float mag) {

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
		mDushVelocity *= mDushMag * mag;
	}

	//A�{�^��������
	if (Controller::IsTriggerButton(0,Controller::bA)){

		//���x��������
		mVelocity += mDushVelocity;

		if (!mKnockbackActive) {
			int handle = -1;
			if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
				handle = Novice::PlayAudio(dush, 0, 1);
			}
		}
		
	}
}
void Player::DushBox(unsigned int BackBoxColor) {

	//A�{�^��������
	if (Controller::IsTriggerButton(0, Controller::bA)) {

		//�_�b�V�����o�t���O
		for (int i = 0; i < kDushBoxMax; i++) {
			if (!mIsDushBoxActive[i]) {
				mDushBoxPosition[i] = mPosition;
				mDushBoxEasingt[i] = 0.0f;
				mIsDushBoxActive[i] = true;
				break;
			}
		}
	}

	//�_�b�V�����o����
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
void Player::PowerParticle() {

	for (int i = 0; i < kPowerParticleMax; i++) {

		if (0 < mStrikePower) {

			if (!mIsPowerParticleActive[i]) {
				float tmpRandTheta = Degree(RAND(0, 360));
				mPowerParticlePosition[i].x = cosf(tmpRandTheta) * 200 + mPosition.x;
				mPowerParticlePosition[i].y = sinf(tmpRandTheta) * 200 + mPosition.y;
				if (mStrikePower != kStrikePowerMax) {
					mPowerParticleStartColor[i] = 0xFFFFFF00;
				} else {
					mPowerParticleStartColor[i] = 0xFF000000;
				}
				mPowerParticleColor[i] = mPowerParticleStartColor[i];
				mPowerParticleEasingt[i] = 0.0f;
				mIsPowerParticleActive[i] = true;
				break;
			}
		}

		if (mIsPowerParticleActive[i]) {
			mPowerParticleEasingt[i] = EasingClamp(0.02f, mPowerParticleEasingt[i]);
			mPowerParticlePosition[i] += (mPosition - mPowerParticlePosition[i]) * 0.1f;
			mPowerParticleColor[i] = mPowerParticleStartColor[i] | ColorEasingMove(0x00, 0xA0, easeOutCirc(mPowerParticleEasingt[i]));
			mPowerParticleSize[i] = EasingMove(0.0f, kPowerParticleSize1, easeOutCirc(mPowerParticleEasingt[i]));
			if (mPowerParticleEasingt[i] >= 0.4f) {
				mIsPowerParticleActive[i] = false;
			}
		}
	}
}
void Player::Mark() {

	//X�{�^����������Position��ݒ聕�t���O��true�ɂ���
	if (Controller::IsTriggerButton(0, Controller::bX)) {

		mMarkPosition = mPosition;
		mMarkDurableValue = kMarkDurableMax;
		mMarkFrame = 0;
		mMarkScaleEasingt = 0.0f;
		mIsMarkActive = true;

	}
	//RB�̃A�j���[�V�����Ɏg���t���[�����X�V����
	if (mIsMarkActive){
		mMarkFrame++;

		//�}�[�N�̃A�j���[�V����
		mMarkScaleEasingt = EasingClamp(0.02f, mMarkScaleEasingt);
		mMarkScaleColor = ColorEasingMove(0xFFFFFFA0, 0xFFFFFF00, easeOutCirc(mMarkScaleEasingt));
		mMarkScale = EasingMove({ 1.0f,1.0f }, { 5.0f, 5.0f }, easeOutSine(mMarkScaleEasingt));
		if (mMarkScaleEasingt == 1.0f) {
			mMarkScaleEasingt = 0.0f;
		}
	}
	//������
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

	//�͈͊O�Ƀ_�b�V���𑱂�����}�[�N��؂�
	if (mIsUseDushOutOfRange) {
		mUseDushFrame++;
	}
	//�_�b�V���𗘗p���Ĕ͈͊O�ɏo�悤�Ƃ����u�ԁA�t���[��������������
	if (!mIsOldUseDushOutOfRange && mIsUseDushOutOfRange) {
		mUseDushFrame = 0;
	}
	//�R��ȏ�_�b�V���Ŕ����悤�Ƃ�����}�[�N��؂�
	if (3 <= mUseDushCount) {
		mUseDushCount = 0;
		mUseDushFrame = 0;
		mIsMarkActive = false;
		mIsUseDushOutOfRange = false;
		mIsOldUseDushOutOfRange = false;
	}
	//������̂ɂU�O�t���[������������J�E���g������������
	if (60 <= mUseDushFrame) {
		mUseDushCount = 0;
		mIsUseDushOutOfRange = false;
		mIsOldUseDushOutOfRange = false;
	}

	//���������߂�
	mMarkLength = (mPosition - mMarkPosition).Length();
	mMarkLength = Clamp(mMarkLength, 0, kMarkMaxLength);

	//�}�[�N�̌��E�������ő�ɒB������ԐF�ɂ���
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
	//�}�[�N�����E�����ɒB������AUI���V�F�C�N����
	if (mIsMarkLimitAnimationActive) {

		mMarkAnimationEasingt = EasingClamp(0.033f, mMarkAnimationEasingt);
		mMarkShakeMag = EasingMove(1.0f, 0.0f, easeOutSine(mMarkAnimationEasingt));

		mMarkShake.x = RAND(-3.0f, 5.0f) * mMarkShakeMag;
		mMarkShake.y = RAND(-3.0f, 5.0f) * mMarkShakeMag;
	}

}
void Player::Strike(bool isFever, bool isOldFever, Screen& screen) {

	//�t�B�[�o�[��Ԃ��n�܂��� || �I�����
	if ((isFever && !isOldFever) || (!isFever && isOldFever)) {
		mIsMarkActive = false;
		mIsStrikeActive = false;
	}

	//�t�B�[�o�[���̓p���[�}�b�N�X
	if (isFever) {
		mStrikePower = kStrikePowerMax;
	}

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

	if (mStrikeTutorial) {
		mStrikeTutorial = 0;
	}

	if (mIsStrikeActive) {

		if (mIsStraightStrikeActive)
		{
			mStraightStrikeOldTheta = mStraightStrikeTheta;

			mStrikeDirection = (mMarkPosition - mPosition).Normalized();

			//A����������X���[������
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

			//�ړ����I��������
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
void Player::StrikeBoxInit(Screen& screen) {

	//������
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

	//�X�g���C�N���I��������̃A�j���[�V����
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

	//A�{�^��������
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

	//�Q�[���ɓ�������e������������
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
		mMarkDurableValue--;
		if (mMarkDurableValue == 0) {
			mIsMarkActive = false;
		}
		mIsStrikeActive = false;

		int handle = -1;
		if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
			handle = Novice::PlayAudio(hit, 0, 1);
		}
	}
	if (mKnockbackActive) {
		mColor = 0xFFFFFF50;
		mKnockBackT = EasingClamp(0.033f, mKnockBackT);
		mPosition = EasingMove(mKnockbackStart, mKnockbackEnd, easeOutSine(mKnockBackT));
		if (!Collision(mPosition, 0, mMarkPosition, kMarkMaxLength)) {
			Vec2 tmpDirection = (mPosition - mMarkPosition).Normalized();
			mPosition = mMarkPosition + tmpDirection * kMarkMaxLength;
		}
		if (mKnockBackT >= 0.7f) {
			mKnockBackT = 0.0f;
			mColor = WHITE;
			mKnockbackActive = false;
		}
	}
}


void Player::Draw(Screen& screen, bool isReady, bool isFever, unsigned int feverGaugeColor, unsigned int backLineColor) {

	//�c���`��
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

	//�X�g���C�N���o
	if (mIsStrikeBoxActive) {

		for (int i = 0; i < kStrikeBoxMax; i++) {
			screen.DrawRectAngle(mStrikeBoxPosition[i], mStrikeBoxSize, mStrikeBoxSize, mStrikeBoxAngle[i], backLineColor, kFillModeWireFrame, true);
		}
	}

	//�_�b�V�����o
	for (int i = 0; i < kDushBoxMax; i++) {
		if (mIsDushBoxActive[i]) {
			screen.DrawRectAngle(mDushBoxPosition[i], mDushBoxSize[i], mDushBoxSize[i], mDushBoxAngle[i], mDushBoxColor[i], kFillModeWireFrame, true);
		}
	}

	//�p���[�I�[��
	for (int i = 0; i < kPowerParticleMax; i++) {
		if (mIsPowerParticleActive[i]) {
			screen.DrawSquare(mPowerParticlePosition[i], mPowerParticleSize[i], mPowerParticleColor[i], kFillModeSolid, true);
		}
	}
	
	//�}�[�N�`��
	if (mIsMarkActive){

		if (!isFever) {
			screen.DrawPicture((mPosition - mMarkPosition) / 2.0f + mMarkPosition, (mPosition - mMarkPosition).Length(), 10, BetweenTheta(mMarkPosition - mPosition), 0, 0, 192, mMarkColor);
		} else {
			screen.DrawPicture((mPosition - mMarkPosition) / 2.0f + mMarkPosition, (mPosition - mMarkPosition).Length(), 10, BetweenTheta(mMarkPosition - mPosition), 0, 0, 192, feverGaugeColor);
		}
		screen.DrawPicture(mMarkPosition, mSize, 0, 100, 100, mark, mMarkScaleColor, mMarkScale);
		screen.DrawPicture(mMarkPosition, mSize, 0, 100, 100, mark, WHITE);
		//�}�[�N�̒���
		if (!isFever && mIsMarkActive) {
			screen.DrawBox({ mPosition.x - 125.0f + mMarkShake.x, mPosition.y + 100 + mMarkShake.y }, mMarkLength * (250.0f / kMarkMaxLength), 25, 0.0f, mMarkColor, kFillModeSolid, true);
		}
		else if (isFever) {
			screen.DrawBox({ mPosition.x - 125.0f + mMarkShake.x, mPosition.y + 100 + mMarkShake.y }, 250.0f, 25, 0.0f, feverGaugeColor, kFillModeSolid, true);
		}
		screen.DrawUI({ mPosition.x + mMarkShake.x, mPosition.y + 87.5f + mMarkShake.y }, 250, 25, 0, 1000, 100, lengthflame, WHITE, { 1.0f,1.0f }, true);
	}

	//�v���C���[�{�̕`��
	screen.DrawPicture(mPosition, mSize, 0, 100, 100, toge, mColor);

	//�X�g���C�N����(��)�`��
	if (mIsMarkActive && !mIsStrikeActive) {
		if (0 < mStrikePower) {
			screen.DrawAnime({ mPosition.x, mPosition.y - (40 / screen.GetZoom()) }, 60 / screen.GetZoom(), 30 / screen.GetZoom(), 0.0f, 0, 200, 100, 2, 20, mMarkFrame, rb, 0xFFFFFFE5);
		} else {
			screen.DrawPicture({ mPosition.x, mPosition.y - (40 / screen.GetZoom()) }, 360, 120, 0.0f, 300, 100, nopower, WHITE);
		}
	}

}
void Player::DrawStrikeUI(Screen& screen, bool isFever, unsigned int feverGaugeColor) {

	//�X�g���C�N�p���[�����߂�
	mStrikePower = Clamp(mStrikePower, 0, kStrikePowerMax);
	if (!isFever) {
		if (mStrikePower != kStrikePowerMax) {
			screen.DrawBox({ 50.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, WHITE, kFillModeSolid, false);
		} else {
			screen.DrawBox({ 50.0f, 62.5f }, 50 * mStrikePower, 25, 0.0f, RED, kFillModeSolid, false);
		}
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

	Novice::ScreenPrintf(0,  0, "mPowerParticleEasingt[0] : %f", mPowerParticleEasingt[0]);
	Novice::ScreenPrintf(0, 20, "mPowerParticleSize[0] : %d", mPowerParticleSize[0]);

}
void Player::LoadTexture() {

	//�摜
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
	powerparticle = Novice::LoadTexture("./Resources/Player/playerfeverparticle.png");
	senden = Novice::LoadTexture("./Resources/Outgame/Title/senden.png");

	//��
	hit = Novice::LoadAudio("./Sounds/hit.wav");
	dush = Novice::LoadAudio("./Sounds/dush.wav");

}


void Player::TitleUpdate(unsigned int BackBoxColor) {

	//�O��|�W�V�������擾����
	mOldPosition = mPosition;

	//���x������������
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
		//�ʏ�ړ�
		NormalMove();

		//�_�b�V��
		Dush(0.5f);

	} else {

		AutoMove();
	}


	//���x��������
	mPosition.x += mVelocity.x;
	mPosition.y -= mVelocity.y;

	//��ʊO�ɏo���Ȃ�
	mPosition.x = Clamp(mPosition.x, 0, Screen::kWindowWidth);
	mPosition.y = Clamp(mPosition.y, 0, Screen::kWindowHeight);

	//�_�b�V�����o
	DushBox(BackBoxColor);

	//�c��
	TitleShadow();
}
void Player::AutoMove() {

	//���X�Ɍ�����ς���
	mDirectionPoint += (mTargetPoint - mPosition) * 0.0002f;

	if (Collision(mPosition, mSize * 0.8f, mTargetPoint, 50.0f)) {
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

	//A�{�^��������
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

	//�c���`��
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

	//�_�b�V�����o
	for (int i = 0; i < kDushBoxMax; i++) {
		if (mIsDushBoxActive[i]) {
			screen.DrawRectAngle(mDushBoxPosition[i], mDushBoxSize[i] / 2.0, mDushBoxSize[i] / 2.0, mDushBoxAngle[i], mDushBoxColor[i], kFillModeWireFrame, false);
		}
	}

	//�v���C���[�{�̕`��
	screen.DrawPicture(mPosition, mSize / 2.0, 0, 100, 100, toge, mColor, { 1.0f, 1.0f }, false);

	//��`�`��
	//screen.DrawPicture(mPosition, 400, 300, 0.0f, 1000, 800, senden, WHITE, { 1.0f,1.0f }, false);

}
