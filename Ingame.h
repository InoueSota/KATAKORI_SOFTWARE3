#pragma once
#include "Screen.h"
#include "Vec2.h"

class Player;
class Ingame
{
public:
	Ingame() {
		Init();
	}

	void Init();
	void DebagUpdate(Player& player);
	void BackGroundDraw();

	//デバッグ用
	static constexpr int kAxisLength = 50;
	static constexpr int kLineInterval = 150;
	Vec2 mXaxisStartPosition[kAxisLength];
	Vec2 mXaxisEndPosition[kAxisLength];
	Vec2 mYaxisStartPosition[kAxisLength];
	Vec2 mYaxisEndPosition[kAxisLength];
	bool XaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	bool YaxisNearPlayer(Vec2 startposition, Vec2 endposition, Player& player);
	void DebagDraw(Screen& screen);
};

