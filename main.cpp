#include "main.h"

const char kWindowTitle[] = "トゲスマッシュ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Screen::kWindowWidth, Screen::kWindowHeight);

	// 画像読み込み
	int end = Novice::LoadTexture("./Resources/Outgame/End/end.png");
	ui.LoadTexture();
	title.LoadTexture();
	player.LoadTexture();

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

			//Aボタン押下時ゲームシーン（初期化を行う）
			if (title.mIsKatakoriClear && Controller::IsTriggerButton(0, Controller::bA)) {
				screen.Init();
				player.Init();
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					snake[i].Init();
					tsuchinoko[i].Init();
				}
				ui.Init();
				fever.Init();
				map.Init();
				scene = INGAME;
			}
			break;
		case INGAME:

			//８を押すと画面が止まる
			if (Key::IsTrigger(DIK_8) && !isStop) {
				isStop = true;
			}
			else if (Key::IsTrigger(DIK_8)) {
				isStop = false;
			}

			if (isStop) {

				//UIアップデート
				ui.Update();


				//初期化
				if (Key::IsTrigger(DIK_R) || Controller::IsTriggerButton(0,Controller::bSTART)) {
					screen.Init();
					player.Init();
					for (int i = 0; i < Enemy::kEnemyMax; i++) {
						snake[i].Init();
						tsuchinoko[i].Init();
					}
					ui.Init();
					title.Init();
					fever.Init();
					map.Init();
					scene = TITLE;
				}

				//ヒットストップしてないとき
				if (!screen.GetHitStop())
				{
					//ゲームを始めているとき
					if ((ui.mIsStart || !ui.mIsReady) && 0 < ui.mTimeLeft)
					{
						//プレイヤーアップデート
						player.Update(screen, fever.mIsFever, fever.mIsOldFever);

						//ストライクしてないとき
						if (!player.mIsStrikeActive)
						{
							//敵アップデート
							for (int i = 0; i < Enemy::kEnemyMax; i++) {
								snake[i].Update(ui.mTimeLeft, player.mPosition);
								tsuchinoko[i].Update(player.mPosition, ui.mTimeLeft);
							}
						}
					}

					//制限時間の経過
					ui.TimeLimit();
				}

				//制限時間をフィーバーに入ったら延ばす
				ui.ExtendTime(fever.mIsFever, fever.mIsOldFever);


				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				//敵がプレイヤーを追いかけるか判定
				for (int i = 0; i < Enemy::kEnemyMax; i++) {

					//ヘビ
					if (Collision(player.mPosition, player.mRadius, snake[i].mHeadPosition, snake[i].mLockonRadius)) {
						snake[i].LockOn(player.mPosition, player.mRadius);
					}
					else {
						snake[i].IsPlayerLockon = false;
					}

					//ツチノコ
					if (Collision(player.mPosition, player.mRadius, tsuchinoko[i].mCenterPosition, tsuchinoko[i].mLockonRadius)) {
						tsuchinoko[i].LockOn(player.mPosition, player.mRadius);
					}
					else {
						tsuchinoko[i].IsPlayerLockon = false;
					}
				}


				//敵同士の当たり判定
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					for (int j = 0; j < Enemy::kEnemyMax; j++) {
						if (i != j) {
							//ヘビ同士の当たり判定
							if (Collision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, snake[j].mHeadPosition, snake[j].mHeadRadius / 2)) {
								snake[i].mHeadPosition = AfterCollision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, snake[j].mHeadPosition, snake[j].mHeadRadius / 2);
							}
							//ツチノコ（頭同士）の当たり判定
							if (Collision(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mHeadPosition, tsuchinoko[j].mRadius)) {
								tsuchinoko[i].mHeadPosition = AfterCollision(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mHeadPosition, tsuchinoko[j].mRadius);
							}
							//ツチノコ（尾同士）の当たり判定
							if (Collision(tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius)) {
								tsuchinoko[i].mTailPosition = AfterCollision(tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius);
							}
						}
						//ヘビとツチノコ（頭）の当たり判定
						if (Collision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, tsuchinoko[j].mHeadPosition, tsuchinoko[j].mRadius)) {
							snake[i].mHeadPosition = AfterCollision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, tsuchinoko[j].mHeadPosition, tsuchinoko[j].mRadius);
						}
						//ヘビとツチノコ（尾）の当たり判定
						if (Collision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius)) {
							snake[i].mHeadPosition = AfterCollision(snake[i].mHeadPosition, snake[i].mHeadRadius / 2, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius);
						}
						//ツチノコ（頭と尾）の当たり判定
						if (Collision(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius)) {
							tsuchinoko[i].mHeadPosition = AfterCollision(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius, tsuchinoko[j].mTailPosition, tsuchinoko[j].mRadius);
						}
					}
				}



				//１フレームに倒した敵を計算する（毎フレーム初期化）
				fever.mSnakeDefeat = 0;
				fever.mTsuchinokoDefeat = 0;

				//プレイヤーとの当たり判定
				//ノックバックしていないとき（ノックバック中は無敵）
				if (!player.mKnockbackActive) {
					for (int i = 0; i < Enemy::kEnemyMax; i++) {
						if (snake[i].mShakeTimer == -1) {
							//ヘビの頭
							if (!snake[i].mIsDeath && (CircleCapsuleCollsion(player, snake[i].mHeadPosition, snake[i].mHeadRadius * 0.6f))) {

								//フィーバーじゃないとき
								if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {
									player.SetKnockbackPosition(snake[i].mHeadPosition, snake[i].mHeadRadius);
									ui.mCombo--;
									ui.mIsWarning = true;
								}
								//フィーバーのとき
								else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

									//準備フェーズはスコアを加算しない処理
									if (ui.mIsReady) {
										ui.SnakeScore(player.mIsStrikeActive, player.mSize, snake[i].mHeadPosition);
										ui.AddCombo();
										fever.mSnakeDefeat++;
										fever.mSnakeDefeatStrike++;
									}
									screen.SetHitStop();
									snake[i].mShakeTimer = snake[i].kMaxShakeTimer;
									//ダッシュタイマーリセット
									player.mDushTimer = 0;
								}
								
							}

							//ヘビの体
							for (int j = 0; j < Snake::kBodyMax; j++) {

								if (!snake[i].mIsDeath && CircleCapsuleCollsion(player, snake[i].mBodyPosition[j], snake[i].mBodyRadius) && !player.mKnockbackActive) {

									//ダッシュで倒したらパワーを１増やす
									if (!player.mIsStrikeActive) {
										player.mStrikePower++;
									}
									//ダッシュ使用時
									else {

										//準備フェーズはフィーバーゲージを増やさない処理
										if (ui.mIsReady) {
											fever.mSnakeDefeat++;
											fever.mSnakeDefeatStrike++;
											if (!fever.mIsFever) {
												for (int k = 0; k < Fever::kMaxEnemy; k++) {
													if (!fever.particlecreat[k].IsUse) {
														fever.particlecreat[k].IsUse = 1;
														fever.particlecreat[k].Pos = snake[i].mHeadPosition;
														break;
													}
												}
											}
										}
									}

									//準備フェーズはスコアを加算しない処理
									if (ui.mIsReady) {
										ui.SnakeScore(player.mIsStrikeActive, player.mSize, snake[i].mHeadPosition);
										ui.AddCombo();
									}
									screen.SetHitStop();
									snake[i].mShakeTimer = snake[i].kMaxShakeTimer;
									break;
								}
							}
						}
						if (snake[i].mShakeTimer > 0) {
							snake[i].mShakeTimer--;
						} else if (snake[i].mShakeTimer == 0) {
							snake[i].mIsDeath = true;
						}

						if (tsuchinoko[i].mShakeTimer == -1) {
							//ツチノコの頭と尾
							if (!tsuchinoko[i].mIsDeath && (CircleCapsuleCollsion(player, tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius * 0.6f) || CircleCapsuleCollsion(player, tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius * 0.6f))) {

								//フィーバーじゃないとき
								if (!fever.mIsFever && !(player.mDushTimer > 0 && player.mIsStrikeActive)) {

									//ツチノコの頭か尾のどちらに当たったかを判定し、ノックバックの開始位置を変える
									//頭の場合
									if (CircleCapsuleCollsion(player, tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius)) {
										player.SetKnockbackPosition(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius);
									}
									//尾の場合
									else {
										player.SetKnockbackPosition(tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius);
									}

									ui.mCombo--;
									ui.mIsWarning = true;
								}
								//フィーバーのとき
								else if (fever.mIsFever || (player.mDushTimer > 0 && player.mIsStrikeActive)) {

									//準備フェーズはスコアを加算しない処理
									if (ui.mIsReady) {
										ui.TsuchinokoScore(player.mIsStrikeActive, player.mSize, tsuchinoko[i].mCenterPosition);
										ui.AddCombo();
										fever.mTsuchinokoDefeat++;
										fever.mTsuchinokoDefeatStrike++;
									}
									screen.SetHitStop();
									tsuchinoko[i].mShakeTimer = tsuchinoko[i].kMaxShakeTimer;
									//ダッシュタイマーリセット
									player.mDushTimer = 0;
								}
								

							}

							//ツチノコの体
							for (int j = 0; j < Tsuchinoko::kBodyMax; j++) {

								if (!tsuchinoko[i].mIsDeath && CircleCapsuleCollsion(player, tsuchinoko[i].mBodyPosition[j], tsuchinoko[i].mBodyRadius) && !player.mKnockbackActive) {

									//ダッシュで倒したらパワーを１増やす
									if (!player.mIsStrikeActive) {
										player.mStrikePower++;
									}
									//ダッシュ使用時
									else {

										//準備フェーズはフィーバーゲージを増やさない処理
										if (ui.mIsReady) {
											fever.mTsuchinokoDefeat++;
											fever.mTsuchinokoDefeatStrike++;
											if (!fever.mIsFever) {
												for (int k = 0; k < Fever::kMaxEnemy; k++) {
													if (!fever.particlecreat[k].IsUse) {
														fever.particlecreat[k].IsUse = 1;
														fever.particlecreat[k].Pos = tsuchinoko[i].mCenterPosition;
														break;
													}
												}
											}
										}
									}

									//準備フェーズはスコアを加算しない処理
									if (ui.mIsReady) {
										ui.TsuchinokoScore(player.mIsStrikeActive, player.mSize, tsuchinoko[i].mCenterPosition);
										ui.AddCombo();
									}
									screen.SetHitStop();
									tsuchinoko[i].mShakeTimer = tsuchinoko[i].kMaxShakeTimer;
									break;
								}

							}
							
						}
						if (tsuchinoko[i].mShakeTimer > 0) {
							tsuchinoko[i].mShakeTimer--;
						} else if (tsuchinoko[i].mShakeTimer == 0) {
							tsuchinoko[i].mIsDeath = true;
						}
					}
				}



				if (fever.mIsFever) {
					fever.mFeverGaugeStrikeEndFlag = 0;
					fever.mSnakeDefeatStrike = 0;
					fever.mTsuchinokoDefeatStrike = 0;
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
				player.Knockback();



				//追尾している敵の数に応じてスピードを変える（毎フレーム初期化）
				player.LockonCount = 0;

				//追尾している数の計算
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					if (snake[i].IsPlayerLockon) {
						player.LockonCount++;
					}
					if (tsuchinoko[i].IsPlayerLockon) {
						player.LockonCount++;
					}
				}

				//追尾している敵の数が多いほど敵の速度が上がる
				for (int i = 0; i < Enemy::kEnemyMax; i++) {
					snake[i].mSpeed = 5 + player.LockonCount * 0.5;
					tsuchinoko[i].mCenterSpeed = 5 + player.LockonCount * 0.3;
				}

				//フィーバー
				fever.Update(screen, player.mPosition);

				//マップアップデート（フィーバーカラーにする）
				map.Update(fever.mIsFever, fever.mIsOldFever);

				//残像アップデート
				player.Shadow(screen.GetHitStop());

				//ゲーム開始時に初期化する
				if (!ui.mIsOldReady && ui.mIsReady) {
					player.Init();
					for (int i = 0; i < Enemy::kEnemyMax; i++) {
						snake[i].Init();
						tsuchinoko[i].Init();
					}
				}


				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				//ズームアップデート
				screen.ZoomUpdate(fever.mIsFever, fever.mIsOldFever, player.mIsStrikeActive, player.mIsOldStrikeActive);

				//スクロールアップデート
				screen.ScrollUpdate(player);

				//シェイクアップデート
				screen.ShakeUpdate(player.mKnockbackSet);

				//ヒットストップアップデート
				screen.HitStopUpdate();



				//タイムアップの文字が出てしばらくしたらスコア画面に移行する
				if (ui.mIsTimeUpFinish) {
					scene = OUTGAME;
				}
			}

			break;
		case OUTGAME:

			//Aボタン押下時にタイトル（初期化を行う）
			if (Controller::IsTriggerButton(0, Controller::bX)) {
				title.Init();
				scene = TITLE;
			}
			break;
		}

		//BGM
		switch (scene)
		{
		case TITLE:
			break;
		case INGAME:
			break;
		case OUTGAME:
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

			break;
		case INGAME:

			//背景描画
			map.Draw(screen);



			//カウントダウン描画（プレイヤーの後ろに描画し、邪魔にならないようにする）
			ui.DrawBackTimeLimit(screen);



			//敵描画
			for (int i = 0; i < Enemy::kEnemyMax; i++) {
				snake[i].Draw(screen);
				tsuchinoko[i].Draw(screen);
			}



			//プレイヤー描画
			player.Draw(screen, ui.mIsReady);



			//ＵＩ描画
			ui.Draw(screen);

			//ミニマップの位置の描画
			//敵描画
			for (int i = 0; i < Enemy::kEnemyMax; i++) {
				screen.DrawMiniMap(snake[i].mHeadPosition, WHITE);
				screen.DrawMiniMap(tsuchinoko[i].mCenterPosition, WHITE);
			}
			//マークと線描画
			if (player.mIsMarkActive) {
				screen.DrawLine(screen.MiniMapTransform(player.mMarkPosition), screen.MiniMapTransform(player.mPosition), RED, false);
				screen.DrawMiniMap(player.mMarkPosition, RED, kFillModeSolid, 4.0f);
			}
			//プレイヤーの位置描画
			screen.DrawMiniMap(player.mPosition, 0xFFFF00FF, kFillModeSolid, 4.0f);



			//ストライクUI描画
			player.DrawStrikeUI(screen);



			//フィーバー
			fever.Draw(screen);


			break;
		case OUTGAME:
			Novice::DrawSprite(0, 0, end, 1, 1, 0.0f, WHITE);
			int Result[7];
			int tmpScore = ui.mScore;
			for (int i = 6; i > -1; i--) {
				Result[i] = tmpScore / powf(10, i);
				tmpScore = tmpScore % (int)powf(10, i);
				screen.DrawUI(ui.mScorePosition[i], ui.mTimeUISize, 32 * Result[i], 32, 32, ui.mTimeNumber, WHITE, ui.mComboScale);
			}
			break;
		}

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
