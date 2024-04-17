//
// Created by nova003080 on 2021/3/29.
//

#ifndef NOVATRAFFICC__SDK_CALLBACK_H
#define NOVATRAFFICC__SDK_CALLBACK_H
#include "../NovaUtils.h"

class Callback {
public:
    Callback(){};
    virtual void onReceive(string tag,string filePath);
};


#endif //NOVATRAFFICC__SDK_CALLBACK_H
