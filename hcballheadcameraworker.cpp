#include "hcballheadcameraworker.h"
#include <QImage>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QDebug>

HCBallheadCameraWorker* g_this = nullptr;
bool HCBallheadCameraWorker::m_haveBack = false;

HCBallheadCameraWorker::HCBallheadCameraWorker(QObject *parent)
    : QObject{parent}
{
    g_this = this;
}

HCBallheadCameraWorker::~HCBallheadCameraWorker()
{
//    //撤销布防上传通道
//    if (!NET_DVR_CloseAlarmChan_V30(m_lHandle))
//    {
//        printf("NET_DVR_CloseAlarmChan_V30 failed, error code: %d\n", NET_DVR_GetLastError());
//        NET_DVR_Logout(m_lUserID);
//        NET_DVR_Cleanup();
//        return;
//    }

//    //注销用户
//    NET_DVR_Logout(m_lUserID);

//    //释放SDK资源
    //    NET_DVR_Cleanup();
}

bool HCBallheadCameraWorker::initSdk()
{
    // 初始化 SDK
    BOOL res = NET_DVR_Init();
    if(res != 1){
        emit showMsg(QString("sdk初始化错误：res = %1").arg(res));
        return false;
    }
    //emit showMsg(QString("sdk初始化成功：res = %1").arg(res));
    //设置报警回调函数
    if(!m_haveBack){
        if(NET_DVR_SetDVRMessageCallBack_V50(0, MessageCallback, nullptr)){
            m_haveBack = true;
        }else{
            m_haveBack = false;
        }
    }

    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    return true;
}

void HCBallheadCameraWorker::slotInit()
{
    //initBackupPath();
    initSdk();
    //logInCamera(m_ip, m_port, m_user, m_pwd);
    //startAlarm();
}

void HCBallheadCameraWorker::slotAddCamera(int workGroup, QString ip, int port, QString user, QString pwd)
{
    cameraInfo_s* camera = new cameraInfo_s;
    m_cameraList << camera;
    camera->workGroup = workGroup;
    camera->ip = ip;
    camera->port = port;
    camera->user = user;
    camera->pwd = pwd;

    initBackupPath(camera);
    logInCamera(camera);
}

void HCBallheadCameraWorker::slotReflushBackupPath()
{
    for(int i=0; i<m_cameraList.size(); i++){
        initBackupPath(m_cameraList.at(i));
    }
}

void HCBallheadCameraWorker::MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
    Q_UNUSED(pAlarmer);
    Q_UNUSED(dwBufLen);
    Q_UNUSED(pUser);

    cameraInfo_s* camera = nullptr;
    for(int i=0; i<g_this->m_cameraList.size(); i++){
        if(pAlarmer->lUserID == g_this->m_cameraList.at(i)->lUserID){
            camera = g_this->m_cameraList.at(i);
        }
    }

    emit g_this->showMsg(QString("违法ip【%1】违法代码【%2】").arg(camera->ip).arg(lCommand));

    if(!camera) return;

    switch (lCommand)
    {
        case COMM_ALARM_RULE:                           // 异常行为检测报警信息
        {
            //qDebug() << pAlarmer->sDeviceIP;
            NET_VCA_RULE_ALARM struCIDAlarmInfo = { 0 };
            memcpy(&struCIDAlarmInfo, pAlarmInfo, sizeof(NET_VCA_RULE_ALARM));

            int len = struCIDAlarmInfo.dwPicDataLen;    // 图片数据长度
            QByteArray btImageData;                     // 图片数据
            if(len > 0)
            {
                for(int i=0; i<len; i++){
                    btImageData.append((char)struCIDAlarmInfo.pImage[i]);
                }
                emit g_this->signalIllegalAct(camera->workGroup);
                g_this->saveImage(camera, btImageData);
                emit g_this->signalGetImage(btImageData, camera->ip);
            }
        }
        break;
        case COMM_ITS_PLATE_RESULT:                           // 异常行为检测报警信息
        {
/*            tagNET_VCA_FACESNAP_RESULT struCIDAlarmInfo = { 0 };
            memcpy(&struCIDAlarmInfo, pAlarmInfo, sizeof(tagNET_VCA_FACESNAP_RESULT));

            int len = struCIDAlarmInfo.dwFacePicLen;    // 图片数据长度
            QByteArray btImageData;   */                  // 图片数据

            emit g_this->signalIllegalAct(camera->workGroup);
            //emit g_this->showMsg("违法");

//            //emit g_this->showMsg(QString("len[%1]").arg(len));
//            if(len > 0)
//            {
//                for(int i=0; i<len; i++){
//                    btImageData.append((char)struCIDAlarmInfo.pBuffer1[i]);
//                }
//                emit g_this->signalIllegalAct(camera->workGroup);
//                g_this->saveImage(camera, btImageData);
//                emit g_this->signalGetImage(btImageData, camera->ip);
//            }
        }
        break;
    default:
        emit g_this->showMsg(QString("其他报警，报警信息类型:%1").arg(lCommand));
        break;
    }

}

