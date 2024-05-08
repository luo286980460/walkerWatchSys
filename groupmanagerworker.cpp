#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QDate>
#include <QDir>
#include <QApplication>

#include "groupmanagerworker.h"
#include "hcballheadcamera.h"
#include "e_stakever0.h"
#include "qtimer.h"
#include "hccolumnspeaker.h"
#include "novacontroller.h"
#include "YuanHongBox.h"

#define CFG_INI "/cfg.ini"

groupManagerWorker::groupManagerWorker(QObject *parent)
    : QObject{parent}
{

}

void groupManagerWorker::slotInit()
{
    initGroup();
    initCamera();
    initYuanHongBox();
    initE_StakeVer0();
    initTimerSec();
}

void groupManagerWorker::slotIllegalAct(int GroupId)
{
    // 违法行为在此管理
    emit showMsg(QString("group[%1]start illagel").arg(GroupId));
    emit m_eStake->signalIllegalAct(GroupId);
    m_groupList.at(GroupId)->CurrentDuration = 0;

    emit m_groupList.at(GroupId)->speaker->signalIllegalAct();
    emit m_groupList.at(GroupId)->nova->signalIllegalAct();

}

void groupManagerWorker::slotIllegalTest()
{
    for(int i=0; i<m_groupCount; i++){
        slotIllegalAct(i);
    }
}

