#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QObject>
#include <QThread>

class groupManagerWorker;

class groupManager : public QObject
{
    Q_OBJECT
public:
    explicit groupManager(QObject *parent = nullptr);
    ~groupManager();

    void start();
    void stop();

signals:
    void showMsg(QString msg);
    void signalInitWorker();
    void signalReflushBackupPath();     // 刷新违法图片保存文件夹
    void signalIllegalTest();
    void signalUpdateDayOrNight(bool isDaytime);    // 更新白天黑夜状态
    void signalUpdateDayNightTime(QString dayTimeStart, QString dayTimeSopt);

private:
    QThread m_workerThread;                      // 作线程
    groupManagerWorker *m_work = nullptr;        // 工作类
};

#endif // GROUPMANAGER_H
