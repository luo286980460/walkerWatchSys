#ifndef HCBALLHEADCAMERAWORKER_H
#define HCBALLHEADCAMERAWORKER_H

#include <QObject>
#include "include/HCCamera/HCNetSDK.h"

typedef struct {
    int workGroup;        // 相机工作组
    LONG lUserID;         // 注册设备 id
    LONG lHandle;         // 布防句柄
    QString ip;           // 相机ip
    int port;             // 相机端口
    QString user;         // 相机用户
    QString pwd;          // 相机密码
    QString backPath;     // 照片保存路径
}cameraInfo_s;

class HCBallheadCameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit HCBallheadCameraWorker(QObject *parent = nullptr);
    ~HCBallheadCameraWorker();

private:
    bool initSdk();

signals:
    void showMsg(QString msg);
    void signalIllegalAct(int GroupId);         // 违法组
    void signalGetImage(QByteArray imagedata, QString ip);

public slots:
    void slotInit();
    void slotAddCamera(int workGroup, QString ip, int port, QString user, QString pwd);
    void slotReflushBackupPath();        // 刷新违法图片保存文件夹

private:
    static void CALLBACK MessageCallback(LONG lCommand,             // 海康回调函数
                                         NET_DVR_ALARMER *pAlarmer,
                                         char *pAlarmInfo,
                                         DWORD dwBufLen,
                                         void* pUser);

    QList<cameraInfo_s*> m_cameraList; // 相机列表
    bool logInCamera(cameraInfo_s* camera);
    bool startAlarm(cameraInfo_s* camera);
    void initBackupPath(cameraInfo_s* camera);
    void saveImage(cameraInfo_s* camera, QByteArray imageData);

    static bool m_haveBack; // 是否已经注册回调

};

#endif // HCBALLHEADCAMERAWORKER_H
