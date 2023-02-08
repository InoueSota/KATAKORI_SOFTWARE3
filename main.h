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



//�C���X�^���X��
Screen screen;
Title title;
Result result;
UI ui;
Map map;
Player player;
Enemy enemy;
Fever fever;
Bgm bgm;

//�X�C�b�`���� main.cpp �𕪂�����
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = TITLE;
