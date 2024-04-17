#pragma once
#include <fstream>
#include <sstream>
#include <list>
#include <ctime>

#include "protocalparser.h"
#include "fileutils.h"
#include "devicenowplayItem.h"
#include "devicetype.h"
#include "devicenowplayList.h"
#include "socketresult.h"
#include "novautils.h"

#ifdef WIN32
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

struct BrightnessItem {
	int environment;//环境亮度
	int screen;//屏体亮度

	BrightnessItem() {
		environment = 0;
		screen = 0;
	}

    BrightnessItem(int env, int scrn) {
        environment = env;
        screen = scrn;
    }
};

struct DeviceNetBasicParam {
	std::string deviceIp; //"192.168.0.220"
	int port;//500
	std::string mark;//255.255.255.0
	std::string gate;//192.168.0.1
	std::string serviceIp;//当卡为客户端时候，这个就是服务端对应IP；当卡为服务端时，无效。

	DeviceNetBasicParam() {
		deviceIp = "";
		port = 0;
		mark = "";
		gate = "";
		serviceIp = "";
	}
};

struct BackUpsInfo
{
    int code;//错误码
    std::string times;//备份时间
    BackUpsInfo() {
        code = 0;
        times = "";
    }
};

struct PreStoredScreenEnabled
{
    int code;//错误码
    int preStoredScreenEnabled;//备份时间
    PreStoredScreenEnabled() {
        code = 0;
        preStoredScreenEnabled = 0;
    }
};

struct DeviceSize {
	int width;
	int height;
	DeviceSize() {
		width = 0;
		height = 0;
	}
};

struct DeviceMemorySize {
	int totolSize;//单位MB
	int remainSize;//单位MB

	DeviceMemorySize() {
		totolSize = 0;
		remainSize = 0;
	}
	DeviceMemorySize(int totolSize, int remainSize) {
		this->totolSize = totolSize;
		this->remainSize = remainSize;
	}
};

struct TimeZoneParam {
	long long utcTime;//1565409600000
	std::string timeZoneId;//Asia/Shanghai
	std::string gmt;//GMT+08:00

	TimeZoneParam() {
		utcTime = 0;
		timeZoneId = "";
		gmt = "";
	}

	TimeZoneParam(long long utcTime, std::string timeZoneId, std::string gmt) {
		this->utcTime = utcTime;
		this->timeZoneId = timeZoneId;
		this->gmt = gmt;
	}
};

struct DeviceNTPParam {
	bool isOpen;
	std::string addr; //utf-8 ntp服务器地址

	DeviceNTPParam() {
		isOpen = false;
		addr = "";
	}
	DeviceNTPParam(bool isOpen, std::string addr) {
		this->isOpen = isOpen;
		this->addr = addr;
	}
};

struct DevicePowerByTimeParam {
	tm start;
	tm end;

	DevicePowerByTimeParam() {

	}
	DevicePowerByTimeParam(tm start, tm end) {
		this->start = start;
		this->end = end;
	}
};

struct PlayByTimeParam {
	tm start;
	tm end;
	int id;

	PlayByTimeParam() {
		id = 0;
	}
	PlayByTimeParam(tm start, tm end, int id) {
		this->start = start;
		this->end = end;
		this->id = id;
	}
};

struct BrightnessByTimeParam {
	tm start;
	tm end;
	int brightness;

	BrightnessByTimeParam() {
		brightness = 0;
	}
	BrightnessByTimeParam(tm start,
		tm end,
		int brightness) {
		this->start = start;
		this->end = end;
		this->brightness = brightness;
	}

};

struct DeviceSecretParam {
	std::string password; //viplex账号密码 默认123456
	int encryptType;
	bool isMD5Open;
	std::string newPassword; //des密码 必须是8位
	std::string oldPassword; //旧加密秘钥，如果没有加密过则为空

	DeviceSecretParam() {
		password = "";
		encryptType = 0;
		isMD5Open = false;
		newPassword = "";
		oldPassword = "";
	}
	DeviceSecretParam(std::string password, int encryptType, bool isMD5Open, std::string newPassword, std::string oldPassword) {
		this->password = password;
		this->encryptType = encryptType;
		this->isMD5Open = isMD5Open;
		this->newPassword = newPassword;
        this->oldPassword = oldPassword;
	}
};

