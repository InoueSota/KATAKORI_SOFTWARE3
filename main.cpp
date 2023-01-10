#include "main.h"

const char kWindowTitle[] = "トゲ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Screen::kWindowWidth, Screen::kWindowHeight);

	// 画像読み込み
	int explanation = Novice::LoadTexture("./Resources/Debugs/Explanation.png");

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

			//初期化
			if (Key::IsTrigger(DIK_R)){
				player.Init();
				snake.Init();
			}

			//プレイヤーアップデート
			player.Update();

			//敵アップデート
			snake.Update();

			//当たり判定
			collision(player, snake);

			//スクロール値をアップデートする
			screen.SetScroll(player);

			//線の位置を画面内に固定する
			//ingame.DebagUpdate(player);

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
			ingame.BackGroundDraw();
			//グリッド線描画
			ingame.DebagDraw(screen);
			//敵描画
			snake.Draw(screen);
			//プレイヤー描画
			player.Draw(screen);

			//当たり判定のデバッグ
			for (int i = 0; i < 5; i++) {
				if (collisiontest[i]) {
					Novice::DrawBox(0 + i * 35, 0, 30, 30, 0, RED, kFillModeSolid);
				}
			}

			//制作中の操作説明を一時的に描画する
			Novice::DrawSprite(Screen::kWindowWidth - 420, Screen::kWindowHeight - 220, explanation, 1, 1, 0.0f, WHITE);

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
