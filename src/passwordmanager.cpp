/**
 * Nemo Password Manager: D-Bus Service for changing and generating passwords
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **/


#include "passwordmanager.h"
#include "passwordmanageradaptor.h"
#include "passwordmanager_pam.h"
#include "passwordmanager_pwgen.h"

#include <QDBusConnection>


const char *
PasswordManager::SERVICE_NAME = "org.nemo.passwordmanager";

const char *
PasswordManager::OBJECT_PATH = "/org/nemo/passwordmanager";


PasswordManager::PasswordManager(QObject *parent)
    : QObject(parent)
    , store()
{
    QDBusConnection connection = QDBusConnection::systemBus();
    if (!connection.registerService(SERVICE_NAME)) {
        qFatal("Cannot register D-Bus service at %s", SERVICE_NAME);
    }

    if (!connection.registerObject(OBJECT_PATH, this)) {
        qFatal("Cannot register object at %s", OBJECT_PATH);
    }

    new PasswordManagerAdaptor(this);
}

PasswordManager::~PasswordManager()
{
}

void
PasswordManager::generatePassword()
{
    bool loginEnabled = isLoginEnabled();
    QString password = PasswordManagerPwGen::generate();

    QString message;
    if (PasswordManagerPAM::set(password, &message)) {
        if (!(store.set(password))) {
            emit error("Could not save password");
        }
        emit passwordChanged(password);
        if (!loginEnabled) {
            emit loginEnabledChanged(true);
        }
    } else {
        emit error(message);
    }
}

QString
PasswordManager::getGeneratedPassword()
{
    return store.get();
}

void
PasswordManager::setPassword(const QString &password)
{
    bool loginEnabled = isLoginEnabled();

    QString message;
    if (PasswordManagerPAM::set(password, &message)) {
        if (password == "") {
            store.disableLogin();
        } else {
            store.set("");
        }
        emit passwordChanged("");
        if (!loginEnabled && password != "") {
            emit loginEnabledChanged(true);
        } else if (loginEnabled && password == "") {
            emit loginEnabledChanged(false);
        }
    } else {
        emit error(message);
    }
}

bool
PasswordManager::isLoginEnabled()
{
    return store.isLoginEnabled();
}

void
PasswordManager::quit()
{
    QCoreApplication::quit();
}
