INCLUDEPATH += $$PWD
DEFINES += QS_LOG_LINE_NUMBERS    # 为每个日志消息自动写入文件和行
#DEFINES += QS_LOG_DISABLE         # 日志代码被替换为no-op
DEFINES += QS_LOG_SEPARATE_THREAD # 消息将从单独的线程排队和写入
SOURCES += $$PWD/QsLogDest.cpp \
    $$PWD/QsLog.cpp \
    $$PWD/QsLogDestConsole.cpp \
    $$PWD/QsLogDestFile.cpp \
    $$PWD/QsLogDestFunctor.cpp

HEADERS += $$PWD/QsLogDest.h \
    $$PWD/QsLog.h \
    $$PWD/QsLogDestConsole.h \
    $$PWD/QsLogLevel.h \
    $$PWD/QsLogDestFile.h \
    $$PWD/QsLogDisableForThisFile.h \
    $$PWD/QsLogDestFunctor.h

OTHER_FILES += \
    $$PWD/QsLogChanges.txt \
    $$PWD/QsLogReadme.txt \
    $$PWD/LICENSE.txt
