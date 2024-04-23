#pragma once

#include <iostream> 
#include <stdio.h>
#include "winsock.h"
#include <string.h>
#include <vector>
#include "DES.h"
#include "md5.h"
#include "AES_Cipher.h"
#include "novasdk_global.h"

//using namespace std;

//#define byte char
//#define null NULL
//#define boolean bool
#define _IP_MARK "."


static int PACKET_WITHOUT_DATA_SIZE = 7;
static int PACKET_START_OFFSET = 0;
static char PACKET_TRANSFER_FLAG = (char)0xEE;
static char PACKET_START_FLAG = (char)0xAA;
static char PACKET_END_FLAG = (char)0xCC;

static char PACKET_DEVICEID_ARG1 = (char)0xff;
static char PACKET_DEVICEID_ARG2 = (char)0xff;
static char DES_ADD_SIZE = 7;
//协议格式2.0标志code
static char EXPAND_FRAME_FLAG = 0xEF;

static char WHAT_NO_RSP = -0x01;
static char WHAT_HEART_BEAT = 0x00;
static char WHAT_GET_DEVICE_TYPE_REQ = 0x01;
static char WHAT_GET_DEVICE_TYPE_RSP = 0x02;
static char WHAT_SCREEN_POWER_REQ = 0x05;
static char WHAT_SCREEN_POWER_RSP = 0x06;

static char WHAT_SCREEN_BRIGHTNESS_MODE_REQ = 0x07;
static char WHAT_SCREEN_BRIGHTNESS_MODE_RSP = 0x08;
static char WHAT_DEVICE_SET_TIME_REQ = 0x09;
static char WHAT_DEVICE_SET_TIME_RSP = 0x0a;
static char WHAT_SCREEN_POINT_DETECT_REQ = 0x0B;
static char WHAT_SCREEN_POINT_DETECT_RSP = 0x0C;
static char WHAT_DEVICE_REBOOT_REQ = 0x0d;
static char WHAT_DEVICE_REBOOT_RSP = 0x0e;
static char WHAT_FILENAME_REQ = 0x11;
static char WHAT_FILENAME_RSP = 0x12;
static char WHAT_FILESEND_REQ = 0x13;
static char WHAT_FILESEND_RSP = 0x14;

static char WHAT_SET_ENVIRONMENT_ALARM_REQ = 0x15;
static char WHAT_SET_ENVIRONMENT_ALARM_RSP = 0x16;

static char WHAT_SCREEN_BRIGHTNESS_SET_AUTO_MODE_PARAM_REQ = 0x17;
static char WHAT_SCREEN_BRIGHTNESS_SET_AUTO_MODE_PARAM_RSP = 0x18;

static char WHAT_SET_DEVICE_IP_REQ = 0x19;
static char WHAT_SET_DEVICE_IP_RSP = 0x1A;

static char WHAT_PLAYBYLIST_REQ = 0x1B;
static char WHAT_PLAYBYLIST_RSP = 0x1C;

static char WHAT_DEVICE_RESET_IP_REQ = 0x21;
static char WHAT_DEVICE_RESET_IP_RSP = 0x22;

static char WHAT_QUERY_VERSIONINFO_REQ = 0x23;
static char WHAT_QUERY_VERSIONINFO_RSP = 0x24;

static char WHAT_SCREEN_MY_BOARD_POWER_AND_POWER_REQ = 0x25;
static char WHAT_SCREEN_MY_BOARD_POWER_AND_POWER_RSP = 0x26;

static char WHAT_GET_ENVIRONMENT_ALARM_REQ = 0x29;
static char WHAT_GET_ENVIRONMENT_ALARM_RSP = 0x2A;

static char WHAT_SCREEN_BRIGHTNESS_GET_AUTO_MODE_PARAM_REQ = 0x2B;
static char WHAT_SCREEN_BRIGHTNESS_GET_AUTO_MODE_PARAM_RSP = 0x2C;

static char WHAT_GET_PLAYING_ITEM_REQ = 0x2D;
static char WHAT_GET_PLAYING_ITEM_RSP = 0x2E;
static char WHAT_GET_PLAYING_ALL_REQ = 0x3A;
static char WHAT_GET_PLAYING_ALL_RSP = 0x3B;

