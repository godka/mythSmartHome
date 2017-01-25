#include "mythSpeechFactory.hh"


mythSpeechFactory::mythSpeechFactory()
{
}


VirtualSpeech* mythSpeechFactory::CreateNew(cJSON* root)
{
	cJSON* m_service = cJSON_GetObjectItem(root, "service");
	if (m_service){
		std::string service = m_service->valuestring;
		if (service == "light_smartHome"){
			return new mythLightSmartHome(root);
		}
		else{
			return new mythCommonSmartHome(root);
		}
	}
	else{
		return new mythCommonSmartHome(root);
	}
}

mythSpeechFactory::~mythSpeechFactory()
{
}
