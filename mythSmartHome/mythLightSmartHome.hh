#pragma once
#include "VirtualSpeech.hh"
class mythLightSmartHome :
	public VirtualSpeech
{
public:
	mythLightSmartHome(cJSON* root);
	~mythLightSmartHome();
	void Start();
};

