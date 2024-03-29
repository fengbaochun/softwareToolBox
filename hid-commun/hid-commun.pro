QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    usbHid.cpp

HEADERS += \
    MainWindow.h \
    hidapi-win/include/hidapi.h \
    usbHid.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lhidapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lhidapi

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/hidapi-win/x64/ -lhidapi

INCLUDEPATH += $$PWD/hidapi-win/x64
DEPENDPATH += $$PWD/hidapi-win/x64
