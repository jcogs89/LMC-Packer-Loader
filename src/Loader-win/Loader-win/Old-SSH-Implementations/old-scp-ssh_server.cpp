#include "ssh_server.h"

/* This is a sample implementation of a libssh based SSH server */
/*
Copyright 2003-2011 Aris Adamantiadis
This file is part of the SSH Library
You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action. It's not a reference on how terminal
clients must be made or how a client should react.
*/

//#include "config.h"

#include <libssh/libssh.h>
#include <libssh/server.h>
#include <libssh/callbacks.h>
#include <string>
#include "sshwraper.h"
#include <windows.h>
#include <stdio.h>
#include <io.h>


#ifdef HAVE_ARGP_H
#include <argp.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <poll.h>
//#include <pty.h>

#define SSHD_USER "libssh"
#define SSHD_PASSWORD "libssh"

#ifndef KEYS_FOLDER
#ifdef _WIN32
#define KEYS_FOLDER
#else
#define KEYS_FOLDER "/etc/ssh/"
#endif
#endif

#include <WS2tcpip.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define write _write
#define open _open
#endif

#ifdef WITH_PCAP
const char* pcap_file = "debug.server.pcap";
ssh_pcap_file pcap;

static void set_pcap(ssh_session session) {
    if (!pcap_file)
        return;
    pcap = ssh_pcap_file_new();
    if (ssh_pcap_file_open(pcap, pcap_file) == SSH_ERROR) {
        printf("Error opening pcap file\n");
        ssh_pcap_file_free(pcap);
        pcap = NULL;
        return;
    }
    ssh_set_pcap_file(session, pcap);
}

static void cleanup_pcap() {
    ssh_pcap_file_free(pcap);
    pcap = NULL;
}
#endif


static int fetch_files(ssh_session session) {
    int size;
    char buffer[16384];
    int mode;
    char* filename;
    int r;
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_READ, "DOWNLOADS");
    printf("scp");
    if (ssh_scp_init(scp) != SSH_OK) {
        fprintf(stderr, "error initializing scp: %s\n", ssh_get_error(session));
        return -1;
    }
    printf("Trying to download 3 files (a,b,d) and 1 directory (c)\n");
    do {

        r = ssh_scp_pull_request(scp);
        switch (r) {
        case SSH_SCP_REQUEST_NEWFILE:
            size = ssh_scp_request_get_size(scp);
            filename = _strdup(ssh_scp_request_get_filename(scp)); //added _ before strdup
            mode = ssh_scp_request_get_permissions(scp);
            printf("downloading file %s, size %d, perms 0%o\n", filename, size, mode);
            free(filename);
            ssh_scp_accept_request(scp);
            r = ssh_scp_read(scp, buffer, sizeof(buffer));
            if (r == SSH_ERROR) {
                fprintf(stderr, "Error reading scp: %s\n", ssh_get_error(session));
                return -1;
            }
            printf("done\n");
            break;
        case SSH_ERROR:
            fprintf(stderr, "Error: %s\n", ssh_get_error(session));
            return -1;
        case SSH_SCP_REQUEST_WARNING:
            fprintf(stderr, "Warning: %s\n", ssh_scp_request_get_warning(scp));
            break;
        case SSH_SCP_REQUEST_NEWDIR:
            filename = _strdup(ssh_scp_request_get_filename(scp)); //added _ before strdup
            mode = ssh_scp_request_get_permissions(scp);
            printf("downloading directory %s, perms 0%o\n", filename, mode);
            free(filename);
            ssh_scp_accept_request(scp);
            break;
        case SSH_SCP_REQUEST_ENDDIR:
            printf("End of directory\n");
            break;
        case SSH_SCP_REQUEST_EOF:
            printf("End of requests\n");
            goto end;
        }
    } while (1);
end:
    return 0;
}

