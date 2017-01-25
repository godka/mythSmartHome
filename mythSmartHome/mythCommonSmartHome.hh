#pragma once
#include "VirtualSpeech.hh"
class mythCommonSmartHome :
	public VirtualSpeech
{
public:
	mythCommonSmartHome(cJSON* root);
	~mythCommonSmartHome();
	void Start();
};

