#pragma once
#include <Novice.h>
#include "ControllerInput.h"
#include "Key.h"
#include "Vec2.h"
#include "Screen.h"
#include "Outgame.h"
#include "Ingame.h"
#include "Player.h"
#include "Snake.h"
#include "Tsuchinoko.h"
#include "Collision.h"
#include "Fever.h"

class Enemy
{
public:
	//�G�̍ő吔
	static constexpr int kEnemyMax = 10;
};

//�C���X�^���X��
Screen screen;
Title title;
UI ui;
Map map;
Player player;
Snake snake[Enemy::kEnemyMax];
Tsuchinoko tsuchinoko[Enemy::kEnemyMax];
Fever fever;

//�X�C�b�`���� main.cpp �𕪂�����
enum Scene
{
	TITLE,
	INGAME,
	OUTGAME
};
Scene scene = TITLE;
