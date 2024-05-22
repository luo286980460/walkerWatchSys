#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTimer>

class WorkGroup;
class E_StakeVer0;
class groupManager;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void closeEvent(QCloseEvent* event);        // 重写关闭按钮事件

private:
    void init();
    void initGroupManager();
    void initTimerSec();
    void clsOverTimeIllegalPics();  //  删除超过保存时间的违法图片

private slots:
    void showMsg(QString msg);
    void slotGetImage(QByteArray imagedata, QString ip);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);   // 托盘点击响应槽
    void slotUpdateIps(QString ip0, QString ip1, QString ip2, QString ip3);

    void on_clearBtn_clicked();

    void on_testBtn_clicked();

private:
    Ui::Widget *ui;
    groupManager* m_groupManager = nullptr;
    WorkGroup* m_group1 = nullptr;
    WorkGroup* m_group2 = nullptr;
    QString m_ip0;
    QString m_ip1;
    QString m_ip2;
    QString m_ip3;
    QTimer* m_timerSec;             // 秒钟定时器
    QString m_dayTimeStart;                   // 白天开始
    QString m_dayTimeStop;                    // 白天结束

private:
    // 最小化到系统托盘
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    //窗口管理动作
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    void initSystemTray();                      // 初始化系统托盘
};
#endif // WIDGET_H
