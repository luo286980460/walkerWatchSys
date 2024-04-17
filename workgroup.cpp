#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QDate>
#include "workgroup.h"
#include "hcballheadcamera.h"
#include "hccolumnspeaker.h"
#include "novacontroller.h"

#define CFG_INI "/cfg.ini"

WorkGroup::WorkGroup(int groupId, QObject *parent)
    : QObject{parent}
    , m_groupId(groupId)
    //, m_sendIllegalSignalFlag(3)
    , m_sendIllegalSignal(true)
{

}

void WorkGroup::init()
{
    initCfg();
    //clsOverTimeIllegalPics();
    initNova();
    initSpeaker();
    initCamera();
    initTimer();
}

void WorkGroup::test()
{
    slotIllegalAct();
}

void WorkGroup::initCfg()
{
    QString iniPath = QCoreApplication::applicationDirPath() + CFG_INI;
    if(!QFileInfo::exists(iniPath)){
        emit showMsg("****** cfg.ini 配置文件丢失 ******");
        return;
    }
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    m_HCCameraIp1 = settings.value(QString("Group%1/HCCameraIp1").arg(m_groupId), "-1").toString();
    m_HCCameraIndex1 = settings.value(QString("Group%1/HCCameraIndex1").arg(m_groupId), -1).toInt();
    m_HCCameraIp2 = settings.value(QString("Group%1/HCCameraIp2").arg(m_groupId), "-1").toString();
    m_HCCameraIndex2 = settings.value(QString("Group%1/HCCameraIndex2").arg(m_groupId), -1).toInt();
    m_HCSpeakerIp = settings.value(QString("Group%1/HCSpeakerIp").arg(m_groupId), "-1").toString();
    m_HCSpeakerTimes = settings.value(QString("Group%1/HCSpeakerTimes").arg(m_groupId), -1).toInt();
    m_HCSpeakerContent = settings.value(QString("Group%1/HCSpeakerContent").arg(m_groupId), "-1").toString();
    m_HCSpeakerVolume = settings.value(QString("Group%1/HCSpeakerVolume").arg(m_groupId), -1).toInt();
    m_NovaControllerIp = settings.value(QString("Group%1/NovaControllerIp").arg(m_groupId), "-1").toString();
    m_Back2DefaultProgram = settings.value(QString("Group%1/Back2DefaultProgram").arg(m_groupId), -1).toInt();
    m_HCSpeakerPlayMode = settings.value(QString("Group%1/HCSpeakerPlayMode").arg(m_groupId), "-1").toString().toUpper();
    m_SaveDays = settings.value(QString("Other/SaveDays"), -1).toInt();
    m_DHSpeakerId = settings.value(QString("Group%1/DHSpeakerId").arg(m_groupId), -1).toInt();
    m_SpeakerType = settings.value(QString("Group%1/SpeakerType").arg(m_groupId), -1).toInt();

//    emit showMsg(QString("Group:[%1]\nm_HCCameraIp1[%2] m_HCCameraIndex1[%3]\nm_HCCameraIp2[%4] m_HCCameraIndex2[%5]\n\
//m_HCSpeakerIp[%6] m_HCSpeakerTimes[%7] m_HCSpeakerContent[%8] m_HCSpeakerVolume[%9]\n\
//m_NovaControllerIp[%10] m_Back2DefaultProgram[%11] m_PlayMode[%12]")
//                .arg(m_groupId).arg(m_HCCameraIp1).arg(m_HCCameraIndex1).arg(m_HCCameraIp2).arg(m_HCCameraIndex2)
//                .arg(m_HCSpeakerIp).arg(m_HCSpeakerTimes).arg(m_HCSpeakerContent).arg(m_HCSpeakerVolume)
//                .arg(m_NovaControllerIp).arg(m_Back2DefaultProgram).arg(m_PlayMode));

    emit showMsg(QString("Group%1/DHSpeakerId").arg(m_groupId));
    emit showMsg(QString("分组号[%1]\n相机IP1[%2]\t回调下标1[%3]\n相机IP2[%4]\t回调下标2[%5]\n\
音柱IP[%6]\t音柱播放次数[%7]\t音柱播放内容[%8]\t音柱播放音量[%9]\n\
屏幕IP[%10]\t屏幕恢复默认节目间隔[%11]\t播放模式[%12]\n\
大华音柱ID[%13]\t音柱类型[%14]")
                             .arg(m_groupId).arg(m_HCCameraIp1).arg(m_HCCameraIndex1).arg(m_HCCameraIp2).arg(m_HCCameraIndex2)
                             .arg(m_HCSpeakerIp).arg(m_HCSpeakerTimes).arg(m_HCSpeakerContent).arg(m_HCSpeakerVolume)
                             .arg(m_NovaControllerIp).arg(m_Back2DefaultProgram).arg(m_HCSpeakerPlayMode)
                             .arg(m_DHSpeakerId).arg(m_SpeakerType));

    if(m_HCCameraIp1 == "-1" || m_HCCameraIp2 == "-1" || m_HCSpeakerIp == "-1" || m_NovaControllerIp == "-1"
        || m_HCCameraIndex1 == -1 || m_HCCameraIndex2 == -1 || m_Back2DefaultProgram == -1
        || m_HCSpeakerContent == "-1" || m_HCSpeakerTimes == -1 || m_HCSpeakerVolume ==-1
        || m_HCSpeakerPlayMode == "-1" || m_SaveDays == -1 || m_DHSpeakerId == -1 || m_SpeakerType == -1){
        emit showMsg("************ cfg.ini 配置文件数据有误，请检查配置并重启 ************");
    }

    if(m_groupId == 1)
        emit signalUpdateIps(m_HCCameraIp1, m_HCCameraIp2, "-1", "-1");
    if(m_groupId == 2)
        emit signalUpdateIps("-1", "-1", m_HCCameraIp1, m_HCCameraIp2);
}

