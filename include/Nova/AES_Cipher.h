#ifndef _AES_CIPHER_H_
#define _AES_CIPHER_H_
#include <iostream> 
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <windows.h>
#include "novasdk_global.h"

//#define byte char
//#define null NULL
#define boolean bool
#define _IP_MARK "."

class AES_ECB_Cipher
{
public:
    explicit AES_ECB_Cipher(unsigned char* key, bool use_cbc = false) : mKey(key), mState(NULL), use_cbc(use_cbc) {};

    /*int encode(char* src, int src_len, char* dest, int dest_len);
    int decode(char* src, int src_len, char* dest, int dest_len);*/
    int encode(unsigned char* src, int src_len, unsigned char* dest, int dest_len);
    int decode(unsigned char* src, int src_len, unsigned char* dest, int dest_len);
private:
    int MAX_DECODE_BUF_LEN = 100000;
    typedef unsigned char state_t[4][4];
    int strToUChar(char *ch, unsigned char *uch);
    int ucharToStr(unsigned char *uch, char *ch);
    unsigned char mRoundKey[240];
    unsigned char* mKey;
    state_t* mState;
    bool use_cbc;
    static const unsigned char scSbox[256];
    static const unsigned char scRsbox[256];
    static const unsigned char scRcon[255];
    static const unsigned int KEYLEN;
    static const unsigned int NR;
    static const unsigned int NB;
    static const unsigned int NK;

    static inline unsigned char getSBoxValue(unsigned char num);
    static inline unsigned char getSBoxInvert(unsigned char num);
    static inline unsigned char xtime(unsigned char num);
    static inline unsigned char Multiply(unsigned char x, unsigned char y);

    void AddRoundKey(unsigned char round);
    void InvAddRoundKey(unsigned char round);
    void KeyExpansion();

    void MixColumns();
    void SubBytes();
    void ShiftRows();
    void Cipher();
    void AES128_ECB_encrypt(unsigned char* input, unsigned char* out);

    void InvMixColumns();
    void InvSubBytes();
    void InvShiftRows();
    void InvCipher();
    void AES128_ECB_decrypt(unsigned char* input, unsigned char* out);
};

#endif
