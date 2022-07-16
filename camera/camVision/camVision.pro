QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    camera.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# opencv 中的include文件夹
INCLUDEPATH+=   D:\opencv4.5.3\qt_new_build\install\include
##opencv 编译后的文件夹中的lib文件夹
LIBS += D:\opencv4.5.3\qt_new_build\install\x64\mingw\lib\libopencv_*.dll.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
