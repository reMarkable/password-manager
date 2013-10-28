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


#ifndef ORG_NEMO_PASSWORDMANAGER_H
#define ORG_NEMO_PASSWORDMANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDBusContext>

#include "passwordmanager_store.h"

class PasswordManager : public QObject, protected QDBusContext {
    Q_OBJECT

    public:
        PasswordManager(QObject *parent=NULL);
        virtual ~PasswordManager();

    public slots:
        void generatePassword();
        QString getGeneratedPassword();
        void setPassword(const QString &password);
        bool isLoginEnabled();
        void quit();

    signals:
        void passwordChanged(const QString &password);
        void loginEnabledChanged(bool enabled);
        void error(const QString &message);

    private:
        static const char *SERVICE_NAME;
        static const char *OBJECT_PATH;

    private:
        bool isPrivileged();

    private:
        PasswordManagerStore store;
        QTimer autoclose;
};

#endif /* ORG_NEMO_PASSWORDMANAGER_H */
