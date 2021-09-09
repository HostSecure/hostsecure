QT -= gui
QT += mqtt

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    Program.cpp \
    TestMmiHandler.cpp

HEADERS += \
    TestMmiHandler.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


