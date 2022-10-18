INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/ControlCAN.h\
    $$PWD/can.h \
    $$PWD/commun.h \
    $$PWD/serial.h

SOURCES += \
    $$PWD/can.cpp \
    $$PWD/commun.cpp \
    $$PWD/serial.cpp

QT       += serialport

win32: LIBS += -L$$PWD/./ -lControlCAN