static char WHAT_GET_ERROR_POINT_REQ1 = 0x36;
static char WHAT_GET_ERROR_POINT_RSP1 = 0x37;
static char WHAT_GET_ERROR_POINT_RSP2 = 0x38;
static char WHAT_GET_ERROR_POINT_REQ2 = 0x39;

static char WHAT_SET_TIMING_PLAYLIST_PARAM_REQ = 0x41;
static char WHAT_SET_TIMING_PLAYLIST_PARAM_RSP = 0x42;

static char WHAT_SCREEN_BRIGHTNESS_SET_TIMING_MODE_PARAM_REQ = 0x43;
static char WHAT_SCREEN_BRIGHTNESS_SET_TIMING_MODE_PARAM_RSP = 0x44;

static char WHAT_SET_VOLUME_PARAM_REQ = 0x45;
static char WHAT_SET_VOLUME_PARAM_RSP = 0x46;

static char WHAT_GET_SDCARD_SIZE_REQ = 0x47;
static char WHAT_GET_SDCARD_SIZE_RSP = 0x48;

static char WHAT_SET_FTP_DOWNLOAD_REQ = 0x49;
static char WHAT_SET_FTP_DOWNLOAD_RSP = 0x50;

static char GET_SCREEN_BRIGHTNESS_SET_TIMING_MODE_PARAM_INFO = (char)0x51;
static char GET_SCREEN_OPEN_CLOSE_SET_TIMING_PARAM_INFO = (char)0x52;
static char GET_SCREEN_TIMING_PLAYLIST_PARAM_INFO = (char)0x53;

static char WHAT_SECRET_DES_MD5_REQ = 0x70;
static char WHAT_SECRET_DES_MD5_RSP = 0x71;

static char WHAT_DEVICE_NAME_REQ = 0x7E;
static char WHAT_DEVICE_NAME_RSP = 0x7F;

static char WHAT_CLEAN_PLAYLIST_REQ = 0x7C;
static char WHAT_CLEAN_PLAYLIST_RSP = 0x7D;

static char WHAT_SCREENSHOT_REQ = (char)0x80;
static char WHAT_SCREENSHOT_RSP = (char)0x81;

static char WHAT_SCREEN_WIDTH_HEIGHT_REQ = (char)0x82;
static char WHAT_SCREEN_WIDTH_HEIGHT_RSP = (char)0x83;

static char WHAT_SCREEN_MY_BOARD_POWER_REQ = (char)0x84;
static char WHAT_SCREEN_MY_BOARD_POWER_RSP = (char)0x85;

static char WHAT_FUNCTION_CARD_POWER_REQ = (char)0x86;
static char WHAT_FUNCTION_CARD_POWER_RSP = (char)0x87;

static char WHAT_REGION_UPDATE_REQ = (char)0x88;
static char WHAT_REGION_UPDATE_RSP = (char)0x89;

static char WHAT_SCREEN_OPEN_CLOSE_SET_TIMING_PARAM_REQ = (char)0x8A;
static char WHAT_SCREEN_OPEN_CLOSE_SET_TIMING_PARAM_RSP = (char)0x8B;

static char WHAT_SYSTEM_UPDATE_REQ = (char)0x90;
static char WHAT_SYSTEM_UPDATE_RSP = (char)0x91;

static char SET_TIME_AND_ZONE = (char)0x95;
static char GET_TIME_AND_ZONE = (char)0x96;
static char SET_NTP_PARAM = (char)0x97;
static char GET_NTP_PARAM = (char)0x98;

static char SET_RELAY = (char)0x9e;
static char WHAT_APP_REMOVE = (char)0x9f;

static char WHAT_VIRTUAL_CONNECT_PARAM_REQ = (char)0xF3;
static char WHAT_VIRTUAL_CONNECT_PARAM_RSP = (char)0xF4;

static char WHAT_FILESEND_OVER_RSP = (char)0xF9;

static char WHAT_GET_SURVEIL = (char)0xA0;

