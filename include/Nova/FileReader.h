//
// Created by Lightning on 2017/5/1.
//

#ifndef DES2_0_FILEREADER_H
#define DES2_0_FILEREADER_H

#include "require.h"
#include "novasdk_global.h"

NOVA_TRAFFIC_DLL void ascToBinary(int character, int *ones, int index);

NOVA_TRAFFIC_DLL char BinaryToasc(int *ones, int index);

NOVA_TRAFFIC_DLL void readBinarys(char *temp, int *s);

NOVA_TRAFFIC_DLL void writeBinary(int *temp, char *s);

NOVA_TRAFFIC_DLL int readFile64(FILE *fd, int *s);

NOVA_TRAFFIC_DLL int readChar64(char* butf, int offset, int max, int* s);

NOVA_TRAFFIC_DLL void writeFile64(FILE *fd, int *s);

NOVA_TRAFFIC_DLL void closeFile(FILE *fd);

#endif //DES2_0_FILEREADER_H
