#include "Ingame.h"
#include "Player.h"
#include "Function.h"



//�Q�[�����̂t�h
void UI::Init() {

	//��������
	mTimeUISize = 48;
	mTimePosition[0].x = Screen::kWindowWidth / 2.0 - mTimeUISize / 2.0 - 5.0f;
	mTimePosition[0].y = mTimeUISize / 2.0 + 5.0f;
	mTimePosition[1].x = Screen::kWindowWidth / 2.0 + mTimeUISize / 2.0 + 5.0f;
	mTimePosition[1].y = mTimePosition[0].y;
	mTimeLeft = kTimeLimit;
	mTimeFrame = 0;
	
	//�R���{
	mIsCombo = false;
	mIsComboScaleAnime = false;
	mComboPosition.x = Screen::kWindowWidth - mTimeUISize / 2.0 - 5.0f;
	mComboPosition.y = 100.0f;
	mComboScale.x = 1.0f;
	mComboScale.y = 1.0f;
	mCombo = 0;
	mComboCoolTime = 0;

}
void UI::Update() {

	//��������
	TimeLimit();

	//�R���{
	Combo();
}
void UI::TimeLimit() {

	//�t���[����60�ɂȂ������b�o��
	if (mTimeFrame == 60)
	{
		mTimeElapsed++;
		mTimeFrame = 0;
	}

	//���Ԃ̃t���[�������Z����
	mTimeFrame++;

	//�c�莞�� = �������� - �o�ߎ���
	mTimeLeft = kTimeLimit - mTimeElapsed;
}
void UI::Combo() {

	//�R���{�𑫂�
	if (mIsCombo) {
		mCombo++;
		mComboCoolTime = 0;
		//�g�k�A�j���[�V����
		mComboScale.x = 2.0f;
		mComboScale.y = 2.0f;
		mIsComboScaleAnime = true;
		mIsCombo = false;
	}

	//�N�[���^�C�������Z����
	mComboCoolTime++;

	//�R���{�̏�����
	if (mComboCoolTime > kComboCoolTime) {
		mCombo = 0;
	}

	//�g�k�A�j���[�V����
	if (mIsComboScaleAnime) {
		mComboScale.x -= 0.2f;
		mComboScale.y -= 0.2f;
		mComboScale.x = Clamp(mComboScale.x, 1.0f, 2.0f);
		mComboScale.y = Clamp(mComboScale.y, 1.0f, 2.0f);
		if (mComboScale.x == 1.0f) {
			mIsComboScaleAnime = false;
		}
		
	}

}
void UI::Draw(Screen& screen) {

	if (!mIsLoadTexture) {
		mTimeNumber = Novice::LoadTexture("./Resources/UI/Time/number.png");
		mIsLoadTexture = true;
	}

	//��������
	//10�̈�
	screen.DrawUI(mTimePosition[0], mTimeUISize, 32 * (mTimeLeft / 10), 32, 32, mTimeNumber, WHITE);
	//1�̈�
	screen.DrawUI(mTimePosition[1], mTimeUISize, 32 * (mTimeLeft % 10), 32, 32, mTimeNumber, WHITE);

	//�R���{
	//1�̈�
	screen.DrawUI(mComboPosition, mTimeUISize, 32 * mCombo, 32, 32, mTimeNumber, WHITE, mComboScale);

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

}
void Map::Update(Player& player) {

	for (int i = 0; i < kAxisLength; i++)
	{
		if (XaxisNearPlayer(mYaxisStartPosition[i], mYaxisEndPosition[i], player))
		{
			mYaxisStartPosition[i].y = player.mPosition.y + (Screen::kWindowHeight / 2);
			mYaxisEndPosition[i].y   = player.mPosition.y - (Screen::kWindowHeight / 2);
		}
		if (YaxisNearPlayer(mXaxisStartPosition[i], mXaxisEndPosition[i], player))
		{
			mXaxisStartPosition[i].x = player.mPosition.x + (Screen::kWindowWidth / 2);
			mXaxisEndPosition[i].x   = player.mPosition.x - (Screen::kWindowWidth / 2);
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

	Novice::DrawBox(0, 0, Screen::kWindowWidth, Screen::kWindowHeight, 0.0, 0x589E35FF, kFillModeSolid);

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