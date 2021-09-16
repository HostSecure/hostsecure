QT -= gui
QT += mqtt
TARGET = messagehandler

TEMPLATE = lib
CONFIG += staticlib c++2a

# If release build
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

INCLUDEPATH += include

SOURCES += \
    src/mqttclientbase.cpp \
    src/msg/msgedge.cpp \
    src/msg/msgdevice.cpp

HEADERS += \
    include/mqttclientbase.h \
    include/msg/msgbase.h \
    include/msg/msgedge.h \
    include/msg/msgdevice.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
