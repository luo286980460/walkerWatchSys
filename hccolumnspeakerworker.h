#ifndef HCCOLUMNSPEAKERWORKER_H
#define HCCOLUMNSPEAKERWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>

typedef struct {
    int SpakerType;         // 音柱类型（0海康1大华）
    QString ip;             // 音柱ip
    int id;                 // 大华音柱id（音柱为海康时，不需要此值）
    QString SpeakerContent; // 音柱播放内容（TTS）或者文件名（MP3）
    int SpeakerVolume;      // 音柱播放音量（1-10）
    int SpeakerPlayMode;    // 音柱播放类型（TTS/MP3）
    int HCSpeakerTimes;     // 音柱播放时间（单位秒）或者次数
}speaker_s;

class HCColumnSpeakerWorker : public QObject
{
    Q_OBJECT
public:
    explicit HCColumnSpeakerWorker(QString ip, QString HCSpeakerContent, int HCSpeakerTimes,
                                   int HCSpeakerVolume, QString HCSpeakerPlayMode,
                                   int DHSpeakerId, int SpeakerType, QObject *parent = nullptr);

signals:
    void showMsg(QString msg);

private:
    void init();
    void connectColumnSpeaker();
    void initTime();
    void setSpeakerVolume(int volume);

public slots:
    void slotInit();
    void slotReadyRead();                       // 读取音柱返回信息
    void slotCmdPlayMp3NameType(QString name,   // 播放MP3 （文件名， 播放类型）
                                int Type,
                                int times);
    void slotCmdPlayTTS(QString name, int Type, // TTS 播放
                        int times);
    void slotCmdStopPlay();                     // 停止播放
    void slotCmdSetVolume(int volume);          // 调节音量
    QString QString2Hex(QString str);           // 文字转16进制
    void slotIllegalAct();                      // 音柱违法行为
    void slotIllegalActOver();                  // 音柱违法行为结束
    void stopPlay();


private:
    QTcpSocket* m_HCColumnSpeaker = nullptr;
    QString m_ip;                   // ip
    int m_port;                     // 端口
    QString m_pwd;                  // 密码
    int m_connectTimes = 0;         // 重连次数
    bool m_canCmd = true;           // 当前是否接收命令
    QTimer* m_timer;                // s 级定时器
    int m_timeFlag = 0;             //
    QString m_HCSpeakerContent;     // 音柱播放内容
    int m_HCSpeakerTimes;           // 音柱播放次数
    int m_HCSpeakerVolume;          // 音柱音量
    QString m_HCSpeakerPlayMode;    // 音柱播放模式（MP3/TTS）不区分大小写
    int m_DHSpeakerId;              // 大华音柱ID
    int m_SpeakerType;              // 音柱类型 0HC 1DH

    // 大华
    void DaHuaTTS();
    void DaHuaMP3();
    void DaHuaVolume(int id, int volume);
    QNetworkReply* m_reply = nullptr;
    QNetworkRequest m_request;
    QUrlQuery m_postData;;
    QNetworkAccessManager m_manager;


};

#endif // HCCOLUMNSPEAKERWORKER_H
