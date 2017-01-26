#pragma once
#include "VirtualSpeech.hh"
class mythNopSmartHome :
	public VirtualSpeech
{
public:
	mythNopSmartHome(cJSON* root);
	void Start();
	~mythNopSmartHome();
};

