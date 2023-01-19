#include "fever.h"
#include "Novice.h"

void Fever::Update() {

	if (mIsFever) {
		mFeverGauge -= 1;
		if (mFeverGauge < 0) {
			mIsFever = false;
			mFeverGauge = 0;
		}
	} else {
		mFeverGauge += mSnakeDefeat * 100;
		mFeverGauge += mTsuchinokoDefeat * 100;
	}

	if (mFeverGauge > 1000) {
		mIsFever = true;
	}

}

void Fever::Draw() {

	Novice::ScreenPrintf(0, 120, "%d", mFeverGauge);

}