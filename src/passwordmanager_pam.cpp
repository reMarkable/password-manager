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


#include "passwordmanager_pam.h"

#include <security/pam_appl.h>
#include <stdio.h>

#ifdef PASSWORDMANAGER_DEBUG
#  define PW_MGR_PAM_DEBUG(msg, ...) fprintf(stderr, msg "\n", ## __VA_ARGS__)
#else
#  define PW_MGR_PAM_DEBUG(msg, ...)
#endif /* PASSWORDMANAGER_DEBUG */

static int
pam_conversation_func(int num_msg, const struct pam_message **msg,
        struct pam_response **resp, void *appdata_ptr)
{
    const char *password = static_cast<const char *>(appdata_ptr);
    struct pam_response *reply = static_cast<struct pam_response *>
        (calloc(num_msg, sizeof(struct pam_response)));

    for (int i=0; i<num_msg; i++) {
        PW_MGR_PAM_DEBUG("<-- %s", msg[i]->msg);
        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_ON:
            case PAM_PROMPT_ECHO_OFF:
                reply[i].resp = strdup(password);
                PW_MGR_PAM_DEBUG("--> %s", reply[i].resp);
                break;
            default:
                break;
        }
    }

    *resp = reply;

    return PAM_SUCCESS;
}

bool
PasswordManagerPAM::set(const QString &password, QString *error)
{
    // TODO: get username from UID_MIN in /etc/login.defs
    return passwd("nemo", password, error);
}

bool
PasswordManagerPAM::passwd(const QString &username, const QString &password, QString *error)
{
    QByteArray user = username.toLocal8Bit();
    QByteArray pass = password.toLocal8Bit();

    struct pam_conv conversation = {
        pam_conversation_func,
        static_cast<void *>(pass.data()),
    };

    pam_handle_t *handle = NULL;
    int result;

    PW_MGR_PAM_DEBUG("pam_start()");
    result = pam_start("passwd", user.constData(), &conversation, &handle);
    if (result != PAM_SUCCESS) {
        if (error) {
            *error = QString("pam_start returned %1").arg(result);
        }
        return false;
    }

    PW_MGR_PAM_DEBUG("pam_chauthtok()");
    result = pam_chauthtok(handle, 0);
    if (result != PAM_SUCCESS) {
        if (error) {
            *error = QString("pam_chauthtok returned %1").arg(result);
        }
        return false;
    }

    PW_MGR_PAM_DEBUG("pam_end()");
    result = pam_end(handle, result);
    if (result != PAM_SUCCESS) {
        if (error) {
            *error = QString("pam_end returned %1").arg(result);
        }
        return false;
    }

    return true;
}
