#pragma once
#include <istream>
#include "novasdk_global.h"

class NOVA_TRAFFIC_DLL SocketResult
{
public:
	SocketResult(char* data, int len);
	~SocketResult();
	char* toStrChar(int start);
	char* getOrgChar();
    std::string toStr(int start);
	int getLen();
    bool isSetSuccess();

private:
	static const int SUCCESS = 1;
	/**
	 * data的长度
	 */
    int len;
    /**
     * 控制卡返回的数据，“指令码+数据域”部分
     */
	char* data;
	bool isSuccesed(int minLen);

};