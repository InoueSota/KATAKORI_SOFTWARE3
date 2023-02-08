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
#include "Bgm.h"



//インスタンス化
Screen screen;
Title title;
Change change;
Result result;
UI ui;
Map map;
Player player;
Enemy enemy;
Fever fever;
Bgm bgm;

//スイッチ文で main.cpp を分けるやつ
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = TITLE;
