#pragma once
#include <stdio.h>
extern "C"{
#include "speech_recognizer.h"
#include "msp_errors.h"
#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "ws2_32.lib") 
#else
#include <unistd.h>
#define INVALID_SOCKET  (~0)
#define SOCKET_ERROR    (-1)
#include <wchar.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define closesocket close
#include <sys/time.h>
#endif // !WIN32
#include "cJSON.h"
#include "qtts.h"
}
#include "mythLog.hh"
#include "mythCommonUtils.hh"
#include "mythSoundManager.hh"
class VirtualSpeech
{
public:
	virtual void Start();
	VirtualSpeech(cJSON* root);
	~VirtualSpeech();
protected:
	void on_boardcast(const char* sendData);
	int on_speak(const char* src_text);
	//our common delay
	cJSON* _root;
};

