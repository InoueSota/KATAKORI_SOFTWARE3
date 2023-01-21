#include "fever.h"
#include "Novice.h"
#include "Easing.h"

void Fever::Update(Screen& screen) {

	//フィーバーシステム
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

	if (mFeverGauge >= 1000) {
		mFeverGauge = 1000;
		mIsFever = true;
	}

	//フィーバーパーティクル
	for (int i = 0; i < kMaxEnemy; i++) {
		if (particlecreat[i].IsUse == 1) {
			for (int j = 0; j < kMaxParticle; j++) {
				particlecreat[i].particle[j].IsUse = 1;
				particlecreat[i].particle[j].Pos = particlecreat[i].Pos;
				particlecreat[i].IsUse = 2;
			}
		}
	}

	for (int i = 0; i < kMaxEnemy; i++) {
		for (int j = 0; j < kMaxParticle; j++) {
			if (particlecreat[i].particle[j].IsUse == 1) {
				float angle = Degree(RAND(0, 360));
				particlecreat[i].particle[j].StartPos = particlecreat[i].particle[j].Pos;
				particlecreat[i].particle[j].EndPos.x = particlecreat[i].particle[j].Pos.x + cosf(angle) * mParticleRadius + RAND(-100, 100);
				particlecreat[i].particle[j].EndPos.y = particlecreat[i].particle[j].Pos.y + sinf(angle) * mParticleRadius + RAND(-100, 100);
				particlecreat[i].particle[j].IsUse = 2;
			}else if (particlecreat[i].particle[j].IsUse == 2) {
				if (particlecreat[i].particle[j].T < 1) {
					particlecreat[i].particle[j].T += 0.033;
					if (particlecreat[i].particle[j].T > 1) {
						particlecreat[i].particle[j].T = 1;
					}
					float easedT = easeOutSine(particlecreat[i].particle[j].T);

					particlecreat[i].particle[j].Pos.x = ((1 - easedT) * particlecreat[i].particle[j].StartPos.x) + (easedT * particlecreat[i].particle[j].EndPos.x);
					particlecreat[i].particle[j].Pos.y = ((1 - easedT) * particlecreat[i].particle[j].StartPos.y) + (easedT * particlecreat[i].particle[j].EndPos.y);
				} else {
					particlecreat[i].particle[j].IsUse = 3;
					particlecreat[i].particle[j].T = 0;
				}
			} else if (particlecreat[i].particle[j].IsUse == 3) {
				particlecreat[i].particle[j].StartPos = screen.WorldTransform(particlecreat[i].particle[j].Pos);
				particlecreat[i].particle[j].EndPos.x = 50 + 1.18 * mFeverGauge;
				particlecreat[i].particle[j].EndPos.y = 30;
				particlecreat[i].particle[j].IsUse = 4;
			} else if (particlecreat[i].particle[j].IsUse == 4) {
				if (particlecreat[i].particle[j].T < 1) {
					particlecreat[i].particle[j].T += 0.033;
					if (particlecreat[i].particle[j].T > 1) {
						particlecreat[i].particle[j].T = 1;
					}
					float easedT = easeOutSine(particlecreat[i].particle[j].T);

					particlecreat[i].particle[j].Pos.x = ((1 - easedT) * particlecreat[i].particle[j].StartPos.x) + (easedT * particlecreat[i].particle[j].EndPos.x);
					particlecreat[i].particle[j].Pos.y = ((1 - easedT) * particlecreat[i].particle[j].StartPos.y) + (easedT * particlecreat[i].particle[j].EndPos.y);
				} else {
					particlecreat[i].particle[j].IsUse = 0;
					particlecreat[i].particle[j].T = 0;
					particlecreat[i].IsUse = 0;
				}
			}
		}
	}

}

void Fever::Draw(Screen& screen) {
	
	screen.DrawBox({ 49,19 }, 1.18 * 1000, 22, 0.0f, BLACK, kFillModeWireFrame, false);
	screen.DrawBox({ 50,20 }, 1.18 * mFeverGauge, 20, 0.0f, WHITE, kFillModeSolid, false);

	for (int i = 0; i < kMaxEnemy; i++) {
		for (int j = 0; j < kMaxParticle; j++) {
			if (!(particlecreat[i].particle[j].IsUse == 4)) {
				screen.DrawBox(particlecreat[i].particle[j].Pos, 10, 10, 0, WHITE, kFillModeSolid);
			} else {
				screen.DrawBox(particlecreat[i].particle[j].Pos, 10, 10, 0, WHITE, kFillModeSolid, false);
			}
		}
	}
	

	Novice::ScreenPrintf(0, 120, "%f", particlecreat[0].particle[0].EndPos.x);

}