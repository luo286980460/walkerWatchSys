#pragma once
#ifndef NOVADES
#define NOVADES

extern "C" {
    #include "FileReader.h"
    #include "Encrypt.h"
}
 
extern int DES_CBC_ENCODE(char* intput, int intputCount, int* IV, int* K, char** d);

extern int DES_CBC_DECODE(char* intput, int intputCount, int* IV, int* K, char** d);

//º”√‹
extern int DES_ENCODE(char* buf, int length, char* key, char** data);
//Ω‚√‹
extern int DES_DECODE(char* buf, int length, char* key, char** data);
 


#endif //NOVADES


