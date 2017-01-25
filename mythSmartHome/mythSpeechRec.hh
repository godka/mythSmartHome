#pragma once
extern "C"{
#include "speech_recognizer.h"
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"
}
#include <string>
#include "cJSON.h"
#include "mythSpeechFactory.hh"
//a wonderful class about speech rec
//it should be used in singleton mode
//created by MythKAst
//2017/1/24
#include "mythLog.hh"
class mythSpeechRec
{
public:
	static mythSpeechRec* GetIntance();
	static mythSpeechRec* mmythSpeechRec;
	static mythSpeechRec* CreateNew(){
		auto _speechrec = new mythSpeechRec();
		if (_speechrec->Success){
			return _speechrec;
		}
		else{
			delete _speechrec;
			return nullptr;
		}
	}
	static void on_result_static(const char* result, char is_last){
		mythSpeechRec::GetIntance()->on_result(result, is_last);
	}

	static void on_speech_begin_static(){
		mythSpeechRec::GetIntance()->on_speech_begin();
	}

	static void on_speech_end_static(int reason){
		mythSpeechRec::GetIntance()->on_speech_end(reason);
	}
	virtual void on_result(const char *result, char is_last);
	void on_result_decode(const char* str);
	virtual void on_speech_begin();
	virtual void on_speech_end(int reason);

	~mythSpeechRec();
	bool Success;
	void StartLoop();
protected:
	mythSpeechRec();
private:
	std::string _result;
	std::string _session_begin_params;
	int g_isListening;
};

