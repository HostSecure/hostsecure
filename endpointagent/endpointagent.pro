QT -= gui
QT += dbus network mqtt

CONFIG += c++2a
CONFIG -= app_bundle

#DBUS_ADAPTORS += DBusInterface.xml
#DBUS_INTERFACES += DBusInterface.xml

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




