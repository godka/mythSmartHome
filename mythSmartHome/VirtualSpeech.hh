#pragma once
#include <stdio.h>
extern "C"{
#ifdef WIN32
#include "./include/speech_recognizer.h"
#else
#include "speech_recognizer.h"
#endif
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
/* wav��Ƶͷ����ʽ */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = ��һ���ṹ��Ĵ�С : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = ͨ���� : 1
	int				samples_per_sec;        // = ������ : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = ÿ���ֽ��� : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = ÿ�������ֽ��� : wBitsPerSample / 8
	short int       bits_per_sample;        // = ����������: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = �����ݳ��� : FileSize - 44 
} wave_pcm_hdr;

class VirtualSpeech
{
public:
	static inline int mythDelay(unsigned long usec)
	{
#ifdef WIN32
		Sleep((DWORD) (usec));
#else
		usleep(usec * 1000);
#endif
		return 0;
	}
	virtual void Start();
	VirtualSpeech(cJSON* root);
	~VirtualSpeech();
protected:
	void on_boardcast(const char* sendData);
	int on_speak(const char* src_text);
	//our common delay
	cJSON* _root;
private:
	wave_pcm_hdr default_wav_hdr;
};

