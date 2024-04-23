#include "hccolumnspeakerworker.h"
#include "speakerHeader.h"
#include <QJsonDocument>
#include <QJsonObject>

HCColumnSpeakerWorker::HCColumnSpeakerWorker(QString ip, QString HCSpeakerContent, int HCSpeakerTimes,
                                             int HCSpeakerVolume, QString HCSpeakerPlayMode,
                                             int DHSpeakerId, int SpeakerType, QObject *parent)
    : QObject{parent}
    , m_ip(ip)
    , m_port(34508)
    , m_pwd("Aa123456")
    , m_HCSpeakerContent(HCSpeakerContent)
    , m_HCSpeakerTimes(HCSpeakerTimes)
    , m_HCSpeakerVolume(HCSpeakerVolume)
    , m_HCSpeakerPlayMode(HCSpeakerPlayMode)
    , m_DHSpeakerId(DHSpeakerId)
    , m_SpeakerType(SpeakerType)
{

}

void HCColumnSpeakerWorker::init()
{
    m_HCColumnSpeaker = new QTcpSocket(this);

    connect(m_HCColumnSpeaker, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState state){
        switch(state){
        case QAbstractSocket::ConnectedState:
            emit showMsg(QString("音柱连接成功![%1]").arg(m_ip));
            setSpeakerVolume(m_HCSpeakerVolume);
            break;
        case QAbstractSocket::ConnectingState:
            emit showMsg(QString("音柱正在连接![%1]").arg(m_ip));
            break;
        case QAbstractSocket::UnconnectedState:
            emit showMsg(QString("音柱已断连，准备重连![%1]").arg(m_ip));
            m_connectTimes++;
            connectColumnSpeaker();
            break;
        case QAbstractSocket::HostLookupState:
        case QAbstractSocket::BoundState:
        case QAbstractSocket::ListeningState:
        case QAbstractSocket::ClosingState:
            break;
        }
    });

    connect(m_HCColumnSpeaker, &QTcpSocket::readyRead, this, &HCColumnSpeakerWorker::slotReadyRead);


    if(m_HCColumnSpeaker->open(QIODevice::ReadWrite)){
        connectColumnSpeaker();
    }else{
        m_HCColumnSpeaker->deleteLater();
        m_HCColumnSpeaker = nullptr;

        emit showMsg("m_HCColumnSpeaker: 类打开失败");
    }
}

void HCColumnSpeakerWorker::connectColumnSpeaker()
{
    if(m_connectTimes == 3) {
        emit showMsg(QString("音柱连接第[%1]次，放弃连接[%2]").arg(m_connectTimes).arg(m_ip));
        m_connectTimes = 0;
        return;
    }else{
        emit showMsg(QString("音柱连接第[%1]次，尝试连接[%2]").arg(m_connectTimes).arg(m_ip));
    }
    QVariant value;
    if(m_HCColumnSpeaker){
        m_HCColumnSpeaker->connectToHost(m_ip, m_port);
        m_HCColumnSpeaker->setSocketOption(QAbstractSocket::KeepAliveOption, value);
    }
}

void HCColumnSpeakerWorker::initTime()
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [=](){
        if(m_timeFlag < 3){
            m_timeFlag++;
            //qDebug() << "m_timeFlag: " << m_timeFlag;

            if(m_timeFlag == 3){
                m_canCmd = true;
                m_timer->stop();
            }
        }
    });

}

void HCColumnSpeakerWorker::setSpeakerVolume(int volume)
{
    if(volume < 2 || volume > 7) volume = 5;
    QString cmd = QString(CMD_SET_VOLUME).arg(volume);
    if(m_HCColumnSpeaker){
        m_HCColumnSpeaker->write(cmd.toLocal8Bit());
        m_HCColumnSpeaker->waitForBytesWritten();
        m_canCmd = false;
        m_timer->start();
    }
}

void HCColumnSpeakerWorker::slotInit()
{
    if(m_SpeakerType == 0){
        init();
        initTime();
    }
}

void HCColumnSpeakerWorker::slotReadyRead()
{
    if(m_HCColumnSpeaker){
        emit showMsg(QString("%1[%2]").arg(QString(m_HCColumnSpeaker->readAll())).arg(m_ip));
    }
}

