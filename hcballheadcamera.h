#ifndef HCBALLHEADCAMERA_H
#define HCBALLHEADCAMERA_H

#include <QObject>
#include <QThread>

class HCBallheadCameraWorker;

class HCBallheadCamera : public QObject
{
    Q_OBJECT
public:
    explicit HCBallheadCamera(QString ip, int HCCameraIndex, QObject *parent = nullptr);
    ~HCBallheadCamera();

    void start();
    void stop();

signals:
    void showMsg(QString msg);
    void signalInitWorker();
    void signalIllegalAct();
    void signalGetImage(QByteArray imagedata, QString ip);
    void signalUpDateBackUpPathDir();

private:
    QThread m_workerThread;                          // 作线程
    HCBallheadCameraWorker *m_work = nullptr;        // 工作类
};

#endif // HCBALLHEADCAMERA_H
