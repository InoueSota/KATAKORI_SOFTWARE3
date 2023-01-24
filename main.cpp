#include "main.h"

const char kWindowTitle[] = "トゲスマッシュ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Screen::kWindowWidth, Screen::kWindowHeight);

	// 画像読み込み
	int explanation = Novice::LoadTexture("./Resources/Debugs/Explanation.png");
	int end = Novice::LoadTexture("./Resources/Outgame/End/end.png");
	ui.LoadTexture();
	title.LoadTexture();

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

			//Aボタン押下時
			if (title.mIsKatakoriClear && Controller::IsTriggerButton(0, Controller::bA)) {
				screen.Init();
				player.Init();
				for (int i = 0; i < Snake::kMaxSnake; i++) {
					snake[i].Init();
				}
				for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
					tsuchinoko[i].Init();
				}
				ui.Init();
				scene = INGAME;
			}
			break;
		case INGAME:

			//UIアップデート
			ui.Update();


			//初期化
			if (Key::IsTrigger(DIK_R) || Controller::IsTriggerButton(0, Controller::bY)){
				screen.Init();
				player.Init();
				for (int i = 0; i < Snake::kMaxSnake; i++) {
					snake[i].Init();
				}
				for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
					tsuchinoko[i].Init();
				}
				ui.Init();
				title.Init();
				scene = TITLE;
			}
			
			//ヒットストップしてないとき
			if (!screen.GetHitStop())
			{

				//プレイヤーアップデート
				player.Update(screen, fever.mIsFever, fever.mIsOldFever);


				//ストライクしてないとき
				if (!player.mIsStrikeActive)
				{
					//制限時間の経過を止める
					ui.TimeLimit();

					//敵アップデート
					for (int i = 0; i < Snake::kMaxSnake; i++) {
						snake[i].Update(ui.mTimeLeft, player.mPosition);
					}
					for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
						tsuchinoko[i].Update(player.mPosition, ui.mTimeLeft);
					}
				}
			}


			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			//敵がプレイヤーを追いかけるか判定
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				if (Collision(player.mPosition, player.mRadius, snake[i].mHeadPosition, snake[i].mLockonRadius)) {
					snake[i].LockOn(player.mPosition, player.mRadius);
				}
				else {
					snake[i].IsPlayerLockon = false;
				}
			}
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
				if (Collision(player.mPosition, player.mRadius, tsuchinoko[i].mCenterPosition, tsuchinoko[i].mLockonRadius)) {
					tsuchinoko[i].LockOn(player.mPosition, player.mRadius);
				}
				else {
					tsuchinoko[i].IsPlayerLockon = false;
				}
			}


			//敵同士の当たり判定
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				for (int j = 0; j < Snake::kMaxSnake; j++) {
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


			//プレイヤーとの当たり判定
			//ヘビ
			fever.mSnakeDefeat = 0;
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				//頭と尾
				if (!snake[i].mIsDeath && (CircleCapsuleCollsion(player, snake[i].mHeadPosition, snake[i].mHeadRadius)))
				{
					if (!fever.mIsFever) {
						player.SetKnockbackPosition(snake[i].mHeadPosition, snake[i].mHeadRadius);
						player.mSizeMax -= Player::kToHeadMinusValue;
						ui.MissSnakeScore(player.mIsStrikeActive);
						ui.mCombo = 0;
						ui.mIsWarning = true;
					} else {
						ui.SnakeScore(player.mIsStrikeActive, player.mSizeMax);
						ui.AddCombo();
						screen.SetHitStop();
						snake[i].mIsDeath = true;
						fever.mSnakeDefeat++;
					}
					
				}

				//体
				for (int j = 0; j < Snake::kBodyMax; j++)
				{
					if (!snake[i].mIsDeath && CircleCapsuleCollsion(player, snake[i].mBodyPosition[j], snake[i].mBodyRadius) && !player.mKnockbackActive)
					{
						if (!player.mIsStrikeActive) {
							player.mStrikePower++;
							player.mSizeMax -= Player::kToBodyMinusValue;
						}
						else {
							player.mSizeMax += Player::kToBodyPlusValue;
						}
						ui.SnakeScore(player.mIsStrikeActive, player.mSizeMax);
						ui.AddCombo();
						screen.SetHitStop();
						snake[i].mIsDeath = true;
						fever.mSnakeDefeat++;
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


			//ツチノコ
			fever.mTsuchinokoDefeat = 0;
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {

				//頭と尾
				if (!tsuchinoko[i].mIsDeath && (CircleCapsuleCollsion(player, tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius) || CircleCapsuleCollsion(player, tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius)))
				{
					if (!fever.mIsFever) {
						if (CircleCapsuleCollsion(player, tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius)) {
							player.SetKnockbackPosition(tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius);
						}
						else {
							player.SetKnockbackPosition(tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius);
						}
						player.mSizeMax -= Player::kToHeadMinusValue;
						ui.MissTsuchinokoScore(player.mIsStrikeActive);
						ui.mCombo = 0;
						ui.mIsWarning = true;
					} else {
						ui.TsuchinokoScore(player.mIsStrikeActive, player.mSizeMax);
						ui.AddCombo();
						screen.SetHitStop();
						tsuchinoko[i].mIsDeath = true;
						fever.mTsuchinokoDefeat++;
					}
					
				}

				//体
				for (int j = 0; j < Tsuchinoko::kBodyMax; j++)
				{
					if (!tsuchinoko[i].mIsDeath && CircleCapsuleCollsion(player,tsuchinoko[i].mBodyPosition[j], tsuchinoko[i].mBodyRadius) && !player.mKnockbackActive)
					{
						if (!player.mIsStrikeActive) {
							player.mStrikePower++;
							player.mSizeMax -= Player::kToBodyMinusValue;
						}
						else {
							player.mSizeMax += Player::kToBodyPlusValue;
						}
						ui.TsuchinokoScore(player.mIsStrikeActive, player.mSizeMax);
						ui.AddCombo();
						screen.SetHitStop();
						tsuchinoko[i].mIsDeath = true;
						fever.mTsuchinokoDefeat++;
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


			//敵の数に応じてスピードを変える
			player.LockonCount = 0;

			for (int i = 0; i < Snake::kMaxSnake; i++) {
				if (snake[i].IsPlayerLockon) {
					player.LockonCount++;
				}
			}
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
				if (tsuchinoko[i].IsPlayerLockon) {
					player.LockonCount++;
				}
			}
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				snake[i].mSpeed = 5 + player.LockonCount * 0.5;
			}
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
				tsuchinoko[i].mCenterSpeed = 5 + player.LockonCount * 0.3;
			}


			//フィーバー
			fever.Update(screen);


			//残像アップデート
			player.Shadow(screen.GetHitStop());
			

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			//ズームアップデート
			screen.ZoomUpdate(fever.mIsFever, fever.mIsOldFever);

			//スクロールアップデート
			screen.ScrollUpdate(player);

			//シェイクアップデート
			screen.ShakeUpdate(player.mKnockbackSet);

			//ヒットストップアップデート
			screen.HitStopUpdate();


			if (ui.mIsTimeUpFinish || player.mSizeMax == 0.0f) {
				scene = OUTGAME;
			}


			break;
		case OUTGAME:

			//Aボタン押下時
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

			title.Draw(screen);

			break;
		case INGAME:

			//背景描画
			map.Draw(screen);
			//カウントダウン描画
			ui.DrawBackTimeLimit(screen);
			//敵描画
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				snake[i].Draw(screen);
			}
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
				tsuchinoko[i].Draw(screen);
			}
			//プレイヤー描画
			player.Draw(screen);

			//制作中の操作説明を一時的に描画する
			//Novice::DrawSprite(Screen::kWindowWidth - 420, Screen::kWindowHeight - 220, explanation, 1, 1, 0.0f, WHITE);

			//ＵＩ描画
			ui.Draw(screen);

			//ミニマップの位置の描画
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				screen.DrawMiniMap(snake[i].mHeadPosition, WHITE);
			}
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
				screen.DrawMiniMap(tsuchinoko[i].mCenterPosition, WHITE);
			}
			screen.DrawMiniMap(player.mPosition, 0xFFFF00FF, kFillModeSolid, 4.0f);

			//ストライクパワー描画
			player.DrawStrikePower(screen);

			//フィーバー
			fever.Draw(screen);

			break;
		case OUTGAME:
			Novice::DrawSprite(0, 0, end, 1, 1, 0.0f, WHITE);
			int Result[6];
			int tmpScore = ui.mScore;
			for (int i = 5; i > -1; i--) {
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
