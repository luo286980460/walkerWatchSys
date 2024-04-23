//
// Created by nova003080 on 2021/3/29.
//

#ifndef NOVATRAFFICC__SDK_FILERECEIVETASK_H
#define NOVATRAFFICC__SDK_FILERECEIVETASK_H

#include "Callback.h"
#include "FileServer.h"
#include "../NovaUtils.h"
#include <mutex>
#include <condition_variable>

typedef  std::string String;

class FileReceiveTask : public Callback {
private :
    String id;
    String savePath;
    String fileName;
    std::mutex clock;
    std::condition_variable cv;
    boolean isSuccess = false;
public:
    FileReceiveTask(String savePath, String fileName);

    String getId();

    boolean prepare();

    void release();

    boolean receive(long timeOut);

    void onReceive(String tag, String filePath);
};

#endif //NOVATRAFFICC__SDK_FILERECEIVETASK_H
