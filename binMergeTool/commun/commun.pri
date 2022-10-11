INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/ControlCAN.h\
    $$PWD/can.h \
    $$PWD/commun.h

SOURCES += \
    $$PWD/can.cpp \
    $$PWD/commun.cpp

win32: LIBS += -L$$PWD/./ -lControlCAN



