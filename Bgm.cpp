#include "Bgm.h"
#include <Novice.h>




void Bgm::Init() {
	titlehandle = -1;
	tutorialhandle = -1;
	ingamehandle = -1;
	feverhandle = -1;
	resulthandle = -1;
}

void Bgm::LoadBgm() {

	title = Novice::LoadAudio("./Sounds/title.mp3");
	tutorial = Novice::LoadAudio("./Sounds/tutorial.mp3");
	ingame = Novice::LoadAudio("./Sounds/ingame.mp3");
	fever = Novice::LoadAudio("./Sounds/fever.mp3");
	result = Novice::LoadAudio("./Sounds/result.mp3");
}