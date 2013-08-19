TEMPLATE=app
TARGET=nemo-password-manager

QT += dbus
QT -= gui

LIBS += -lpam

# Uncomment the following line for debugging output on stderr
#DEFINES += PASSWORDMANAGER_DEBUG

DEPENDPATH += . ..
INCLUDEPATH += . ..

SOURCES += main.cpp

SOURCES += passwordmanager.cpp
HEADERS += passwordmanager.h

SOURCES += passwordmanager_pam.cpp
HEADERS += passwordmanager_pam.h

SOURCES += passwordmanager_store.cpp
HEADERS += passwordmanager_store.h

SOURCES += passwordmanager_pwgen.cpp
HEADERS += passwordmanager_pwgen.h


# Generated at qmake time by ../password-manager.pro
SOURCES += passwordmanageradaptor.cpp
HEADERS += passwordmanageradaptor.h


# Well-known name for our D-Bus service
DBUS_SERVICE_NAME=org.nemo.passwordmanager


# Installation
service.files = ../dbus/$${DBUS_SERVICE_NAME}.service
service.path = /usr/share/dbus-1/system-services/

conf.files = ../dbus/$${DBUS_SERVICE_NAME}.conf
conf.path = /etc/dbus-1/system.d/

target.path = /usr/bin/

INSTALLS += target service conf