//屏幕监控信息
struct ScreenMonitorData {
	struct DevMapping {
		int devIndex;             //该种设备串联上的第几个
		int devType;              //设备类型
	};
	//接收卡的监控信息
	struct ScannerMonitorInfo {
		float temprature;          //接收卡本板温度
        float voltage;             //接收卡本板电压
        std::list<DevMapping> devMappingList;   //设备搜索映射
		int devWorkStatus;         //工作状态 0正常，1错误，2未知

		ScannerMonitorInfo() {
			temprature = 0;
			voltage = 0;
			devWorkStatus = 0;
		}
	};
	struct PortOfSenderMonitorInfo {
        std::list<DevMapping> devMappingList;   //设备搜索映射
		int devWorkStatus;         //工作状态 0正常，1错误，2未知
	};
	//发送卡监控信息
	struct SenderMonitorInfo {
		bool isDVIConnected;         // DVI源是否连接
		int mDVIRate;                // DVI刷新率，仅在DVI连接时才有效
		int devWorkStatus;           //工作状态 0正常，1错误，2未知
        std::list<DevMapping> devMappingList;   //设备搜索映射
        std::list<PortOfSenderMonitorInfo> reduPortOfSenderList;   //进入冗余状态的网口的列表
		SenderMonitorInfo() {
			isDVIConnected = false;
			mDVIRate = 0;
			devWorkStatus = 0;
		}
	};
	
	//执行结果，0失败，1成功
	int code;       
	//接收卡状态
    std::list<ScreenMonitorData::ScannerMonitorInfo> scannerMonitorInfoList;
	//发送卡监控
    std::list<ScreenMonitorData::SenderMonitorInfo> senderMonitorInfoList;

	ScreenMonitorData() {
		code = 0;
	}
};

//视频源控制
struct VideoControlInfo {
	int code;		  //响应码 0失败、1成功
	int videoSource;      //视频源 0 内部、1 HDMI
	//boolean isScale;      //是否启用全屏缩放（仅HDMI源生效）
	//int offsetX;		  //输出位置x坐标
	//int offsetY;		  //输出位置y坐标
	VideoControlInfo() {
		code = 0;
		videoSource = 0;
	}
};

//分辨率信息
struct EdidInfo {
	int code;		 //响应码 0失败、1成功
	int width;           //宽
	int height;		     //高
	int fieldRate;		 //帧率

	EdidInfo() {
		code = 0;
		width = 0;
		height = 0;
		fieldRate = 0;
	}
};

//分辨率信息
struct CustomDisplay {
	int code;		 //响应码 0失败、1成功
	int width;           //宽
	int height;		     //高

	CustomDisplay() {
		code = 0;
		width = 0;
		height = 0;
	}
};

struct ResolutionInfo {
	int code;		 //响应码 0失败、1成功
	std::string resolution;

	ResolutionInfo() {
		code = 0;
		resolution = "";
	}
};


struct CpuAndMemoryInfo {
	int code;		 //响应码 0失败、1成功
	int cpu;          //cpu使用率
	int totalMemory;  //总内存
	int freeMemory;	  //剩余内存

	CpuAndMemoryInfo() {
		code = 0;
		cpu = 0;
		totalMemory = 0;
		freeMemory = 0;
	}
};

struct PointDetectInfo {
    int code;
    int badPointSize;

	PointDetectInfo() {
		code = 0;
		badPointSize = 0;
	}
};

struct Resolution {
    int displayMode;
    std::string resolution;
	Resolution() {
		displayMode = 0;
		resolution = "";
	}
};

struct CustomResolution {
    int displayMode;
    int width;
    int height;
	CustomResolution() {
		displayMode = 0;
		width = 0;
		height = 0;
	}
};

struct BrightnessInfo {
    char mode;
    char value;
};

struct TextAttr {
    int result;
	bool enableUpdateWithStream;
    bool aliasing;
    bool adaptation;
	bool updateWithStream;

	TextAttr() {
		result = 0;
		enableUpdateWithStream = false;
		aliasing = false;
		adaptation = false;
		updateWithStream = false;
	}
};

