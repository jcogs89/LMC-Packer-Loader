/*
 * sshwraper.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: cbai
 */
//requires libssh
//www.libssh.org
//use your package manager or build it for windows
#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
#include "sshwraper.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

//to do

int verify_knownhost(ssh_session session)
{
	printf("\nverify host\n");
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    int cmp;
    int rc;

    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return -1;
    }

    rc = ssh_get_publickey_hash(srv_pubkey,
                                SSH_PUBLICKEY_HASH_SHA1,
                                &hash,
                                &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return -1;
    }

    state = ssh_session_is_known_server(session);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */

            break;
        case SSH_KNOWN_HOSTS_CHANGED:
            printf( "Host key for server changed: it is now:\n");
            ssh_print_hexa("Public key hash", hash, hlen);
            printf( "For security reasons, connection will be stopped\n");
            ssh_clean_pubkey_hash(&hash);

            return -1;
        case SSH_KNOWN_HOSTS_OTHER:
            printf( "The host key for this server was not found but an other"
                    "type of key exists.\n");
            printf( "An attacker might change the default server key to"
                    "confuse your client into thinking the key does not exist\n");
            ssh_clean_pubkey_hash(&hash);

            return -1;
        case SSH_KNOWN_HOSTS_NOT_FOUND:
            printf( "Could not find known host file.\n");
            printf( "If you accept the host key here, the file will be"
                    "automatically created.\n");
            break;

            /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */

        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            printf("The server is unknown. Do you trust the host key? (yes)\n");
            printf( "Public key hash: %s\n>>>", hexa);
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            scanf("%9s",buf);

            cmp = strncasecmp(buf, "yes", 3);
            if (cmp != 0)
            {
            	printf("\ndid not add host %i\n",cmp);
            	fflush(stdin);
            	//this is is ------------------------------------------------------------------------------------
            	cin.clear();
            	cin.ignore(numeric_limits<streamsize>::max(),'\n');
            	// ^those lines
                return -1;
            }


            rc = ssh_session_update_known_hosts(session);
            if (rc < 0) {
                fprintf(stderr, "Error %s\n", strerror(errno));
                return -1;
            }

            break;
        case SSH_KNOWN_HOSTS_ERROR:
            fprintf(stderr, "Error %s", ssh_get_error(session));
            ssh_clean_pubkey_hash(&hash);
            return -1;
    }

    ssh_clean_pubkey_hash(&hash);
    return 0;
}

int connect()
{
	  int port = 22;
	  char usern[] = {'c','b','a','i'};
	  ssh_session my_ssh_session;
	  int rc;
	  printf("\n1\n");
	  my_ssh_session = ssh_new();
	  if (my_ssh_session == NULL)
	  {
		  printf("\nfailed to create new ssh session\n");
	    return(-1);
	  }
	  //												V ip to connect to -----------------------------
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, &usern);

	  rc = ssh_connect(my_ssh_session);
	  if (rc != SSH_OK)
	  {
	    printf( "Error connecting to localhost: %s\n",
	            ssh_get_error(my_ssh_session));
	    ssh_free(my_ssh_session);
	    return(-1);
	  }

	  if (verify_knownhost(my_ssh_session) < 0)
	  {
	    ssh_disconnect(my_ssh_session);
	    ssh_free(my_ssh_session);
	    exit(-1);
	  }

	  char *password;
	  password = getpass("Password: ");
	  rc = ssh_userauth_password(my_ssh_session, NULL, password);
	  if (rc != SSH_AUTH_SUCCESS)
	  {
	    fprintf(stderr, "Error authenticating with password: %s\n",
	            ssh_get_error(my_ssh_session));
	    ssh_disconnect(my_ssh_session);
	    ssh_free(my_ssh_session);
	    exit(-1);
	  }
	  printf("\nconnected\n");
	  //ToDo PRIORITY 1 - send payload
	  //https://api.libssh.org/stable/libssh_tutor_forwarding.html

	  ssh_disconnect(my_ssh_session);
	  ssh_free(my_ssh_session);
	  return(1);
}
