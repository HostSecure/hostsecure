QT -= gui
QT += sql mqtt

#CONFIG += c++11 console
CONFIG += c++2a
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

DEPENDPATH += ../messagehandler/lib/include
INCLUDEPATH += ../messagehandler/lib/include
LIBS += -L../messagehandler/lib -lmessagehandler

SOURCES += \
    main.cpp \
    src/databasedatafileparser.cpp \
    src/databasehandler.cpp \
    src/databasemanager.cpp \
    src/databasemqttclient.cpp \
    src/loghandler.cpp \
    src/testhandler.cpp

HEADERS += \
    src/databasedatafileparser.h \
    src/databasehandler.h \
    src/databasemanager.h \
    src/databasemqttclient.h \
    src/loghandler.h \
    src/testhandler.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


