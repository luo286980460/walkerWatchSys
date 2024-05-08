#include "YuanHongBox.h"
#include "YuanHongBoxWorker.h"

YuanHongBox::YuanHongBox(QObject *parent)
    : QObject{parent}
{
    m_work = new YuanHongBoxWorker;

    m_work->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);
    connect(this, &YuanHongBox::signalStart, m_work, &YuanHongBoxWorker::slotStart);
    connect(m_work, &YuanHongBoxWorker::signalIllegalAct, this, &YuanHongBox::signalIllegalAct);
    connect(m_work, &YuanHongBoxWorker::showMsg, this, &YuanHongBox::showMsg);
    connect(this, &YuanHongBox::signalAddBox, m_work, &YuanHongBoxWorker::slotAddBox);
}

void YuanHongBox::start()
{
    m_workerThread.start();
    emit signalStart();
}

YuanHongBox::~YuanHongBox()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void YuanHongBox::stop()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
