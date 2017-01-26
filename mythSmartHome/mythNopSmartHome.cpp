#include "mythNopSmartHome.hh"


mythNopSmartHome::mythNopSmartHome(cJSON* root)
	:VirtualSpeech(root)
{

}

void mythNopSmartHome::Start()
{
	mythLog::GetInstance()->printf("[mythNopSmartHome]Start loop\n");
	on_speak(
		"我没有理解你的话"
	);
}

mythNopSmartHome::~mythNopSmartHome()
{
}
