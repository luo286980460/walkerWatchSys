#include <QTimer>
#include <QTextCodec>
#include <QThread>
#include "e_stakever0worker.h"

#define CONTENT1 "行人"
#define CONTENT2 "穿越"

#define DISPLAY_FEFAULT "FF 40 FF 01 02 03 04 00 AA"
#define DISPLAY_FONT    "FF 55 %1 %2 AA"
//#define DISPLAY_ClOSE   "FF 55 %1 00 AA"

E_StakeVer0Worker::E_StakeVer0Worker(int EstakeControlType, QString portOrIp, QObject *parent)
    : QObject{parent}
    , m_EstakeControlType(EstakeControlType)
    , m_portOrIp(portOrIp)
{

}

void E_StakeVer0Worker::initTimer()
{
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, [=]{
        if(m_serialPort){
            sendPrograme();
        }
    });

    m_timer->setInterval(2000);
    m_timer->start();

}

void E_StakeVer0Worker::initSerialPort()
{
    m_serialPort = new QSerialPort(m_portOrIp);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    if(m_serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "***************安全桩打开成功";
        if(m_serialPort){
            QString cmd = DISPLAY_FEFAULT;
            m_serialPort->write(QByteArray::fromHex(cmd.toLatin1()));
            m_serialPort->waitForBytesWritten();
            qDebug() << "***************cmd" << cmd;
        }
    }else{
        qDebug() << "***************安全桩打开失败";
    }
}

void E_StakeVer0Worker::initNetWorke()
{
    // 待实现
}

QString E_StakeVer0Worker::qstr2Hex(QString instr)
{
    if(instr.isEmpty()) return instr;

    QTextCodec *codec = QTextCodec::codecForName("GB2312");  //set char code
    QByteArray encodedString = codec->fromUnicode(instr);//convert to unicode
    instr = encodedString.toHex(QChar::Space); //hex add space example:FFAC21 convert to FF AC 21

    return instr;
}

void E_StakeVer0Worker::sendPrograme()
{
    QString content;
    QString cmd;

    if(m_serialPort){
        //发送文字
        switch(m_currentCntent){
        case 1:
            content = CONTENT1;
            m_currentCntent = 2;
            break;
        case 2:
            content = CONTENT2;
            m_currentCntent = 1;
            break;
        default:
            break;
        }

        for(int i=0; i<m_eStakeList.size(); i++){

            //showMsg(QString("组[%1]upId[%2]downId[%3]").arg(i).arg(m_eStakeList.at(i)->upId).arg(m_eStakeList.at(i)->downId));
            QString tmp;
            if(!m_eStakeList.at(i)->working) {
                tmp = QString(DISPLAY_FONT).arg(m_eStakeList.at(i)->upId, 2, 16, QLatin1Char('0')).arg("A0 F0");
                cmd.append(" ").append(tmp);
                tmp = QString(DISPLAY_FONT).arg(m_eStakeList.at(i)->downId
                                                , 2, 16, QLatin1Char('0')).arg("A0 F0");
                cmd.append(" ").append(tmp);
            }else{
                tmp = QString(DISPLAY_FONT).arg(m_eStakeList.at(i)->upId, 2, 16, QLatin1Char('0')).arg(qstr2Hex(content.at(0)));
                cmd.append(" ").append(tmp);
                tmp = QString(DISPLAY_FONT).arg(m_eStakeList.at(i)->downId, 2, 16, QLatin1Char('0')).arg(qstr2Hex(content.at(1)));
                cmd.append(" ").append(tmp);
            }
        }

        cmd = cmd.toUpper();
        //qDebug() << "****************cmd: " << cmd;
        m_serialPort->write(QByteArray::fromHex(cmd.toLatin1()));
    }

}

void E_StakeVer0Worker::slotInit()
{
    if(m_EstakeControlType == 0){
        initSerialPort();
    }else if(m_EstakeControlType == 1){
        initNetWorke();
    }else{
        return;
    }
    initTimer();
}

void E_StakeVer0Worker::slotAddEStake(int workGroup, int upId, int downId)
{
    eStake_s* eStake= new eStake_s;
    m_eStakeList << eStake;
    eStake->workGroup = workGroup;
    eStake->upId = upId;
    eStake->downId = downId;
    eStake->working = false;
}

void E_StakeVer0Worker::slotIllegalAct(int GroupId)
{
    for(int i=0; i<m_eStakeList.size(); i++){
        if(m_eStakeList.at(i)->workGroup == GroupId){
            m_eStakeList.at(i)->working = true;
        }
    }
}

void E_StakeVer0Worker::slotIllegalActOver(int GroupId)
{
    for(int i=0; i<m_eStakeList.size(); i++){
        if(m_eStakeList.at(i)->workGroup == GroupId){
            m_eStakeList.at(i)->working = false;
        }
    }
}

