#ifndef YUANHONGBOX_H
#define YUANHONGBOX_H

#include <QObject>
#include <QThread>

class YuanHongBoxWorker;

class YuanHongBox : public QObject
{
    Q_OBJECT
public:
    explicit YuanHongBox(QObject *parent = nullptr);
    ~YuanHongBox();

    void start();       //开始线程
    void stop();        //结束线程

signals:
    void showMsg(QString msg);
    void signalStart();
    void signalIllegalAct(int GroupId);         // 违法组
    void signalAddBox(int workGroup, QString ip);

private:
    QThread m_workerThread;
    YuanHongBoxWorker *m_work = nullptr;


};

#endif // YUANHONGBOX_H
