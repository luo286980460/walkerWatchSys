#ifndef HCBALLHEADCAMERA_H
#define HCBALLHEADCAMERA_H

#include <QObject>
#include <QThread>

class HCBallheadCameraWorker;

class HCBallheadCamera : public QObject
{
    Q_OBJECT
public:
    explicit HCBallheadCamera(QObject *parent = nullptr);
    ~HCBallheadCamera();

    void start();
    void stop();

signals:
    void showMsg(QString msg);
    void signalInitWorker();
    void signalIllegalAct(int GroupId);
    void signalGetImage(QByteArray imagedata, QString ip);
    void signalAddCamera(int workGroup, QString ip, int port, QString user, QString pwd);
    void signalReflushBackupPath();     // 刷新违法图片保存文件夹

private:
    QThread m_workerThread;                          // 作线程
    HCBallheadCameraWorker *m_work = nullptr;        // 工作类
};

#endif // HCBALLHEADCAMERA_H
