#include "main.h"

const char kWindowTitle[] = "トゲ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Screen::kWindowWidth, Screen::kWindowHeight);

	// 画像読み込み
	int explanation = Novice::LoadTexture("./Resources/Debugs/Explanation.png");

	//乱数の初期化
	srand(time(nullptr));

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
			break;
		case INGAME:

			//UIアップデート
			ui.Update();

			//ズーム値アップデート
			screen.SetZoom();

			//初期化
			if (Key::IsTrigger(DIK_R)){
				screen.Init();
				player.Init();
				for (int i = 0; i < Snake::kMaxSnake; i++) {
					snake[i].Init();
				}
				for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
					tsuchinoko[i].Init();
				}
			}

			//プレイヤーアップデート
			player.Update(screen);

			//ストライクしてないとき
			if (!player.mIsStrikeActive)
			{
				//敵アップデート
				for (int i = 0; i < Snake::kMaxSnake; i++) {
					snake[i].Update();
				}
				for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {
					tsuchinoko[i].Update(player.mPosition);
				}
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
			for (int i = 0; i < Snake::kMaxSnake; i++) {
				collision(player, snake[i]);
				//頭と尾
				if (!snake[i].mIsDeath && (CircleCapsuleCollsion(player, snake[i].mHeadPosition, snake[i].mHeadRadius)))
				{

				}

				//体
				for (int j = 0; j < Snake::kBodyMax; j++)
				{
					if (!snake[i].mIsDeath && CircleCapsuleCollsion(player, snake[i].mBodyPosition[j], snake[i].mBodyRadius))
					{
						ui.TsuchinokoScore(player.mIsStrikeActive);
						ui.AddCombo();
						snake[i].mIsDeath = true;
					}
				}
			}
			//ツチノコ
			for (int i = 0; i < Tsuchinoko::kMaxTsuchinoko; i++) {

				collisionTsuchinoko(player, tsuchinoko[i]);
				//頭と尾
				if (!tsuchinoko[i].mIsDeath && (CircleCapsuleCollsion(player, tsuchinoko[i].mHeadPosition, tsuchinoko[i].mRadius) || Collision(player.mPosition, player.mRadius, tsuchinoko[i].mTailPosition, tsuchinoko[i].mRadius)))
				{

				}

				//体
				for (int j = 0; j < Tsuchinoko::kBodyMax; j++)
				{
					if (!tsuchinoko[i].mIsDeath && CircleCapsuleCollsion(player,tsuchinoko[i].mBodyPosition[j], tsuchinoko[i].mBodyRadius))
					{
						ui.TsuchinokoScore(player.mIsStrikeActive);
						ui.AddCombo();
						tsuchinoko[i].mIsDeath = true;
					}
				}

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//スクロール値をアップデートする
			screen.SetScroll(player);

			break;
		case OUTGAME:
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
			break;
		case INGAME:

			//背景描画
			map.Draw(screen);
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
			Novice::DrawSprite(Screen::kWindowWidth - 420, Screen::kWindowHeight - 220, explanation, 1, 1, 0.0f, WHITE);

			//ＵＩ描画
			ui.Draw(screen);

			break;
		case OUTGAME:
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
