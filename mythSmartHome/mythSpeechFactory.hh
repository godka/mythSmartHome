#pragma once
#include "VirtualSpeech.hh"
#include "mythLightSmartHome.hh"
#include "mythCommonSmartHome.hh"
#include "mythNopSmartHome.hh"
#include <string>
class mythSpeechFactory
{
public:
	static VirtualSpeech* CreateNew(cJSON* root);
	~mythSpeechFactory();
private:
	mythSpeechFactory();
};