void WorkGroup::initCamera()
{
    // 相机1
    m_HCBallheadCamera1 = new HCBallheadCamera(m_HCCameraIp1, m_HCCameraIndex1, this);
    connect(m_HCBallheadCamera1, &HCBallheadCamera::showMsg, this, &WorkGroup::showMsg);
    connect(m_HCBallheadCamera1, &HCBallheadCamera::signalIllegalAct, this, &WorkGroup::slotIllegalAct);
    connect(m_HCBallheadCamera1, &HCBallheadCamera::signalGetImage, this, &WorkGroup::signalGetImage);
    connect(this, &WorkGroup::signalUpDateBackUpPathDir, m_HCBallheadCamera1, &HCBallheadCamera::signalUpDateBackUpPathDir);

    m_HCBallheadCamera1->start();

    // 相机2
    m_HCBallheadCamera2 = new HCBallheadCamera(m_HCCameraIp2, m_HCCameraIndex2, this);
    connect(m_HCBallheadCamera2, &HCBallheadCamera::showMsg, this, &WorkGroup::showMsg);
    connect(m_HCBallheadCamera2, &HCBallheadCamera::signalIllegalAct, this, &WorkGroup::slotIllegalAct);
    connect(m_HCBallheadCamera2, &HCBallheadCamera::signalGetImage, this, &WorkGroup::signalGetImage);
    connect(this, &WorkGroup::signalUpDateBackUpPathDir, m_HCBallheadCamera2, &HCBallheadCamera::signalUpDateBackUpPathDir);

    m_HCBallheadCamera2->start();
}

void WorkGroup::initSpeaker()
{
    m_HCColumnSpeaker = new HCColumnSpeaker(m_HCSpeakerIp, m_HCSpeakerContent, m_HCSpeakerTimes,
                                            m_HCSpeakerVolume, m_HCSpeakerPlayMode, m_DHSpeakerId,
                                            m_SpeakerType, this);
    connect(m_HCColumnSpeaker, &HCColumnSpeaker::showMsg, this, &WorkGroup::showMsg);

    m_HCColumnSpeaker->start();
}

void WorkGroup::initNova()
{
    m_NovaController = new NovaController(m_NovaControllerIp, m_Back2DefaultProgram, this);
    connect(m_NovaController, &NovaController::showMsg, this, &WorkGroup::showMsg);
    m_NovaController->start();
}

void WorkGroup::initTimer()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=](){
        m_sendIllegalSignal = true;
        m_timer->stop();

//        qDebug() << m_HCBallheadCamera1;
//        qDebug() << m_HCBallheadCamera2;

//        // 跨日判断是否需要删除备份文件
//        QString currentTime = QTime::currentTime().toString("hh:mm:ss") ;
//        if(currentTime == "15:25:00"){
//            clsOverTimeIllegalPics();
//            if(m_HCBallheadCamera1){
//                qDebug() << "m_HCBallheadCamera1->signalUpDateBackUpPathDir();";
//                emit m_HCBallheadCamera1->signalUpDateBackUpPathDir();
//            }
//            if(m_HCBallheadCamera2){
//                qDebug() << "m_HCBallheadCamera1->signalUpDateBackUpPathDir();";
//                emit m_HCBallheadCamera2->signalUpDateBackUpPathDir();
//            }
//        }
    });
}

void WorkGroup::clsOverTimeIllegalPics()
{
    QDate currDate = QDate::currentDate();
    QString backUpPathDir = (QString("%1/illegalPics").arg(QApplication::applicationDirPath()));
    QDir dir(backUpPathDir);
    if(dir.exists()){
        foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            QDate date = QDate::fromString(fileInfo.fileName(), "yyyyMMdd");

            if(date.daysTo(currDate) > m_SaveDays){
                QDir tmp(fileInfo.filePath());
                tmp.removeRecursively();
            }
        }
    }
}

void WorkGroup::slotIllegalAct()
{
    if(m_sendIllegalSignal){
        emit showMsg(QString("Group 报警 组ID: %1").arg(m_groupId));
        if(m_NovaController){
            emit m_NovaController->signalIllegalAct();
        }
        if(m_HCColumnSpeaker){
            emit m_HCColumnSpeaker->signalIllegalAct();
        }
        m_sendIllegalSignal = false;
        m_timer->setInterval(3000);
        m_timer->start();
    }
}
