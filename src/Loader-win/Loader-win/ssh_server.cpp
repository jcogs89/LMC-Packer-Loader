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

int scp_rec(ssh_session session, ssh_scp scp)
{
    int rc;
    int size1, mode;
    char* filename, * buffer;

    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE)
    {
        fprintf(stderr, "Error receiving information about file: %s\n",
            ssh_get_error(session));
        return SSH_ERROR;
    }

    size1 = ssh_scp_request_get_size(scp);
    filename = _strdup(ssh_scp_request_get_filename(scp));
    mode = ssh_scp_request_get_permissions(scp);
    printf("Receiving file %s, size %d, permissions 0%o\n",
        filename, size1, mode);
    free(filename);

    buffer = (char*)malloc(size1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        return SSH_ERROR;
    }

    ssh_scp_accept_request(scp);
    rc = ssh_scp_read(scp, buffer, size1);
    if (rc == SSH_ERROR)
    {
        fprintf(stderr, "Error receiving file data: %s\n",
            ssh_get_error(session));
        free(buffer);
        return rc;
    }
    printf("Done\n");

    send(1, buffer, size1, 0); //ToDo no idea what 0 is
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

static int copy_fd_to_chan(socket_t fd, int revents, void* userdata) {
    ssh_channel chan = (ssh_channel)userdata;
    char buf[2048];
    int sz = 0;

    if (!chan) {
        closesocket(fd);
        return -1;
    }
    if (revents & POLLIN) {
        sz = recv(fd, buf, 2048, 0); //ToDo what is 0?
        if (sz > 0) {
            ssh_channel_write(chan, buf, sz);
        }
    }
    if (revents & POLLHUP) {
        ssh_channel_close(chan);
        sz = -1;
    }
    return sz;
}

static int copy_chan_to_fd(ssh_session session,
    ssh_channel channel,
    void* data,
    uint32_t len,
    int is_stderr,
    void* userdata) {
    int fd = *(int*)userdata;
    int sz;
    (void)session;
    (void)channel;
    (void)is_stderr;

    char* ptemp = (char*)data; //ToDo required char * not VOID *, not sure if this fucks shit by changing it.
    sz = send(fd, ptemp, len, 0); //ToDo what is 0?
    return sz;
}

static void chan_close(ssh_session session, ssh_channel channel, void* userdata) {
    int fd = *(int*)userdata;
    (void)session;
    (void)channel;

    closesocket(fd);
}

struct ssh_channel_callbacks_struct cb = {
    .userdata = NULL,
    .channel_data_function = copy_chan_to_fd,
    .channel_eof_function = chan_close,
    .channel_close_function = chan_close,

};

static int main_loop(ssh_channel chan)
{
    printf("main loop\n");
    ssh_session session = ssh_channel_get_session(chan);
    socket_t fd;
    struct termios* term = NULL;
    struct winsize* win = NULL;
    //pid_t childpid;
    ssh_event event;
    short events;

    printf("mainloop2 auth\n");
    // ToDo removed cuz broken.
    //childpid = forkpty(&fd, NULL, term, win);
    //if (childpid == 0) {
    //    execl("/bin/bash", "/bin/bash", (char*)NULL);
    //    abort();
    //}
    printf("main loop3\n");
    cb.userdata = &fd;
    ssh_callbacks_init(&cb);
    ssh_set_channel_callbacks(chan, &cb);
    printf("main loop4\n");
    events = POLLIN | POLLPRI | POLLERR | POLLHUP | POLLNVAL;
    printf("main loop4.5\n");
    event = ssh_event_new();
    printf("main loop5\n");
    if (event == NULL) {
        printf("Couldn't get a event\n");
        return -1;
    }
    if (ssh_event_add_fd(event, fd, events, copy_fd_to_chan, chan) != SSH_OK) {
        printf("Couldn't add an fd to the event\n");
        return -1;
    }
    if (ssh_event_add_session(event, session) != SSH_OK) {
        printf("Couldn't add the session to the event\n");
        return -1;
    }
    printf("stuff\n");
    //here
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_READ, "DOWNLOADS");
    //ssh_scp_init(scp);
    //printf("stuff\n");
    rec(session, scp);
    printf("main loop6\n");
    do {
        ssh_event_dopoll(event, 1000);
    } while (!ssh_channel_is_closed(chan));
    printf("main loop7\n");

    ssh_event_remove_fd(event, fd);

    ssh_event_remove_session(event, session);

    ssh_event_free(event);
    printf("end main\n");
    return 0;
}

int ssh_server_alt(int portp, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key)
{
    ssh_session session;
    ssh_bind sshbind;
    ssh_message message;
    ssh_channel chan = 0;
    char buf[2048];
    int auth = 0;
    int sftp = 0;
    int i;
    int r;
    int port = portp;

    sshbind = ssh_bind_new();
    session = ssh_new();

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);

    //printf("%s",ssh_host_dsa_key.c_str());
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, ssh_host_dsa_key.c_str());
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, ssh_host_rsa_key.c_str());
    printf("main loop\n");