void groupManagerWorker::initGroup()
{
    QString iniPath = QCoreApplication::applicationDirPath() + CFG_INI;

    if(!QFileInfo::exists(iniPath)){
        emit showMsg("****** cfg.ini 配置文件丢失 ******");
        return;
    }
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // 获取设备组数量
    m_groupCount = settings.value(QString("Base/GroupCount"), -1).toInt();
    if(m_groupCount == -1) emit showMsg("************ cfg.ini 配置文件数据有误<Base/GroupCount == -1>，请检查配置并重启 ************");
    // 获取违法图片保存天数
    m_SaveDays = settings.value(QString("Base/SaveDays"), 7).toInt();
    if(m_SaveDays == -1) emit showMsg("************ cfg.ini 配置文件数据有误<Base/SaveDays == -1>，请检查配置并重启 ************");

    for(int i=0; i<m_groupCount; i++){
        groupInfo_s* group = new groupInfo_s;
        m_groupList << group;

        // 获取相机 数量
        group->CameraCount = settings.value(QString("Group%1/CameraCount").arg(i), -1).toInt();
        if(group->CameraCount >= 0){
            // 获取相机 ip
            for(int j=0; j<group->CameraCount; j++){
                group->CameraIpList << settings.value(QString("Group%1/CameraIp%2").arg(i).arg(j,2,10,QLatin1Char('0')), "-1").toString();
                if(group->CameraIpList.last() == "-1"){
                    emit showMsg(QString("************ cfg.ini 配置文件 Group%1/CameraIp%2 有误************").arg(i).arg(j,2,10,QLatin1Char('0')));
                }
            }
        }else{
            emit showMsg("************ 相机数量错误 ************");
        }


        // 获取分析盒 数量
        group->YuanHongBoxCount = settings.value(QString("Group%1/YuanHongBoxCount").arg(i), -1).toInt();
        if(group->YuanHongBoxCount >= 0){
            // 获取分析盒 ip
            for(int j=0; j<group->YuanHongBoxCount; j++){
                group->YuanHongBoxIpList << settings.value(QString("Group%1/YuanHongBoxIp%2").arg(i).arg(j,2,10,QLatin1Char('0')), "-1").toString();
                if(group->YuanHongBoxIpList.last() == "-1"){
                    emit showMsg(QString("************ cfg.ini 配置文件 Group%1/YuanHongBoxIp%2 有误************").arg(i).arg(j,2,10,QLatin1Char('0')));
                }
            }
        }else{
            emit showMsg("************ 分析盒数量错误 ************");
        }


        // 获取安全桩版本
        m_EstakeVersion = settings.value(QString("Base/EstakeVersion"), -1).toInt();
        // 获取安全桩控制类型（0串口1网口）
        m_EstakeControlType = settings.value(QString("Base/EstakeControlType"), -1).toInt();
        // 获取安全桩串口控制端口 或者 控制ip
        if(m_EstakeControlType == 0){
            m_portOrIp = settings.value(QString("Base/EstakeCom"), "-1").toString();
            if(m_portOrIp == "-1"){
                emit showMsg("************ 串口控制安全桩请提供正确串口名 ************");
            }
        }else if(m_EstakeControlType == 1){
            m_portOrIp = settings.value(QString("Base/EstakeIp"), "-1").toString();
            if(m_portOrIp == "-1"){
                emit showMsg("************ 网口控制安全桩请提供正确网口名 ************");
            }
        }else{
            emit showMsg("************ 安全桩类型错误（0/1） ************");
        }

        // 获取音柱类型
        group->SpakerType = settings.value(QString("Group%1/SpakerType").arg(i), -1).toInt();
        // 获取音柱ip
        group->SpakerIp = settings.value(QString("Group%1/SpakerIp").arg(i), "-1").toString();
        // 获取音柱播放时间
        group->SpeakerTimes = settings.value(QString("Group%1/SpeakerTimes").arg(i), -1).toInt();
        // 获取音柱播放内容
        group->SpeakerContent = settings.value(QString("Group%1/SpeakerContent").arg(i), "-1").toString();
        // 获取音柱播放音量（1-10）
        group->SpeakerVolume = settings.value(QString("Group%1/SpeakerVolume").arg(i), -1).toInt();
        // 获取音柱播放类型（TTS/MP3）
        group->SpeakerPlayMode = settings.value(QString("Group%1/SpeakerPlayMode").arg(i), "-1").toString();
        // 获取诺瓦控制卡ip
        group->NovaControllerIp = settings.value(QString("Group%1/NovaControllerIp").arg(i), "-1").toString();
        // 获取恢复默认状态的时间（单位秒）
        group->Back2DefaultProgram = settings.value(QString("Group%1/Back2DefaultProgram").arg(i), -1).toInt();
        // 获取大华音柱id（音柱为海康时，不需要此值）
        group->SpeakerId = settings.value(QString("Group%1/SpeakerId").arg(i), -1).toInt();
        // 获取安全桩上层灯拨码
        group->EstakeUpLightId = settings.value(QString("Group%1/EstakeUpLightId").arg(i), -1).toInt();
        // 获取安全桩下层灯拨码
        group->EstakeDownLightId = settings.value(QString("Group%1/EstakeDownLightId").arg(i), -1).toInt();
        // 获取告警时长（单位秒）
        group->AlarmDuration = settings.value(QString("Group%1/AlarmDuration").arg(i), -1).toInt();

        group->CurrentDuration = group->AlarmDuration+1;

        // 根据音柱类型判断配置文件是否正确
        if(group->SpakerType == 0){
            if(group->SpeakerVolume < 2 || group->SpeakerVolume > 7){
                emit showMsg("************ 海康音柱音量限定<2~7> ************");
            }
        }else if(group->SpakerType == 1){
            if(group->SpeakerVolume < 1 || group->SpeakerVolume > 10){
                emit showMsg("************ 大华音柱音量限定<1~10> ************");
            }
            if(group->SpeakerId == -1){
                emit showMsg("************ 大华音柱id错误 ************");
            }
        }

        if(group->SpakerType == -1 || group->SpakerIp == "-1"
            || group->SpeakerTimes == -1 || group->SpeakerContent == "-1" || group->SpeakerPlayMode == "-1"
            || group->NovaControllerIp == "-1" || group->EstakeUpLightId == -1|| group->EstakeDownLightId == -1
            || group->AlarmDuration == -1) {
            emit showMsg("************ cfg.ini 配置文件数据有误，请检查配置并重启 ************");
        }

        emit showMsg(QString("\n\
分组号[%1]\t\t相机数量[%2]\t\t音柱类型[%3]\t\t音柱ip[%4]\n\
音柱播放时间[%5]\t音柱播放内容[%6]\t音柱播放音量[%7]\t音柱播放类型[%8]\n\
诺瓦ip[%9]\t恢复默认时间[%10]\t大华音柱id[%11]\t\t安全桩版本[%12]\n\
安全桩控制类型[%13]\t安全桩串口[%14]\t安全桩网口[%15]\t上拨码[%16]\n\
下拨码[%17]\t\t告警时长[%18]\t\t分析盒数量[%19]\n")
                         .arg(i).arg(group->CameraCount).arg(group->SpakerType).arg(group->SpakerIp)
                         .arg(group->SpeakerTimes).arg(group->SpeakerContent).arg(group->SpeakerVolume).arg(group->SpakerType)
                         .arg(group->NovaControllerIp).arg(group->Back2DefaultProgram).arg(group->SpeakerId).arg(m_EstakeVersion)
                         .arg(m_EstakeControlType).arg(m_portOrIp).arg(m_portOrIp).arg(group->EstakeUpLightId)
                         .arg(group->EstakeDownLightId).arg(group->AlarmDuration).arg(group->YuanHongBoxCount));

        /* 初始化音柱 */
        group->speaker = new HCColumnSpeaker(group->SpakerIp, group->SpeakerContent, group->SpeakerTimes,
                                             group->SpeakerVolume, group->SpeakerPlayMode, group->SpeakerId,
                                             group->SpakerType);
        connect(group->speaker, &HCColumnSpeaker::showMsg, this, &groupManagerWorker::showMsg);
        group->speaker->start();

        /* 初始化诺瓦屏 */
        group->nova = new NovaController(group->NovaControllerIp, group->Back2DefaultProgram, this);
        connect(group->nova, &NovaController::showMsg, this, &groupManagerWorker::showMsg);
        group->nova->start();
    }
}

