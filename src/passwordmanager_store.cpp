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

void
PasswordManagerStore::set(const QString &password)
{
    this->password = password;
    save();
}

void
PasswordManagerStore::load()
{
    // TODO: retrieve password
}

void
PasswordManagerStore::save()
{
    // TODO: store password
}
