QT += quick mqtt

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

DEPENDPATH += ../messagehandler/lib/include
INCLUDEPATH += ../messagehandler/lib/include
LIBS += -L../messagehandler/lib -lmessagehandler

SOURCES += \
    main.cpp \
    src/datastore.cpp \
    src/hostsecurecollection.cpp \
    # Models
    src/edgemodel.cpp \
    src/usbdevicemodel.cpp \
    # Common
    src/commonutils.cpp

HEADERS += \
    src/datastore.h \
    src/hostsecurecollection.h \
    # Models
    src/edgemodel.h \
    src/usbdevicemodel.h \
    # Common
    src/commonutils.h

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