//获取视频源控制方式
static char  WHAT_GET_VIDEO_SOURCE_CONTROL_MODE = (char)0xA1;
//获取视频源分辨率
static char  WHAT_GET_VIDEO_SOURCE = (char)0xA2;
//设置视频源控制方式
static char  WHAT_SET_VIDEO_SOURCE_CONTROL_MODE = (char)0xA3;
//设置视频源分辨率
static char  WHAT_SET_VIDEO_SOURCE = (char)0xA4;
//设置内部视频源分辨率
static char WHAT_SET_INTERNAL_VIDEO_SOURCE = (char)0xA5;
//获取内部视频源分辨率
static char WHAT_GET_INTERNAL_VIDEO_SOURCE = (char)0xA6;
//获取所有播放列表id
static char  WHAT_GET_ALL_PLAYLIST_ID = (char)0xA7;
//自动亮度调节参数
static char  WHAT_SET_AUTO_BRIGHTNESS_TIME_PARAM = (char)0xAA;
//文件下载
static char WHAT_DOWNLOAD_FILES = (char)0xAB;
//删除节目
static char  WHAT_DELETE_PLAYLIST = (char)0xAE;
//获取cpu和内存使用率
static char WHAT_GET_CPU_AND_MEMORY_RATE = (char)0xAF;
//获取所有媒体文件名称
static char WHAT_GET_ALL_MEDIA_FILENAME = (char) 0xB0;
//获取某个媒体文件
static char WHAT_GET_MEDIA_FILE = (char)0xB5;
//获取文件名称
static char  WHAT_GET_FILENAME_BY_TYPE = (char)0xB9;
//获取系统信息
static char  WHAT_GET_SYSTEM_INFO = (char)0xC2;
//获取屏幕亮度控制模式和当前亮度
static char WHAT_GET_BRIGHTNESS_PARAM = (char)0xC3;
//获取ap是否打开
static char GET_WIFI_TYPE = (byte) 0x9b;
//获取ap信息
static char WHAT_GET_AP_INFO = (char)0xC4;
//设置ap是否可用
static char WHAT_SET_AP_ENABLE = (char)0xC5;
//设置ap信息
static char WHAT_SET_AP_CONFIG = (char)0xC6;
//get 移动网络信息
static char WHAT_GET_MOBILE_INFO = (char)0xC9;
//wifi相关
static char  WHAT_GET_WIFI_STATE = (char)0xCA;
static char  WHAT_SET_WIFI_STATE = (char)0xCB;
static char  WHAT_GET_WIFI_LIST = (char)0xCE;
static char  WHAT_CONNECT_WIFI = (char)0xCF;

//字体相关
static char  WHAT_GET_FONT_LIST = (byte)0xD1;
static char  WHAT_SET_FONT = (byte)0xD2;


//获取系统传感器信息
static char WHAT_GET_SYSTEM_SENSOR_INFO = (char)0xC7;
//set 系统传感器信息
static char WHAT_SET_SYSTEM_SENSOR_INFO = (char)0xC8;
//音量
static char  WHAT_GET_VOLUME_PARAM = (char)0xA9;
//虚链接
static char  WHAT_GET_VIRTUAL_PARAM = (char)0xD3;
//lora信息
static char  WHAT_GET_LORAINFO = (char)0xD6;
static char  WHAT_SET_LORAINFO = (char)0xD7;
//同步播放开关
static char  WHAT_SET_SYNC_ENABLE = (char)0xBC;
//获取同步播放状态
static char  WHAT_GET_SYNC_ENABLE = (char)0xBD;
//获取同步播放状态
static char  WHAT_SET_ANTIALIAS = (char)0xE5;
//获取同步播放状态
static char  WHAT_GET_ANTIALIAS = (char)0xE6;

static char  WHAT_GET_SUPPORT_VIDEO_SOURCE = (char)0xE7;

static char  WHAT_UPDATE_TEXT = (char)0xE8;

static char  WHAT_UPDATE_IMG = (char)0xE9;

//static char  WHAT_GET_SUPPORT_VIDEO_SOURCE = (char)0xEA;
//
//static char  WHAT_GET_SUPPORT_VIDEO_SOURCE = (char)0xEB;

//设置ping ip
static short WHAT_SET_PING_IP = (short)0x0101;
//get ping ip
static short WHAT_GET_PING_IP = (short)0x0102;


//获取设备的SN号
static char WHAT_GET_SN = (char) 0xD5;

