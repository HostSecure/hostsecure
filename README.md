# HostSecure Intrusion Detection System
HostSecure IDS is a data-intensive security management platform for intrusion detection and prevention for industrial systems. It is a lightweight, scalable and a service-oriented software solution with a centralized management platform, along with a decentralized publish-subscribe messaging architecture for connectivity and network communication. 

Keywords: Host Intrusion Detection System, Centralized Management, Cloud and Edge Computing, MQTT, Relational Databases, Service-Oriented Design, Publish-Subscribe Architecture, Message-Oriented Middleware

This installation guide is based on Debian OS (Linux). 

## Prerequisites
- [Qt 6.1.3](https://www.qt.io/download)
- [Mosquitto](https://mosquitto.org/download/) (Optional)
- [MQTT-explorer](http://mqtt-explorer.com/) (Optional)

## Build project
Open HostSecure.pro in QtCreator and choose which application you want to build. 

To build this project you first need to build QtMqtt from source, since this is only included in Qt for Automation.

### QtMqtt
```
git clone --branch 6.1.3 https://github.com/qt/qtmqtt
cd qtmqtt
mkdir build
cd build
PATH_TO_QT_COMPILER/bin/qmake .. 
make 
make install
```
You may need to use `sudo make` if `make` fails.

## Running Mosqitto MQTT server on host
After installing Mosquitto you just need to start the service.
```
# To start the service
sudo service mosquitto start

# To stop the service
sudo service mosquitto stop
```

## USBGuard
As an example security service, the USBGuard USB Whitelisting mechanism was chosen as a Security Service integrated with the Endpoint Agent
- [Usbguard](https://usbguard.github.io/)
- [Compilation & Installation](https://usbguard.github.io/documentation/compilation.html)

For this project, release version 1.0.0 was used. Remember to configure the service correct according to the instruction, otherwise you might block all your computer USB devices.

Alternatively, the USBGuard can be downloaded using common Linux packet managers, such as DNF/YUM (Fedora/CentOS/Redhat) or APT/APT-Get (Ubuntu). Our project used the USBGuard RPM (Version 1.0.0) package from the official Fedora Linux repository.
```
# To install USBGuard on Fedora Linux
sudo dnf install usbguard

# To generate a usb device policy
sudo usbguard generate-policy > /etc/usbguard/rules.conf

# To start the service
sudo systemctl start usbguard
sudo systemctl start usbguard-dbus

# To stop the service
sudo systemctl stop usbguard
sudo systemctl stop usbguard-dbus

# To verify the status of the service
sudo systemctl status usbguard
sudo systemctl status usbguard-dbus
```

## Database setup
By default, the log, database, and database config files directory is set to be the directory above where the executable can be found. To change this location, set the environment `HOSTSECURE_DATA_DIR` to the wanted directory.

To populate the productvendor table: Add a file `HOSTSECURE_DATA_DIR/productvendors.txt` with each line consisting of one productvendor pair like so: `"<productid>,<productname>,<vendorid>,<vendorname>"`.
To find the product and vendor IDs, run the application, attach a device, and check the MMI as the MMI will show the combined ID as `vendor:product`.

To populate the virushash tale: Add a file `HOSTSECURE_DATA_DIR/virushashes.txt` with each line consisting of one virushash like so `"<virushash>,<description>"`.
