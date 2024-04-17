#pragma once
#ifndef  FileClient_HEAD
#define FileClient_HEAD

#include "../NovaTraffic.h"
#include "../NovaUtils.h"
#include <mutex>
#include <map>
#include "Callback.h"
#include <thread>
#include "novasdk_global.h"

typedef string String;
class NOVA_TRAFFIC_DLL FileClient{
private:
	SOCKET* clientSock;
	static const int TAG_LENGTH = 50;
	map<String, String> taskMap;
	Callback*  callback;
	mutex lockMutex;
public:
	FileClient(SOCKET* socket, map<String, String> taskMap, Callback* callback);
	~FileClient();
	void recive();
private:
	string checkTag();
	int getFileLenght();
	void writeFile();
	static void run(FileClient* client);
};
#endif // ! FileClient_HEAD
