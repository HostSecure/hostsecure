QT -= gui
QT += mqtt
TARGET = messagehandler

TEMPLATE = lib
CONFIG += staticlib c++17

INCLUDEPATH += include

SOURCES += \
    src/MqttInterface.cpp \
    src/DatabaseHandler.cpp \
    src/EdgeHandler.cpp \
    src/MmiHandler.cpp \

HEADERS += \
    include/MqttInterface.h \
    include/DatabaseHandler.h \
    include/EdgeHandler.h \
    include/MmiHandler.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
