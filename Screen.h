#pragma once
#include <Novice.h>
#include "Vec2.h"

class Player;
class Screen
{
public:

	static constexpr int kWindowWidth = 1280;
	static constexpr int kWindowHeight = 720;
	static constexpr int kMiniMapSize = 100;

	Screen() {
		Init();
	}

private:

	Vec2 mWorldCenter;		//ワールドの中心
	Vec2 mScroll;			//スクロール値

	bool mIsZoomEasing;		//ズームフラグ
	bool mIsStikeZoomEasing;//ズームフラグ（ストライク専用）
	float mZoom;			//ズーム値
	float mZoomEasingt;		//ズームを徐々に行うイージング
	float mZoomStartValue;	//ズーム開始値
	float mZoomEndValue;	//ズーム終了値

	bool mIsShakeActive;	//シェイクフラグ
	Vec2 mScreenShake;		//シェイク値
	float mShakeMag;		//シェイク倍率
	float mShakeEasingt;	//シェイクを徐々に収めるイージング

	bool mIsHitStop;		//ヒットストップシェイク
	int mHitStopFrame;		//ヒットストップフレーム

	Vec2 mMiniMapCenter;	//ミニマップの中心
	float mMiniMapZoom;		//ミニマップのズーム値

public:


	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 被弾時のカメラシェイク
	/// </summary>
	/// <param name="condition">シェイクする条件</param>
	void ShakeUpdate(bool condition);

	/// <summary>
	/// カメラシェイク（ X軸 と Y軸 どちらも同じ値 ）
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="condition">シェイクする条件</param>
	void ShakeUpdate(int min, int max, bool condition);

	/// <summary>
	/// カメラシェイク（ X軸 と Y 軸 別の値 ）
	/// </summary>
	/// <param name="minX">最小値（ X軸 ）</param>
	/// <param name="maxX">最大値（ X軸 ）</param>
	/// <param name="minY">最小値（ Y軸 ）</param>
	/// <param name="maxY">最大値（ Y軸 ）</param>
	/// <param name="condition">シェイクする条件</param>
	void ShakeUpdate(int minX, int maxX, int minY, int maxY, bool condition);

	/// <summary>
	///	ヒットストップ
	/// </summary>
	void HitStopUpdate();

	/// <summary>
	/// ヒットストップフラグをtrueにする
	/// </summary>
	inline void SetHitStop() { mIsHitStop = true; }

	/// <summary>
	/// ヒットストップフラグを取得する
	/// </summary>
	inline bool GetHitStop() { return mIsHitStop; }

	/// <summary>
	/// スクロール
	/// </summary>
	void ScrollUpdate(Player& player);

	/// <summary>
	/// スクロール値を取得する
	/// </summary>
	inline Vec2 GetScroll() { return mScroll; }

	/// <summary>
	/// ズーム
	/// </summary>
	void ZoomUpdate(bool isFever, bool isOldFever, bool isStrikeActive, bool isOldStrikeActive);

	/// <summary>
	/// ズーム値を取得する
	/// </summary>
	inline float GetZoom() { return mZoom; }


	//--------------------------------------------------------------------------------------------//


	/// <summary>
	/// 線を描画する
	/// </summary>
	/// <param name="startposition">描画する線の始点座標</param>
	/// <param name="endposition">描画する線の終点座標</param>
	/// <param name="color">描画する四角形の色</param>
	void DrawLine(Vec2 startposition, Vec2 endposition, unsigned int color = WHITE, bool isScroll = true);

