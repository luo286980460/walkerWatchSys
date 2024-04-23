#pragma once
#include "NovaUtils.h"
#include "novasdk_global.h"

class NOVA_TRAFFIC_DLL DeviceNowPlayItem
{
private :
	bool isOpen;
	int playId;
	char* content;
public:
	DeviceNowPlayItem(bool isOpen, int playId, char* content);
	~DeviceNowPlayItem();
	bool playIsOpen();
	int getPlayId();
	char* getContent();
};

