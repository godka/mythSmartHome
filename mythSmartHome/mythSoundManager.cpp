#include "mythSoundManager.hh"

mythSoundManager*  mythSoundManager::mmythSoundManager = nullptr;
mythSoundManager::mythSoundManager()
{
	//SDL_AudioSpec  
	_audio_pos = nullptr;
	_audio_len = 0;
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = 16000;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = 1;
	wanted_spec.silence = 0;
	wanted_spec.samples = 512;
	wanted_spec.callback = fill_audio_static;
	wanted_spec.userdata = this;
	if (SDL_OpenAudio(&wanted_spec, NULL) < 0){
		mythLog::GetInstance()->printf("[on_speak]can't open audio.\n");
	}
	SDL_PauseAudio(0);
	mmythSoundManager = this;
}
void mythSoundManager::fill_audio(Uint8 *stream, int len)
{
	SDL_memset(stream, 0, len);
	if (_audio_len == 0 || _audio_pos == nullptr)
		return;
	len = (len > _audio_len ? _audio_len : len);

	SDL_MixAudio(stream, _audio_pos, len, SDL_MIX_MAXVOLUME);
	_audio_pos += len;
	_audio_len -= len;
}

mythSoundManager* mythSoundManager::GetIntance()
{
	if (!mmythSoundManager){
		return CreateNew();
	}
	else{
		return mmythSoundManager;
	}
}

void mythSoundManager::Play(void* data,int audio_len)
{
	_audio_pos = (Uint8*) data;
	_audio_len = audio_len;
	while (_audio_len > 0)//Wait until finish  
		SDL_Delay(1);
	//SDL_PauseAudio(1);
}

mythSoundManager::~mythSoundManager()
{
}
