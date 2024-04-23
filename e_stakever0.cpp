#include "e_stakever0.h"
#include "e_stakever0worker.h"

E_StakeVer0::E_StakeVer0(int EstakeControlType, QString portOrIp, QObject *parent)
    : QObject{parent}
{
    m_work = new E_StakeVer0Worker(EstakeControlType, portOrIp);
    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);
    connect(m_work, &E_StakeVer0Worker::showMsg, this,&E_StakeVer0::showMsg);
    connect(this, &E_StakeVer0::signalInitWorker, m_work,&E_StakeVer0Worker::slotInit);
    connect(this, &E_StakeVer0::signalAddEStake, m_work,&E_StakeVer0Worker::slotAddEStake);
    connect(this, &E_StakeVer0::signalIllegalAct, m_work,&E_StakeVer0Worker::slotIllegalAct);
    connect(this, &E_StakeVer0::signalllegalActOver, m_work,&E_StakeVer0Worker::slotIllegalActOver);
}

E_StakeVer0::~E_StakeVer0()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void E_StakeVer0::start()
{
    m_workerThread.start();
    emit signalInitWorker();
}

void E_StakeVer0::stop()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
