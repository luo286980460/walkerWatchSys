#include "widget.h"
#include "ui_widget.h"
#include "workgroup.h"

#include <QMenu>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("行人过节危险预警提示系统_2024.04.16");
    setFixedSize(size());

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    initSystemTray();   // 初始化系统托盘
    initGroups();
    initTimerSec();
}

void Widget::initGroups()
{
    m_group1 = new WorkGroup(1, this);
    connect(m_group1, &WorkGroup::showMsg, this,&Widget::showMsg);
    connect(m_group1, &WorkGroup::signalGetImage, this,&Widget::slotGetImage);
    connect(m_group1, &WorkGroup::signalUpdateIps, this,&Widget::slotUpdateIps);
    m_group1->init();


    m_group2 = new WorkGroup(2, this);
    connect(m_group2, &WorkGroup::showMsg, this,&Widget::showMsg);
    connect(m_group2, &WorkGroup::signalGetImage, this,&Widget::slotGetImage);
    connect(m_group2, &WorkGroup::signalUpdateIps, this,&Widget::slotUpdateIps);
    m_group2->init();
}

void Widget::initTimerSec()
{
    m_timerSec = new QTimer;
    connect(m_timerSec, &QTimer::timeout, this, [=](){
        //qDebug() << QTime::currentTime().toString("hh:mm:ss");
        if(QTime::currentTime().toString("hh:mm:ss") == "00:00:00"){
            //initBackupPath();
            if(m_group1) emit m_group1->signalUpDateBackUpPathDir();
            if(m_group2) emit m_group1->signalUpDateBackUpPathDir();

            clsOverTimeIllegalPics();
        }
    });
    m_timerSec->setInterval(1000);
    m_timerSec->start();
}

void Widget::clsOverTimeIllegalPics()
{
    if(m_group1){
        m_group1->clsOverTimeIllegalPics();
    }else if(m_group2){
        m_group2->clsOverTimeIllegalPics();
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible())
    {
        hide(); //隐藏窗口
        event->ignore(); //忽略事件
    }
}

void Widget::initSystemTray()
{
    //托盘初始化
        QIcon icon = QIcon(":/icon.png");
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(icon);
        trayIcon->setToolTip("江苏尤特智能科技有限公司");
        trayIcon->show(); //必须调用，否则托盘图标不显示

        //创建菜单项动作(以下动作只对windows有效)
        minimizeAction = new QAction("最小化~", this);
        connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide())); //绑定信号槽

        maximizeAction = new QAction("最大化~", this);
        connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

        restoreAction = new QAction("还原~", this);
        connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

        quitAction = new QAction("退出~", this);
        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用，qApp对应的是程序全局唯一指针

        //创建托盘菜单(必须先创建动作，后添加菜单项，还可以加入菜单项图标美化)
        trayIconMenu = new QMenu(this);
        trayIconMenu->addAction(minimizeAction);
        trayIconMenu->addAction(maximizeAction);
        trayIconMenu->addAction(restoreAction);
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(quitAction);
        trayIcon->setContextMenu(trayIconMenu);


        connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    //trayIcon->showMessage("title","你单击了"); //后面两个默认参数
    show();
    break;
    case QSystemTrayIcon::DoubleClick:
    show();
    break;
    case QSystemTrayIcon::MiddleClick:
    trayIcon->showMessage("title","你中键了");
    break;
    default:
    break;
    }
}

void Widget::slotUpdateIps(QString ip0, QString ip1, QString ip2, QString ip3)
{
    if(ip0 != "-1") {
        ui->ipLabel0->setText(ip0);
        m_ip0 = ip0;
    }
    if(ip1 != "-1") {
        ui->ipLabel1->setText(ip1);
        m_ip1 = ip1;
    }
    if(ip2 != "-1") {
        ui->ipLabel2->setText(ip2);
        m_ip2 = ip2;
    }
    if(ip3 != "-1") {
        ui->ipLabel3->setText(ip3);
        m_ip3 = ip3;
    }
}

void Widget::showMsg(QString msg)
{
    ui->historyMsg->appendPlainText(msg);
}

void Widget::slotGetImage(QByteArray imagedata, QString ip)
{

    //showMsg(QString("imagedata = %1").arg(imagedata.size()));
    QImage image;
    image.loadFromData(imagedata);

    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.scaled(ui->imageLabel0->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel* imageLabel = nullptr;

    if(ip == m_ip0){
        imageLabel = ui->imageLabel0;
    }else if(ip == m_ip1){
        imageLabel = ui->imageLabel1;
    }else if(ip == m_ip2){
        imageLabel = ui->imageLabel2;
    }else if(ip == m_ip3){
        imageLabel = ui->imageLabel3;
    }else{
        imageLabel = nullptr;
    }
    if(imageLabel){
        imageLabel->setScaledContents(true);
        imageLabel->setPixmap(QPixmap::fromImage(image));
    }
}

void Widget::on_clearBtn_clicked()
{
    ui->historyMsg->clear();
}


void Widget::on_testBtn_clicked()
{
    m_group1->test();
    m_group2->test();
}

