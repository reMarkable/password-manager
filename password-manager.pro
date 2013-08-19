TEMPLATE = subdirs
SUBDIRS = src

system(qdbusxml2cpp \
    -c PasswordManagerAdaptor \
    -a src/passwordmanageradaptor.h:src/passwordmanageradaptor.cpp \
    dbus/org.nemo.passwordmanager.xml)
