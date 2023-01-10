#pragma once
#include <Windows.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")

// コントローラの最大接続数 1～4
#define MAX_CONTROLLER_NUM 1

// Xinputコントローラー
class Controller
{

public:
	// ボタンコード
	enum Button {
		bUP			= 0x0001,		// 上
		bDOWN		= 0x0002,		// 下
		bLEFT		= 0x0004,		// 左
		bRIGHT		= 0x0008,		// 右
		bSTART		= 0x0010,		// メニュー
		bBACK		= 0x0020,		// ビュー
		lTHUMB		= 0x0040,		// L3
		rTHUMB		= 0x0080,		// R3
		lSHOULDER	= 0x0100,		// L1
		rSHOULDER	= 0x0200,		// R1
		bA			= 0x1000,		// A
		bB			= 0x2000,		// B
		bX			= 0x4000,		// X
		bY			= 0x8000,		// Y
		lTrigger	= 0x000F0000,	// L2
		rTrigger	= 0x00FF0000,	// R2
	};

	// スティックの向き
	enum StickDirection {
		lsdUP,		// LEFT STICK DIRECTION UP
		lsdDOWN,	// LEFT STICK DIRECTION DOWN
		lsdRIGHT,	// LEFT STICK DIRECTION RIGHT
		lsdLEFT,	// LEFT STICK DIRECTION LEFT
		rsdUP,		// RIGHT STICK DIRECTION UP
		rsdDOWN,	// RIGHT STICK DIRECTION DOWN
		rsdRIGHT,	// RIGHT STICK DIRECTION RIGHT
		rsdLEFT		// RIGHT STICK DIRECTION LEFT
	};

	struct StickMagnitude
	{
		int x;
		int y;
	};

private:

	static XINPUT_STATE state[MAX_CONTROLLER_NUM];			// 現在の入力状態
	static XINPUT_STATE previousState[MAX_CONTROLLER_NUM];	// 前回の入力状態

	static BYTE triggerDeadZone[MAX_CONTROLLER_NUM];		// トリガーのデッドゾーン

	static SHORT leftStickDeadZone[MAX_CONTROLLER_NUM];		// 左スティックのデッドゾーン
	static SHORT rightStickDeadZone[MAX_CONTROLLER_NUM];	// 右スティックのデッドゾーン

	static int number; // コントローラーの数

public:

	/// <summary>
	/// すべてのコントローラーの状態をセットする
	/// </summary>
	/// <returns>接続数</returns>
	static void SetState();

	/// <summary>
	/// 現在使われているか
	/// </summary>
	static bool IsUse();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// トリガーのデッドゾーンを変更する
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="deadzone">デッドゾーン　：　0～255</param>
	/// <returns>成功　or　失敗</returns>
	static bool SetTriggerDeadZone(int number, int deadzone);

	/// <summary>
	/// 右スティックのデッドゾーンを変更する
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="deadzone">デッドゾーン　：　0～32768</param>
	/// <returns>成功　or　失敗</returns>
	static bool SetRightStickDeadZone(int number, int deadzone);

	/// <summary>
	/// 左スティックのデッドゾーンを変更する
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="deadzone">デッドゾーン　：　0～32768</param>
	/// <returns>成功　or　失敗</returns>
	static bool SetLeftStickDeadZone(int number, int deadzone);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// ボタンが押された時
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="button">ボタン番号</param>
	/// <returns></returns>
	static bool IsTriggerButton(int number, Button button);

	/// <summary>
	/// ボタンが押されている時
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="button">ボタン番号</param>
	/// <returns></returns>
	static bool IsPressedButton(int number, Button button);

	/// <summary>
	/// ボタンが離された時
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="button">ボタン番号</param>
	/// <returns></returns>
	static bool IsReleaseButton(int number, Button button);

	/// <summary>
	/// スティックが指定の向きに倒された時
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="direction">スティック向き</param>
	/// <returns></returns>
	static bool IsStickDirection(int number, StickDirection direction);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 右トリガー
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="out">0～255</param>
	/// <returns>成功　or　失敗</returns>
	static bool GetRightTrigger(int number, int& out);

	/// <summary>
	/// 左トリガー
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="out">0～255</param>
	/// <returns>成功　or　失敗</returns>
	static bool GetLeftTrigger(int number, int& out);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 右スティック
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="outx">X軸</param>
	/// <param name="outy">Y軸</param>
	/// <returns>成功　or　失敗</returns>
	static bool GetRightStick(int number, int& outx, int& outy);

	static bool GetRightStick(int number, StickMagnitude& out);

	/// <summary>
	/// 左スティック
	/// </summary>
	/// <param name="number">コントローラー番号</param>
	/// <param name="outx">X軸</param>
	/// <param name="outy">Y軸</param>
	/// <returns>成功　or　失敗</returns>
	static bool GetLeftStick(int number, int& outx, int& outy);

	static bool GetLeftStick(int number, StickMagnitude& out);

	static int GetConnectionNumber() { return number; }

	static bool IsUseController();

private:

	Controller() = delete;
	Controller(const Controller&) = delete;
	void operator =(const Controller&) = delete;

};