bool HCBallheadCameraWorker::logInCamera(cameraInfo_s *camera)
{
    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    struLoginInfo.bUseAsynLogin = false;                                //同步登录方式
    struLoginInfo.wPort = camera->port;                                 //设备服务端口
    strcpy_s(struLoginInfo.sDeviceAddress, camera->ip.toLocal8Bit());   //设备IP地址
    strcpy_s(struLoginInfo.sUserName, camera->user.toLocal8Bit());      //设备登录用户名
    strcpy_s(struLoginInfo.sPassword, camera->pwd.toLocal8Bit());       //设备登录密码
//    strcpy_s(struLoginInfo.sDeviceAddress, "192.168.1.64");   //设备IP地址
//    strcpy_s(struLoginInfo.sUserName, "admin");      //设备登录用户名
//    strcpy_s(struLoginInfo.sPassword, "utis123456");       //设备登录密码
    struLoginInfo.byHttps = 2;
    struLoginInfo.byLoginMode = 2;

    camera->lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (camera->lUserID < 0)
    {
        emit showMsg(QString("登陆失败, 错误代码: %1[%2]").arg(NET_DVR_GetLastError()).arg(camera->ip));
        NET_DVR_Cleanup();
        return false;
    }else{
        //设备登录成功
        startAlarm(camera);
        emit showMsg(QString("设备登录成功[%2]").arg(camera->ip));
    }
    return true;

}

bool HCBallheadCameraWorker::startAlarm(cameraInfo_s *camera)
{
    //启用布防
    NET_DVR_SETUPALARM_PARAM  struAlarmParam = { 0 };
    struAlarmParam.dwSize = sizeof(struAlarmParam);
    //不需要设置其他报警布防参数，不支持
    struAlarmParam.byLevel = 1;
    struAlarmParam.byAlarmInfoType = 1;
    struAlarmParam.byRetAlarmTypeV40 = 1;
//    struAlarmParam.byRetDevInfoVersion = 1;
//    struAlarmParam.byRetVQDAlarmType = 0;
    struAlarmParam.byFaceAlarmDetection = 0;
//    struAlarmParam.bySupport = 0;
//    struAlarmParam.byBrokenNetHttp = 0;
//    struAlarmParam.wTaskNo = 0;
    struAlarmParam.byDeployType = 1;
    struAlarmParam.bySubScription = 0XFF;
//    struAlarmParam.byRes1[0] = 0;
//    struAlarmParam.byRes1[1] = 0;
//    struAlarmParam.bySupport = 0;
//    struAlarmParam.byAlarmTypeURL = 0;
//    struAlarmParam.byCustomCtrl = 0;

    // 开始布防
    camera->lHandle = NET_DVR_SetupAlarmChan_V41(camera->lUserID, &struAlarmParam);
        if (camera->lHandle < 0)
        {
        emit showMsg(QString("布防失败, 错误代码: %1[%2]").arg(NET_DVR_GetLastError()).arg(camera->ip));
            return false;
        }
        emit showMsg(QString("布防成功[%1]").arg(camera->ip));
    return true;
}

void HCBallheadCameraWorker::initBackupPath(cameraInfo_s* camera)
{
    camera->backPath = (QString("%1/illegalPics/%2/%3")
                            .arg(QApplication::applicationDirPath())
                            .arg(QDateTime::currentDateTime().toString("yyyyMMdd"))
                            .arg(camera->ip));

    QDir dir = QDir(camera->backPath);
    if(!dir.exists()){
        dir.mkpath(camera->backPath);
    }
}

void HCBallheadCameraWorker::saveImage(cameraInfo_s *camera, QByteArray imageData)
{
    QString filePathName = QString("%1/%2.jpg")
                               .arg(camera->backPath)
                               .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    //emit showMsg(filePathName);
    QImage image;
    image.loadFromData(imageData);
    bool res = image.save(filePathName.toLatin1(), "JPG");
    //emit showMsg(QString("保存图片返回值[%1] %2").arg(res).arg(filePathName));
}
