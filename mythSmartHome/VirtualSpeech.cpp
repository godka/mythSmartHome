#include "VirtualSpeech.hh"


void VirtualSpeech::Start()
{
	//do nothing
}

VirtualSpeech::VirtualSpeech(cJSON* root)
{
	_root = root;
	default_wav_hdr =
	{
		{ 'R', 'I', 'F', 'F' },
		0,
		{ 'W', 'A', 'V', 'E' },
		{ 'f', 'm', 't', ' ' },
		16,
		1,
		1,
		16000,
		32000,
		2,
		16,
		{ 'd', 'a', 't', 'a' },
		0
	};
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
	printf("send ok:%s\n", sendData);
}

int VirtualSpeech::on_speak(const char* src_text)
{
	int          ret = -1;
	char* buffer = (char*) malloc(1024 * 1024 * 10);
	int index = 0;
	const char*  sessionID = NULL;
	unsigned int audio_len = 0;
	wave_pcm_hdr wav_hdr = default_wav_hdr;
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
	char* params = "voice_name = xiaowanzi, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
	if (NULL == src_text)
	{
		printf("params is error!\n");
		return ret;
	}
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int) strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		return ret;
	}
	memcpy(buffer + index, &wav_hdr, sizeof(wav_hdr));
	index += sizeof(wav_hdr);
	while (1)
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			memcpy(buffer + index, data, audio_len);
			index += audio_len;
			//fwrite(data, audio_len, 1, fp);
			wav_hdr.data_size += audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
		printf(">");
		mythDelay(1);

	}//合成状态synth_status取值请参阅《讯飞语音云API文档》
	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	memcpy(buffer + 4, &wav_hdr.size_8, sizeof(wav_hdr.size_8));
	memcpy(buffer + 40, &wav_hdr.data_size, sizeof(wav_hdr.data_size));
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n", ret);
	}
	PlaySound((LPCTSTR) buffer, NULL, SND_MEMORY);
	free(buffer);
	return ret;
}
