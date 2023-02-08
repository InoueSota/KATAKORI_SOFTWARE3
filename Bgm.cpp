#include "Bgm.h"
#include <Novice.h>




void Bgm::Init() {
	tutorialhandle = -1;
	ingamehandle = -1;
}

void Bgm::LoadBgm() {

	tutorial = Novice::LoadAudio("./Sounds/tutorial.mp3");
	ingame = Novice::LoadAudio("./Sounds/ingame.mp3");

}