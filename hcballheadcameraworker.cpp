#include "hcballheadcameraworker.h"
#include <QImage>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QDebug>

HCBallheadCameraWorker* g_this0 = nullptr;
HCBallheadCameraWorker* g_this1 = nullptr;
HCBallheadCameraWorker* g_this2 = nullptr;
HCBallheadCameraWorker* g_this3 = nullptr;

HCBallheadCameraWorker::HCBallheadCameraWorker(QString ip, int HCCameraIndex, QObject *parent)
    : QObject{parent}
    , m_ip(ip)
    , m_port(8000)
    , m_user("admin")
    , m_pwd("utis123456")
    , m_HCCameraIndex(HCCameraIndex)
{

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

QString HCBallheadCameraWorker::getIp()
{
    return m_ip;
}

bool HCBallheadCameraWorker::initSdk()
{
    // 初始化 SDK
    BOOL res = NET_DVR_Init();
    if(res != 1){
        emit showMsg(QString("sdk初始化错误：res = %1").arg(res));
        return false;
    }

    //将路径QString转为void*格式
    QByteArray byteArray = m_ip.toLatin1();
    char* charArray = byteArray.data();
    void* voidPath = static_cast<void*>(charArray);

//    emit showMsg(QString("m_HCCameraIndex+++%1").arg(m_HCCameraIndex));

    //设置报警回调函数
    switch(m_HCCameraIndex){
    case 0:
        NET_DVR_SetDVRMessageCallBack_V50(m_HCCameraIndex, MessageCallback0, voidPath);
        //emit showMsg(QString("**************m_HCCameraIndex=%1").arg(m_HCCameraIndex));
        break;
    case 1:
        NET_DVR_SetDVRMessageCallBack_V50(m_HCCameraIndex, MessageCallback1, voidPath);
        //emit showMsg(QString("**************m_HCCameraIndex=%1").arg(m_HCCameraIndex));
        break;
    case 2:
        NET_DVR_SetDVRMessageCallBack_V50(m_HCCameraIndex, MessageCallback2, voidPath);
        //emit showMsg(QString("**************m_HCCameraIndex=%1").arg(m_HCCameraIndex));
        break;
    case 3:
        NET_DVR_SetDVRMessageCallBack_V50(m_HCCameraIndex, MessageCallback3, voidPath);
        //emit showMsg(QString("**************m_HCCameraIndex=%1").arg(m_HCCameraIndex));

//        emit showMsg(QString("gthis0->getIp()****%1").arg(g_this0->getIp()));
//        emit showMsg(QString("gthis1->getIp()****%1").arg(g_this1->getIp()));
//        emit showMsg(QString("gthis2->getIp()****%1").arg(g_this2->getIp()));
//        emit showMsg(QString("gthis3->getIp()****%1").arg(g_this3->getIp()));

        break;
    default:
        break;
    }

//    NET_DVR_SetDVRMessageCallBack_V50(m_HCCameraIndex, MessageCallback, c);
//    emit showMsg(QString("**************m_HCCameraIndex=%1").arg(m_HCCameraIndex));

    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    return true;
}

bool HCBallheadCameraWorker::logInCamera(QString ip, int port, QString user, QString pwd)
{
    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

    struLoginInfo.bUseAsynLogin = false;                          //同步登录方式
    struLoginInfo.wPort = m_port;                                 //设备服务端口
    strcpy_s(struLoginInfo.sDeviceAddress, m_ip.toLocal8Bit());   //设备IP地址
    strcpy_s(struLoginInfo.sUserName, m_user.toLocal8Bit());      //设备登录用户名
    strcpy_s(struLoginInfo.sPassword, m_pwd.toLocal8Bit());       //设备登录密码
    struLoginInfo.byHttps = 2;
    struLoginInfo.byLoginMode = 2;

    m_lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (m_lUserID < 0)
    {
        emit showMsg(QString("登陆失败, 错误代码: %1[%2]").arg(NET_DVR_GetLastError()).arg(m_ip));
        NET_DVR_Cleanup();
        return false;
    }else{
        //设备登录成功
        emit showMsg(QString("设备登录成功[%2]").arg(m_ip));
        return true;
    }

}

bool HCBallheadCameraWorker::startAlarm()
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

    m_lHandle = NET_DVR_SetupAlarmChan_V41(m_lUserID, &struAlarmParam);
    if (m_lHandle < 0)
    {
        //printf("NET_DVR_SetupAlarmChan_V41 failed, error code:  %d\n", NET_DVR_GetLastError());
        emit showMsg(QString("布防失败, 错误代码: %1[%2]").arg(NET_DVR_GetLastError()).arg(m_ip));
        NET_DVR_Logout(m_lUserID);
        NET_DVR_Cleanup();
        return false;
    }
    emit showMsg(QString("布防成功[%1]").arg(m_ip));
    return true;
}

void HCBallheadCameraWorker::initBackupPath()
{
    //QString dataTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    m_backUpPathDir = (QString("%1/illegalPics/%2/%3")
                           .arg(QApplication::applicationDirPath())
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd"))
                           .arg(m_ip));

    QDir dir = QDir(m_backUpPathDir);
    if(!dir.exists()){
        dir.mkpath(m_backUpPathDir);
    }

    //qDebug() << QString("%1/%2.jpg").arg(m_backUpPathDir).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
}

