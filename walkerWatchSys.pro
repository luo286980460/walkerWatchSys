QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    YuanHongBox.cpp \
    YuanHongBoxWorker.cpp \
    e_stakever0.cpp \
    e_stakever0worker.cpp \
    groupmanager.cpp \
    groupmanagerworker.cpp \
    hcballheadcamera.cpp \
    hcballheadcameraworker.cpp \
    hccolumnspeaker.cpp \
    hccolumnspeakerworker.cpp \
    main.cpp \
    novacontroller.cpp \
    novacontrollerworker.cpp \
    widget.cpp

HEADERS += \
    NovaHeader.h \
    YuanHongBox.h \
    YuanHongBoxWorker.h \
    e_stakever0.h \
    e_stakever0worker.h \
    groupmanager.h \
    groupmanagerworker.h \
    hcballheadcamera.h \
    hcballheadcameraworker.h \
    hccolumnspeaker.h \
    hccolumnspeakerworker.h \
    novacontroller.h \
    novacontrollerworker.h \
    speakerHeader.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/HCCamera/ -lHCCore

INCLUDEPATH += $$PWD/lib/HCCamera
DEPENDPATH += $$PWD/lib/HCCamera

win32: LIBS += -L$$PWD/lib/HCCamera/ -lHCNetSDK

INCLUDEPATH += $$PWD/lib/HCCamera
DEPENDPATH += $$PWD/lib/HCCamera

win32: LIBS += -L$$PWD/lib/HCCamera/ -lGdiPlus

INCLUDEPATH += $$PWD/lib/HCCamera
DEPENDPATH += $$PWD/lib/HCCamera

win32: LIBS += -L$$PWD/lib/HCCamera/ -lPlayCtrl

INCLUDEPATH += $$PWD/lib/HCCamera
DEPENDPATH += $$PWD/lib/HCCamera

win32: LIBS += -L$$PWD/lib/HCCamera/HCNetSDKCom/ -lHCAlarm

INCLUDEPATH += $$PWD/lib/HCCamera/HCNetSDKCom
DEPENDPATH += $$PWD/lib/HCCamera/HCNetSDKCom

win32: LIBS += -L$$PWD/lib/HCCamera/HCNetSDKCom/ -lHCGeneralCfgMgr

INCLUDEPATH += $$PWD/lib/HCCamera/HCNetSDKCom
DEPENDPATH += $$PWD/lib/HCCamera/HCNetSDKCom

win32: LIBS += -L$$PWD/lib/HCCamera/HCNetSDKCom/ -lHCPreview

INCLUDEPATH += $$PWD/lib/HCCamera/HCNetSDKCom
DEPENDPATH += $$PWD/lib/HCCamera/HCNetSDKCom

win32: LIBS += -L$$PWD/lib/Nova/ -lNovaSDK

INCLUDEPATH += $$PWD/lib/Nova
DEPENDPATH += $$PWD/lib/Nova

RESOURCES += \
    res.qrc

win32: LIBS += -L$$PWD/lib/libhv/ -lhv

INCLUDEPATH += $$PWD/lib/libhv
DEPENDPATH += $$PWD/lib/libhv
