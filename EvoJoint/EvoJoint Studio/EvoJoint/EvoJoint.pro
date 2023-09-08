QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include($$PWD/plot)
#include($$PWD/plot/plot.pri)

include($$PWD/motor)
include($$PWD/page)


SOURCES += \
    main.cpp \
    MainWindow.cpp \
    motor/ctl.cpp \
    motor/uProtocol.cpp \
    page/ctlPage.cpp \
    page/devCfgPage.cpp \
    qcustomplot.cpp \
    widgetplot2d.cpp

HEADERS += \
    MainWindow.h \
    motor/ctl.h \
    motor/uProtocol.h \
    page/ctlPage.h \
    page/devCfgPage.h \
    qcustomplot.h \
    widgetplot2d.h

FORMS += \
    MainWindow.ui \
    page/ctlPage.ui \
    page/devCfgPage.ui \
    widgetplot2d.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
