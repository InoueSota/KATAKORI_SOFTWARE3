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
>>>>>>> フィーバ�E

//�C���X�^���X��
Screen screen;
UI ui;
Map map;
Player player;
Snake snake[Snake::kMaxSnake];
Tsuchinoko tsuchinoko[Tsuchinoko::kMaxTsuchinoko];
Fever fever;

//�X�C�b�`���� main.cpp �𕪂�����
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = INGAME;
