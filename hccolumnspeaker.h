#ifndef HCCOLUMNSPEAKER_H
#define HCCOLUMNSPEAKER_H

#include <QObject>
#include <QThread>

class HCColumnSpeakerWorker;

class HCColumnSpeaker : public QObject
{
    Q_OBJECT
public:
    explicit HCColumnSpeaker(QString ip, QString HCSpeakerContent, int HCSpeakerTimes,
                             int HCSpeakerVolume, QString HCSpeakerPlayMode,
                             int DHSpeakerId, int SpeakerType, QObject *parent = nullptr);
    ~HCColumnSpeaker();

    void start();
    void stop();

signals:
    void showMsg(QString msg);
    void signalInitWorker();
    void signalCmdPlayMp3NameType(QString name,     // 播放MP3 （文件名， 播放类型）
                                  int Type,
                                  int times);
    void signalCmdPlayTTS(QString name, int Type,   // TTS 播放
                          int times);
    void signalCmdStopPlay();                       // 停止播放
    void signalCmdSetVolume(int volume);            // 调节音量
    void signalIllegalAct();                        // 音柱违法行为
    void signalllegalActOver();                     // 音柱违法行为结束

private:
    QThread m_workerThread;                         // 作线程
    HCColumnSpeakerWorker *m_work = nullptr;        // 工作类
};

#endif // HCCOLUMNSPEAKER_H
