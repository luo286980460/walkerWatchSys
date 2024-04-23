#include <QApplication>

#include "novacontrollerworker.h"
#include "include/Nova/NovaTraffic.h"
#include "NovaHeader.h"

NovaControllerWorker::NovaControllerWorker(QString ip, int Back2DefaultProgram, QObject *parent)
    : QObject{parent}
    , m_ip(ip)
    , m_port(5000)
    , m_Back2DefaultProgramTime(Back2DefaultProgram)
    , m_Back2DefaultProgramTimeFlag(Back2DefaultProgram)
{

}

void NovaControllerWorker::playProgram(int id)
{
    if(m_traffic && m_timer){

        emit showMsg(QString("播放节目 %1 ").arg(id));

        switch (id) {
        case 1:
            playProgram1();
            break;
        case 2:
            playProgram2();
            break;
        case 3:
            playProgram3();
            break;
        default:
            break;
        }
        if(id != 1){
            m_Back2DefaultProgramTimeFlag = 0;
        }
        m_playFlag = 0;
    }
}

void NovaControllerWorker::playProgram1()
{
    // 下发文件
    int res = m_traffic->sendFile((QApplication::applicationDirPath() + "/Pics/" + PROGRAM1_ITEM1_PIC).toLocal8Bit(),
                                  QString(PROGRAM1_ITEM1_PIC).toLocal8Bit());

    //emit showMsg(QString("playProgram1() sendFile res = %1").arg(res));

    // 发送节目
    char program[] = PROGRAM1;
    m_traffic->sendPlayList(1, program);

    //emit showMsg(QString("playProgram1() sendPlayList res = %1").arg(res));
}

void NovaControllerWorker::playProgram2()
{
    // 下发文件
    int res = m_traffic->sendFile((QApplication::applicationDirPath() + "/Pics/" + PROGRAM2_ITEM1_PIC).toLocal8Bit(),
                                  QString(PROGRAM2_ITEM1_PIC).toLocal8Bit());

    //emit showMsg(QString("playProgram2() sendFile res = %1").arg(res));

    // 发送节目
    char program[] = PROGRAM2;
    m_traffic->sendPlayList(2, program);

    //emit showMsg(QString("playProgram1() sendPlayList res = %1").arg(res));
}

void NovaControllerWorker::playProgram3()
{
    // 下发文件 1
    int res = m_traffic->sendFile((QApplication::applicationDirPath() + "/Pics/" + PROGRAM3_ITEM1_PIC).toLocal8Bit(),
                                  QString(PROGRAM3_ITEM1_PIC).toLocal8Bit());

    //emit showMsg(QString("playProgram3() sendFile1 res = %1").arg(res));

    // 下发文件 2
    res = m_traffic->sendFile((QApplication::applicationDirPath() + "/Pics/" + PROGRAM3_ITEM2_PIC).toLocal8Bit(),
                              QString(PROGRAM3_ITEM2_PIC).toLocal8Bit());

    //emit showMsg(QString("playProgram3() sendFile2 res = %1").arg(res));

    // 发送节目
    char program[] = PROGRAM3;
    m_traffic->sendPlayList(3, program);

    //emit showMsg(QString("playProgram1() sendPlayList res = %1").arg(res));
}

void NovaControllerWorker::slotInit()
{
    m_traffic = new NovaTraffic(m_ip.toLocal8Bit(), m_port);

    // 播放节目1 默认节目
    playProgram(1);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [=](){
        //showMsg(QString("m_playFlag:%1 m_ProgramInterval:%2 ").arg(m_playFlag).arg(m_ProgramInterval));
        //showMsg(QString("m_Back2DefaultProgramTimeFlag:%1 m_Back2DefaultProgramTime:%2 ").arg(m_Back2DefaultProgramTimeFlag).arg(m_Back2DefaultProgramTime));
        if(m_playFlag < m_ProgramInterval){         // 不能发送节目
            m_playFlag++;
            if(m_Back2DefaultProgramTimeFlag < m_Back2DefaultProgramTime){    // 等待发送节目
                m_Back2DefaultProgramTimeFlag++;
            }

        }else if(m_playFlag == m_ProgramInterval){  // 可发送节目
            if(m_Back2DefaultProgramTimeFlag == m_Back2DefaultProgramTime){
                // 播放节目1 默认节目
                playProgram(1);
                m_Back2DefaultProgramTimeFlag++;
            }else if(m_Back2DefaultProgramTimeFlag < m_Back2DefaultProgramTime){    // 等待发送节目
                m_Back2DefaultProgramTimeFlag++;
            }
        }
    });
    m_timer->start();
}

void NovaControllerWorker::slotIllegalAct()
{
    qDebug() << QString("大屏违法:%1").arg(m_ip);
    if(m_traffic){
        // 播放节目2        临时
        playProgram(2);
    }
}