struct VirtualParam {
    bool isOpen; 
	int secondTime;
	int id;
	VirtualParam() {
		isOpen = false;
		secondTime = 0;
		id = 0;
	}
};

struct ResultParam {
	int code;
	std::string data;

	ResultParam() {
		code = 0;
		data = "";
	}
};

class NOVA_TRAFFIC_DLL NovaTraffic {
private:

	static const int SUCCESS = 1;
	static const int TEMP_SIZE = 100;
	static const int BIG_SIZE = 4096;
	static const int SCREENSHOT_SIZE = 65535;
	static const int BIG_STRING_SIZE = 2 * 4096;
	static const int MAX_FAIL_COUNT = 3;

private:
	const char* ip;
	u_short port;
	int mTimeout;
#ifdef WIN32
	SOCKET mSocket;
#else
	int mSocket;
#endif

	ProtocalParser* mProtocalParser;
	char* mDesStr;
    char* mAesStr;
    char* mSM4Str;

private:
#ifdef WIN32

	char* recv(SOCKET socket, short what, int* recvSize);

	char* recv(SOCKET sockClient, short what, int* recvSize, int temp_size);

	int sendPlayFileName(SOCKET socket, int id);

	int sendFileName(SOCKET socket, const char* name, int len);

	int sendContent(SOCKET socket, const char* content, int len, int id);

	int recvFileOverRsp(SOCKET socket);

	int sendPlayId(SOCKET socket, int id);

	int normalSet(char sendWhat, char recvWhat, char* bytes, int dataSize);

	SOCKET getSocket();

	void closeSocket(SOCKET novaSocket);

	int sendFileContent(SOCKET socket, const char* fileName, const int outSize, int* lastZize, int* itemNum);

	SocketResult* getData(short req, short rsp, char* buf, int dataSize);

	SocketResult* getData(SOCKET socket, short req, short rsp, char* buf, int dataSize);

	SocketResult* getBigData(short req, short rsp, char* buf, int dataSize);

	SocketResult* getData(short req, short rsp, char* buf, int dataSize, int bufferSize);

	SocketResult* getData(SOCKET socket, short req, short rsp, char* buf, int dataSize, int bufferSize);

	//获取点检数据
	SocketResult* getDetectErrorPointData(SOCKET socket, char req, char rsp, char rsp1, char* buf, int dataSize);

	SocketResult* getDetectErrorPointData(SOCKET socket, char req, char rsp, char rsp1, char* buf, int dataSize, int bufferSize);
#else

	char* recv(int socket, short what, int* recvSize);

	char* recv(int sockClient, short what, int* recvSize, int temp_size);

	int sendPlayFileName(int socket, int id);

	int sendFileName(int socket, const char* name, int len);

	int sendContent(int socket, const char* content, int len, int id);

	int recvFileOverRsp(int socket);

	int sendPlayId(int socket, int id);

	int normalSet(char sendWhat, char recvWhat, char* bytes, int dataSize);

	int getSocket(int timeout = 20 * 1000);

	void closeSocket(int novaSocket);

	int sendFileContent(int socket, const char* fileName, const int outSize, int* lastZize, int* itemNum);

	SocketResult* getData(short req, short rsp, char* buf, int dataSize);

	SocketResult* getData(int socket, short req, short rsp, char* buf, int dataSize);

	SocketResult* getBigData(short req, short rsp, char* buf, int dataSize);

	SocketResult* getData(short req, short rsp, char* buf, int dataSize, int bufferSize);

	SocketResult* getData(int socket, short req, short rsp, char* buf, int dataSize, int bufferSize);

	//获取点检数据
	SocketResult* getDetectErrorPointData(int socket, char req, char rsp, char rsp1, char* buf, int dataSize);

	SocketResult* getDetectErrorPointData(int socket, char req, char rsp, char rsp1, char* buf, int dataSize, int bufferSize);
#endif

	char* makeSendPacket(short what, char* data, int dataSize, int* outSize);

protected:
#ifdef WIN32

	void setSocket(SOCKET socket);
#else

	void setSocket(int socket);
#endif

public:
#ifdef WIN32

	NovaTraffic(SOCKET socket);
#else

	NovaTraffic(int socket);
#endif

