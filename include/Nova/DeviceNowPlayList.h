#pragma once
#include "NovaUtils.h"
#include "novasdk_global.h"

class NOVA_TRAFFIC_DLL DeviceNowPlayList
{
private: int playId;
		char* content;
public :
	DeviceNowPlayList(int playId, char* content);
	~DeviceNowPlayList();
	int getPlayId();
	char* getContent();
};