//高级定时设置set
const char WHAT_SET_ADVANCE_TIMING_PARAM = (char) 0xDC;
//高级定时设置get
const char WHAT_GET_ADVANCE_TIMING_PARAM = (char) 0xDD;
//高级定时remove
const char WHAT_REMOVE_ADVANCE_TIMING_PARAM = (char) 0xDE;
//设置软SIM deviceKey
const short WHAT_SET_DEVICE_KEY =(short)0x0103;
//切换SIM卡工作模式(只能从实体Sim-->软SIM)
const short WHAT_SWITCH_TO_SOFTSIM =(short)0x0105;
//获取软sim模块信息
const short WHAT_GET_SOFT_SIM =(short)0x0108;
//获取ip白名单
const short WHAT_GET_WHITE_IP =(short)0x0109;
//添加ip白名单
const short WHAT_ADD_WHITE_IP =(short)0x010A;
//删除ip白名单
const short WHAT_DELETE_WHITE_IP =(short)0x010B;

const short WHAT_GET_SECRET_DES_MD5_REQ =(short)0x010C;

const short WHAT_SET_SERVER_IP_LISTS_REQ = (short)0x010D;

const short WHAT_GET_SERVER_IP_LISTS_REQ = (short)0x010E;

const short WHAT_GET_LOG_ZIP_REQ = (short)0x010F;

const short WHAT_SET_SSH_ENABLED_REQ = (short)0x0110;

const short WHAT_TEST = (short)0x0202;

const short WHAT_LOGIN =(short)0x0808;
const short WHAT_CHANGE_PASSWORD =(short)0x0809;

static int SUCCESS = 1;
static int UNKNOWN_HOST = -1;
static int IO_ERROR = -2;
static int SEND_FILE_NAME_NOT_RECV_ERROR = -3;
static int SEND_FILE_NAME_RECV_ERROR = -4;
static int SEND_FILE_BLOCK_NOT_RECV_ERROR = -5;
static int SEND_FILE_BLOCK_RECV_ERROR = -6;
static int SEND_FILE_BLOCK_NOT_RECV_END_ERROR = -7;
static int SEND_FILE_BLOCK_RECV_END_ERROR = -8;
static int PLAY_LIST_NOT_RECV_ERROR = -9;
static int PLAY_LIST_RECV_ERROR = -10;
static int SEND_NOT_RECV_ERROR = -11;
static int SEND_RECV_ERROR = -12;
static int FILE_NOT_EXIST_ERROR = -13;
static int NOVA_SOCKET_ERROR = -14;
static int PARAM_ERROR = -15;

NOVA_TRAFFIC_DLL void SDKLog(string msg, const char* buffer, char what, int length);
NOVA_TRAFFIC_DLL void SDKLog(string msg, char* buffer, char what, int length);
NOVA_TRAFFIC_DLL void SDKLog(string msg, unsigned char* buffer, char what, int length);
NOVA_TRAFFIC_DLL void intToBytes(char* buffer, int offset, int value);
NOVA_TRAFFIC_DLL short pubCalcCRC(char* bytes, int len);
NOVA_TRAFFIC_DLL short bytesToShort(char* buffer, int offset);
NOVA_TRAFFIC_DLL void shortToBytes(char* buffer, int offset, short value);
NOVA_TRAFFIC_DLL void normalIntToBytes(char* buffer, int offset, int value);
NOVA_TRAFFIC_DLL int ipToInt(const string& strIP);
NOVA_TRAFFIC_DLL int bytesToInt(char* buffer, int offset);
NOVA_TRAFFIC_DLL const std::vector<string> split(const string& s, const char& c);
NOVA_TRAFFIC_DLL tm getTm(int year, int  month, int  day, int  hour, int  minute, int  second);
NOVA_TRAFFIC_DLL char* makeSendPacketEx(char what, char* data, int dataSize, int encryptedType, char* key, int* outSize);
// std::string 转换为 UTF - 8 编码
NOVA_TRAFFIC_DLL std::string string_To_UTF8(const std::string& str);
//utf8转换为std::string
NOVA_TRAFFIC_DLL std::string UTF8_To_string(const std::string& str);
NOVA_TRAFFIC_DLL bool string_To_Bool(const string& str);
//通用的delete方法
NOVA_TRAFFIC_DLL void deleteObj(void** p);
NOVA_TRAFFIC_DLL void deleteArry(void** p);