	NovaTraffic(const char* ip, const u_short port);

	~NovaTraffic();

	void setTimeout(int timeout);

	/// <summary>
	/// 设置Des加密秘钥
	/// </summary>
	/// <param name="password"></param>
	void setDesKey(char* password);

	/// <summary>
	/// 设置Aes加密秘钥
	/// </summary>
	/// <param name="password"></param>
	void setAesKey(char* password);

    /// <summary>
    /// 设置SM4加密秘钥
    /// </summary>
    /// <param name="password"></param>
    void setSM4Key(char* password);

	/// <summary>
	/// 清除加密秘钥
	/// </summary>
	void clearPassword();

	/// <summary>
	/// 设置软sim key
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool setDeviceKey(const std::string& str);

	/// <summary>
	/// 设置设备名称
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	int setDeviceName(const char* name);

	/// <summary>
	/// 获取设备名称
	/// </summary>
	/// <returns></returns>
	char* getDeviceName();

	/// <summary>
	/// 发送播放列表
	/// </summary>
	/// <param name="id"></param>
	/// <param name="content"></param>
	/// <returns></returns>
	int sendPlayList(int id, char* content);

	/// <summary>
	/// 发送播放列表
	/// </summary>
	/// <param name="id"></param>
	/// <param name="content"></param>
	/// <returns></returns>
	int sendPlayList(int id, const char* content);

	/// <summary>
	/// 指定播放列表ID
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	int playListById(int id);

	/// <summary>
	/// 获取所有播放列表ID
	/// </summary>
	/// <returns></returns>
    std::list<int> getAllPlaylistId();

	/// <summary>
	/// 指定ID删除播放列表
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool deletePlaylistById(int id);

	/// <summary>
	/// 发送局部更新
	/// </summary>
	/// <param name="id"></param>
	/// <param name="content"></param>
	/// <returns></returns>
	int sendLocalUpdate(int id, char* content);

	/// <summary>
	/// 发送局部更新
	/// </summary>
	/// <param name="id"></param>
	/// <param name="content"></param>
	/// <returns></returns>
	int sendLocalUpdate(int id, const char* content);

	/// <summary>
	/// 移除局部更新
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	int removeLocalUpdate(int id);

	/// <summary>
	/// 发送文件
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="saveFileName"></param>
	/// <returns></returns>
	int sendFile(const char* fileName, const char* saveFileName);

	/// <summary>
	/// 设置设备亮度
	/// </summary>
	/// <param name="brightness"></param>
	/// <returns></returns>
	int setBrightness(int brightness);

	/// <summary>
	/// 清理文件
	/// </summary>
	/// <param name="type">清理文件类型</param>
	/// <returns></returns>
	int cleanUpFiles(int type);

	/// <summary>
	/// 设置设备开关屏
	/// </summary>
	/// <param name="power"></param>
	/// <returns></returns>
	int setPower(bool power);

	/// <summary>
	/// 获取当前播放列表内容
	/// </summary>
	/// <returns></returns>
	DeviceNowPlayItem* getNowPlayContent();

	/// <summary>
	/// 获取全部播放列表内容
	/// </summary>
	/// <returns></returns>
	DeviceNowPlayList* getNowPlayAllContent();

	/// <summary>
	/// 获取设备状态
	/// </summary>
	/// <returns></returns>
	DeviceType* getDeviceType();

	/// <summary>
	/// 设置设备时间
	/// </summary>
	/// <param name="date"></param>
	/// <returns></returns>
	int setTime(tm* date);

	/// <summary>
	/// 重启设备
	/// </summary>
	/// <returns></returns>
	int restartDevice();

	/// <summary>
	/// 设置自动亮度调节
	/// </summary>
	/// <param name="brightnessItem">必须是8组</param>
	/// <returns></returns>
	int setBrightnessAuto(BrightnessItem* brightnessItem);

	/// <summary>
	/// 获取自动亮度调节
	/// </summary>
	/// <returns></returns>
	BrightnessItem* getBrightnessAuto();

	/// <summary>
	/// 控制本板电源和屏幕黑屏
	/// </summary>
	/// <param name="power"></param>
	/// <returns></returns>
	int setBoardPowerAndScreenPower(bool power);

