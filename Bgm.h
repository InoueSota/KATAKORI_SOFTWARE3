#pragma once



class Bgm
{
public:
	Bgm() {
		Init();
	};

	void Init();

	int tutorialhandle;
	int tutorial;
	int ingamehandle;
	int ingame;
	void LoadBgm();

};