#ifdef WITH_PCAP
    set_pcap(session);
#endif
    printf("main loop\n");
    if (ssh_bind_listen(sshbind) < 0) {
        printf("Error listening to socket: %s\n", ssh_get_error(sshbind));
        return 1;
    }
    r = ssh_bind_accept(sshbind, session);
    if (r == SSH_ERROR) {
        printf("error accepting a connection : %s\n", ssh_get_error(sshbind));
        return 1;
    }
    if (ssh_handle_key_exchange(session)) {
        printf("ssh_handle_key_exchange: %s\n", ssh_get_error(session));
        return 1;
    }
    printf("main loop\n");
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
                    auth = 1;
                    ssh_message_auth_reply_success(message, 0);
                    break;
                }
                // not authenticated, send default message
            case SSH_AUTH_METHOD_NONE:
            default:
                ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_PASSWORD);
                ssh_message_reply_default(message);
                break;
            }
            break;
        default:
            ssh_message_reply_default(message);
        }
        ssh_message_free(message);
    } while (!auth);
    if (!auth) {
        printf("auth error: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        return 1;
    }
    do {
        message = ssh_message_get(session);
        if (message) {
            switch (ssh_message_type(message)) {
            case SSH_REQUEST_CHANNEL_OPEN:
                if (ssh_message_subtype(message) == SSH_CHANNEL_SESSION) {
                    chan = ssh_message_channel_request_open_reply_accept(message);
                    break;
                }
            default:
                ssh_message_reply_default(message);
            }
            ssh_message_free(message);
        }
    } while (message && !chan);
    if (!chan) {
        printf("error : %s\n", ssh_get_error(session));
        ssh_finalize();
        return 1;
    }
    do {
        message = ssh_message_get(session);
        if (message && ssh_message_type(message) == SSH_REQUEST_CHANNEL &&
            ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL) {
            //            if(!strcmp(ssh_message_channel_request_subsystem(message),"sftp")){
            sftp = 1;
            ssh_message_channel_request_reply_success(message);
            break;
            //           }
        }
        if (!sftp) {
            ssh_message_reply_default(message);
        }
        ssh_message_free(message);
    } while (message && !sftp);
    if (!sftp) {
        printf("error : %s\n", ssh_get_error(session));
        return 1;
    }
    printf("it works !\n");
    //gets to here
    do {
        i = ssh_channel_read(chan, buf, 2048, 0);
        if (i > 0) {
            ssh_channel_write(chan, buf, i);
            if (write(1, buf, i) < 0) {
                printf("error writing to buffer\n");
                return 1;
            }
        }
    } while (i > 0);
    ssh_disconnect(session);
    ssh_bind_free(sshbind);
#ifdef WITH_PCAP
    cleanup_pcap();
#endif
    ssh_finalize();
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


    //ssh_scp scp = ssh_scp_new(session, SSH_SCP_READ, "DOWNLOADS");
    //ssh_scp_init(scp);
    //scp_rec(session, scp);
    printf("post auth\n");
    /* wait for a channel session */
    do {
        message = ssh_message_get(session);
        if (message) {
            if (ssh_message_type(message) == SSH_REQUEST_CHANNEL_OPEN &&
                ssh_message_subtype(message) == SSH_CHANNEL_SESSION) {
                chan = ssh_message_channel_request_open_reply_accept(message);
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
        printf("Error: cleint did not ask for a channel session (%s)\n",
            ssh_get_error(session));
        ssh_finalize();
        return 1;
    }
    printf("post channel\n");

    /* wait for a shell */
    do {
        message = ssh_message_get(session);
        if (message != NULL) {
            if (ssh_message_type(message) == SSH_REQUEST_CHANNEL) {
                if (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL) {
                    shell = 1;
                    ssh_message_channel_request_reply_success(message);
                    ssh_message_free(message);
                    break;
                }
                else if (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY) {
                    ssh_message_channel_request_reply_success(message);
                    ssh_message_free(message);
                    continue;
                }
            }
            ssh_message_reply_default(message);
            ssh_message_free(message);
        }
        else {
            break;
        }
    } while (!shell);

    printf("post shell\n");

    if (!shell) {
        printf("Error: No shell requested (%s)\n", ssh_get_error(session));
        return 1;
    }

    printf("it works !\n");
    //ssh_scp scp = ssh_scp_new(session, SSH_SCP_READ, "DOWNLOADS");
    //ssh_scp_init(scp);
    //scp_rec(session, scp);
    main_loop(chan);

    ssh_disconnect(session);
    ssh_bind_free(sshbind);
#ifdef WITH_PCAP
    cleanup_pcap();
#endif
    ssh_finalize();
    return 0;
}