	/// <summary>
	/// 控制本板电源
	/// </summary>
	/// <param name="power"></param>
	/// <returns></returns>
	int setBoardPower(bool power);

	/// <summary>
	/// 控制多功能卡电源
	/// </summary>
	/// <param name="cardIndex">0-255 多功能卡位置索引</param>
	/// <param name="roadIndex">0-7 电源路数索引</param>
	/// <param name="power"></param>
	/// <returns></returns>
	int setMultiFunctionCardPower(int cardIndex, int roadIndex, bool power);

	/// <summary>
	/// 恢复设备出厂IP到192.168.0.220
	/// </summary>
	void resetDeviceIpToDefault();

	/// <summary>
	/// 获取控制卡App版本信息
	/// </summary>
	/// <returns></returns>
	char* getDeviceVersion();

	/// <summary>
	/// 设置屏体基本参数
	/// </summary>
	/// <param name="mDeviceBasicParam"></param>
	void setDeviceNetBasicParam(DeviceNetBasicParam mDeviceBasicParam);

	/// <summary>
	/// 获取屏幕尺寸（宽高）
	/// </summary>
	/// <returns></returns>
	DeviceSize* getDeviceSize();

	/// <summary>
	/// 设置设备音量0 - 100
	/// </summary>
	/// <param name="volume"></param>
	/// <returns></returns>
	int setDeviceVolume(int volume);

	/// <summary>
	/// 获取设备音量
	/// </summary>
	/// <returns></returns>
	char getVolumeValue();

	/// <summary>
	/// 获取设备硬盘空间
	/// </summary>
	/// <returns></returns>
	DeviceMemorySize* getDeviceMemorySize();

	/// <summary>
	/// 设置设备时区时间
	/// </summary>
	/// <param name="timeZoneParam"></param>
	/// <returns></returns>
	int setTimeAndTimeZone(TimeZoneParam timeZoneParam);

	/// <summary>
	/// 获取设备时区时间
	/// </summary>
	/// <returns></returns>
	TimeZoneParam* getTimeAndTimeZone();

	/// <summary>
	/// 设置设备NTP对时参数
	/// </summary>
	/// <param name="deviceNTPParam"></param>
	/// <returns></returns>
	int setDeviceNTPParam(DeviceNTPParam deviceNTPParam);

	/// <summary>
	/// 获取设备NTP对时参数
	/// </summary>
	/// <returns></returns>
	DeviceNTPParam* getDeviceNTPParam();

	/// <summary>
	/// 设置屏体关闭温度，高于此温度关屏
	/// </summary>
	/// <param name="value">系统最小值为 70  0 表示不进行关屏处理</param>
	/// <returns></returns>
	int setCloseScreenTemperature(int value);

	/// <summary>
	/// 获取屏体关闭温度
	/// </summary>
	/// <returns></returns>
	int getCloseScreenTemperature();

	/// <summary>
	/// 设置虚连接播放文件
	/// </summary>
	/// <param name="isOpen">false-不开启  true-开启虚连接检测</param>
	/// <param name="secondTime">单位秒，多长时间无数据交互开启虚连接</param>
	/// <param name="id">播放文件id 1 代表play001.lst文件</param>
	/// <returns></returns>
	int setVirtualConnection(bool isOpen, int secondTime, int id);

	/// <summary>
	/// 发送FTP文件下载地址
	/// </summary>
	/// <param name="url"></param>
	/// <returns></returns>
	int sendFtpDownloadaddress(std::string url);

	/// <summary>
	/// 升级
	/// </summary>
	/// <param name="path">升级包名称</param>
	/// <returns></returns>
	int updateFileName(std::string path);

	/// <summary>
	/// 移除APP
	/// </summary>
	/// <param name="pageName"></param>
	/// <returns></returns>
	int removeAppPageName(std::string pageName);

	int setRelayPowers(bool power1, bool power2, bool power3, bool power4);

	/// <summary>
	/// 获取当前播放截图
	/// </summary>
	/// <param name="filePath">截图文件名</param>
	/// <returns></returns>
	int getDeviceScreenshot(const std::string filePath);

