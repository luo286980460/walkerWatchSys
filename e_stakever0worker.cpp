#include <QTimer>
#include <QTextCodec>
#include <QThread>
#include "e_stakever0worker.h"

#define CONTENT1 "禁止"
#define CONTENT2 "穿越"

#define DISPLAY_FEFAULT "FF 40 FF 01 02 03 04 00 AA"
#define DISPLAY_FONT "FF 55 %1 %2 AA"

E_StakeVer0Worker::E_StakeVer0Worker(QString port, QObject *parent)
    : QObject{parent}
    , m_port(port)
{

}

void E_StakeVer0Worker::initTimer()
{
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, [=]{
        if(m_serialPort){
            //发送文字
            QString content;
            QString cmd;
            if(m_currentCntent == 2){
                m_currentCntent = 1;
                content = CONTENT1;
            }else if(m_currentCntent == 1){
                m_currentCntent = 2;
                content = CONTENT2;
            }else{
                m_currentCntent = 1;
                content = CONTENT1;
            }

//            qDebug() << "****************timeout: " << content;
//            for(int i=0; i<m_groupList.size(); i++){
//                qDebug() << QString("groupId[%1]\tlightId1[%2]\tlightId2[%3]")
//                                .arg(m_groupList.at(i)->groupId)
//                                .arg(m_groupList.at(i)->lightId1)
//                                .arg(m_groupList.at(i)->lightId2);
//            }

            for(int i=0; i<m_groupList.size(); i++){
                int id1 = m_groupList.at(i)->lightId1;
                int id2 = m_groupList.at(i)->lightId2;
                QString cmd1 = QString(DISPLAY_FONT).arg(id1, 2, 16, QLatin1Char('0')).arg(qstr2Hex(content.at(0)));
                QString cmd2 = QString(DISPLAY_FONT).arg(id2, 2, 16, QLatin1Char('0')).arg(qstr2Hex(content.at(1)));
                cmd += " ";
                cmd += cmd1;
                cmd += " ";
                cmd += cmd2;
            }

            cmd = cmd.toUpper();
            cmd.remove(0,1);
            //qDebug() << "****************cmd: " << cmd;
            m_serialPort->write(QByteArray::fromHex(cmd.toLatin1()));
        }
    });

    m_timer->setInterval(2000);
    m_timer->start();

}

void E_StakeVer0Worker::initSerialPort()
{
    m_serialPort = new QSerialPort(m_port);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    if(m_serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "***************安全桩打开成功";
        if(m_serialPort){
            QString cmd = DISPLAY_FEFAULT;
            m_serialPort->write(QByteArray::fromHex(cmd.toLatin1()));
        }
    }else{
        qDebug() << "***************安全桩打开失败";
    }
}

QString E_StakeVer0Worker::qstr2Hex(QString instr)
{
    if(instr.isEmpty()) return instr;

    QTextCodec *codec = QTextCodec::codecForName("GB2312");  //set char code
    QByteArray encodedString = codec->fromUnicode(instr);//convert to unicode
    instr = encodedString.toHex(QChar::Space); //hex add space example:FFAC21 convert to FF AC 21

    return instr;
}

void E_StakeVer0Worker::slotInit()
{
    initSerialPort();
    initTimer();
}

void E_StakeVer0Worker::slotAddGroup(int groupId, int lightId1, int lightId2)
{
    groupInfo_s* gInfo = new groupInfo_s;
    gInfo->groupId = groupId;
    gInfo->lightId1 = lightId1;
    gInfo->lightId2 = lightId2;
    m_groupList.append(gInfo);
}

