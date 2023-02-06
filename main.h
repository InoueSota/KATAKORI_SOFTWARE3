#pragma once
#include <Novice.h>
#include "ControllerInput.h"
#include "Key.h"
#include "Vec2.h"
#include "Screen.h"
#include "Outgame.h"
#include "Ingame.h"
#include "Player.h"
#include "Enemy.h"
#include "Snake.h"
#include "Tsuchinoko.h"
#include "Collision.h"
#include "Fever.h"



//インスタンス化
Screen screen;
Title title;
Result result;
UI ui;
Map map;
Player player;
Snake snake[Enemy::kEnemyMax];
Tsuchinoko tsuchinoko[Enemy::kEnemyMax];
Fever fever;

//スイッチ文で main.cpp を分けるやつ
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = TITLE;
