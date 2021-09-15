QT += quick mqtt

CONFIG += c++2a

INCLUDEPATH += src

DEPENDPATH += ../messagehandler/lib/include
INCLUDEPATH += ../messagehandler/lib/include
LIBS += -L../messagehandler/lib -lmessagehandler

SOURCES += \
    main.cpp \
    src/hostsecurecollection.cpp \
    src/edgemodel.cpp \
    src/devicemodel.cpp \
    src/mmimqttclient.cpp

HEADERS += \
    src/hostsecurecollection.h \
    src/edgemodel.h \
    src/devicemodel.h \
    src/mmimqttclient.h

RESOURCES += \
    qml/main/qml.qrc \
    qml/Common/Common.qrc \
    qml/Images/Images.qrc \
    qml/IconTheme/IconTheme.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


