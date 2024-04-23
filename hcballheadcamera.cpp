#include "hcballheadcamera.h"
#include "hcballheadcameraworker.h"

HCBallheadCamera::HCBallheadCamera(QObject *parent)
    : QObject{parent}
{
    m_work = new HCBallheadCameraWorker();
    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);

    connect(m_work, &HCBallheadCameraWorker::showMsg, this,&HCBallheadCamera::showMsg);
    connect(this, &HCBallheadCamera::signalInitWorker, m_work,&HCBallheadCameraWorker::slotInit);
    connect(m_work, &HCBallheadCameraWorker::signalIllegalAct, this,&HCBallheadCamera::signalIllegalAct);
    connect(m_work, &HCBallheadCameraWorker::signalGetImage, this,&HCBallheadCamera::signalGetImage);
    connect(this, &HCBallheadCamera::signalReflushBackupPath, m_work,&HCBallheadCameraWorker::slotReflushBackupPath);
    connect(this, &HCBallheadCamera::signalAddCamera, m_work,&HCBallheadCameraWorker::slotAddCamera);
}

HCBallheadCamera::~HCBallheadCamera()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void HCBallheadCamera::start()
{
    m_workerThread.start();
    emit signalInitWorker();
}

void HCBallheadCamera::stop()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