	/// <summary>
	/// 设置定时开屏列表
	/// </summary>
	/// <param name="li">为null 或 0 清除操作 ；新表覆盖旧表</param>
	/// <returns></returns>
	int setDevicePowerByTimeList(std::list<DevicePowerByTimeParam> li);

	/// <summary>
	/// 获取定时开屏列表
	/// </summary>
	/// <returns></returns>
    std::list<DevicePowerByTimeParam>* getDevicePowerByTimeList();

	/// <summary>
	/// 设置定时播放列表
	/// </summary>
	/// <param name="list"></param>
	/// <returns></returns>
    int setPlayByTimeList(std::list<PlayByTimeParam> list);

	/// <summary>
	/// 获取定时播放列表
	/// </summary>
	/// <returns></returns>
    std::list<PlayByTimeParam>* getPlayByTimeList();

	/// <summary>
	/// 设置定时亮度列表
	/// </summary>
	/// <param name="li"></param>
	/// <returns></returns>
    int setBrightnessByTimeList(std::list<BrightnessByTimeParam> li);

	/// <summary>
	/// 获取定时亮度列表
	/// </summary>
	/// <returns></returns>
    std::list<BrightnessByTimeParam>* getBrightnessByTimeList();

	/// <summary>
	/// 设置设备加密信息
	/// </summary>
	/// <param name="mDeviceSecretParam"></param>
	/// <param name="isUseNewProtocol"></param>
	/// <returns></returns>
	int setDeviceSecretParam(DeviceSecretParam mDeviceSecretParam, bool isUseNewProtocol = false);

	/// <summary>
	/// 获取监控卡信息
	/// </summary>
	/// <returns></returns>
	std::string getSurveil();

	/// <summary>
	/// 获取视频控制模式
	/// </summary>
	/// <returns></returns>
	VideoControlInfo getVideoControlInfo();

	/// <summary>
	/// 设置视频控制模式
	/// </summary>
	/// <param name="videoSource"></param>
	/// <returns></returns>
	int setVideoControlInfo(int videoSource);

	/// <summary>
	/// 获取HDMI视频源分辨率
	/// </summary>
	/// <returns></returns>
	EdidInfo getEdidInfo();

	/// <summary>
	/// 设置HDMI视频源分辨率
	/// </summary>
	/// <param name="info"></param>
	/// <returns></returns>
	int setEdidInfo(EdidInfo info);

	/// <summary>
	/// 设置内源分辨率
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="refreshRate"></param>
	/// <returns></returns>
	int setCustomResolution(int width, int height, int refreshRate);

	/// <summary>
	/// 获取内源分辨率
	/// </summary>
	/// <returns></returns>
	ResolutionInfo getCurrentResolution();

	/// <summary>
	/// 设置自动亮度参数
	/// </summary>
	/// <param name="time"></param>
	/// <param name="count"></param>
	/// <returns></returns>
	bool setAutoBrightnessTimeParam(char time, char count);

	/// <summary>
	/// 获取CPU和内存使用情况
	/// </summary>
	/// <returns></returns>
	CpuAndMemoryInfo getCpuAndMemoryInfo();

	/// <summary>
	/// 发送心跳信息
	/// </summary>
	std::string sendHeartBeat(int timeOutSec);

	/// <summary>
	/// 获取所有媒体文件名称
	/// </summary>
	/// <returns></returns>
    std::list<std::string> getAllMediaFileName();

	/// <summary>
	/// 获取文件列表
	/// </summary>
	/// <param name="type">
	/// 文件类型 
	/// 0播放列表 
	/// 1局部更新 
	/// 2媒体文件 
	/// 3运行日志</param>
	/// <returns></returns>
    std::list<std::string> getFileListName(char type);

	/// <summary>
	/// 获取系统信息
	/// </summary>
	/// <returns></returns>
	std::string getSystemInfo();

	/// <summary>
	/// 获取屏幕亮度控制模式和亮度值
	/// </summary>
	/// <returns></returns>
	BrightnessInfo getBrightnessInfo();

	/// <summary>
	/// 获取wifi ap状态
	/// </summary>
	/// <returns>-1 失败，0未打开，1打开</returns>
	int getWifiApState();