int scp_receive(ssh_session session, ssh_scp scp)
{
    int rc;
    int size, mode;
    char* filename, * buffer;
    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE)
    {
        fprintf(stderr, "Error receiving information about file: %s\n",
            ssh_get_error(session));
        return SSH_ERROR;
    }
    size = ssh_scp_request_get_size(scp);
    filename = _strdup(ssh_scp_request_get_filename(scp)); //changed strdup to _strdup
    mode = ssh_scp_request_get_permissions(scp);
    printf("Receiving file %s, size %d, permissions 0%o\n",
        filename, size, mode);
    free(filename);
    buffer = (char*)malloc(size); // had to add char* CHANGE
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        return SSH_ERROR;
    }
    ssh_scp_accept_request(scp);
    rc = ssh_scp_read(scp, buffer, size);
    if (rc == SSH_ERROR)
    {
        fprintf(stderr, "Error receiving file data: %s\n",
            ssh_get_error(session));
        free(buffer);
        return rc;
    }
    printf("Done\n");
    write(1, buffer, size);
    free(buffer);
    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_EOF)
    {
        fprintf(stderr, "Unexpected request: %s\n",
            ssh_get_error(session));
        return SSH_ERROR;
    }
    return SSH_OK;
}


static int auth_password(const char* user, const char* password) {
    if (strcmp(user, SSHD_USER))
        return 0;
    if (strcmp(password, SSHD_PASSWORD))
        return 0;
    return 1; // authenticated
}
#ifdef HAVE_ARGP_H
const char* argp_program_version = "libssh server example "
SSH_STRINGIFY(LIBSSH_VERSION);
const char* argp_program_bug_address = "<libssh@libssh.org>";

/* Program documentation. */
static char doc[] = "libssh -- a Secure Shell protocol implementation";

/* A description of the arguments we accept. */
static char args_doc[] = "BINDADDR";



/* The options we understand. */
static struct argp_option options[] = {
  {
    .name = "port",
    .key = 'p',
    .arg = "PORT",
    .flags = 0,
    .doc = "Set the port to bind.",
    .group = 0
  },
  {
    .name = "hostkey",
    .key = 'k',
    .arg = "FILE",
    .flags = 0,
    .doc = "Set the host key.",
    .group = 0
  },
  {
    .name = "dsakey",
    .key = 'd',
    .arg = "FILE",
    .flags = 0,
    .doc = "Set the dsa key.",
    .group = 0
  },
  {
    .name = "rsakey",
    .key = 'r',
    .arg = "FILE",
    .flags = 0,
    .doc = "Set the rsa key.",
    .group = 0
  },
  {
    .name = "verbose",
    .key = 'v',
    .arg = NULL,
    .flags = 0,
    .doc = "Get verbose output.",
    .group = 0
  },
  {
          .name = "no-default-keys",
          .key = 'n',
          .arg = NULL,
          .flags = 0,
          .doc = "Do not set default key locations.",
          .group = 0
     },
  {NULL, 0, 0, 0, NULL, 0}
};

/* Parse a single option. */
static error_t parse_opt(int key, char* arg, struct argp_state* state) {
    /* Get the input argument from argp_parse, which we
     * know is a pointer to our arguments structure.
     */
    ssh_bind sshbind = state->input;

    switch (key) {
    case 'p':
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT_STR, arg);
        port = atoi(arg);
        break;
    case 'd':
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, arg);
        break;
    case 'k':
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_HOSTKEY, arg);
        break;
    case 'r':
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, arg);
        break;
    case 'v':
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_LOG_VERBOSITY_STR, "3");
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1) {
            /* Too many arguments. */
            argp_usage(state);
        }
        ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, arg);
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1) {
            /* Not enough arguments. */
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };
#endif /* HAVE_ARGP_H */

static int authenticate(ssh_session session) {
    ssh_message message;

    do {
        message = ssh_message_get(session);
        if (!message)
            break;
        switch (ssh_message_type(message)) {
        case SSH_REQUEST_AUTH:
            switch (ssh_message_subtype(message)) {
            case SSH_AUTH_METHOD_PASSWORD:
                printf("User %s wants to auth with pass %s\n",
                    ssh_message_auth_user(message),
                    ssh_message_auth_password(message));
                if (auth_password(ssh_message_auth_user(message),
                    ssh_message_auth_password(message))) {
                    ssh_message_auth_reply_success(message, 0);
                    ssh_message_free(message);
                    return 1;
                }
                ssh_message_auth_set_methods(message,
                    SSH_AUTH_METHOD_PASSWORD |
                    SSH_AUTH_METHOD_INTERACTIVE);
                // not authenticated, send default message
                ssh_message_reply_default(message);
                break;

            case SSH_AUTH_METHOD_NONE:
            default:
                printf("User %s wants to auth with unknown auth %d\n",
                    ssh_message_auth_user(message),
                    ssh_message_subtype(message));
                ssh_message_auth_set_methods(message,
                    SSH_AUTH_METHOD_PASSWORD |
                    SSH_AUTH_METHOD_INTERACTIVE);
                ssh_message_reply_default(message);
                break;
            }
            break;
        default:
            ssh_message_auth_set_methods(message,
                SSH_AUTH_METHOD_PASSWORD |
                SSH_AUTH_METHOD_INTERACTIVE);
            ssh_message_reply_default(message);
        }
        ssh_message_free(message);
    } while (1);
    return 0;
}


