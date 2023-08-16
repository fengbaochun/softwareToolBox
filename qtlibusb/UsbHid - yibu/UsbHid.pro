QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UsbHid.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    UsbHid.h \
    libusb.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -llibusb-1.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -llibusb-1.0
else:unix:!macx: LIBS += -L$$PWD/./ -llibusb-1.0

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.




unix:!macx|win32: LIBS += -L$$PWD/./ -lusb

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
