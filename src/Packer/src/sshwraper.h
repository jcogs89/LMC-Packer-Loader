/*
 * sshwraper.h
 *
 *  Created on: Mar 16, 2020
 *      Author: cbai
 */
#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
int verify_knownhost(ssh_session session);

int connect();
