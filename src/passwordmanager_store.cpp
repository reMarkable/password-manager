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


#include "passwordmanager_store.h"

#include <QFile>

#define PASSWORDMANAGER_STATEFILE "/etc/nemo-password-manager.conf"
#define PASSWORDMANAGER_STATEFILE_TMP PASSWORDMANAGER_STATEFILE ".tmp"


PasswordManagerStore::PasswordManagerStore()
    : password("")
{
    load();
}

PasswordManagerStore::~PasswordManagerStore()
{
}

QString
PasswordManagerStore::get()
{
    return password;
}

bool
PasswordManagerStore::set(const QString &password)
{
    // Allow remote logins by starting sshd
    if (system("systemctl start sshd.socket") != 0) {
        qWarning() << "Could not start sshd.socket";
    }

    this->password = password;
    return save();
}

bool
PasswordManagerStore::isLoginEnabled()
{
    // Password is enabled if the state file exists
    QFile file(PASSWORDMANAGER_STATEFILE);
    return file.exists();
}

void
PasswordManagerStore::disableLogin()
{
    // Remove password file to disable login
    QFile file(PASSWORDMANAGER_STATEFILE);
    if (file.exists()) {
        file.remove();
    }

    // Prevent remote logins by stopping sshd
    if (system("systemctl stop sshd.socket") != 0) {
        qWarning() << "Could not stop sshd.socket";
    }
}

bool
PasswordManagerStore::load()
{
    QFile file(PASSWORDMANAGER_STATEFILE);

    if (!file.exists()) {
        // File does not exist (yet)
        password = "";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        // Could not open file for reading
        password = "";
        return false;
    }

    QByteArray utf8 = file.readAll();
    if (!utf8.isEmpty()) {
        // Successfully read the password
        password = QString::fromUtf8(utf8);
    }

    file.close();
    return true;
}

bool
PasswordManagerStore::save()
{
    QByteArray utf8 = password.toUtf8();

    QFile tmp(PASSWORDMANAGER_STATEFILE_TMP);

    if (tmp.exists()) {
        // Remove existing temporary file
        tmp.remove();
    }

    if (!tmp.open(QIODevice::WriteOnly)) {
        // Could not open file for writing
        return false;
    }

    tmp.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    qint64 written = tmp.write(utf8);
    tmp.close();

    if (written != utf8.size()) {
        // Not all bytes were written - remove the file
        tmp.remove();
        return false;
    }

    if (QFile::exists(PASSWORDMANAGER_STATEFILE)) {
        QFile::remove(PASSWORDMANAGER_STATEFILE);
    }

    if (!tmp.rename(PASSWORDMANAGER_STATEFILE)) {
        // Could not rename temporary file
        tmp.remove();
        return false;
    }

    return true;
}