	/// <summary>
	/// 设置wifi ap状态
	/// </summary>
	/// <param name="b"></param>
	/// <returns></returns>
	int setWifiApState(bool b);

	/// <summary>
	/// 获取wifi ap配置信息
	/// </summary>
	/// <returns></returns>
	std::string getWifiApConfig();

	/// <summary>
	/// 设置wifi ap配置信息
	/// </summary>
	/// <param name="config"></param>
	/// <returns></returns>
	int setWifiApConfig(std::string config);

	/// <summary>
	/// 获取移动网络信息
	/// </summary>
	/// <returns></returns>
	std::string getMobileInfo();

	/// <summary>
	/// 获取wifi状态
	/// </summary>
	/// <returns></returns>
	int getWifiState();

	/// <summary>
	/// 设置wifi状态
	/// </summary>
	/// <param name="open"></param>
	/// <returns></returns>
	bool setWifiState(bool open);

	/// <summary>
	/// 获取wifi列表
	/// </summary>
	/// <returns></returns>
	std::string getWifiList();

	/// <summary>
	/// 连接wifi
	/// </summary>
	/// <param name="data"></param>
	/// <returns></returns>
	bool connectWifi(std::string data);

	/// <summary>
	/// 设置字体
	/// </summary>
	/// <param name="data"></param>
	/// <returns></returns>
	bool setFont(std::string data);

	/// <summary>
	/// 获取字体列表
	/// </summary>
	/// <returns></returns>
	std::string getFontList();

	/// <summary>
	/// 获取传感器信息
	/// </summary>
	/// <returns></returns>
	std::string getSensorInfo();

	/// <summary>
	/// 设置传感器信息
	/// </summary>
	/// <param name="info"></param>
	/// <returns></returns>
	bool setSensorInfo(std::string info);

	/// <summary>
	/// 获取虚链接信息
	/// </summary>
	/// <returns></returns>
	VirtualParam* getVirtualConnection();

	/// <summary>
	/// 获取lora信息
	/// </summary>
	/// <returns></returns>
	ResultParam getLoraInfo();

	/// <summary>
	/// 设置lora信息
	/// </summary>
	/// <param name="info"></param>
	/// <returns></returns>
	bool setLoraInfo(std::string info);

	/// <summary>
	/// 获取屏幕开关状态
	/// </summary>
	/// <returns></returns>
	int getScreenStatus();

	/// <summary>
	/// 设置同步播放状态
	/// </summary>
	/// <param name="enable"></param>
	/// <returns></returns>
	bool setSyncPlay(bool enable);

	/// <summary>
	/// 获取同步播放状态
	/// </summary>
	/// <returns></returns>
	bool getSyncPlay();

	/// <summary>
	/// 获取设备sn号
	/// </summary>
	/// <returns></returns>
	std::string getSn();

	/// <summary>
	/// 获取设备Mac地址
	/// </summary>
	/// <returns></returns>
	std::string getMac();

	/// <summary>
	/// 开始点检
	/// </summary>
	/// <returns></returns>
	PointDetectInfo startScreenPointDetect();

	/// <summary>
	/// 获取点检数据
	/// </summary>
	/// <returns></returns>
	std::string getDetectErrorPointInfo();

	/// <summary>
	/// 获取设备抗锯齿信息
	/// </summary>
	/// <returns></returns>
	TextAttr getAntiAliasing();

	/// <summary>
	/// 设置设备抗锯齿信息
	/// </summary>
	/// <param name="openAliasing"></param>
	/// <param name="openAdaptation"></param>
	/// <param name="openUpdateWithStram"></param>
	void setAntiAliasing(bool openAliasing, bool openAdaptation, bool openUpdateWithStram);

	/// <summary>
	/// 获取分辨率支持列表
	/// </summary>
	/// <returns></returns>
	std::string getSupportVideoSource();

	/// <summary>
	/// 设置自定义ping地址
	/// </summary>
	/// <param name="ip"></param>
	/// <returns></returns>
	bool setPingIp(std::string ip);

	/// <summary>
	/// 获取自定义ping地址
	/// </summary>
	/// <returns></returns>
	std::string getPingIp();

	/// <summary>
	/// 下载文件
	/// </summary>
	/// <param name="filesList"></param>
	/// <returns></returns>
	bool downloadFiles(std::string filesList);

