#include "Fever.h"
#include "Novice.h"
#include "Easing.h"
#include "Function.h"
#include "Key.h"



void Fever::Init() {
	mSnakeDefeat = 0;
	mTsuchinokoDefeat = 0;
	mFeverGauge = 0;
	mIsFever = false;
	mIsOldFever = false;
	feverGauge.Pos = {};
	mSnakeDefeat = 0;
	mSnakeDefeatStrike = 0;
	mTsuchinokoDefeatStrike = 0;
	mFeverGaugeStrikeEndFlag = 2;
	mFeverStop = false;
}


void Fever::Update(Screen& screen, Vec2 PlayerPos, bool isStrikeActive) {

	//前回のフィーバーフラグを取得
	mIsOldFever = mIsFever;

	//フィーバーシステム
	if (mIsFever) {
		if (!mFeverStop) {
			mFeverGauge--;
		}
		if (mFeverGauge < 0 && !isStrikeActive) {
			mIsFever = false;
			mFeverGauge = 0;
		}
	} else {
		if (mSnakeDefeat || mTsuchinokoDefeat) {
			mFeverGauge += mSnakeDefeat * 30;
			mFeverGauge += mTsuchinokoDefeat * 30;
			feverGauge.Flag = 1;
			feverGauge.Timer = kfeverGaugeDelayTime;
		}
		
		if (mFeverGaugeStrikeEndFlag == 2) {
			int DefeatStrike = mTsuchinokoDefeatStrike + mSnakeDefeatStrike;
			if (!(DefeatStrike < 2)) {
				mFeverGauge += DefeatStrike * 30 * (1 + (DefeatStrike * 0.2));
			}
			mSnakeDefeatStrike = 0;
			mTsuchinokoDefeatStrike = 0;
			mFeverGaugeStrikeEndFlag = 0;
			feverGauge.Flag = 1;
		}
	}

	if (mFeverGauge >= 1000 || Key::IsTrigger(DIK_F)) {
		mFeverGauge = 1000;
		mIsFever = true;
	}

	if (feverGauge.Flag == 1) {
		feverGauge.StartPos = feverGauge.Pos;
		feverGauge.EndPos.x = 50 + 1.18 * mFeverGauge;
		feverGauge.Flag = 2;
	} else if (feverGauge.Flag == 2) {
		feverGauge.Timer--;
		if (!feverGauge.Timer) {
			feverGauge.Flag = 3;
		}
	}else if (feverGauge.Flag == 3) {
		if (feverGauge.T < 1) {
			feverGauge.T += 0.033;
			if (feverGauge.T > 1) {
				feverGauge.T = 1;
			}
			float easedT = easeOutSine(feverGauge.T);

			feverGauge.Pos.x = ((1 - easedT) * feverGauge.StartPos.x) + (easedT * feverGauge.EndPos.x);
			
		} else {
			feverGauge.Flag = 0;
			feverGauge.T = 0;
			feverGauge.Timer = kfeverGaugeDelayTime;
		}
	}
	if (mIsFever) {
		feverGauge.Pos.x = 50 + 1.18 * mFeverGauge;

		feverGauge.R += feverGauge.Rnum;
		feverGauge.G += feverGauge.Gnum;
		feverGauge.B += feverGauge.Bnum;

		if (feverGauge.R > 255) {
			feverGauge.R = 255;
		}
		if (feverGauge.R < 0) {
			feverGauge.R = 0;
		}
		if (feverGauge.G > 255) {
			feverGauge.G = 255;
		}
		if (feverGauge.G < 0) {
			feverGauge.G = 0;
		}
		if (feverGauge.B > 255) {
			feverGauge.B = 255;
		}
		if (feverGauge.B < 0) {
			feverGauge.B = 0;
		}

		if (feverGauge.R <= 0 || feverGauge.R >= 255) {
			feverGauge.Rnum *= -1;
		}
		if (feverGauge.G <= 0 || feverGauge.G >= 255) {
			feverGauge.Gnum *= -1;
		}
		if (feverGauge.B <= 0 || feverGauge.B >= 255) {
			feverGauge.Bnum *= -1;
		}
		feverGauge.color = (feverGauge.R * pow(16, 6)) + (feverGauge.G * pow(16, 4)) + (feverGauge.B * pow(16, 2)) + feverGauge.alpha;

	}

	if (mIsFever) {
		for (int i = 0; i < kMaxPlayerFeverParticle; i++) {
			if (!playerFeverParticle[i].IsUse) {
				playerFeverParticle[i].Pos = PlayerPos;
				playerFeverParticle[i].IsUse = 1;
				playerFeverParticle[i].Timer = kPlayerFeverParticleTime;
				break;
			}
		}
		for (int i = 0; i < kMaxPlayerFeverParticle; i++) {
			if (playerFeverParticle[i].IsUse) {
				playerFeverParticle[i].velocity += 0.2;
				playerFeverParticle[i].Pos.y += playerFeverParticle[i].velocity;
				playerFeverParticle[i].Pos.x += RAND(-5, 5);
				playerFeverParticle[i].alpha -= 2;
				if (playerFeverParticle[i].alpha <= 0) {
					playerFeverParticle[i].alpha = 0;
				}
				playerFeverParticle[i].R -= 16;
				playerFeverParticle[i].G -= 4;
				playerFeverParticle[i].B -= 8;
				if (playerFeverParticle[i].R <= 20) {
					playerFeverParticle[i].R = 20;
				}
				if (playerFeverParticle[i].G <= 30) {
					playerFeverParticle[i].G = 15;
				}
				if (playerFeverParticle[i].B <= 15) {
					playerFeverParticle[i].B = 15;
				}
				playerFeverParticle[i].Timer--;
				playerFeverParticle[i].color = (playerFeverParticle[i].R * pow(16, 6)) + (playerFeverParticle[i].G * pow(16, 4)) + (playerFeverParticle[i].B * pow(16, 2)) + (playerFeverParticle[i].alpha / 10);
				if (!playerFeverParticle[i].Timer) {
					playerFeverParticle[i] = DefaultParticle;
				}
			}
		}
	}


	//フィーバーパーティクル
	for (int i = 0; i < kMaxEnemy; i++) {
		if (particlecreat[i].IsUse == 1) {
			for (int j = 0; j < kMaxParticle; j++) {
				particlecreat[i].particle[j].IsUse = 1;
				particlecreat[i].particle[j].Pos = particlecreat[i].Pos;
				particlecreat[i].IsUse = 2;
				int Defeat = mTsuchinokoDefeatStrike + mSnakeDefeatStrike;
				if (Defeat == 1) {
					particlecreat[i].particle[j].color = 0xFF1919FF;
				} else if (Defeat == 2) {
					particlecreat[i].particle[j].color = 0xFF7C19FF;
				} else if (Defeat == 3) {
					particlecreat[i].particle[j].color = 0xFFE819FF;
				} else if (Defeat == 4) {
					particlecreat[i].particle[j].color = 0x92E819FF;
				} else if (Defeat == 5) {
					particlecreat[i].particle[j].color = 0x919FEBFF;
				} else{
					particlecreat[i].particle[j].color = 0xDF4AEBFF;
				}
					
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

	if (!mIsLoadTexture) {
		PlayerFeverParticleTexture = Novice::LoadTexture("./Resources/Player/playerfeverparticle.png");
		FeverUITexture = Novice::LoadTexture("./Resources/UI/Fever/fever.png");
		FeverEndWarning = Novice::LoadTexture("./Resources/UI/Fever/feverendwarning.png");
		mIsLoadTexture = true;
	}

	/*if (mIsFever && mFeverGauge < 100) {
		mWarningAlphat = EasingClamp(0.5f, mWarningAlphat);
		mWarningColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutCirc(mWarningAlphat));
		if (mWarningAlphat == 1.0f) {
			mWarningAlphat = 0.0f;
			mIsWarningRed = true;
		}
		screen.DrawUI({ 640, 360 }, Screen::kWindowWidth, Screen::kWindowHeight, 0, Screen::kWindowWidth, Screen::kWindowHeight, FeverEndWarning, mWarningColor);
	} else {
		mWarningAlphat = 0.0f;
		mWarningColor = 0xFFFFFF00;
	}*/
	
	mFeverGauge = Clamp(mFeverGauge, 0, 1000);
	screen.DrawBox({ 49,14 }, 1.18 * 1000, 22, 0.0f, BLACK, kFillModeWireFrame, false);
	screen.DrawBox({ 50,15 }, 1.18 * mFeverGauge, 20, 0.0f, 0xFFCFCFFF, kFillModeSolid, false);
	if (!(feverGauge.Pos.x <= 50)) {
		if (mIsFever) {
			screen.DrawBox({ 50,15 }, feverGauge.Pos.x - 50, 20, 0, feverGauge.color, kFillModeSolid, false);
			Novice::SetBlendMode(kBlendModeAdd);
			Novice::DrawQuad(30, 0, feverGauge.Pos.x + 20, 0, 30, 50, feverGauge.Pos.x + 20, 50, 0, 0, 100, 100, PlayerFeverParticleTexture, feverGauge.color);
			Novice::SetBlendMode(kBlendModeNormal);
		} else {
			screen.DrawBox({ 50,15 }, feverGauge.Pos.x - 50, 20, 0, 0xFF000099, kFillModeSolid, false);
		}
	}
	Novice::DrawSprite(55, 15, FeverUITexture, 1, 1, 0, WHITE);

	if (mIsFever) {
		for (int i = 0; i < kMaxPlayerFeverParticle; i++) {
			if (playerFeverParticle[i].IsUse) {
				Novice::SetBlendMode(kBlendModeAdd);
				screen.DrawPicture(playerFeverParticle[i].Pos, 400, 0, 100, 100, PlayerFeverParticleTexture, playerFeverParticle[i].color);
				Novice::SetBlendMode(kBlendModeNormal);
			}
		}
	}

	for (int i = 0; i < kMaxEnemy; i++) {
		for (int j = 0; j < kMaxParticle; j++) {
			if (particlecreat[i].particle[j].IsUse) {
				if (!(particlecreat[i].particle[j].IsUse == 4)) {
					screen.DrawBox(particlecreat[i].particle[j].Pos, 20, 20, 0, particlecreat[i].particle[j].color, kFillModeSolid);
				} else {
					screen.DrawBox(particlecreat[i].particle[j].Pos, 20 * screen.GetZoom(), 20 * screen.GetZoom(), 0, particlecreat[i].particle[j].color, kFillModeSolid, false);
				}
			}
		}
	}
	
}