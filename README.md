# Hostsecure
HostSecure is a data intensive security management platform for intrusion 
detection and prevention for industrial systems.

This installation guide is based on debian linux.

## Download dependencies
- [Qt 6.1.3](https://www.qt.io/download)

Optional
- [Mosquitto](https://mosquitto.org/download/)
- [MQTT-explorer](http://mqtt-explorer.com/)


# Build guide
To build the project we first need to build QtMqtt from source since this is only included in Qt for Automation 

## QtMqtt
- git clone https://github.com/qt/qtmqtt.git
- cd qtmqtt
- git checkout --track origin/6.1.3
- mkdir build
- cd build
- PATH_TO_QT_COMPILER/bin/qmake .. 
- make 
- make install

# TODO
Add how to start mosquitto server and how to use MQTT-explorer