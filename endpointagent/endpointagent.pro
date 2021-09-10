QT -= gui
QT += dbus network mqtt

CONFIG += c++17 console
CONFIG -= app_bundle

#DBUS_ADAPTORS += DBusInterface.xml
#DBUS_INTERFACES += DBusInterface.xml
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

DEPENDPATH += ../messagehandler/lib/include
INCLUDEPATH += ../messagehandler/lib/include
LIBS += -L../messagehandler/lib -lmessagehandler

SOURCES += \
    main.cpp \
    src/detector.cpp \
    src/parser.cpp \
    src/rulehandler.cpp \
    src/servicehandler.cpp \
    src/uplinkhandler.cpp

HEADERS += \
    src/detector.h \
    src/parser.h \
    src/rulehandler.h \
    src/servicehandler.h \
    src/uplinkhandler.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




