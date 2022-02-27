QT       += core gui
QT       += serialport
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chart/chart.cpp \
    chart/qcustomplot.cpp \
    chart/serial.cpp \
    main.cpp \
    mainwindow.cpp \
    serial_tool/serial_tool.cpp

HEADERS += \
    chart/chart.h \
    chart/qcustomplot.h \
    chart/serial.h \
    mainwindow.h \
    serial_tool/serial_tool.h

FORMS += \
    chart/chart.ui \
    mainwindow.ui \
    serial_tool/serial_tool.ui

## opencv 中的include文件夹
#INCLUDEPATH+=   D:\opencv3.4.1\opencv\build\include

##opencv 编译后的文件夹中的lib文件夹
#LIBS += D:\opencv3.4.1\opencv\build\x64\vc15\lib\opencv_world341d.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