void HCColumnSpeakerWorker::slotCmdPlayMp3NameType(QString name, int Type, int times)
{
    if(m_canCmd){
        QString cmd = QString(CMD_PLAY_MP3_NAME_TYPE).arg(name).arg(Type).arg(times);
        emit showMsg(QString("%1---[%2]").arg(cmd).arg(m_ip));

        if(m_HCColumnSpeaker && (m_HCColumnSpeaker->state() == QTcpSocket::ConnectedState)){
            m_HCColumnSpeaker->write(cmd.toLocal8Bit());
            m_HCColumnSpeaker->waitForBytesWritten();
        }else{
            emit showMsg(QString("音柱连接出现问题![%1]").arg(m_ip));
            if(m_HCColumnSpeaker)
                m_HCColumnSpeaker->connectToHost(m_ip, m_port);
        }
        m_canCmd = false;
        m_timeFlag = 0;
        m_timer->start();
    }
}

void HCColumnSpeakerWorker::slotCmdPlayTTS(QString name, int Type, int times)
{
    if(m_canCmd){
        QString cmd = QString(CMD_PLAY_TTS_NAME_TYPE).arg(QString2Hex(name)).arg(Type).arg(times);
        emit showMsg(QString("%1---[%2]").arg(cmd).arg(m_ip));

        if(m_HCColumnSpeaker && (m_HCColumnSpeaker->state() == QTcpSocket::ConnectedState)){
            m_HCColumnSpeaker->write(cmd.toLocal8Bit());
            m_HCColumnSpeaker->waitForBytesWritten();
        }else{
            emit showMsg(QString("音柱连接出现问题![%1]").arg(m_ip));
            if(m_HCColumnSpeaker)
                m_HCColumnSpeaker->connectToHost(m_ip, m_port);
        }

        m_canCmd = false;
        m_timeFlag = 0;
        m_timer->start();
    }
}

void HCColumnSpeakerWorker::slotCmdStopPlay()
{
    if(m_canCmd){
        QString cmd = CMD_STOP_PLAY;
        emit showMsg(QString("%1---[%2]").arg(cmd).arg(m_ip));

        if(m_HCColumnSpeaker && (m_HCColumnSpeaker->state() == QTcpSocket::ConnectedState)){
            m_HCColumnSpeaker->write(cmd.toLocal8Bit());
            m_HCColumnSpeaker->waitForBytesWritten();
        }else{
            emit showMsg(QString("音柱连接出现问题![%1]").arg(m_ip));
            m_HCColumnSpeaker->connectToHost(m_ip, m_port);
        }

        m_canCmd = false;
        m_timeFlag = 0;
        m_timer->start();
    }
}

void HCColumnSpeakerWorker::slotCmdSetVolume(int volume)
{
    if(m_canCmd){
        if(volume < 1 || volume > 7) {
            emit showMsg("音量值错误，范围：1~7");
            return;
        }
        QString cmd = QString(CMD_SET_VOLUME).arg(volume);
        emit showMsg(QString("%1---[%2]").arg(cmd).arg(m_ip));

        if(m_HCColumnSpeaker && (m_HCColumnSpeaker->state() == QTcpSocket::ConnectedState)){
            m_HCColumnSpeaker->write(cmd.toLocal8Bit());
            m_HCColumnSpeaker->waitForBytesWritten();
        }else{
            emit showMsg(QString("音柱连接出现问题![%1]").arg(m_ip));
            m_HCColumnSpeaker->connectToHost(m_ip, m_port);
        }

        m_canCmd = false;
        m_timeFlag = 0;
        m_timer->start();
    }
}

QString HCColumnSpeakerWorker::QString2Hex(QString str)
{
    QByteArray byteArray = str.toUtf8();    // 将文本转换为字节数组
    QString hexString = byteArray.toHex();  // 将字节数组转换为十六进制字符串
    return hexString;
}

void HCColumnSpeakerWorker::slotIllegalAct()
{
    qDebug() << QString("音柱违法:%1m_HCSpeakerPlayMode[%2]").arg(m_ip).arg(m_HCSpeakerPlayMode);

    if(m_HCSpeakerPlayMode == "TTS"){
        if(m_SpeakerType == 0){
            slotCmdPlayTTS(m_HCSpeakerContent, 1, m_HCSpeakerTimes); //横穿危险立即返回
        }else if(m_SpeakerType == 1){
            DaHuaTTS();
        }else{
            return;
        }
    }else if(m_HCSpeakerPlayMode == "MP3"){
        if(m_SpeakerType == 0){
            slotCmdPlayMp3NameType("1", 1, m_HCSpeakerTimes);
        }else if(m_SpeakerType == 1){
            DaHuaMP3();
        }else{
            return;
        }
    } else{
        return;
    }
}

