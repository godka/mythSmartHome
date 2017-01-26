#include "mythSpeechRec.hh"

mythSpeechRec* mythSpeechRec::GetIntance()
{
	if (!mmythSpeechRec){
		return CreateNew();
	}
	else{
		return mmythSpeechRec;
	}
}

mythSpeechRec*  mythSpeechRec::mmythSpeechRec = nullptr;
mythSpeechRec::mythSpeechRec()
{
	int	ret = MSP_SUCCESS;
	g_isListening = 0;
	const char* login_params = "appid = 587a592d, work_dir = .";
	int aud_src = 0;
	_session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8,vad_speech_tail = 500";
	ret = MSPLogin(NULL, NULL, login_params);
	if (MSP_SUCCESS != ret)	{
		mythLog::GetInstance()->printf("[MSPLogin]failed code %d.\n", ret);
		MSPLogout();
		Success = false;
		mmythSpeechRec = nullptr;
	}
	else{
		mythLog::GetInstance()->printf("[MSPLogin]success code %d.\n", ret);
		Success = true;
		mmythSpeechRec = this;
	}
}


mythSpeechRec::~mythSpeechRec()
{
}

void mythSpeechRec::on_result(const char *result, char is_last){
	if (result){
		_result += result;
		if (is_last){
			mythLog::GetInstance()->printf("[on_result]%s\n", _result.c_str());
			int str_len = _result.size();
			int ret = 0;
			const char* rec_text = MSPSearch("nlp_version =2.0", _result.c_str(), (unsigned int*) &str_len, &ret);
			if (MSP_SUCCESS != ret){
				mythLog::GetInstance()->printf("[MSPSearch]failed code is:%d\n", ret);
			}
			else{
				mythLog::GetInstance()->printf("[MSPSearch]success code is:%d\n", ret);
				on_result_decode(rec_text);
			}
		}
	}

}

void mythSpeechRec::on_result_decode(const char* str){
	cJSON* root = cJSON_Parse(str);
	if (!root){
		mythLog::GetInstance()->printf("[on_result_decode]str to json failed\n");
		return;
	}
	VirtualSpeech* speech = mythSpeechFactory::CreateNew(root);
	if (speech){
		speech->Start();
	}
	delete speech;
	cJSON_Delete(root);
}

void mythSpeechRec::on_speech_begin()
{
	_result = "";
	g_isListening = 1;
	mythLog::GetInstance()->printf("[on_speech_begin]start listen\n");
}

void mythSpeechRec::on_speech_end(int reason)
{
	mythLog::GetInstance()->printf("[on_speech_end]reason:%d\n", reason);
	if (reason >= 11200){
		g_isListening = 2;
	}
	else{
		g_isListening = 0;
	}
}

void mythSpeechRec::StartLoop()
{
	int errcode;
	struct speech_rec iat;
	char isquit = 0;
	struct speech_rec_notifier recnotifier = { on_result_static, on_speech_begin_static, on_speech_end_static };
#ifdef WIN32
	errcode = sr_init(&iat, _session_begin_params.c_str(), SR_MIC, DEFAULT_INPUT_DEVID, &recnotifier);
#else
	errcode = sr_init(&iat, session_begin_params.c_str(), SR_MIC, &recnotifier);
#endif
	if (errcode) {
		printf("speech recognizer init failed\n");
		return;
	}

	while (1) {
		if (g_isListening > 1){
			break;
		}
		if (!g_isListening){
			sr_stop_listening(&iat);
			if (errcode = sr_start_listening(&iat)) {
				printf("start listen failed %d\n", errcode);
				isquit = 1;
			}
			if (isquit)
				break;
		}
		SDL_Delay(1);
	}
	sr_uninit(&iat);
}
