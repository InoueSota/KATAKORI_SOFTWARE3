#pragma once



class Bgm
{
public:
	Bgm() {
		Init();
	};

	void Init();

	int titlehandle;
	int title;
	int tutorialhandle;
	int tutorial;
	int ingamehandle;
	int ingame;
	void LoadBgm();

};