void groupManagerWorker::initCamera()
{
    m_camera = new HCBallheadCamera();
    connect(m_camera, &HCBallheadCamera::showMsg, this, &groupManagerWorker::showMsg);
    connect(this, &groupManagerWorker::signalReflushBackupPath, m_camera, &HCBallheadCamera::signalReflushBackupPath);
    connect(m_camera, &HCBallheadCamera::signalIllegalAct, this, &groupManagerWorker::slotIllegalAct);
    m_camera->start();

    for(int i=0; i<m_groupCount; i++){
        for(int j=0; j<m_groupList.at(i)->CameraCount; j++){
            emit m_camera->signalAddCamera(i, m_groupList.at(i)->CameraIpList.at(j), 8000, "admin", "utis123456");
        }
    }
}

void groupManagerWorker::initYuanHongBox()
{
    m_yuanHongBox = new YuanHongBox();
    connect(m_yuanHongBox, &YuanHongBox::showMsg, this, &groupManagerWorker::showMsg);
    connect(m_yuanHongBox, &YuanHongBox::signalIllegalAct, this, &groupManagerWorker::slotIllegalAct);
    m_yuanHongBox->start();

    for(int i=0; i<m_groupCount; i++){
        for(int j=0; j<m_groupList.at(i)->YuanHongBoxCount; j++){
            //emit showMsg(QString("组【%1】boxCount【%2】").arg(i).arg(m_groupList.at(i)->YuanHongBoxIpList.at(j)));
            qDebug() << QString("send组【%1】boxCount【%2】").arg(i).arg(m_groupList.at(i)->YuanHongBoxIpList.at(j));
            emit m_yuanHongBox->signalAddBox(i, m_groupList.at(i)->YuanHongBoxIpList.at(j));
        }
    }
}

void groupManagerWorker::initE_StakeVer0()
{
    m_eStake = new E_StakeVer0(m_EstakeControlType, m_portOrIp);
    connect(m_eStake, &E_StakeVer0::showMsg, this, &groupManagerWorker::showMsg);
    m_eStake->start();

    for(int i=0; i<m_groupCount; i++){
        emit m_eStake->signalAddEStake(i, m_groupList.at(i)->EstakeUpLightId, m_groupList.at(i)->EstakeDownLightId);
    }
}

void groupManagerWorker::initTimerSec()
{
    m_timerSec = new QTimer();
    connect(m_timerSec, &QTimer::timeout, this, [=]{
        for(int i=0; i<m_groupList.size(); i++){
            if(m_groupList.at(i)->CurrentDuration < m_groupList.at(i)->AlarmDuration){
                m_groupList.at(i)->CurrentDuration++;
                continue;
            }

            if(m_groupList.at(i)->CurrentDuration == m_groupList.at(i)->AlarmDuration){
                m_groupList.at(i)->CurrentDuration++;
                emit m_eStake->signalllegalActOver(i);
                showMsg(QString("组[%1]违法结束").arg(i));
            }

            //emit showMsg(QString("组[%1]当前持续[%2]").arg(i).arg(m_groupList.at(i)->CurrentDuration));
        }

    });
    m_timerSec->setInterval(1000);
    m_timerSec->start();
}
