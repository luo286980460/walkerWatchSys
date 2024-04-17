#include "widget.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 防止程序多次启动
    QSharedMemory sharedMemory("MySharedMemory");
    if (sharedMemory.attach()) {
        return 1;
    }
    if (!sharedMemory.create(1)) {
        return 1;
    }

    Widget w;
    w.show();
    return a.exec();
}
