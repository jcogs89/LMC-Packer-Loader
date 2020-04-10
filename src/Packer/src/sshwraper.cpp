/*
 * sshwraper.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: cbai
 */
 //ajksdhgfjksdhlfsjkadhfk
//requires libssh
//www.libssh.org
//use your package manager or build it for windows
//https://www.libssh.org/get-it/
//https://github.com/Microsoft/vcpkg/

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#define LIBSSH_STATIC 1
#include <libssh/libssh.h>
#include "sshwraper.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "dirlist.h"
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>
#include "Helpers.h"
#include <io.h>

//https://codeforces.com/blog/entry/13981
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif

#if __cplusplus >= 201402L
#include <shared_mutex>
#endif
//

//#include <bits/stdc++.h>
using namespace std;

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define write _write
#define open _open
#endif

int getpass(const char* prompt, char* password2, bool show_asterisk = true)
{
    const char BACKSPACE = 8;
    const char RETURN = 13;

    string password;

    unsigned char ch = 0;

    cout << prompt << endl;

    DWORD con_mode;
    DWORD dwRead;

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hIn, &con_mode);
    SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN)
    {
        if (ch == BACKSPACE)
        {
            if (password.length() != 0)
            {
                if (show_asterisk)
                    cout << "\b \b";
                password.resize(password.length() - 1);
            }
        }
        else
        {
            password += ch;
            if (show_asterisk)
                cout << '*';
        }
    }
    cout << endl;
    const char* cstr = password.c_str();
    strncpy(password2, cstr, 90);
    return 1;
}

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
            sscanf_s("%9s",buf);

            cmp = strncasecmp(buf, "yes", 3);
            if (cmp != 0)
            {
            	printf("\ndid not add host %i\n",cmp);
            	fflush(stdin);
            	//this is is ------------------------------------------------------------------------------------
            	cin.clear();
            	//cin.ignore(numeric_limits<streamsize>::max(),'\n');
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

/*
int scp_write(ssh_session session)
{
  //https://api.libssh.org/stable/libssh_tutor_scp.html
  ssh_scp scp;
  int rc;

  scp = ssh_scp_new
    (session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
  if (scp == NULL)
  {
    fprintf(stderr, "Error allocating scp session: %s\n",
            ssh_get_error(session));
    return SSH_ERROR;
  }

  rc = ssh_scp_init(scp);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error initializing scp session: %s\n",
            ssh_get_error(session));
    ssh_scp_free(scp);
    return rc;
  }

  //put shit here
  vector<string> stage = dirlist("./Payloads/");
  	printf("\n");
  	unsigned int id;
  	while (1)
  	{
  		//10 is magic, trust me
  		dirprint(stage, 10);
  		printf("\nEnter number for file to be packed\n>> ");
  		id = intinput();
  		if (id>stage.size()-1)
  		{
  			cout << "Invalid Option\n\n";
  			continue;
  		}
  		else
  		{
  			//cout << stage[id];
  			printf("\n%i\n",id);
  			break;
  		}
  	}

  	//jenk
  	//magic again
  	string outp= "./Payloads/"+stage[id].substr(10)+".zips";
  	string tmp = stage[id].substr(10);
  	const char *fname= tmp.c_str();
  	string iput = stage[id];
    ifstream file(stage[id], ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {

    	printf("\nid did work\n");

   }
    else
    {
    	printf("id didnt work");
    	exit(0);
    }

  //								V pdir to make
  rc = ssh_scp_push_directory(scp, "payload", 0777);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't create remote directory: %s\n", ssh_get_error(session));
    return rc;
  }

  rc = ssh_scp_push_file(scp, fname, size, 0777);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't open remote file: %s\n",
            ssh_get_error(session));
    return rc;
  }
  char* a = &buffer[0];
  rc = ssh_scp_write(scp, a, size);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Can't write to remote file: %s\n",
            ssh_get_error(session));
    return rc;
  }

  ssh_scp_close(scp);
  ssh_scp_free(scp);
  return SSH_OK;
}
*/

int connect(char *ip)
{
	  int port = 22; //ToDo change to variable
	  char usern[] = {'p','i'}; // ToDo change to variable
	  //char usern[] = {'c','b','a','i'};
	  ssh_session my_ssh_session;
	  int rc;
	  my_ssh_session = ssh_new();
	  if (my_ssh_session == NULL)
	  {
		  printf("\nfailed to create new ssh session\n");
	    return(-1);
	  }
	  //												V ip to connect to -----------------------------
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, ip);
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
	  ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, &usern);

	  rc = ssh_connect(my_ssh_session);
	  printf("\n%i\n",rc);
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
	  ssh_userauth_none(my_ssh_session, NULL);
	  printf("\nhost v\n");
      char password[100];
      getpass("Password: ", password,  true);
	  //printf("password:%s",password);
	  rc = ssh_userauth_password(my_ssh_session, NULL, password);
	  //rc = ssh_userauth_kbdint(my_ssh_session, usern ,NULL);
	  //rc = ssh_userauth_publickey_auto(my_ssh_session, NULL, NULL);
	  printf("\n%i\n",rc);
	  while (rc != SSH_AUTH_SUCCESS)
	  {
	    fprintf(stderr, "Error authenticating with password: %s\n",ssh_get_error(my_ssh_session));
        getpass("Password: ", password, true);
	    //printf("password:%s",password);
	    rc = ssh_userauth_password(my_ssh_session, NULL, password);
	    //ssh_disconnect(my_ssh_session);
	    //ssh_free(my_ssh_session);
	    //exit(-1);
	  }
	  printf("\nconnected\n");

      //const char ip_remote[20] = "127.0.0.1";//set this to be the ip of the loader may be able to use char array of local host instead of IP
	  //direct_forwarding(my_ssh_session, ip_remote);
      
      
	  // scp_write(my_ssh_session); TODO DOES NOT WRITE ----------------------------------------------------------------

	  ssh_disconnect(my_ssh_session);
	  ssh_free(my_ssh_session);
	  return(1);
}

//int direct_forwarding(ssh_session session, const char ip_remote[20])//fix port number for redirects and the importation of IP addresses
//{
//	ssh_channel forwarding_channel;
//	int rc;
//	const char loopback[20] = "127.0.0.1"; //may need to change to ip of remote system
//	char payload[40] = "Please for the love of god work";//Change this to a vector binary with payload //ToDo
//	int nbytes, nwritten;
//	forwarding_channel = ssh_channel_new(session);
//	if (forwarding_channel == NULL) {
//    return rc;
//  }
//
//  rc = ssh_channel_open_forward(forwarding_channel,
//                                ip_remote, 8570, //port that the loader is monitoring NEED TO GET IP OF REMOTE MACHINE
//                                loopback, 8570);//port that the packer is monitoring (May need to change localhost to IP of that system
//  if (rc != SSH_OK)
//  {
//    ssh_channel_free(forwarding_channel);
//    return rc;
//  }
//
//  nbytes = strlen(payload); //For using vectors we will need to change this
//  nwritten = ssh_channel_write(forwarding_channel,
//                           &payload,
//                           nbytes);
//  if (nbytes != nwritten)
//  {
//    ssh_channel_free(forwarding_channel);
//    return SSH_ERROR;
//  }
//
//
//  ssh_channel_free(forwarding_channel);
//  return SSH_OK;
//}

int rec(ssh_session session, ssh_scp scp)
{
  int rc;
  int size1, mode;
  char *filename, *buffer;

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

  write(1, buffer, size1);
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
 
