/*
 * sshwraper.h
 *
 *  Created on: Mar 16, 2020
 *      Author: cbai
 */
#include <string>
#define LIBSSH_STATIC 1
#include <libssh/libssh.h>

int verify_knownhost(ssh_session session);
int connect(char *ip);
int rec(ssh_session session, ssh_scp scp);
int direct_forwarding(ssh_session session, const char ip_remote[20]);
