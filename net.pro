CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
LIBS +=-lpthread
SOURCES += \
    InputCmdLinux.cpp \
    main.cpp \
    YYDebug.cpp \
    Gwd376Linux.cpp \
    BackUp.cpp

HEADERS += \
    clientmsg.h \
    Gwd376Linux.h \
    server.h \
    YYDebug.h

