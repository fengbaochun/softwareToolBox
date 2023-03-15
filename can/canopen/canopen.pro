include(canopen.pri)


TEMPLATE = lib
TARGET = canopen

DEFINES += CANOPEN_BUILD
CONFIG += $${CANOPEN_LIBRARY_TYPE} create_prl
VERSION = $${CANOPEN_VERSION}

#depend libs
CANOPEN_DEPENDPATH = $${CANOPEN_DIRPATH}/depend_libs
CANOPEN_NEEDLIBS = -lControlCAN
LIBS += -L$${CANOPEN_DEPENDPATH} $${CANOPEN_NEEDLIBS}

INCLUDEPATH += $${CANOPEN_INCLUDEPATH}

# build dir
BuildDir =build_$$QT_VERSION

DESTDIR = $${CANOPEN_LIBPATH}
CONFIG(debug, debug|release) {
    OBJECTS_DIR = $$OUT_PWD/debug/$$BuildDir/.obj
    MOC_DIR = $$OUT_PWD/debug/$$BuildDir/.moc
    RCC_DIR = $$OUT_PWD/debug/$$BuildDir/.rcc
    UI_DIR = $$OUT_PWD/debug/$$BuildDir/.ui
} else {
    OBJECTS_DIR = $$OUT_PWD/release/$$BuildDir/.obj
    MOC_DIR = $$OUT_PWD/release/$$BuildDir/.moc
    RCC_DIR = $$OUT_PWD/release/$$BuildDir/.rcc
    UI_DIR = $$OUT_PWD/release/$$BuildDir/.ui
}

HEADERS += \
    $$PWD/applicfg.h \
    $$PWD/can.h \
    $$PWD/can_driver.h \
    $$PWD/canfestival.h \
    $$PWD/data.h \
    $$PWD/dcf.h \
    $$PWD/def.h \
    $$PWD/emcy.h \
    $$PWD/lifegrd.h \
    $$PWD/lss.h \
    $$PWD/nmtMaster.h \
    $$PWD/nmtSlave.h \
    $$PWD/objacces.h \
    $$PWD/objdictdef.h \
    $$PWD/pdo.h \
    $$PWD/sdo.h \
    $$PWD/states.h \
    $$PWD/sync.h \
    $$PWD/sysdep.h \
    $$PWD/timer.h \
    $$PWD/timers_driver.h \
    $$PWD/timerscfg.h \
    $$PWD/ControlCAN.H \
    $$PWD/config.h \
    $$PWD/master.h

SOURCES += \
    $$PWD/canfestival.c \
    $$PWD/dcf.c \
    $$PWD/emcy.c \
    $$PWD/lifegrd.c \
    $$PWD/lss.c \
    $$PWD/nmtMaster.c \
    $$PWD/nmtSlave.c \
    $$PWD/objacces.c \
    $$PWD/pdo.c \
    $$PWD/sdo.c \
    $$PWD/states.c \
    $$PWD/sync.c \
    $$PWD/timer.c \
    $$PWD/timers_win32.c \
    $$PWD/master.c \
    $$PWD/symbols.c


DISTFILES +=


