#ifndef WORKGROUP_H
#define WORKGROUP_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class HCColumnSpeaker;
class HCBallheadCamera;
class NovaController;

class WorkGroup : public QObject
{
    Q_OBJECT
public:
    explicit WorkGroup(int groupId, QObject *parent = nullptr);

    void init();
    void test();

private:
    void initCfg();
    void initCamera();
    void initSpeaker();
    void initNova();
    void initTimer();

public:
    void clsOverTimeIllegalPics();  //  删除超过保存时间的违法图片

signals:
    void showMsg(QString msg);
    void signalIllegalAct();
    void signalGetImage(QByteArray imagedata, QString ip);
    void signalUpdateIps(QString ip0, QString ip1, QString ip2, QString ip3);

signals:
    void signalUpDateBackUpPathDir();

private slots:
    void slotIllegalAct();          // 发生违法行为

private:
    int m_groupId;
    QString m_HCCameraIp1;          // 相机1 IP
    int m_HCCameraIndex1;           // 相机1回调编号
    QString m_HCCameraIp2;          // 相机2 IP
    int m_HCCameraIndex2;           // 相机2回调编号
    QString m_HCSpeakerIp;          // 音柱 IP
    int m_HCSpeakerTimes;           // 音柱播放次数
    QString m_HCSpeakerContent;     // 音柱播放内容
    int m_HCSpeakerVolume;          // 音柱音量
    QString m_HCSpeakerPlayMode;    // 音柱播放模式（MP3/TTS）不区分大小写
    QString m_NovaControllerIp;     // 屏幕 IP
    int m_Back2DefaultProgram;      // 恢复默认节目需要的时间 单位s
    int m_SaveDays;                 // 违法图片保存天数
    int m_DHSpeakerId;              // 大华音柱id
    int m_SpeakerType;              // 音柱类型 0HC 1DH

    HCBallheadCamera* m_HCBallheadCamera1 = nullptr;    // 相机1
    HCBallheadCamera* m_HCBallheadCamera2 = nullptr;    // 相机2
    HCColumnSpeaker* m_HCColumnSpeaker = nullptr;       // 音柱
    NovaController* m_NovaController = nullptr;         // 屏幕

    QTimer* m_timer;                // 违法信号间隔定时器
    //int m_sendIllegalSignalFlag;  // 发送违法信号间隔
    //QTimer* m_timerSec;           // 秒级定时器
    bool m_sendIllegalSignal;       // 发送违法信号
};

#endif // WORKGROUP_H
