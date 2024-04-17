#pragma once

#include <iostream>
#include <string>
#include "NovaUtils.h"
#include "novasdk_global.h"

NOVA_TRAFFIC_DLL char* readFile(char* fileName);
NOVA_TRAFFIC_DLL void writeFileAppend(const char* fileName, const char* buffer, std::streamsize len);
//文件存在则清空，不存在则创建新文件
NOVA_TRAFFIC_DLL bool createEmptyFile(const char* filename);


