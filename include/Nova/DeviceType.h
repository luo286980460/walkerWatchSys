#pragma once
#include <ctime>
#include "novasdk_global.h"

class NOVA_TRAFFIC_DLL DeviceType
{
private :
	tm* date;
	bool isboxDoorOpen;
	bool isscreenPowerOpen;
	int temperature;
	bool isinputSourceInside;
	int environmentBrightness;
	int brightnessType; //1-自动  2-手动 3-定时 
	int screenBrightness;
	int isscreenOpen;
public:
	DeviceType();
	~DeviceType();
	tm* getDate();
	void setDate(tm* date);
	bool isBoxDoorOpen();
	void setBoxDoorOpen(bool isBoxDoorOpen);
	bool isScreenPowerOpen();
	void setScreenPowerOpen(bool isScreenPowerOpen);
	int isScreenOpen();
	void setScreenOpen(int isScreenOpen);
	int getTemperature();
	void setTemperature(int temperature);
	bool isInputSourceInside();
	void setInputSourceInside(bool isInputSourceInside);
	int getEnvironmentBrightness();
	void setEnvironmentBrightness(int environmentBrightness);
	int getBrightnessType();
	void setBrightnessType(int brightnessType);
	int getScreenBrightness();
	void setScreenBrightness(int screenBrightness);
};

