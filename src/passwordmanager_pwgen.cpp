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


#include "passwordmanager_pwgen.h"

#include <QtGlobal>
#include <stdio.h>

/* Length of a generated password */
#define PASSWORD_LENGTH 10

QString
PasswordManagerPwGen::generate()
{
    static const char CHARS[] = "acefhjkmnsuvwxyz3579";
    unsigned char password[PASSWORD_LENGTH];

    FILE *fp = fopen("/dev/urandom", "rb");
    size_t result = fread(password, sizeof(password), 1, fp);
    fclose(fp);

    if (result != 1) {
        return QString();
    }

    for (int i=0; i<sizeof(password); i++) {
        password[i] = CHARS[password[i] % (sizeof(CHARS)-1)];
    }
    password[sizeof(password)-1] = '\0';

    return QString((const char *)(password));
}
