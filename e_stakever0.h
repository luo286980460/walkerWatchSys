#ifndef E_STAKEVER0_H
#define E_STAKEVER0_H

#include <QObject>
#include <QThread>

class E_StakeVer0Worker;

class E_StakeVer0 : public QObject
{
    Q_OBJECT
public:
    explicit E_StakeVer0(int EstakeControlType, QString portOrIp, QObject *parent = nullptr);
    ~E_StakeVer0();

    void start();
    void stop();

signals:
    void showMsg(QString msg);
    void signalInitWorker();
    void signalAddEStake(int workGroup, int upId, int downId);
    void signalIllegalAct(int GroupId);         // 安全桩违法行为
    void signalllegalActOver(int GroupId);      // 安全桩违法行为结束

private:
    QThread m_workerThread;                     // 作线程
    E_StakeVer0Worker *m_work = nullptr;        // 工作类
};

#endif // E_STAKEVER0_H