void HCBallheadCameraWorker::saveImage(QByteArray imageData) {
    //qDebug() << QString("saveImage: %1 %2").arg(m_ip).arg(m_HCCameraIndex);
    QString filePathName = QString("%1/%2.jpg")
                               .arg(m_backUpPathDir)
                               .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QImage image;
    image.loadFromData(imageData);
    //image.save((QApplication::applicationDirPath() + "/illegalPics/" + FILENAME).toLocal8Bit());
    bool res = image.save(filePathName.toLatin1(), "JPG");
    emit showMsg(QString("保存图片返回值[%1] %2").arg(res).arg(filePathName));
    //qDebug() << "filePathName;" << filePathName;
}

void HCBallheadCameraWorker::slotInit()
{
    initBackupPath();
    initSdk();
    logInCamera(m_ip, m_port, m_user, m_pwd);
    startAlarm();

    //g_this g_this = this;
    switch(m_HCCameraIndex){
    case 0:
        g_this0 = this;
        break;
    case 1:
        g_this1 = this;
        break;
    case 2:
        g_this2 = this;
        break;
    case 3:
        g_this3 = this;
        /*
        showMsg(QString("**************构造：%1").arg(QString::number((unsigned int)g_this0,16)));
        showMsg(QString("**************构造：%1").arg(QString::number((unsigned int)g_this1,16)));
        showMsg(QString("**************构造：%1").arg(QString::number((unsigned int)g_this2,16)));
        showMsg(QString("**************构造：%1").arg(QString::number((unsigned int)g_this3,16)));*/
        break;
    default:
        break;
    }
}

void HCBallheadCameraWorker::slotUpDateBackUpPathDir()
{
    //qDebug() << "slotUpDateBackUpPathDir";
    initBackupPath();
}

void HCBallheadCameraWorker::MessageCallback0(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
    Q_UNUSED(pAlarmer);
    Q_UNUSED(dwBufLen);
    Q_UNUSED(pUser);

    if(QString(pAlarmer->sDeviceIP) == g_this0->getIp()) {
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
                    emit g_this0->signalIllegalAct();
                    g_this0->saveImage(btImageData);
                    emit g_this0->signalGetImage(btImageData, pAlarmer->sDeviceIP);
                }
            }
            break;
        default:
            emit g_this0->showMsg(QString("其他报警，报警信息类型:%1").arg(lCommand));
            break;
        }
    }
}

void HCBallheadCameraWorker::MessageCallback1(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
    Q_UNUSED(pAlarmer);
    Q_UNUSED(dwBufLen);
    Q_UNUSED(pUser);

    if(QString(pAlarmer->sDeviceIP) == g_this1->getIp()) {
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
                    emit g_this1->signalIllegalAct();
                    //qDebug() << g_this1->getIp();
                    g_this1->saveImage(btImageData);
                    emit g_this1->signalGetImage(btImageData, pAlarmer->sDeviceIP);
            }
        }
        break;
        default:
            emit g_this1->showMsg(QString("其他报警，报警信息类型:%1").arg(lCommand));
            break;
        }
    }
}

void HCBallheadCameraWorker::MessageCallback2(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
    Q_UNUSED(pAlarmer);
    Q_UNUSED(dwBufLen);
    Q_UNUSED(pUser);

    //char* charPath = static_cast<char*>(pUser);


    //qDebug() << (QString("**************构造：%1").arg(QString::number((unsigned int)g_this2,16)));
    //QString msg = QString("call back -----  ip: %1   ------MessageCallback0: %3").arg(g_this2->getIp()).arg("MessageCallback2");
    //emit g_this2->showMsg(msg);
    //emit g_this->signalGetImage(btImageData, g_this->getIp());

    if(QString(pAlarmer->sDeviceIP) == g_this2->getIp()) {
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
                    emit g_this2->signalIllegalAct();
                    //qDebug() << g_this2->getIp();
                    g_this2->saveImage(btImageData);
                    emit g_this2->signalGetImage(btImageData, pAlarmer->sDeviceIP);
            }
        }
        break;
        default:
            emit g_this2->showMsg(QString("其他报警，报警信息类型:%1").arg(lCommand));
            break;
        }
    }
}

void HCBallheadCameraWorker::MessageCallback3(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser)
{
    Q_UNUSED(pAlarmer);
    Q_UNUSED(dwBufLen);
    Q_UNUSED(pUser);
    //char* charPath = static_cast<char*>(pUser);

    if(QString(pAlarmer->sDeviceIP) == g_this3->getIp()) {
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
                    emit g_this3->signalIllegalAct();
                    //qDebug() << g_this3->getIp();
                    g_this3->saveImage(btImageData);
                    emit g_this3->signalGetImage(btImageData, pAlarmer->sDeviceIP);
            }
        }
        break;
        default:
            emit g_this3->showMsg(QString("其他报警，报警信息类型:%1").arg(lCommand));
            break;
        }
    }
}