int ssh_server(int portp, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key)
{
    //printf("\nSSH server initiated.");
    int port = portp;
    ssh_session session;
    ssh_bind sshbind;
    ssh_message message;
    ssh_channel chan = 0;
    int auth = 0;
    int shell = 0;
    int r;

    sshbind = ssh_bind_new();
    session = ssh_new();

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);

    //printf("%s",ssh_host_dsa_key.c_str());
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, ssh_host_dsa_key.c_str());
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, ssh_host_rsa_key.c_str());

    //#ifdef HAVE_ARGP_H
        /*
         * Parse our arguments; every option seen by parse_opt will
         * be reflected in arguments.
         */
         //    argp_parse (&argp, argc, argv, 0, 0, sshbind);
         //#else
         //    (void) argc;
         //    (void) argv;
         //#endif
#ifdef WITH_PCAP
    set_pcap(session);
#endif

    if (ssh_bind_listen(sshbind) < 0) {
        printf("Error listening to socket: %s\n", ssh_get_error(sshbind));
        return 1;
    }
    printf("Started sample libssh sshd on port %d\n", port);
    printf("You can login as the user %s with the password %s\n", SSHD_USER,
        SSHD_PASSWORD);
    r = ssh_bind_accept(sshbind, session);
    if (r == SSH_ERROR) {
        printf("Error accepting a connection: %s\n", ssh_get_error(sshbind));
        return 1;
    }
    if (ssh_handle_key_exchange(session)) {
        printf("ssh_handle_key_exchange: %s\n", ssh_get_error(session));
        return 1;
    }

    /* proceed to authentication */
    auth = authenticate(session);
    if (!auth) {
        printf("Authentication error: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        return 1;
    }

    printf("post auth\n");
    /* wait for a channel session */
    do {
        message = ssh_message_get(session);
        if (message) {
            if (ssh_message_type(message) == SSH_REQUEST_CHANNEL_OPEN &&
                ssh_message_subtype(message) == SSH_CHANNEL_SESSION) {
                chan = ssh_message_channel_request_open_reply_accept(message);



                ssh_session chan_session = ssh_channel_get_session(chan);
                ssh_scp scp = ssh_scp_new(chan_session, SSH_SCP_READ, "DOWNLOADS");
                printf("scp allocate blah");
                ssh_scp_init(scp);
                printf("scp init blah");
                scp_receive(chan_session, scp);



                ssh_message_free(message);
                break;
            }
            else {
                ssh_message_reply_default(message);
                ssh_message_free(message);
            }
        }
        else {
            break;
        }
    } while (!chan);

    if (!chan) {
        printf("Error: client did not ask for a channel session (%s)\n",
            ssh_get_error(session));
        ssh_finalize();
        return 1;
    }
    printf("post channel\n");


    ssh_session chan_session = ssh_channel_get_session(chan);
    fetch_files(chan_session);
    printf("post fetch files");
    /*
    ssh_session chan_session = ssh_channel_get_session(chan);
    ssh_scp scp = ssh_scp_new(chan_session, SSH_SCP_READ, "DOWNLOADS");
    printf("scp allocate");
    rc = ssh_scp_init(scp);
    printf("scp init");
    printf("%d", rc);
    scp_receive(chan_session, scp);
    */



    ssh_disconnect(session);
    ssh_bind_free(sshbind);
#ifdef WITH_PCAP
    cleanup_pcap();
#endif
    ssh_finalize();
    return 0;
}
