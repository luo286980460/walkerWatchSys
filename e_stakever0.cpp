#include "e_stakever0.h"
#include "e_stakever0worker.h"

E_StakeVer0::E_StakeVer0(QString port, QObject *parent)
    : QObject{parent}
{
    m_work = new E_StakeVer0Worker(port);
    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);
    connect(this, &E_StakeVer0::signalInitWorker, m_work,&E_StakeVer0Worker::slotInit);
    connect(this, &E_StakeVer0::signalAddGroup, m_work,&E_StakeVer0Worker::slotAddGroup);
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
