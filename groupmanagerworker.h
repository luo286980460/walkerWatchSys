#ifndef GROUPMANAGERWORKER_H
#define GROUPMANAGERWORKER_H

#include <QObject>
#include <QTimer>
#include <QTime>

class HCBallheadCamera;
class E_StakeVer0;
class HCColumnSpeaker;
class NovaController;
class YuanHongBox;

typedef struct{
    int CameraCount;                // 相机数量
    QStringList CameraIpList;       // 相机ip
    int SpakerType;                 // 音柱类型（0海康1大华）
    QString SpakerIp;               // 音柱ip
    int SpeakerTimes;               // 音柱播放时间（单位秒）或者次数
    QString SpeakerContent;         // 音柱播放内容（TTS）或者文件名（MP3）
    int SpeakerVolume;              // 音柱播放音量（大华1-15，海康2-7）
    int SpeakerNightVolume;         // 音柱晚上播放音量（大华1-15，海康2-7）
    QString SpeakerPlayMode;        // 音柱播放类型（TTS/MP3）
    QString NovaControllerIp;       // 诺瓦控制卡ip
    int Back2DefaultProgram;        // 恢复默认状态的时间（单位秒）
    int SpeakerId;                  // 大华音柱id（音柱为海康时，不需要此值）
    int EstakeUpLightId;            // 安全桩上层灯拨码
    int EstakeDownLightId;          // 安全桩下层灯拨码
    int AlarmDuration;              // 告警时长
    int CurrentDuration;            // 告警已持续时长
    HCColumnSpeaker* speaker;       // 音柱
    NovaController* nova;           // 诺瓦屏
    int YuanHongBoxCount;           // 分析盒数量
    QStringList YuanHongBoxIpList;  // 分析盒ip
}groupInfo_s;

class groupManagerWorker : public QObject
{
    Q_OBJECT
public:
    explicit groupManagerWorker(QObject *parent = nullptr);

private:
    bool getDayAndNightTime(QString DayTime);

signals:
    void showMsg(QString msg);
    void signalReflushBackupPath();     // 刷新违法图片保存文件夹
    void signalUpdateDayOrNight(bool isDaytime);    // 更新白天黑夜状态
    void signalUpdateDayNightTime(QString dayTimeStart, QString dayTimeSopt);

public slots:
    void slotInit();
    void slotIllegalAct(int GroupId);
    void slotIllegalTest();

private:
    void initGroup();                       // 初始化组配置
    void initCamera();                      // 初始化相机
    void initYuanHongBox();                 // 初始化分析盒
    void initE_StakeVer0();                 // 初始化安全桩

    void initNova();                        // 初始化诺瓦

    /* 相机 */
    int m_groupCount = -1;                  // 设备组数量
    QList<groupInfo_s*> m_groupList;        // 组列表
    HCBallheadCamera* m_camera;             // 相机
    int m_SaveDays;                         // 违法图片保存天数

    /* 分析盒 */
    YuanHongBox* m_yuanHongBox;             // 分析盒

    /* 安全桩 */
    E_StakeVer0* m_eStake;                  // 安全桩
    int m_EstakeVersion;                    // 安全桩版本（0默认）
    int m_EstakeControlType;                // 安全桩控制类型（0串口1网口）
    QString m_portOrIp;                     // 安全桩控制器 端口号 或者 ip
    QTimer* m_timerSec;                     // sec定时器
    void initTimerSec();

    /* 音柱 */
    QTime m_dayTimeStart;                   // 白天开始
    QTime m_dayTimeStop;                    // 白天结束

};

#endif // GROUPMANAGERWORKER_H