	/// <summary>
	/// 四角形を描画する
	/// </summary>
	/// <param name="position">描画する四角形の左上座標</param>
	/// <param name="width">描画する四角形の横幅</param>
	/// <param name="height">描画する四角形の縦幅</param>
	/// <param name="angle">描画する四角形の回転角</param>
	/// <param name="color">描画する四角形の色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	/// <param name="isScroll">スクロールするか</param>
	void DrawBox(Vec2 position, float width, float height, float angle = 0.0f, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// 楕円を描画する
	/// </summary>
	/// <param name="position">楕円の中心座標</param>
	/// <param name="radiusX">楕円の半径 (X方向) </param>
	/// <param name="radiusY">楕円の半径 (Y方向) </param>
	/// <param name="angle">楕円の回転角(rad)</param>
	/// <param name="color">楕円の色</param>
	void DrawEllipse(Vec2 position, float radiusX, float radiusY, float angle = 0.0f, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid);

	/// <summary>
	/// 円を描画する
	/// </summary>
	/// <param name="position">円の中心座標</param>
	/// <param name="radius">円の半径</param>
	/// <param name="color">円の色</param>
	void DrawCircle(Vec2 position, float radius, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid);

	//----- DrawBox との違い → Position が中心座標であるところ -----//
	/// <summary>
	/// 正方形を描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="size">大きさ(= 直径)</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	/// <param name="isScroll">スクロールするか</param>
	void DrawSquare(Vec2 position, float size, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// 長方形を描画する（傾きあり）
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="angle">傾き</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	/// <param name="isScroll">スクロールするか</param>
	void DrawRectAngle(Vec2 position, float width, float height, float angle, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);
	
	/// <summary>
	/// 長方形を描画する（傾きなし）
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	/// <param name="isScroll">スクロールするか</param>
	void DrawRectAngle(Vec2 position, float width, float height, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// 静止画（長方形）を描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="angle">傾き</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawPicture(Vec2 position, float width, float height, float angle, float srcW, float srcH, int textureHandle, unsigned int color);

	/// <summary>
	/// 静止画（正方形）を描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="size">大きさ(= 直径)</param>
	/// <param name="angle">傾き</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawPicture(Vec2 position, float size, float angle, float srcW, float srcH, int textureHandle, unsigned int color);

	/// <summary>
	/// 連番画像（長方形）を描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="sheets">画像枚数</param>
	/// <param name="frame">何フレームで切り替えるか</param>
	/// <param name="frameVariable">加算しているフレームの変数</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawAnime(Vec2 position, float width, float height, int srcX, int srcW, int srcH, int sheets, int frame, int& frameVariable, int textureHandle, unsigned int color);

	/// <summary>
	/// 連番画像（正方形）を描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="size">大きさ(= 直径)</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="sheets">画像枚数</param>
	/// <param name="frame">何フレームで切り替えるか</param>
	/// <param name="frameVariable">加算しているフレームの変数</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawAnime(Vec2 position, float size, int srcX, int srcW, int srcH, int sheets, int frame, int& frameVariable, int textureHandle, unsigned int color);

	/// <summary>
	/// ＵＩ（スクロールしない）（長方形）
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="angle">傾き</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X（変数）</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="scale">大きさを変化させる</param>
	void DrawUI(Vec2 position, float width, float height, int srcX, int srcW, int srcH, int textureHandle, unsigned int color, Vec2 scale = { 1.0f, 1.0f }, bool isScroll = false);

	/// <summary>
	/// ＵＩ（スクロールしない）（正方形）
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="size">大きさ(= 直径)</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X（変数）</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="scale">大きさを変化させる</param>
	void DrawUI(Vec2 position, float size, int srcX, int srcW, int srcH, int textureHandle, unsigned int color, Vec2 scale = { 1.0f, 1.0f }, bool isScroll = false);

	/// <summary>
	/// ミニマップの位置を円で描画する
	/// </summary>
	/// <param name="position">中心座標</param>
	/// <param name="color">描画するスプライトの色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	/// <param name="radius">半径</param>
	void DrawMiniMap(Vec2 position, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, float radius = 2.0f);


	//--------------------------------------------------------------------------------------------//


	/// <summary>
	/// ワールド座標に変換する
	/// </summary>
	Vec2 WorldTransform(Vec2 position);

	/// <summary>
	/// ミニマップ座標に変換する
	/// </summary>
	Vec2 MiniMapTransform(Vec2 position);

	/// <summary>
	/// スクリーン座標に変換する
	/// </summary>
	Vec2 ScreenTransform(Vec2 position);
};


