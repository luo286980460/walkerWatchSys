#include "groupmanager.h"
#include "groupmanagerworker.h"

groupManager::groupManager(QObject *parent)
    : QObject{parent}
{
    m_work = new groupManagerWorker();
    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);
    connect(m_work, &groupManagerWorker::showMsg, this,&groupManager::showMsg);
    connect(this, &groupManager::signalInitWorker, m_work,&groupManagerWorker::slotInit);
    connect(this, &groupManager::signalReflushBackupPath, m_work,&groupManagerWorker::signalReflushBackupPath);
}

groupManager::~groupManager()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void groupManager::start()
{
    m_workerThread.start();
    emit signalInitWorker();
}

void groupManager::stop()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
