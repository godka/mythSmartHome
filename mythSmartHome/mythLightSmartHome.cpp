#include "mythLightSmartHome.hh"

mythLightSmartHome::mythLightSmartHome(cJSON* root)
	:VirtualSpeech(root)
{

}

mythLightSmartHome::~mythLightSmartHome()
{
}

void mythLightSmartHome::Start()
{
	//I am in light loop
	mythLog::GetInstance()->printf("Starting in Light loop\n");
	cJSON* out = cJSON_CreateObject();
	cJSON* m_service = cJSON_GetObjectItem(_root, "service");
	cJSON_AddStringToObject(out, "service", m_service ? m_service->valuestring : "undefined");
	cJSON* m_semantic = cJSON_GetObjectItem(_root, "semantic");
	if (m_semantic){
		cJSON* m_slots = cJSON_GetObjectItem(m_semantic, "slots");
		if (m_slots){
			cJSON* m_attr = cJSON_GetObjectItem(m_slots, "attrValue");
			if (m_attr){
				char* _attr = m_attr->valuestring;
				int ret = strcmp(_attr, "¿ª");
				if (ret == 0){
					cJSON_AddStringToObject(out, "command", "on");
				}
				else{
					cJSON_AddStringToObject(out, "command", "off");
				}
			}
			else{
				cJSON_AddStringToObject(out, "command", "undefined");
			}
		}
		else{
			cJSON_AddStringToObject(out, "command", "undefined");
		}
		char* out_str = cJSON_PrintUnformatted(out);
		mythLog::GetInstance()->printf("[Start Boardcast]%s\n",out_str);
		on_boardcast(out_str);
		free(out_str);
	}
	cJSON_Delete(out);
}
