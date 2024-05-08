#ifndef YUANHONGBOXWORKER_H
#define YUANHONGBOXWORKER_H

#include <QObject>
#include "include/libhv/HttpServer.h"

using namespace hv;

typedef struct {
    int workGroup;      // 分析盒工作组
    QString ip;         // 分析盒ip
}boxInfo_s;

class YuanHongBoxWorker : public QObject
{
    Q_OBJECT
public:
    explicit YuanHongBoxWorker(QObject *parent = nullptr);

private:
    int getGroupId(QString ip);

signals:
    void showMsg(QString msg);
    void signalIllegalAct(int GroupId);         // 违法组

public slots:
    void slotStart();
    void slotAddBox(int workGroup, QString ip);

private:
    int m_port = 2333;
    HttpService router;
    HttpServer server;
    QList<boxInfo_s*> m_boxList;        // 分析盒列表

};

#endif // YUANHONGBOXWORKER_H
