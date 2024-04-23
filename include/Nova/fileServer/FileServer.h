//
// Created by nova003080 on 2021/3/29.
//

#ifndef NOVATRAFFICC__SDK_FILESERVER_H
#define NOVATRAFFICC__SDK_FILESERVER_H

#include "../NovaUtils.h"
#include "Callback.h"
#include "FileServerSocket.h"
#include <mutex>
#include <uuids.h>
#include <map>

class FileServer :public Callback {
private:
    string ip;
    int port;
    mutex lockMutex;
    FileServerSocket* serverSocket;
    map<string,Callback*> callbacks;
    const char *newGUID();
public:
   static FileServer &getInstance();

    void init(string ip, int port);

    bool isInit();

    string getIp();

    int getPort();

    string receiveFile(string savePath, string fileName);

    bool checkSocket();

    void addCallback(string tag, Callback *callback);

    void removeCallback(string tag);

    void onReceive(string tag, string filePath) override;
};

#endif //NOVATRAFFICC__SDK_FILESERVER_H
