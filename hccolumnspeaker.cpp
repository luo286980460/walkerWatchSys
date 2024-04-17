#include "hccolumnspeaker.h"
#include "hccolumnspeakerworker.h"

HCColumnSpeaker::HCColumnSpeaker(QString ip, QString HCSpeakerContent, int HCSpeakerTimes,
                                 int HCSpeakerVolume, QString HCSpeakerPlayMode,
                                 int DHSpeakerId, int SpeakerType, QObject *parent)
    : QObject{parent}
{
    m_work = new HCColumnSpeakerWorker(ip, HCSpeakerContent, HCSpeakerTimes, HCSpeakerVolume, HCSpeakerPlayMode, DHSpeakerId, SpeakerType);
    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);

    connect(m_work, &HCColumnSpeakerWorker::showMsg, this,&HCColumnSpeaker::showMsg);
    connect(this, &HCColumnSpeaker::signalInitWorker, m_work,&HCColumnSpeakerWorker::slotInit);
    connect(this, &HCColumnSpeaker::signalCmdPlayMp3NameType, m_work,&HCColumnSpeakerWorker::slotCmdPlayMp3NameType);
    connect(this, &HCColumnSpeaker::signalCmdPlayTTS, m_work,&HCColumnSpeakerWorker::slotCmdPlayTTS);
    connect(this, &HCColumnSpeaker::signalCmdStopPlay, m_work,&HCColumnSpeakerWorker::slotCmdStopPlay);
    connect(this, &HCColumnSpeaker::signalCmdSetVolume, m_work,&HCColumnSpeakerWorker::slotCmdSetVolume);
    connect(this, &HCColumnSpeaker::signalIllegalAct, m_work,&HCColumnSpeakerWorker::slotIllegalAct);
}

HCColumnSpeaker::~HCColumnSpeaker()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void HCColumnSpeaker::start()
{
    m_workerThread.start();
    emit signalInitWorker();
}

void HCColumnSpeaker::stop()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
