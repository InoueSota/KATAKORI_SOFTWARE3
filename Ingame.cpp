#include "Ingame.h"
#include "Player.h"
#include "Function.h"
#include "Easing.h"
#include "ControllerInput.h"



//�Q�[�����̂t�h
void UI::Init() {

	//�n�߂邩�t���O
	mIsStart = false;
	mIsOldStart = true;
	mStartPosition = { -Screen::kWindowWidth / 2.0 + 200,Screen::kWindowHeight / 2.0 };

	//��ʂ̒��S
	mCenterPosition.x = Screen::kWindowWidth / 2.0;
	mCenterPosition.y = Screen::kWindowHeight / 2.0;

	//��������
	mTimeUISize = 48;
	mTimeLastUISize = 256;
	mTimePosition[0].x = Screen::kWindowWidth / 2.0 + mTimeUISize / 2.0 + 5.0f;
	mTimePosition[0].y = mTimeUISize / 2.0 + 45.0f;
	mTimePosition[1].x = Screen::kWindowWidth / 2.0 - mTimeUISize / 2.0 - 5.0f;
	mTimePosition[1].y = mTimePosition[0].y;
	mTimeLeft = kTimeLimit;
	mTimeElapsed = 0;
	mTimeFrame = 0;
	mTimeLastColor = 0xFFFFFFFF;
	mTimeEasingt = 0.0f;
	mIsTimeUpFinish = false;
	mTimeUpAlphat = 0.0f;
	mTimeUpColor = 0xFFFFFF00;
	
	//�R���{
	mIsComboScaleAnime = false;
	mComboPosition[0].x = Screen::kWindowWidth - mTimeUISize;
	mComboPosition[0].y = 300.0f;
	mComboPosition[1].x = Screen::kWindowWidth - mTimeUISize * 2;
	mComboPosition[1].y = mComboPosition[0].y;
	mComboScale.x = 1.0f;
	mComboScale.y = 1.0f;
	mCombo = 0;

	//�X�R�A
	mScore = 0;
	for (int i = 0; i < 6; i++) {
		mScorePosition[i].x = Screen::kWindowWidth - mTimeUISize * (i + 1);
		mScorePosition[i].y = 100.0f;
	}

	//�������ɓ������Ă��܂����Ƃ�
	mIsWarning = false;
	mIsWarningRed = false;
	mWarningAlphat = 0.0f;
	mWarningColor = 0xFFFFFF00;

	//�������
	mXScale = { 1.0f, 1.0f };
	mXScaleActive = false;
	mAScale = { 1.0f, 1.0f };
	mAScaleActive = false;
}
void UI::Update() {

	//�R���{
	Combo();

	//�������ɓ������Ă��܂����Ƃ�
	Warning();

	//��������̐F�������Ă�Ƃ��͌��点��
	if (Controller::IsPressedButton(0, Controller::bX)) {
		mXColor = WHITE;

		if (Controller::IsTriggerButton(0, Controller::bX)) {
			mXEasingt = 0.0f;
			mXScaleActive = true;
		}
	} else {
		mXColor = 0xFFFFFF50;
	}
	if (Controller::IsPressedButton(0, Controller::bA)) {
		mAColor = WHITE;

		if (Controller::IsTriggerButton(0, Controller::bA)) {
			mAEasingt = 0.0f;
			mAScaleActive = true;
		}
	}
	else {
		mAColor = 0xFFFFFF50;
	}

	//�X�P�[���C�[�W���O
	if (mXScaleActive) {

		mXEasingt = EasingClamp(0.2f, mXEasingt);
		mXScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mXEasingt));

		if (mXEasingt == 1.0f) {
			mXScaleActive = false;
		}
	}
	if (mAScaleActive) {

		mAEasingt = EasingClamp(0.2f, mAEasingt);
		mAScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mAEasingt));

		if (mAEasingt == 1.0f) {
			mAScaleActive = false;
		}
	}
}
void UI::TimeLimit() {

	//�G�̔����������邽�߁A�P�t���[���ł�点��
	mIsOldStart = mIsStart;

	//�t���[����60�ɂȂ������b�o��
	if (mTimeFrame == 60 && mTimeLeft != 0)
	{
		mTimeElapsed++;
		mTimeFrame = 0;

		//�J�E���g�_�E���̏�����
		if (mTimeLeft < 10 || !mIsStart) {
			mTimeEasingt = 0.0f;
			mTimeLastColor = 0xFFFFFF70;
			mTimeLastScale = { 1.0f, 1.0f };
		}
	}

	//���Ԃ̃t���[�������Z����
	mTimeFrame++;

	//�c�莞�� = �������� - �o�ߎ���
	if (!mIsStart) {
		mCountDownTime = kToStart - mTimeElapsed;
	} else {
		mTimeLeft = kTimeLimit - mTimeElapsed;
	}

	//�c�莞�Ԃ̌��E�l�̐ݒ�
	mTimeLeft = Clamp(mTimeLeft, 0, kTimeLimit);
	mCountDownTime = Clamp(mCountDownTime, 0, kToStart);

	//�J�E���g�_�E���̓����x�Ƒ傫�����C�[�W���O��������
	if (mTimeLeft < 10 || !mIsStart) {
		mTimeEasingt = EasingClamp(1.0f / 60.0f, mTimeEasingt);
		mTimeLastColor = ColorEasingMove(0xFFFFFF70, 0xFFFFFF00, easeOutSine(mTimeEasingt));
		mTimeLastScale = EasingMove({ 1.0f, 1.0f }, { 2.0f,2.0f }, easeOutSine(mTimeEasingt));
	}

	//�n�߂�t���O��true�ɂ���
	if (!mIsStart && mCountDownTime == 0) {
		mTimeElapsed = 0;
		mIsStart = true;
	}

	//�X�^�[�g����
	if (mIsStart) {
		mStartPosition.x += 30.0f;
	}

	//�^�C���A�b�v����
	if(mTimeLeft == 0) {

		//�^�C���A�b�v�̕����𓧖��x�C�[�W���O������
		mTimeUpAlphat = EasingClamp(0.1f, mTimeUpAlphat);
		mTimeUpColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutSine(mTimeUpAlphat));

		//�V�[�����X�R�A��ʂɑJ�ڂ�����t���O��true�ɂ���
		if (240 < mTimeFrame) {
			mIsTimeUpFinish = true;
		}
	}
}
void UI::Combo() {

	//�g�k�A�j���[�V����
	if (mIsComboScaleAnime) {
		mComboScale.x -= 0.1f;
		mComboScale.y -= 0.1f;
		mComboScale.x = Clamp(mComboScale.x, 1.0f, 1.8f);
		mComboScale.y = Clamp(mComboScale.y, 1.0f, 1.8f);
		if (mComboScale.x == 1.0f) {
			mIsComboScaleAnime = false;
		}
	}
}
void UI::AddCombo() {

	//�R���{�𑫂�
	mCombo++;
	//�g�k�A�j���[�V����
	mComboScale.x = 1.8f;
	mComboScale.y = 1.8f;
	mIsComboScaleAnime = true;
}
void UI::SnakeScore(bool isStrikeActive, float playerSizeMax) {
	if (isStrikeActive) {
		mScore += kSnakeScore / (playerSizeMax / 80.0f) * (1.0f + (float)mCombo / 10);
	}
	else {
		mScore += kSnakeScore * (1.0f + (float)mCombo / 10);
	}
}
void UI::MissSnakeScore(bool isStrikeActive) {
	if (isStrikeActive) {
		mScore -= kSnakeScore;
	}
	else {
		mScore -= kSnakeScore / 2.0;
	}
}
void UI::TsuchinokoScore(bool isStrikeActive, float playerSizeMax) {
	if (isStrikeActive) {
		mScore += kTsuchinokoScore / (playerSizeMax / 80.0f) * (1.0f + (float)mCombo / 10);
	}
	else {
		mScore += kTsuchinokoScore * (1.0f + (float)mCombo / 10);
	}
}
void UI::MissTsuchinokoScore(bool isStrikeActive) {
	if (isStrikeActive) {
		mScore -= kTsuchinokoScore;
	}
	else {
		mScore -= kTsuchinokoScore / 2.0;
	}
}
void UI::Warning() {

	if (mIsWarning) {
		if (!mIsWarningRed) {
			mWarningAlphat = EasingClamp(0.5f, mWarningAlphat);
			mWarningColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutCirc(mWarningAlphat));
			if (mWarningAlphat == 1.0f) {
				mWarningAlphat = 0.0f;
				mIsWarningRed = true;
			}
		}
		else {
			mWarningAlphat = EasingClamp(0.1f, mWarningAlphat);
			mWarningColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeInSine(mWarningAlphat));
			if (mWarningAlphat == 1.0f) {
				mIsWarning = false;
			}
		}
	}
	else {
		mIsWarningRed = false;
		mWarningAlphat = 0.0f;
		mWarningColor = 0xFFFFFF00;
	}
}
void UI::LoadTexture() {
	if (!mIsLoadTexture) {
		mTimeNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
		mTimeLimitNumber = Novice::LoadTexture("./Resources/UI/Time/timelimit.png");
		mStart = Novice::LoadTexture("./Resources/UI/Time/start.png");
		mTimeUp = Novice::LoadTexture("./Resources/UI/Time/timeup.png");
		mComboLetter = Novice::LoadTexture("./Resources/UI/Combo/combo.png");
		mScoreLetter = Novice::LoadTexture("./Resources/UI/Score/score.png");
		mWarningRed = Novice::LoadTexture("./Resources/Player/warningred.png");
		mRadar = Novice::LoadTexture("./Resources/UI/Minimap/radar.png");
		mStick = Novice::LoadTexture("./Resources/UI/Explanation/stick.png");
		mLStick = Novice::LoadTexture("./Resources/UI/Explanation/lstick.png");
		mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
		mA = Novice::LoadTexture("./Resources/UI/Explanation/a.png");
		mIsLoadTexture = true;
	}
}
void UI::DrawBackTimeLimit(Screen& screen) {

	//�J�E���g�_�E���̕`��
	//�J�n
	if (!mIsStart) {
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mCountDownTime % 10), 288, 288, mTimeLimitNumber, 0xFFFFFF70);
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mCountDownTime % 10), 288, 288, mTimeLimitNumber, mTimeLastColor, mTimeLastScale);
	} else if (mStartPosition.x < (Screen::kWindowWidth + Screen::kWindowWidth / 2.0)) {
		screen.DrawUI(mStartPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mStart, 0xFFFFFFB0);
	}
	if (0 < mTimeLeft && mTimeLeft < 10) {
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, 0xFFFFFF70);
		screen.DrawUI(mCenterPosition, mTimeLastUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, mTimeLastColor, mTimeLastScale);
	}
}
void UI::Draw(Screen& screen) {

	//�������ɓ������Ă��܂����Ƃ�
	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mWarningRed, mWarningColor);

	//��������
	if (10 <= mTimeLeft) {
		screen.DrawUI(mTimePosition[1], mTimeUISize, 288 * (mTimeLeft / 10), 288, 288, mTimeLimitNumber, WHITE);
		screen.DrawUI(mTimePosition[0], mTimeUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, WHITE);
	}
	//�^�C���A�b�v
	if (mTimeLeft == 0) {
		screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mTimeUp, mTimeUpColor);
	}

	//�R���{
	if (10 <= mCombo) {
		screen.DrawUI(mComboPosition[1], mTimeUISize, 32 * (mCombo / 10), 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	if (0 <= mCombo) {
		screen.DrawUI(mComboPosition[0], mTimeUISize, 32 * (mCombo % 10), 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	screen.DrawUI({ mScorePosition[0].x, mComboPosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mComboLetter, WHITE);

	//�X�R�A
	mScore = Clamp(mScore, 0, 1000000);
	float Result[6];
	float tmpScore = mScore;
	for (int i = 5; i > -1; i--) {
		Result[i] = tmpScore / powf(10, i);
		tmpScore = (int)tmpScore % (int)powf(10, i);
		screen.DrawUI(mScorePosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mTimeNumber, WHITE, mComboScale);
	}
	screen.DrawUI({ mScorePosition[0].x, mScorePosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mScoreLetter, WHITE);

	//�~�j�}�b�v
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0x00000080, kFillModeSolid, false);
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0xFFFF00FF, kFillModeWireFrame, false);
	screen.DrawUI(mRadarPosition, 150, 75, 0, 200, 100, mRadar, WHITE);

	//�������
	screen.DrawUI(mStickPosition, 50, 0, 100, 100, mStick, 0x606060FF);
	screen.DrawUI({ mStickPosition.x + LeftStickVelocity(15.0f).x, mStickPosition.y - LeftStickVelocity(15.0f).y}, 50, 0, 160, 160, mLStick, WHITE);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 55 }, 50, 0, 160, 160, mX, mXColor, mXScale);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 110 }, 50, 0, 160, 160, mA, mAColor, mAScale);
}

