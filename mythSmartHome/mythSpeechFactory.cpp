#include "mythSpeechFactory.hh"


mythSpeechFactory::mythSpeechFactory()
{
}


VirtualSpeech* mythSpeechFactory::CreateNew(cJSON* root)
{
	cJSON* m_rc = cJSON_GetObjectItem(root, "rc");
	int m_rc_int = m_rc ? m_rc->valueint : 4;
	if (m_rc_int == 0){
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
	else{
		return new mythNopSmartHome(root);
	}
}

mythSpeechFactory::~mythSpeechFactory()
{
}
