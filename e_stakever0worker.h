#ifndef E_STAKEVER0WORKER_H
#define E_STAKEVER0WORKER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

typedef struct {
    int workGroup;          // 安全桩工作组
    int upId;               // 上面灯的拨码
    QString upContent;      // 上面灯的内容
    int downId;             // 下面灯的拨码
    QString downContent;    // 下面灯的内容
    bool working;           // 是否开启工作状态
}eStake_s;

class E_StakeVer0Worker : public QObject
{
    Q_OBJECT
public:
    explicit E_StakeVer0Worker(int EstakeControlType, QString portOrIp, QObject *parent = nullptr);

private:
    void initTimer();
    void initSerialPort();
    void initNetWorke();
    QString qstr2Hex(QString instr);
    void sendPrograme();

    int m_EstakeControlType;            // 安全桩控制器类型（0串口/1网口）
    QString m_portOrIp;                 // 安全桩控制器 端口号 或者 ip
    QList<eStake_s*> m_eStakeList;      // 安全桩列表
signals:
    void showMsg(QString msg);

public slots:
    void slotInit();
    void slotAddEStake(int workGroup, int upId, int downId);
    void slotIllegalAct(int GroupId);       // 安全桩违法行为发生
    void slotIllegalActOver(int GroupId);   // 安全桩违法行为结束

private:
    QTimer* m_timer;
    QSerialPort* m_serialPort = nullptr;
    int m_currentCntent = 2;
    bool m_LightOn = false;

};

#endif // E_STAKEVER0WORKER_H
