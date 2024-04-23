#ifndef E_STAKEVER0WORKER_H
#define E_STAKEVER0WORKER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class E_StakeVer0Worker : public QObject
{
    Q_OBJECT
public:
    explicit E_StakeVer0Worker(QString port, QObject *parent = nullptr);

private:
    void initTimer();
    void initSerialPort();
    QString qstr2Hex(QString instr);

public:
    typedef struct{
        int groupId;
        int lightId1;
        int lightId2;
    }groupInfo_s;

signals:

public slots:
    void slotInit();
    void slotAddGroup(int groupId, int lightId1, int lightId2);

private:
    QString m_port;
    QList<groupInfo_s*> m_groupList;
    QTimer* m_timer;
    QSerialPort* m_serialPort = nullptr;
    int m_currentCntent = 2;

};

#endif // E_STAKEVER0WORKER_H
