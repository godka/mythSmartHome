#include "mythSpeechRec.hh"
#include "SDL2/SDL.h"
int main(int argc, char** argv){
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)){
		mythLog::GetInstance()->printf("SDL init failed!\n");
	}
	mythSpeechRec::CreateNew()->StartLoop();
	return 0;
}