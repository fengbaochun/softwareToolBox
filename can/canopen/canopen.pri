
HEADERS += \
    $$PWD/canopen/canopen.h \
    $$PWD/dict/TestMaser.h \
    $$PWD/dict/TestSlave.h \
    $$PWD/drive/win_qt/ControlCAN.H \
    $$PWD/drive/win_qt/applicfg.h \
    $$PWD/drive/win_qt/can.h \
    $$PWD/drive/win_qt/can_driver.h \
    $$PWD/drive/win_qt/canfestival.h \
    $$PWD/drive/win_qt/config.h \
    $$PWD/drive/win_qt/timers_driver.h \
    $$PWD/drive/win_qt/timerscfg.h \
    $$PWD/inc/data.h \
    $$PWD/inc/dcf.h \
    $$PWD/inc/def.h \
    $$PWD/inc/emcy.h \
    $$PWD/inc/lifegrd.h \
    $$PWD/inc/lss.h \
    $$PWD/inc/nmtMaster.h \
    $$PWD/inc/nmtSlave.h \
    $$PWD/inc/objacces.h \
    $$PWD/inc/objdictdef.h \
    $$PWD/inc/pdo.h \
    $$PWD/inc/sdo.h \
    $$PWD/inc/states.h \
    $$PWD/inc/sync.h \
    $$PWD/inc/sysdep.h \
    $$PWD/inc/timer.h

SOURCES += \
    $$PWD/canopen/canopen.cpp \
    $$PWD/dict/TestMaser.c \
    $$PWD/dict/TestSlave.c \
    $$PWD/drive/win_qt/canfestival.c \
    $$PWD/drive/win_qt/timers_win32.c \
    $$PWD/src/dcf.c \
    $$PWD/src/emcy.c \
    $$PWD/src/lifegrd.c \
    $$PWD/src/lss.c \
    $$PWD/src/nmtMaster.c \
    $$PWD/src/nmtSlave.c \
    $$PWD/src/objacces.c \
    $$PWD/src/pdo.c \
    $$PWD/src/sdo.c \
    $$PWD/src/states.c \
    $$PWD/src/symbols.c \
    $$PWD/src/sync.c \
    $$PWD/src/timer.c


LIBS += -L$$PWD/drive/win_qt/ -lControlCAN
