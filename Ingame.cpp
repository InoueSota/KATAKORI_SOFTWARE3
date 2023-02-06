#include "Ingame.h"
#include "Player.h"
#include "Function.h"
#include "Easing.h"
#include "ControllerInput.h"



//�Q�[�����̂t�h
void UI::Init() {

	//�n�߂邩�t���O
	mIsReady = false;
	mIsStart = false;
	mIsOldStart = true;
	mStartPosition = { -Screen::kWindowWidth / 2.0 + 200,Screen::kWindowHeight / 2.0 };

	//��ʂ̒��S
	mCenterPosition.x = Screen::kWindowWidth / 2.0;
	mCenterPosition.y = Screen::kWindowHeight / 2.0;

	//��������
	mIsExtendTime = false;
	mTimeScale = { 1.0f, 1.0f };
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
	mIsKillCountScaleAnimation = false;
	mKillCountPosition[0].x = Screen::kWindowWidth - mTimeUISize;
	mKillCountPosition[0].y = 300.0f;
	mKillCountPosition[1].x = Screen::kWindowWidth - mTimeUISize * 2;
	mKillCountPosition[1].y = mKillCountPosition[0].y;
	mKillCountPosition[2].x = Screen::kWindowWidth - mTimeUISize * 3;
	mKillCountPosition[2].y = mKillCountPosition[0].y;
	mKillCountScale.x = 1.0f;
	mKillCountScale.y = 1.0f;
	mKillCount = 0;

	//�X�R�A
	mScore = 0;
	for (int i = 0; i < 7; i++) {
		mScorePosition[i].x = Screen::kWindowWidth - mTimeUISize * (i + 1);
		mScorePosition[i].y = 100.0f;
	}
	for (int i = 0; i < kEnemyScoreMax; i++) {
		mIsEnemyScoreActive[i] = false;
	}
	mScoreAnimationEasingt = 0.0f;
	mStartScore = 0.0f;
	mEndScore = 0.0f;

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

	//�O�񏀔������t���O�̕ۑ�
	mIsOldReady = mIsReady;

	//����m�F�t�F�[�Y���I�����邩
	if (!mIsReady) {
		if (Controller::IsTriggerButton(0, Controller::bY)) {
			mIsReady = true;
		}
	}

	//�R���{
	KillCount();

	//�X�R�A�̃A�j���[�V����
	ScoreAnimation();

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
	if (LeftStickVelocity(15.0f).x != 0.0f) {
		mLStickColor = WHITE;
		mLStickBackColor = 0x606060FF;
	} else {
		mLStickColor = 0xFFFFFF40;
		mLStickBackColor = 0x60606040;
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
void UI::ExtendTime(bool isFever, bool isOldFever) {

	//�t�B�[�o�[�ɓ������玞�Ԃ����΂�
	if (isFever && !isOldFever) {
		mExtendTimeEasingt = 0.0f;
		mIsExtendTime = true;
		mTimeElapsed -= 5;
	}

	//�t�B�[�o�[�Ŏ��Ԃ����т���C�[�W���O
	if (mIsExtendTime) {
		mExtendTimeEasingt = EasingClamp(0.01f, mExtendTimeEasingt);
		mTimeScale = EasingMove({ 1.5f, 1.5f }, { 1.0f, 1.0f }, easeOutSine(mExtendTimeEasingt));

		if (mExtendTimeEasingt == 1.0f) {
			mIsExtendTime = false;
		}
	}
}
void UI::TimeLimit() {

	//�G�̔����������邽�߁A�P�t���[���ł�点��
	mIsOldStart = mIsStart;

	if (mIsReady) {

		//�t���[����60�ɂȂ������b�o��
		if (mTimeFrame == 60 && mTimeLeft != 0) {

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
		}
		else {
			mTimeLeft = kTimeLimit - mTimeElapsed;
		}

		//�c�莞�Ԃ̌��E�l�̐ݒ�
		mTimeLeft = Clamp(mTimeLeft, 0, kTimeLimit);
		mTimeElapsed = Clamp(mTimeElapsed, 0, kTimeLimit);
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
		if (mTimeLeft == 0) {

			//�^�C���A�b�v�̕����𓧖��x�C�[�W���O������
			mTimeUpAlphat = EasingClamp(0.1f, mTimeUpAlphat);
			mTimeUpColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutSine(mTimeUpAlphat));

			//�V�[�����X�R�A��ʂɑJ�ڂ�����t���O��true�ɂ���
			if (240 < mTimeFrame) {
				mIsTimeUpFinish = true;
			}
		}
	}


}
void UI::KillCount() {

	//�G�̈ʒu�ɕ\�����Ă���X�R�A������
	for (int i = 0; i < kEnemyScoreMax; i++) {
		if (mIsEnemyScoreActive[i]) {
			mEnemyScoreLife[i]--;
			mEnemyScoreLife[i] = Clamp(mEnemyScoreLife[i], 0, 180);
			if (mEnemyScoreLife[i] == 0) {
				mIsEnemyScoreActive[i] = false;
			}
		}
	}

	//�g�k�A�j���[�V����
	if (mIsKillCountScaleAnimation) {
		mKillCountScale.x -= 0.1f;
		mKillCountScale.y -= 0.1f;
		mKillCountScale.x = Clamp(mKillCountScale.x, 1.0f, 1.8f);
		mKillCountScale.y = Clamp(mKillCountScale.y, 1.0f, 1.8f);
		if (mKillCountScale.x == 1.0f) {
			mIsKillCountScaleAnimation = false;
		}
	}
}
void UI::AddKillCount() {

	//�R���{�𑫂�
	mKillCount++;
	//�g�k�A�j���[�V����
	mKillCountScale.x = 1.8f;
	mKillCountScale.y = 1.8f;
	mIsKillCountScaleAnimation = true;
}
void UI::SnakeScore(bool isStrikeActive, Vec2 enemyPosition) {

	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kSnakeScore * 2.5) * (1.0f + (float)mKillCount / 10);
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = kSnakeScore * (1.0f + (float)mKillCount / 10);;
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::TsuchinokoScore(bool isStrikeActive, Vec2 enemyPosition) {
	if (isStrikeActive) {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = (kTsuchinokoScore * 2.5) * (1.0f + (float)mKillCount / 10);
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < kEnemyScoreMax; i++) {
			if (!mIsEnemyScoreActive[i]) {
				mEnemyScore[i] = kTsuchinokoScore * (1.0f + (float)mKillCount / 10);;
				mEndScore += mEnemyScore[i];
				mStartScore = mScore;
				mScoreAnimationEasingt = 0.0f;
				mEnemyScorePosition[i] = enemyPosition;
				mEnemyScoreLife[i] = 180;
				mIsEnemyScoreActive[i] = true;
				break;
			}
		}
	}
}
void UI::ScoreAnimation() {

	mScoreAnimationEasingt = EasingClamp(0.1f, mScoreAnimationEasingt);
	mScore = EasingMove(mStartScore, mEndScore, easeInSine(mScoreAnimationEasingt));

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
void UI::DrawBackTimeLimit(Screen& screen) {

	//�J�E���g�_�E���̕`��
	//�J�n
	if (!mIsStart && mIsReady) {
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
void UI::Draw(Screen& screen, bool mIsReady) {

	//�������ɓ������Ă��܂����Ƃ�
	screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mWarningRed, mWarningColor);

	//��������
	if (10 <= mTimeLeft && mIsStart) {
		screen.DrawUI(mTimePosition[1], mTimeUISize, 288 * (mTimeLeft / 10), 288, 288, mTimeLimitNumber, WHITE, mTimeScale);
		screen.DrawUI(mTimePosition[0], mTimeUISize, 288 * (mTimeLeft % 10), 288, 288, mTimeLimitNumber, WHITE, mTimeScale);
	}
	//�^�C���A�b�v
	if (mTimeLeft == 0) {
		screen.DrawUI(mCenterPosition, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, mTimeUp, mTimeUpColor);
	}

	//�L���J�E���g
	if (mIsReady) {

		//�X�R�A�̌����傫�������͐F�𔖂�����
		mIsDarkKillCount = false;
		for (int i = 0; i < 3; i++) {
			mKillCountColor[i] = 0xFFFFFF50;
		}

		mKillCount = Clamp(mKillCount, 0, 1000);
		float Result[3];
		float tmpScore = mKillCount;
		for (int i = 2; i > -1; i--) {
			Result[i] = tmpScore / powf(10, i);
			tmpScore = (int)tmpScore % (int)powf(10, i);
			if ((int)Result[i] != 0) {
				mIsDarkKillCount = true;
			}
			if (mIsDarkKillCount) {
				mKillCountColor[i] = WHITE;
			} else {
				mKillCountColor[i] = 0xFFFFFF50;
			}
			screen.DrawUI(mKillCountPosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mKillCountColor[i], mKillCountScale);
		}
		screen.DrawUI({ mScorePosition[0].x - 50.0f, mKillCountPosition[0].y + 50.0f }, 200, 50, 0, 400, 100, mKillCountLetter, WHITE);
	}

	//�G�X�R�A
	for (int j = 0; j < kEnemyScoreMax; j++) {
		if (mIsEnemyScoreActive[j]) {
			float Result[7];
			float tmpScore = mEnemyScore[j];
			for (int i = 6; i > -1; i--) {
				Result[i] = tmpScore / powf(10, i);
				tmpScore = (int)tmpScore % (int)powf(10, i);
				if (powf(10, i) <= mEnemyScore[j]) {
					screen.DrawUI({ mEnemyScorePosition[j].x - (i * 24) / screen.GetZoom(), mEnemyScorePosition[j].y }, 24 / screen.GetZoom(), 32 * (int)Result[i], 32, 32, mNumber, WHITE, { 1.0f,1.0f }, true);
				}
			}
		}

	}
	//�X�R�A
	if (mIsReady) {

		//�X�R�A�̌����傫�������͐F�𔖂�����
		mIsDarkScore = false;
		for (int i = 0; i < 7; i++) {
			mScoreColor[i] = 0xFFFFFF50;
		}

		mScore = Clamp(mScore, 0, 10000000);
		float Result[7];
		float tmpScore = mScore;
		for (int i = 6; i > -1; i--) {
			Result[i] = tmpScore / powf(10, i);
			tmpScore = (int)tmpScore % (int)powf(10, i);
			if ((int)Result[i] != 0) {
				mIsDarkScore = true;
			}
			if (mIsDarkScore) {
				mScoreColor[i] = WHITE;
			} else {
				mScoreColor[i] = 0xFFFFFF50;
			}
			screen.DrawUI(mScorePosition[i], mTimeUISize, 32 * (int)Result[i], 32, 32, mNumber, mScoreColor[i], mKillCountScale);
		}
		screen.DrawUI({ mScorePosition[0].x, mScorePosition[0].y + 50.0f }, 100, 50, 0, 200, 100, mScoreLetter, WHITE);
	}

	//�~�j�}�b�v
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0x00000080, kFillModeSolid, false);
	screen.DrawSquare(mMiniMapPosition, Screen::kMiniMapSize * 2, 0xFFFF00FF, kFillModeWireFrame, false);
	screen.DrawUI(mRadarPosition, 150, 75, 0, 200, 100, mRadar, WHITE);

	//�������
	screen.DrawUI(mStickPosition, 50, 0, 100, 100, mStick, mLStickBackColor);
	screen.DrawUI({ mStickPosition.x + LeftStickVelocity(15.0f).x, mStickPosition.y - LeftStickVelocity(15.0f).y}, 50, 0, 160, 160, mLStick, mLStickColor);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 55 }, 50, 0, 160, 160, mX, mXColor, mXScale);
	screen.DrawUI({ mStickPosition.x, mStickPosition.y - 110 }, 50, 0, 160, 160, mA, mAColor, mAScale);
}
void UI::LoadTexture() {

	mNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
	mTimeLimitNumber = Novice::LoadTexture("./Resources/UI/Time/timelimit.png");
	mStart = Novice::LoadTexture("./Resources/UI/Time/start.png");
	mTimeUp = Novice::LoadTexture("./Resources/UI/Time/timeup.png");
	mKillCountLetter = Novice::LoadTexture("./Resources/UI/Killcount/killcount.png");
	mScoreLetter = Novice::LoadTexture("./Resources/UI/Score/score.png");
	mWarningRed = Novice::LoadTexture("./Resources/Player/warningred.png");
	mRadar = Novice::LoadTexture("./Resources/UI/Minimap/radar.png");
	mStick = Novice::LoadTexture("./Resources/UI/Explanation/stick.png");
	mLStick = Novice::LoadTexture("./Resources/UI/Explanation/lstick.png");
	mX = Novice::LoadTexture("./Resources/UI/Explanation/x.png");
	mA = Novice::LoadTexture("./Resources/UI/Explanation/a.png");
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
	mBackGroundColor = 0xDADADAFF;
	mBackLineColor = 0x202020FF;

	for (int i = 0; i < kBackBoxMax; i++) {
		mIsBackBoxActive[i] = false;
	}
	mBackBoxFrame = 0;
}
void Map::Update(bool isFever, bool isOldFever, unsigned int feverColor) {

	if (isFever && !isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0xDADADAFF;
		mBackGroundEndColor = 0x202020FF;
		mIsBackGroundColorChange = true;
	}
	else if (!isFever && isOldFever) {
		mBackGroundEasingt = 0.0f;
		mBackGroundStartColor = 0x202020FF;
		mBackGroundEndColor = 0xDADADAFF;
		mIsBackGroundColorChange = true;
	}

	if (mIsBackGroundColorChange) {
		mBackGroundEasingt = EasingClamp(0.01f, mBackGroundEasingt);
		mBackGroundColor = ColorEasingMove(mBackGroundStartColor, mBackGroundEndColor, easeOutSine(mBackGroundEasingt));

		if (mBackGroundEasingt == 1.0f) {
			mIsBackGroundColorChange = false;
		}
	}

	//�w�i��`�t���[���̉��Z
	mBackBoxFrame++;

	for (int i = 0; i < kBackBoxMax; i++) {

		//�w�i��`����
		if (!mIsBackBoxActive[i] && (mBackBoxFrame % 3 == 0 || mBackBoxFrame % 3 == 1)) {
			mBackBoxPosition[i].x = RAND(kMapLeft + 200, kMapRight - 200);
			mBackBoxPosition[i].y = RAND(kMapBottom + 200, kMapTop - 200);
			mBackBoxAngle[i] = RAND(Degree(0), Degree(360));
			mBackBoxEndSize[i] = RAND(8, 12) * 20;
			mBackBoxEasingt[i] = 0.0f;
			mIsBackBoxEasingClear[i] = false;
			mIsBackBoxActive[i] = true;
			break;
		}

		//�w�i��`����
		if (mIsBackBoxActive[i]) {
			mBackBoxAngle[i] += Degree(2);
			if (!mIsBackBoxEasingClear[i]) {
				mBackBoxEasingt[i] = EasingClamp(0.02f, mBackBoxEasingt[i]);
				mBackBoxSize[i] = EasingMove(0.0f, mBackBoxEndSize[i], easeOutSine(mBackBoxEasingt[i]));
				if (mBackBoxEasingt[i] == 1.0f) {
					mBackBoxEasingt[i] = 0.0f;
					mIsBackBoxEasingClear[i] = true;
				}
			} else {
				mBackBoxEasingt[i] = EasingClamp(0.1f, mBackBoxEasingt[i]);
				mBackBoxSize[i] = EasingMove(mBackBoxEndSize[i], 0.0f, easeInSine(mBackBoxEasingt[i]));
				if (mBackBoxEasingt[i] == 1.0f) {
					mIsBackBoxActive[i] = false;
				}
			}
		}
	}

	//�w�i��`�̐F��ς���
	if (isFever) {
		mBackLineColor = feverColor;
		for (int i = 0; i < kBackBoxMax; i++) {
			mBackBoxColor = feverColor;
		}
	} else {
		mBackLineColor = BLACK;
		for (int i = 0; i < kBackBoxMax; i++) {
			mBackBoxColor = BLACK;
		}
	}

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

	for (int i = 0; i < kBackBoxMax; i++) {
		screen.DrawRectAngle(mBackBoxPosition[i], mBackBoxSize[i], mBackBoxSize[i], mBackBoxAngle[i], mBackBoxColor, kFillModeWireFrame, true);
	}

	screen.DrawRectAngle({ kMapLeft, 0 }, 10, kMapTop * 2, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ kMapRight, 0 }, 10, kMapTop * 2, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ 0, kMapTop }, kMapLeft * 2, 10, 0.0f, mBackLineColor);
	screen.DrawRectAngle({ 0, kMapBottom }, kMapLeft * 2, 10, 0.0f, mBackLineColor);

}