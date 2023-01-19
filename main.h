#pragma once
#include <Novice.h>
#include "ControllerInput.h"
#include "Key.h"
#include "Vec2.h"
#include "Screen.h"
#include "Ingame.h"
#include "Player.h"
#include "Enemy.h"
#include "Tsuchinoko.h"
<<<<<<< HEAD
#include "Collision.h"
=======
#include "collision.h"
#include "fever.h"
>>>>>>> 繝輔ぅ繝ｼ繝舌・

//インスタンス化
Screen screen;
UI ui;
Map map;
Player player;
Snake snake[Snake::kMaxSnake];
Tsuchinoko tsuchinoko[Tsuchinoko::kMaxTsuchinoko];
Fever fever;

//スイッチ文で main.cpp を分けるやつ
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = INGAME;
