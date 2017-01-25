#include "mythCommonSmartHome.hh"


mythCommonSmartHome::mythCommonSmartHome(cJSON* root)
	:VirtualSpeech(root)
{

}

mythCommonSmartHome::~mythCommonSmartHome()
{
}

void mythCommonSmartHome::Start()
{
	mythLog::GetInstance()->printf("Starting in Common loop\n");
	cJSON* m_answer = cJSON_GetObjectItem(_root, "answer");
	if (m_answer){
		cJSON* m_type = cJSON_GetObjectItem(m_answer, "type");
		if (m_type){
			if (strcmp(m_type->valuestring, "T") == 0){
				cJSON* m_text = cJSON_GetObjectItem(m_answer, "text");
				if (m_text){
					on_speak(m_text->valuestring);
					return;
				}
			}
		}
	}
}