	/// <summary>
	/// 设置高级定时任务
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool setAdvanceTiming(const std::string& str);

	/// <summary>
	/// 获取高级定时任务
	/// </summary>
	/// <returns></returns>
	std::string getAdvanceTiming();

	/// <summary>
	/// 移除高级定时任务
	/// </summary>
	/// <returns></returns>
	bool removeAdvanceTiming();

	/// <summary>
	/// 获取sim卡信息
	/// </summary>
	/// <returns></returns>
	std::string getSimInfo();

	/// <summary>
	/// 切换为软sim
	/// </summary>
	/// <returns></returns>
	bool switchToSoftsim();

	/// <summary>
	/// 获取IP白名单
	/// </summary>
	/// <returns></returns>
	std::string getWhitelist();

	/// <summary>
	/// 移除IP白名单
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool removeWhitelist(const std::string& str);

	/// <summary>
	/// 添加IP白名单
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool addWhitelist(const std::string& str);

	/// <summary>
	/// 设置服务器IP列表
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	bool setServerIPLists(const std::string& str);

	/// <summary>
	/// 获取服务器IP列表
	/// </summary>
	/// <returns></returns>
	std::string getServerIPLists();

	/// <summary>
	/// 获取设备日志压缩地址
	/// </summary>
	/// <returns></returns>
	std::string getLogZipPath();

	/// <summary>
	/// 获取SSH状态
	/// </summary>
	/// <returns></returns>
	int getSSHEnabled();

	/// <summary>
	/// 设置SSH状态
	/// </summary>
	/// <param name="sshEnabled"></param>
	/// <returns></returns>
	int setSSHEnabled(bool sshEnabled);

    /// <summary>
    /// 获取预存画面开关
    /// </summary>
    /// <returns></returns>
    PreStoredScreenEnabled getPreStoredScreenEnabled();

    /// <summary>
    /// 设置预存画面开关
    /// </summary>
    /// <param name="sshEnabled"></param>
    /// <returns></returns>
    int setPreStoredScreenEnabled(bool sshEnabled);

    /// <summary>
    /// 获取备份时间
    /// </summary>
    /// <returns></returns>
    BackUpsInfo* getBackupsTimes();

    /// <summary>
    /// 备份
    /// </summary>
    /// <param name="sshEnabled"></param>
    /// <returns></returns>
    int backups();

    /// <summary>
    /// 测试画面 节目id默认FF
    /// </summary>
    /// <param name="operationType">2:播放 3:保存</param>
    /// <param name="content"></param>
    /// <returns></returns>
    int sendTestLocalUpdate(int operationType, char* content);

    /// <summary>
    /// 测试画面 节目id默认FF
    /// </summary>
    /// <param name="operationType">2:播放 3:保存</param>
    /// <param name="content"></param>
    /// <returns></returns>
    int sendTestLocalUpdate(int operationType, const char* content);

	/// <summary>
	/// 获取是否加密
	/// </summary>
	/// <returns></returns>
	int getIsSecret();

	/// <summary>
	/// 获取路网图
	/// </summary>
	std::string getRDSRoad();

	/// <summary>
	/// 设置路网图
	/// </summary>
	/// <param name="colors"></param>
	int setRDSRoad(std::string colors);

    /// <summary>
    /// 接收卡固化
    /// </summary>
    /// <param name="type">类型</param>
    /// 0：接收卡所有地址数据固化
    /// 1：接收卡参数
    /// 2：Flash拓扑
    /// 3：Gamma表
    /// 4：数据组交换RAM和ICN2055功能RAM
    /// 5：精细灰度教正数据
    /// 6：大表
    /// 7：颜色管理数据
    /// 8：除大表外其他数据
    /// 9：灯板走线表
    /// <returns></returns>
    int solidify(byte type);

    /// <summary>
	/// 获取电源状态
	/// </summary>
	/// <returns></returns>
	int getPowerState();

	/// <summary>
    /// 清理缓存
    /// </summary>
    /// <returns></returns>
    int clearCache();

    /// <summary>
    /// 尝试连接
    /// </summary>
    /// <param name="colors"></param>
    bool attemptConnect(int timeout);

};




