#include "VirtualSpeech.hh"


void VirtualSpeech::Start()
{
	//do nothing
}

VirtualSpeech::VirtualSpeech(cJSON* root)
{
	_root = root;
}


VirtualSpeech::~VirtualSpeech()
{
}

void VirtualSpeech::on_boardcast(const char* sendData)
{
	int sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int yes = 1;
	setsockopt(sclient, SOL_SOCKET, SO_BROADCAST, (const char*) &yes, sizeof(yes));
	struct sockaddr_in sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_port = htons(41234);
	sin.sin_addr.s_addr = inet_addr("255.255.255.255");
	int len = sizeof(sin);
	sendto(sclient, sendData, strlen(sendData), 0, (struct sockaddr *) &sin, len);
	closesocket(sclient);
}

int VirtualSpeech::on_speak(const char* src_text)
{
	unsigned int audio_len = 0;
	int ret = -1;
	int index = 0;
	const char*  sessionID = NULL;
	int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
	char* params = "voice_name = xiaowanzi, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
	if (NULL == src_text)
	{
		printf("params is error!\n");
		return ret;
	}
	mythLog::GetInstance()->printf("[on_speak]start speak:%s\n", src_text);
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		mythLog::GetInstance()->printf("[on_speak]QTTSSessionBegin failed, error code: %d.\n", ret);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int) strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		mythLog::GetInstance()->printf("[on_speak]QTTSTextPut failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		return ret;
	}
	//Play  
	while (1)
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (data)
		{
			mythSoundManager::GetIntance()->Play((void*)data, audio_len);
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}

	if (MSP_SUCCESS != ret)
	{
		mythLog::GetInstance()->printf("[on_speak]QTTSAudioGet failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		return ret;
	}
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		mythLog::GetInstance()->printf("[on_speak]QTTSSessionEnd failed, error code: %d.\n", ret);
	}
	return ret;
}
