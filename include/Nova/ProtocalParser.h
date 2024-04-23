#pragma once
#include "NovaUtils.h"
#include "md5.h"
#include "AES_Cipher.h"
#include "novasdk_global.h"

class NOVA_TRAFFIC_DLL ProtocalParser {
private:
    static const char PACKET_START_FLAG = (char) 0xAA;
    static const char PACKET_END_FLAG = (char) 0xCC;
    static const char PACKET_TRANSFER_FLAG = (char) 0xEE;

    static const int PACKET_MIN_SIZE = 7;
    static const int PACKET_WITHOUT_WHAT_AND_DATA = 6;
    static const int PACKET_WHAT_OFFSET = 3;
    static const int PACKET_DATA_OFFSET = 4;
    static const char DES_ADD_SIZE = 7;
    static const int PACKET_WITHOUT_DATA_SIZE = 7;

    static const int DEFLAUT_PARK_BLOCK_SIZE = 4096;
    static const int END_CRC_SIZE = 2;
    static const int PACKET_CRC_SIZE = 2;
    static const int NO_START_RCV = -1;
    static const int START_RCV = 0;
    static const int START_RCV_END = 1;

    static const char WHAT_SECRET_DES_MD5_REQ = 0x70;

private:
    int pkgType = NO_START_RCV; // -1为 开始 0 开始 1结束

    int maxBufferSize; // 超过就补4096
    int mBufferLen;
    int endNeed = END_CRC_SIZE;
    char *mDesSecret = NULL;
    MD5 *mMd5 = NULL;
    AES_ECB_Cipher *mAesCipher = NULL;
public:
    char *remainBuffer = NULL;
    bool isNeedReload = false;
    int packetSize = 0;

public:
	ProtocalParser();
	~ProtocalParser();
	ProtocalParser(char* mDesSecretint,int secretType);
	char* recvProc(char* buffer, int count, short what);
private:
	//剩余的字符长度
	long remainSize;
	char* recvPackage(char* buffer, int count, short what);
};

