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
	mSize = 64;
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

	//�X�g���C�N���o
	for (int i = 0; i < kStrikeLineMax; i++) {
		mIsStrikeLineActive[i] = false;
	}

	//�c��
	mIsShadowActive = false;
}



void Player::Update(Screen& screen) {

	//�O��|�W�V�������擾����
	mOldPosition = mPosition;

	//���x������������
	mVelocity.setZero();

	//�X�g���C�N�����Ă��Ȃ����ɉ\
	if (!mIsStrikeActive) {

		//�ʏ�ړ�
		NormalMove();

		//�_�b�V��
		Dush();

		//�}�[�L���O
		Mark();

		//���x��������
		mPosition += mVelocity;
	}

	//�X�g���C�N
	Strike();
	StrikeLine(screen);

	//�}�b�v���Ɏ��߂�
	mPosition.x = Clamp(mPosition.x, Map::kMapLeft + (mSize / 2), Map::kMapRight - (mSize / 2));
	mPosition.y = Clamp(mPosition.y, Map::kMapBottom + (mSize / 2), Map::kMapTop - (mSize / 2));

	//�c������
	Shadow();



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
	if (!mIsMarkActive && Controller::IsTriggerButton(0,Controller::bX)){

		mMarkPosition = mPosition;
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
void Player::Strike() {

	if (mIsMarkActive && !mIsStrikeActive){

		//LT�{�^����RT�{�^�������������Ƀt���O��true�ɂ���
		if (Controller::IsPressedButton(0, Controller::lTrigger) && Controller::IsPressedButton(0, Controller::rTrigger)) {

			//�����ɉ����ăC�[�W���O�̑����ʂ�ω�������
			//�}�[�N�Ƃ̋��������߂�
			float tmpDistance = (mMarkPosition - mPosition).Length();

			//������0�������璆�~����
			if (tmpDistance == 0){
				mIsMarkActive = false;
				mIsStrikeActive = false;
			}
			//������0����Ȃ��Ƃ�
			else {
				//�����ʂ������ɉ����ĕς��邽�߂̌v�Z
				float tmpValue = tmpDistance / 100;

				// n / tmpValue �̂Ƃ��An�͋�����100�̂Ƃ���Easingt�̑����ʂɂȂ�
				mStrikeEasingtIncrementValue = 0.25f / tmpValue;

				//�C�[�W���O���̎n�_�ƏI�_�̐ݒ�
				mStrikeStartPosition = mPosition;
				mStrikeEndPosition = mMarkPosition;

				//�t���O��true�ɂ���
				mIsStrikeActive = true;
			}
		}
	}

	if (mIsStrikeActive){

		//�C�[�W���O�ړ�
		mStrikeEasingt = EasingClamp(mStrikeEasingtIncrementValue, mStrikeEasingt);
		mPosition = EasingMove(mStrikeStartPosition, mStrikeEndPosition, easeInSine(mStrikeEasingt));

		//�ړ����I��������
		if (mStrikeEasingt == 1.0f){
			mIsMarkActive = false;
			mIsStrikeActive = false;
		}

	}

	//������
	if (!mIsStrikeActive){

		mStrikeEasingt = 0.0f;

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
void Player::Shadow() {

	//A�{�^��������
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



void Player::Draw(Screen& screen) {

	//�c���`��
	for (int i = 0; i < kShadowMax; i++){

		screen.DrawSquare(mShadowPosition[i], mSize, mShadowColor);

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
		screen.DrawRectAngle(mStrikeLinePosition[i], mStrikeLineWidth[i], mStrikeLineHeight[i], mStrikeLineAngle, mStrikeLineColor[i]);
	}

}
