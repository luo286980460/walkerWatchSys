#ifndef HCBALLHEADCAMERAWORKER_H
#define HCBALLHEADCAMERAWORKER_H

#include <QObject>
#include "include/HCCamera/HCNetSDK.h"

class HCBallheadCameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit HCBallheadCameraWorker(QString ip, int HCCameraIndex, QObject *parent = nullptr);
    ~HCBallheadCameraWorker();

    QString getIp();

private:
    bool initSdk();
    bool logInCamera(QString ip, int port, QString user, QString pwd);
    bool startAlarm();
    void initBackupPath();
    void saveImage(QByteArray imageData);

signals:
    void showMsg(QString msg);
    void signalIllegalAct();
    void signalGetImage(QByteArray imagedata, QString ip);

public slots:
    void slotInit();
    void slotUpDateBackUpPathDir();

private:
    static void CALLBACK MessageCallback0(LONG lCommand,             // 海康回调函数
                                         NET_DVR_ALARMER *pAlarmer,
                                         char *pAlarmInfo,
                                         DWORD dwBufLen,
                                         void* pUser);
    static void CALLBACK MessageCallback1(LONG lCommand,             // 海康回调函数
                                         NET_DVR_ALARMER *pAlarmer,
                                         char *pAlarmInfo,
                                         DWORD dwBufLen,
                                         void* pUser);
    static void CALLBACK MessageCallback2(LONG lCommand,             // 海康回调函数
                                         NET_DVR_ALARMER *pAlarmer,
                                         char *pAlarmInfo,
                                         DWORD dwBufLen,
                                         void* pUser);
    static void CALLBACK MessageCallback3(LONG lCommand,             // 海康回调函数
                                         NET_DVR_ALARMER *pAlarmer,
                                         char *pAlarmInfo,
                                         DWORD dwBufLen,
                                         void* pUser);
    int m_iIndex = 0;
    void* ls_1 = nullptr;
    LONG m_lUserID;         // 注册设备 id
    LONG m_lHandle;         // 布防句柄
    QString m_ip;           // 相机ip
    int m_port;             // 相机端口
    QString m_user;         // 相机用户
    QString m_pwd;          // 相机密码
    int m_HCCameraIndex;    // 相机回调编号
    QString m_backUpPathDir;// 相机备份路径

};

#endif // HCBALLHEADCAMERAWORKER_H
