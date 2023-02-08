#include "main.h"

const char kWindowTitle[] = "トゲスマッシュ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Screen::kWindowWidth, Screen::kWindowHeight);

	// 画像読み込み
	ui.LoadTexture();
	title.LoadTexture();
	result.LoadTexture();
	player.LoadTexture();
	enemy.LoadTexture();
	bgm.LoadBgm();

	bool isStop = true;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		Key::Update();

		// コントローラー
		Controller::SetState();

		///
		/// ↓更新処理ここから
		///


		switch (scene)
		{
		case TITLE:

			title.Update();
			player.TitleUpdate(map.mBackBoxColor);

			//タイトルの文字とプレイヤーが当たったらタイトルの文字を跳ねさせる
			for (int i = 0; i < title.WORDMAX; i++) {
				if (!title.mIsHitTitleWord[i] && CircleCapsuleCollsion(player, title.mTitleWordJudgePosition[i], title.mTitleWordJudgeSize[i])) {
					title.mIsHitTitleWord[i] = true;
				}
			}

			//Xボタン押下時ゲームシーン（初期化を行う）
			if (title.mIsKatakoriClear && Controller::IsTriggerButton(0, Controller::bX)) {
				result.Init();
				screen.Init();
				player.Init();
				enemy.Init();
				ui.Init();
				fever.Init();
				map.Init();
				change.Init();
				change.mIsChangeActive = true;
				int handle = -1;
				if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
					handle = Novice::PlayAudio(bgm.toingame, 0, 1);
				}
			}

			if ((Screen::kWindowWidth - 50.0f) <= change.mMakePosition) {
				change.Init();
				scene = INGAME;
			}

			break;
		case INGAME:

			////８を押すと画面が止まる
			//if (Key::IsTrigger(DIK_8) && !isStop) {
			//	isStop = true;
			//}
			//else if (Key::IsTrigger(DIK_8)) {
			//	isStop = false;
			//}

			if (isStop) {

				//UIアップデート
				ui.Update(player.mStrikeTutorial, player.mIsStrikeActive);

				//敵の音フラグを初期化する
				enemy.mIsPlayHitSound = false;
				enemy.mIsPlayStrikeHitSound = false;
				if (!player.mIsStrikeActive) {
					enemy.mDeadCount = 0;
				}

				//初期化
				if (Key::IsTrigger(DIK_R) || Controller::IsTriggerButton(0,Controller::bSTART)) {
					title.Init();
					screen.Init();
					player.Init();
					Novice::StopAudio(bgm.tutorialhandle);
					Novice::StopAudio(bgm.ingamehandle);
					Novice::StopAudio(bgm.feverhandle);
					bgm.Init();
					change.Init();
					scene = TITLE;
				}

				//ヒットストップしてないとき
				if (!screen.GetHitStop()) {

					//ゲームを始めているとき
					if ((ui.mIsStart || !ui.mIsReady) && 0 < ui.mTimeLeft) {

						//プレイヤーアップデート
						player.Update(screen, fever.mIsFever, fever.mIsOldFever, map.mBackBoxColor);

						//ストライクしてないとき
						if (!player.mIsStrikeActive) {

							//敵アップデート
							enemy.Update(ui.mTimeLeft, player.mPosition, player.LockonCount, ui.mIsReady, fever.mIsFever, screen);
						}
					}
				}

				//制限時間の経過
				ui.TimeLimit();

				//制限時間をフィーバーに入ったら延ばす
				ui.ExtendTime(fever.mIsFever, fever.mIsOldFever);


				//ゲーム開始時に初期化する
				if (!ui.mIsOldReady && ui.mIsReady) {
					fever.Init();
					screen.Init();
					map.Init();
					enemy.Init();
					enemy.Update(ui.mTimeLeft, player.mPosition, player.LockonCount, ui.mIsReady, fever.mIsFever, screen);
					player.Init();
				}


				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				//敵がプレイヤーを追いかけるか判定
				for (int i = 0; i < Enemy::kEnemyMax; i++) {

					//ヘビ
					if (!enemy.snake[i].mIsDeath && Collision(player.mPosition, 0.0f, enemy.snake[i].mHeadPosition, enemy.snake[i].mLockonRadius)) {
						enemy.snake[i].LockOn(player.mPosition, player.mRadius);
					} else {
						enemy.snake[i].IsPlayerLockon = false;
					}
					//ツチノコ
					if (!enemy.tsuchinoko[i].mIsDeath && Collision(player.mPosition, 0.0f, enemy.tsuchinoko[i].mCenterPosition, enemy.tsuchinoko[i].mLockonRadius)) {
						enemy.tsuchinoko[i].LockOn(player.mPosition, player.mRadius);
					} else {
						enemy.tsuchinoko[i].IsPlayerLockon = false;
					}
				}


				//敵同士の当たり判定
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					for (int j = 0; j < Enemy::kEnemyMax; j++) {
						if (i != j) {
							//ヘビ同士の当たり判定
							if (!enemy.snake[i].mIsDeath && !enemy.snake[j].mIsDeath && Collision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius * 0.25f, enemy.snake[j].mHeadPosition, enemy.snake[j].mHeadRadius * 0.25f)) {
								enemy.snake[i].mHeadPosition = AfterCollision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius * 0.25f, enemy.snake[j].mHeadPosition, enemy.snake[j].mHeadRadius * 0.25f);
							}
							//ツチノコ（頭同士）の当たり判定
							if (!enemy.tsuchinoko[i].mIsDeath && !enemy.tsuchinoko[j].mIsDeath && Collision(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mHeadPosition, enemy.tsuchinoko[j].mRadius)) {
								enemy.tsuchinoko[i].mHeadPosition = AfterCollision(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mHeadPosition, enemy.tsuchinoko[j].mRadius);
							}
							//ツチノコ（尾同士）の当たり判定
							if (!enemy.tsuchinoko[i].mIsDeath && !enemy.tsuchinoko[j].mIsDeath && Collision(enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius)) {
								enemy.tsuchinoko[i].mTailPosition = AfterCollision(enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius);
							}
						}
						//ヘビとツチノコ（頭）の当たり判定
						if (!enemy.snake[i].mIsDeath && !enemy.tsuchinoko[j].mIsDeath && Collision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius / 2, enemy.tsuchinoko[j].mHeadPosition, enemy.tsuchinoko[j].mRadius)) {
							enemy.snake[i].mHeadPosition = AfterCollision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius / 2, enemy.tsuchinoko[j].mHeadPosition, enemy.tsuchinoko[j].mRadius);
						}
						//ヘビとツチノコ（尾）の当たり判定
						if (!enemy.snake[i].mIsDeath && !enemy.tsuchinoko[j].mIsDeath && Collision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius / 2, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius)) {
							enemy.snake[i].mHeadPosition = AfterCollision(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius / 2, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius);
						}
						//ツチノコ（頭と尾）の当たり判定
						if (!enemy.tsuchinoko[i].mIsDeath && !enemy.tsuchinoko[j].mIsDeath && Collision(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius)) {
							enemy.tsuchinoko[i].mHeadPosition = AfterCollision(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius, enemy.tsuchinoko[j].mTailPosition, enemy.tsuchinoko[j].mRadius);
						}
					}
				}


				//１フレームに倒した敵を計算する（毎フレーム初期化）
				fever.mSnakeDefeat = 0;
				fever.mTsuchinokoDefeat = 0;


				//プレイヤーとの当たり判定
				//ノックバックしていないとき（ノックバック中は無敵）
				if (!player.mKnockbackActive && (ui.mIsStart || !ui.mIsReady)) {

					for (int i = 0; i < Enemy::kEnemyMax; i++) {
						if (enemy.snake[i].mShakeTimer == -1) {
							for (int j = 0; j < Snake::kBodyMax; j++) {
								//頭と体両方に当たる位置の時
								if (!enemy.snake[i].mIsDeath && (CircleCapsuleCollsion(player, enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius * 0.6f)) && CircleCapsuleCollsion(player, enemy.snake[i].mBodyPosition[j], enemy.snake[i].mBodyRadius * 0.5f) && !player.mKnockbackActive) {
									//フィーバーじゃないとき
									if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {
										player.SetKnockbackPosition(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius);
										ui.mIsWarning = true;
									}
									//フィーバーのとき
									else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

										//準備フェーズはスコアを加算しない処理
										if (ui.mIsReady) {
											ui.SnakeScore(player.mIsStrikeActive, enemy.snake[i].mHeadPosition);
											ui.AddKillCount();
										}
										fever.mSnakeDefeat++;
										fever.mSnakeDefeatStrike++;
										screen.SetHitStop();
										if (!player.mIsStrikeActive) {
											enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.snake[i].mHeadPosition);
										} else {
											enemy.StrikeHitSound();
										}
										enemy.snake[i].mShakeTimer = enemy.snake[i].kMaxShakeTimer;
										//ダッシュタイマーリセット
										player.mDushTimer = 0;
									}
								} else {
									//ヘビの頭
									if (!enemy.snake[i].mIsDeath && (CircleCapsuleCollsion(player, enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius * 0.6f))) {

										//フィーバーじゃないとき
										if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {
											player.SetKnockbackPosition(enemy.snake[i].mHeadPosition, enemy.snake[i].mHeadRadius);
											ui.mIsWarning = true;
										}
										//フィーバーのとき
										else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

											//準備フェーズはスコアを加算しない処理
											if (ui.mIsReady) {
												ui.SnakeScore(player.mIsStrikeActive, enemy.snake[i].mHeadPosition);
												ui.AddKillCount();
											}
											fever.mSnakeDefeat++;
											fever.mSnakeDefeatStrike++;
											screen.SetHitStop();
											if (!player.mIsStrikeActive) {
												enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.snake[i].mHeadPosition);
											} else {
												enemy.StrikeHitSound();
											}
											enemy.snake[i].mShakeTimer = enemy.snake[i].kMaxShakeTimer;
											//ダッシュタイマーリセット
											player.mDushTimer = 0;
										}

									}

									//ヘビの体
									if (!enemy.snake[i].mIsDeath && CircleCapsuleCollsion(player, enemy.snake[i].mBodyPosition[j], enemy.snake[i].mBodyRadius * 0.5f) && !player.mKnockbackActive) {

										//ダッシュで倒したらパワーを１増やす
										if (!player.mIsStrikeActive) {
											if (player.mStrikePower != player.kStrikePowerMax) {
												int handle = -1;
												if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
													handle = Novice::PlayAudio(bgm.startfever, 0, 0.2f);
												}
											}
											player.mStrikePower++;
										}
										//ダッシュ使用時
										else {

											//準備フェーズもフィーバーゲージは増やす！！！
											fever.mSnakeDefeat++;
											fever.mSnakeDefeatStrike++;
											if (!fever.mIsFever) {
												for (int k = 0; k < Fever::kMaxEnemy; k++) {
													if (!fever.particlecreat[k].IsUse) {
														fever.particlecreat[k].IsUse = 1;
														fever.particlecreat[k].Pos = enemy.snake[i].mHeadPosition;
														break;
													}
												}
											}
										}

										//準備フェーズはスコアを加算しない処理
										if (ui.mIsReady) {
											ui.SnakeScore(player.mIsStrikeActive, enemy.snake[i].mHeadPosition);
											ui.AddKillCount();
										}
										screen.SetHitStop();
										if (!player.mIsStrikeActive) {
											enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.snake[i].mHeadPosition);
										} else {
											enemy.StrikeHitSound();
										}
										enemy.snake[i].mShakeTimer = enemy.snake[i].kMaxShakeTimer;
										break;
									}
								}
							}
						}
						if (enemy.snake[i].mShakeTimer > 0) {
							enemy.snake[i].mShakeTimer--;
						} else if (enemy.snake[i].mShakeTimer == 0 && !enemy.snake[i].mIsDeath) {
							if (ui.mIsReady) {
								result.mSnakeKillCount++;
							}
							enemy.snake[i].mIsDeath = true;
						}

						if (enemy.tsuchinoko[i].mShakeTimer == -1) {
							for (int j = 0; j < Tsuchinoko::kBodyMax; j++) {
								//頭と体両方に当たる位置の時
								if (!enemy.tsuchinoko[i].mIsDeath && CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mBodyPosition[j], enemy.tsuchinoko[i].mBodyRadius * 0.6f) && !player.mKnockbackActive && (CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius * 0.7f) || CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius * 0.6f))) {
									//フィーバーじゃないとき
									if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {

										//ツチノコの頭か尾のどちらに当たったかを判定し、ノックバックの開始位置を変える
										//頭の場合
										if (CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius)) {
											player.SetKnockbackPosition(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius);
										}
										//尾の場合
										else {
											player.SetKnockbackPosition(enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius);
										}

										ui.mIsWarning = true;
									}
									//フィーバーのとき
									else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

										//準備フェーズはスコアを加算しない処理
										if (ui.mIsReady) {
											ui.TsuchinokoScore(player.mIsStrikeActive, enemy.tsuchinoko[i].mCenterPosition);
											ui.AddKillCount();
										}
										fever.mTsuchinokoDefeat++;
										fever.mTsuchinokoDefeatStrike++;
										screen.SetHitStop();
										if (!player.mIsStrikeActive) {
											enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.tsuchinoko[i].mCenterPosition);
										} else {
											enemy.StrikeHitSound();
										}
										enemy.tsuchinoko[i].mShakeTimer = enemy.tsuchinoko[i].kMaxShakeTimer;
										//ダッシュタイマーリセット
										player.mDushTimer = 0;
									}
								} else {
									//ツチノコの頭と尾
									if (!enemy.tsuchinoko[i].mIsDeath && (CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius * 0.7f) || CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius * 0.6f))) {

										//フィーバーじゃないとき
										if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {

											//ツチノコの頭か尾のどちらに当たったかを判定し、ノックバックの開始位置を変える
											//頭の場合
											if (CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius)) {
												player.SetKnockbackPosition(enemy.tsuchinoko[i].mHeadPosition, enemy.tsuchinoko[i].mRadius);
											}
											//尾の場合
											else {
												player.SetKnockbackPosition(enemy.tsuchinoko[i].mTailPosition, enemy.tsuchinoko[i].mRadius);
											}

											ui.mIsWarning = true;
										}
										//フィーバーのとき
										else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

											//準備フェーズはスコアを加算しない処理
											if (ui.mIsReady) {
												ui.TsuchinokoScore(player.mIsStrikeActive, enemy.tsuchinoko[i].mCenterPosition);
												ui.AddKillCount();
											}
											fever.mTsuchinokoDefeat++;
											fever.mTsuchinokoDefeatStrike++;
											screen.SetHitStop();
											if (!player.mIsStrikeActive) {
												enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.tsuchinoko[i].mCenterPosition);
											} else {
												enemy.StrikeHitSound();
											}
											enemy.tsuchinoko[i].mShakeTimer = enemy.tsuchinoko[i].kMaxShakeTimer;
											//ダッシュタイマーリセット
											player.mDushTimer = 0;
										}
									}

									//ツチノコの体

									if (!enemy.tsuchinoko[i].mIsDeath && CircleCapsuleCollsion(player, enemy.tsuchinoko[i].mBodyPosition[j], enemy.tsuchinoko[i].mBodyRadius * 0.6f) && !player.mKnockbackActive) {

										//ダッシュで倒したらパワーを１増やす
										if (!player.mIsStrikeActive) {
											if (player.mStrikePower != player.kStrikePowerMax) {
												int handle = -1;
												if (Novice::IsPlayingAudio(handle) == false || handle == -1) {
													handle = Novice::PlayAudio(bgm.startfever, 0, 0.2f);
												}
											}
											player.mStrikePower++;
										}
										//ダッシュ使用時
										else {

											//準備フェーズもフィーバーゲージを増やす！！！
											fever.mTsuchinokoDefeat++;
											fever.mTsuchinokoDefeatStrike++;
											if (!fever.mIsFever) {
												for (int k = 0; k < Fever::kMaxEnemy; k++) {
													if (!fever.particlecreat[k].IsUse) {
														fever.particlecreat[k].IsUse = 1;
														fever.particlecreat[k].Pos = enemy.tsuchinoko[i].mCenterPosition;
														break;
													}
												}
											}
										}

										//準備フェーズはスコアを加算しない処理
										if (ui.mIsReady) {
											ui.TsuchinokoScore(player.mIsStrikeActive, enemy.tsuchinoko[i].mCenterPosition);
											ui.AddKillCount();
										}
										screen.SetHitStop();
										if (!player.mIsStrikeActive) {
											enemy.HitSound(fever.mIsFever, fever.feverGauge.color, player.mStrikePower, enemy.tsuchinoko[i].mCenterPosition);
										} else {
											enemy.StrikeHitSound();
										}
										enemy.tsuchinoko[i].mShakeTimer = enemy.tsuchinoko[i].kMaxShakeTimer;
										break;
									}
								}
							}
						}
						if (enemy.tsuchinoko[i].mShakeTimer > 0) {
							enemy.tsuchinoko[i].mShakeTimer--;
						} else if (enemy.tsuchinoko[i].mShakeTimer == 0 && !enemy.tsuchinoko[i].mIsDeath) {
							if (ui.mIsReady) {
								result.mTsuchinokoKillCount++;
							}
							enemy.tsuchinoko[i].mIsDeath = true;
						}
					}
				}

				//死んだ敵の数をカウントする→一定数超えたらウェーブを進める
				if (ui.mIsStart) {
					for (int i = 0; i < Enemy::kEnemyMax; i++) {
						if (enemy.snake[i].mIsDeath && !enemy.snake[i].mIsOldDeath) {
							enemy.mSnakeDeathCount++;
						}
						if (enemy.tsuchinoko[i].mIsDeath && !enemy.tsuchinoko[i].mIsOldDeath) {
							enemy.mTsuchinokoDeathCount++;
						}
						enemy.snake[i].mIsOldDeath = enemy.snake[i].mIsDeath;
						enemy.tsuchinoko[i].mIsOldDeath = enemy.tsuchinoko[i].mIsDeath;
					}
				}

				//複数キルのスコア
				ui.mTsuchinokoDefeatStrike = fever.mTsuchinokoDefeatStrike;
				ui.mSnakeDefeatStrike = fever.mSnakeDefeatStrike;
				if (player.mIsStrikeActive && ui.StrikeEndFlag == 0) {
					ui.StrikeEndFlag = 1;
				} else if (ui.StrikeEndFlag == 1 && !player.mIsStrikeActive) {
					int DefeatStrike = ui.mTsuchinokoDefeatStrike + ui.mSnakeDefeatStrike;
					if (ui.mIsReady) {
						if (DefeatStrike >= 4) {
							//スーパーキル
							result.mSuperKillCount++;
						} else if (DefeatStrike == 3) {
							//トリプルキル
							result.mTripleKillCount++;
						} else if (DefeatStrike == 2) {
							//ダブルキル
							result.mDoubleKillCount++;
						}
					}
					ui.StrikeEndScore(player.mIsStrikeActive, ui.mTsuchinokoDefeatStrike, ui.mSnakeDefeatStrike);
					ui.StrikeEndFlag = 0;
				}
				ui.StrikeEasing();
				
				

				if (fever.mIsFever) {
					fever.mFeverGaugeStrikeEndFlag = 0;
				} else {
					if (player.mIsStrikeActive && fever.mFeverGaugeStrikeEndFlag == 0) {
						fever.mFeverGaugeStrikeEndFlag = 1;
					}if (!player.mIsStrikeActive && fever.mFeverGaugeStrikeEndFlag == 1) {
						fever.mFeverGaugeStrikeEndFlag = 2;
					}
				}



				if (0 >= ui.mTimeLeft) {
					fever.mFeverStop = true;
				} else {
					fever.mFeverStop = false;
				}


				//プレイヤーのノックバック
				screen.ShakeUpdate(player.mKnockbackSet);
				player.Knockback();



				//追尾している敵の数に応じてスピードを変える（毎フレーム初期化）
				player.LockonCount = 0;

				//追尾している数の計算
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					if (enemy.snake[i].IsPlayerLockon) {
						player.LockonCount++;
					}
					if (enemy.tsuchinoko[i].IsPlayerLockon) {
						player.LockonCount++;
					}
				}

				

				//フィーバー
				fever.Update(screen, player.mPosition, player.mIsStrikeActive);

				//マップアップデート（フィーバーカラーにする）
				map.Update(fever.mIsFever, fever.mIsOldFever, fever.feverGauge.color);

				//残像アップデート
				player.Shadow(screen.GetHitStop(), ui.mIsStart, ui.mIsReady);


				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				//ズームアップデート
				screen.ZoomUpdate(fever.mIsFever, fever.mIsOldFever, player.mIsStrikeActive, player.mIsOldStrikeActive, player.mMarkPosition, player.mPosition);

				//スクロールアップデート
				screen.ScrollUpdate(player);

				//シェイクアップデート
				screen.ShakeUpdate(player.mIsStrikeBoxShakeActive);

				//ヒットストップアップデート
				screen.HitStopUpdate();


				//タイムアップの文字が出てしばらくしたら黒めの矩形を出す
				if (ui.mIsTimeUpFinish) {
					change.mIsChangeActive = true;

					//矩形がある程度でたらリザルトに移行
					if ((Screen::kWindowWidth - 50.0f) <= change.mMakePosition) {
						result.Init();
						scene = OUTGAME;
					}
				}
			}

			break;
		case OUTGAME:

			result.Update(ui.mScore);

			//Aボタン押下時にタイトル（初期化を行う）
			if (Controller::IsTriggerButton(0, Controller::bX)) {
				title.Init();
				screen.Init();
				player.Init();
				Novice::StopAudio(bgm.resulthandle);
				bgm.Init();
				change.Init();
				result.mIsBlackActive = true;
			}
			if (result.mBlackAlpht == 1.0f){
				scene = TITLE;
			}
			break;
		}
		//画面遷移
		change.Update();

		//BGM
		switch (scene)
		{
		case TITLE:

			Novice::StopAudio(bgm.resulthandle);
			if (Novice::IsPlayingAudio(bgm.titlehandle) == false || bgm.titlehandle == -1) {
				bgm.titlehandle = Novice::PlayAudio(bgm.title, 0, 0.2f);
			}

			break;
		case INGAME:
			
			if (fever.mIsFever) {
				Novice::StopAudio(bgm.titlehandle);
				Novice::StopAudio(bgm.tutorialhandle);
				Novice::StopAudio(bgm.ingamehandle);
				if (Novice::IsPlayingAudio(bgm.feverhandle) == false || bgm.feverhandle == -1) {
					bgm.feverhandle = Novice::PlayAudio(bgm.fever, 0, 0.2f);
				}
			} else {
				Novice::StopAudio(bgm.feverhandle);
				if (!ui.mIsReady) {
					Novice::StopAudio(bgm.titlehandle);
					if (Novice::IsPlayingAudio(bgm.tutorialhandle) == false || bgm.tutorialhandle == -1) {
						bgm.tutorialhandle = Novice::PlayAudio(bgm.tutorial, 0, 0.2f);
					}
				}
				else {
					Novice::StopAudio(bgm.tutorialhandle);
					if (Novice::IsPlayingAudio(bgm.ingamehandle) == false || bgm.ingamehandle == -1) {
						bgm.ingamehandle = Novice::PlayAudio(bgm.ingame, 0, 0.2f);
					}
				}
			}
			
			break;
		case OUTGAME:

			Novice::StopAudio(bgm.feverhandle);
			Novice::StopAudio(bgm.ingamehandle);
			if (Novice::IsPlayingAudio(bgm.resulthandle) == false || bgm.resulthandle == -1) {
				bgm.resulthandle = Novice::PlayAudio(bgm.result, 0, 0.2f);
			}

			break;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (scene)
		{
		case TITLE:

			//タイトル画面描画
			title.Draw(screen);

			//プレイヤー描画
			player.TitleDraw(screen);

			//タイトル文字描画
			title.FrontDraw(screen);

			//隠し描画
			change.DrawTitle(screen, ui.mIsStart);

			break;
		case INGAME:

			//背景描画
			map.Draw(screen);


			//カウントダウン描画（プレイヤーの後ろに描画し、邪魔にならないようにする）
			ui.DrawBackTimeLimit(screen);


			//敵描画
			enemy.Draw(screen, screen.GetHitStop());


			//プレイヤー描画
			player.Draw(screen, ui.mIsReady, fever.mIsFever, fever.feverGauge.color, map.mBackLineColor);


			//ＵＩ描画
			ui.Draw(screen, ui.mIsReady);

			//ミニマップの位置の描画
			//敵描画
			for (int i = 0; i < Enemy::kEnemyMax; i++) {
				if (!enemy.snake[i].mIsDeath) {
					screen.DrawMiniMap(enemy.snake[i].mHeadPosition, 0xFFF40EFF);
				}
				if (!enemy.tsuchinoko[i].mIsDeath) {
					screen.DrawMiniMap(enemy.tsuchinoko[i].mCenterPosition, 0x37BBF3FF);
				}

			}
			//マークと線描画
			if (player.mIsMarkActive) {
				screen.DrawLine(screen.MiniMapTransform(player.mMarkPosition), screen.MiniMapTransform(player.mPosition), RED, false);
				screen.DrawMiniMap(player.mMarkPosition, RED, kFillModeSolid, 4.0f);
			}
			//プレイヤーの位置描画
			screen.DrawMiniMap(player.mPosition, WHITE, kFillModeSolid, 4.0f);


			//ストライクUI描画
			player.DrawStrikeUI(screen, fever.mIsFever, fever.feverGauge.color);


			//フィーバー
			fever.Draw(screen);

			//隠し描画
			change.DrawTitle(screen, ui.mIsStart);

			break;
		case OUTGAME:

			//リザルト画面描画
			result.Draw(screen);

			break;
		}

		//画面遷移描画
		change.Draw(screen, ui.mIsStart);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (Key::IsTrigger(DIK_ESCAPE)) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