//�}�b�v
void Map::Init() {

	for (int i = 0; i < kAxisLength; i++)
	{
		mXaxisStartPosition[i].x = -5000;
		mXaxisStartPosition[i].y = (i - kAxisLength / 2) * kLineInterval;
		mXaxisEndPosition[i].x = -mXaxisStartPosition[i].x;
		mXaxisEndPosition[i].y =  mXaxisStartPosition[i].y;
		mYaxisStartPosition[i].x = (i - kAxisLength / 2) * kLineInterval;
		mYaxisStartPosition[i].y = -5000;
		mYaxisEndPosition[i].x =  mYaxisStartPosition[i].x;
		mYaxisEndPosition[i].y = -mYaxisStartPosition[i].y;
	}

	mIsBackGroundColorChange = false;
	mBackGroundColor = 0x589E35FF;

}
void Map::Update(bool isFever, bool isOldFever) {

	if (isFever && !isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x589E35FF;
		mBackGroundEndColor = 0x202020FF;
		mIsBackGroundColorChange = true;
	}
	else if (!isFever && isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x202020FF;
		mBackGroundEndColor = 0x589E35FF;
		mIsBackGroundColorChange = true;
	}

	if (mIsBackGroundColorChange) {
		mBackGroundEasingt = EasingClamp(0.01f, mBackGroundEasingt);
		mBackGroundColor = ColorEasingMove(mBackGroundStartColor, mBackGroundEndColor, easeOutSine(mBackGroundEasingt));

		if (mBackGroundEasingt == 1.0f) {
			mIsBackGroundColorChange = false;
		}
	}

	//for (int i = 0; i < kAxisLength; i++)
	//{
	//	if (XaxisNearPlayer(mYaxisStartPosition[i], mYaxisEndPosition[i], player))
	//	{
	//		mYaxisStartPosition[i].y = player.mPosition.y + (Screen::kWindowHeight / 2);
	//		mYaxisEndPosition[i].y   = player.mPosition.y - (Screen::kWindowHeight / 2);
	//	}
	//	if (YaxisNearPlayer(mXaxisStartPosition[i], mXaxisEndPosition[i], player))
	//	{
	//		mXaxisStartPosition[i].x = player.mPosition.x + (Screen::kWindowWidth / 2);
	//		mXaxisEndPosition[i].x   = player.mPosition.x - (Screen::kWindowWidth / 2);
	//	}
	//}

}
bool Map::XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player) {

	float left  = player.mPosition.x - (Screen::kWindowWidth / 2);
	float right = player.mPosition.x + (Screen::kWindowWidth / 2);
	if (left < startposition.x && endposition.x < right) {
		return true;
	}
	return false;
}
bool Map::YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player) {

	float top    = player.mPosition.y + (Screen::kWindowHeight / 2);
	float bottom = player.mPosition.y - (Screen::kWindowHeight / 2);
	if (bottom < startposition.y && endposition.y < top) {
		return true;
	}
	return false;
}
void Map::Draw(Screen& screen) {

	//�w�i�`��
	Novice::DrawBox(0, 0, Screen::kWindowWidth, Screen::kWindowHeight, 0.0, mBackGroundColor, kFillModeSolid);

	for (int i = 0; i < kAxisLength; i++)
	{
		screen.DrawLine(mXaxisStartPosition[i], mXaxisEndPosition[i]);
		screen.DrawLine(mYaxisStartPosition[i], mYaxisEndPosition[i]);
	}

	screen.DrawLine({ kMapLeft, kMapTop }, { kMapRight, kMapTop }, BLACK);
	screen.DrawLine({ kMapLeft, kMapTop }, { kMapLeft, kMapBottom }, BLACK);
	screen.DrawLine({ kMapLeft, kMapBottom }, { kMapRight, kMapBottom }, BLACK);
	screen.DrawLine({ kMapRight, kMapTop }, { kMapRight, kMapBottom }, BLACK);
}