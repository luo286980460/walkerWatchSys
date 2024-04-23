#pragma once
#ifndef FileServerSocket_HEAD
#define FileServerSocket_HEAD


#include "Callback.h"
#include "FileClient.h"
#include "../NovaTraffic.h"
#include <mutex>
#include <map>
#include <thread>

typedef string String;

class FileServerSocket :public Callback {
private :
    std::map<String, String> taskMap;

    Callback* callback;
    mutex lockMutex;
    static const int TIME_OUT = 10 * 1000;
    SOCKET listenScok;
    static void loopRev(FileServerSocket *server);
public:

    FileServerSocket(Callback *callback);

    boolean start(string &ip, int port);

    void addTask(String uuid, String savePath);

    void onReceive(String tag, String filePath) override;
};


#endif // !FileServerSocket_HEAD