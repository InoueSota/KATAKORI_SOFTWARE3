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
#include "collision.h"

//インスタンス化
Screen screen;
Ingame ingame;
Map map;
Player player;
Snake snake[Snake::kMaxSnake];
Tsuchinoko tsuchinoko[Tsuchinoko::kMaxTsuchinoko];

//スイッチ文で main.cpp を分けるやつ
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = INGAME;
