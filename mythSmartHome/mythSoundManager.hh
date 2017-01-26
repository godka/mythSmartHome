#pragma once
#include "mythCommonUtils.hh"
#include "mythLog.hh"
class mythSoundManager
{
public:
	static mythSoundManager* GetIntance();
	static mythSoundManager* mmythSoundManager;
	static mythSoundManager* CreateNew(){
		return new mythSoundManager();
	}
	void Play(void* data, int audio_len);
	~mythSoundManager();
protected:
	void fill_audio(Uint8 *stream, int len);
private:	
	static void fill_audio_static(void *udata, Uint8 *stream, int len){
		if (udata){
			mythSoundManager* speech = (mythSoundManager*) udata;
			speech->fill_audio(stream, len);
		}
	}
	Uint32  _audio_len;
	Uint8  *_audio_pos;

	mythSoundManager();
};

