#ifndef E_STAKEVER0_H
#define E_STAKEVER0_H

#include <QObject>
#include <QThread>

class E_StakeVer0Worker;

class E_StakeVer0 : public QObject
{
    Q_OBJECT
public:
    explicit E_StakeVer0(QString port, QObject *parent = nullptr);
    ~E_StakeVer0();

    void start();
    void stop();

signals:
    void signalInitWorker();
    void signalAddGroup(int groupId, int lightId1, int lightId2);

private:
    QThread m_workerThread;                     // 作线程
    E_StakeVer0Worker *m_work = nullptr;        // 工作类
};

#endif // E_STAKEVER0_H