void HCColumnSpeakerWorker::slotIllegalActOver()
{
    void stopPlay();
}

void HCColumnSpeakerWorker::stopPlay()
{
    // 待实现
}

void HCColumnSpeakerWorker::DaHuaTTS()
{
    emit showMsg(QString("http://%1/php/exeRealPlayFile.php").arg(m_ip));
    m_request.setUrl(QUrl(QString("http://%1/php/exeRealPlayFile.php").arg(m_ip)));
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // 详情见接口文件 2.27
    m_postData.addQueryItem("jsondata[rtype]", "startbct");     // startbct(开始广播)stopbct(停止广播)getstatus(获取状态)filecontrol(播放控制)
    m_postData.addQueryItem("jsondata[param1]", QString::number(m_DHSpeakerId));           // 设备id
    m_postData.addQueryItem("jsondata[param2]", "3");           // 0:文件  1:终端  2:声卡 3:文本
    m_postData.addQueryItem("jsondata[param4]", m_HCSpeakerContent);     // 播放内容
    m_postData.addQueryItem("jsondata[param6]", "0");           // 持续时间,当持续时间大于文件时间时,将循环播放【默认为】(单位:秒)
    m_postData.addQueryItem("jsondata[param7]",                 // 广播文本播放次数（音源类型为“3”时有效）
                            QString::number(m_HCSpeakerTimes));
    m_postData.addQueryItem("jsondata[param8]",                 // 文本速度【-10 到 10，默认为 0，正常语速】 文字音量（0 到1.0）示例：0,1.0,0.4 背景音乐音量（0 到 1.0）
                            QString("0,%1,0.0").arg(m_HCSpeakerVolume/10.0, 0, 'f', 1));
    m_postData.addQueryItem("jsondata[extdata]", "back");       // 信息原封不动返回

    m_reply = m_manager.post(m_request, m_postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(m_reply, &QNetworkReply::readyRead, [&](){
        if (m_reply->error() == QNetworkReply::NoError) {
            QByteArray data = m_reply->readAll();
            // 处理从服务器获取的数据
            qDebug() << QJsonDocument::fromJson(data).object();
        } else {
            // 处理错误
            qDebug() << "Error: " << m_reply->errorString();
        }
        m_reply->deleteLater();

    });

}

void HCColumnSpeakerWorker::DaHuaMP3()
{
    emit showMsg(QString("http://%1/php/exeRealPlayFile.php").arg(m_ip));
    m_request.setUrl(QUrl(QString("http://%1/php/exeRealPlayFile.php").arg(m_ip)));
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // 详情见接口文件 2.27
    m_postData.addQueryItem("jsondata[rtype]", "startbct");     // startbct(开始广播)stopbct(停止广播)getstatus(获取状态)filecontrol(播放控制)
    m_postData.addQueryItem("jsondata[param1]", QString::number(m_DHSpeakerId));           // 设备id
    m_postData.addQueryItem("jsondata[param2]", "0");           // 0:文件  1:终端  2:声卡 3:文本
    m_postData.addQueryItem("jsondata[param5]", m_HCSpeakerContent);     // 播放内容
    m_postData.addQueryItem("jsondata[param6]", QString::number(m_HCSpeakerTimes));           // 持续时间,当持续时间大于文件时间时,将循环播放【默认为】(单位:秒)
    m_postData.addQueryItem("jsondata[param8]",                 // 文本速度【-10 到 10，默认为 0，正常语速】 文字音量（0 到1.0）示例：0,1.0,0.4 背景音乐音量（0 到 1.0）
                            QString("0,%1,0.0").arg(m_HCSpeakerVolume/10.0, 0, 'f', 1));
    m_postData.addQueryItem("jsondata[extdata]", "back");       // 信息原封不动返回

    m_reply = m_manager.post(m_request, m_postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(m_reply, &QNetworkReply::readyRead, [&](){
        if (m_reply->error() == QNetworkReply::NoError) {
            QByteArray data = m_reply->readAll();
            // 处理从服务器获取的数据
            qDebug() << QJsonDocument::fromJson(data).object();
        } else {
            // 处理错误
            qDebug() << "Error: " << m_reply->errorString();
        }
        m_reply->deleteLater();

    });

}
