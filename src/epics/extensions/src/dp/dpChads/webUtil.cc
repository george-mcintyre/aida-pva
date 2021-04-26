/*
==============================================================================

  Abs:  web Utility Class.

  Name: webUtil.cc

  Proto: webUtil.h

  Rem:  This contains methods for C++ applications to use for web access.
        The first such method is openURL, which fetches a web page at a 
	given URL.

        openURL does not log exceptions.  If something doesn't work in 
        openURL, it throws an exception.  The caller must decide what
        do do (log or retry).

        Some day, if anyone ever writes another C++ Data Provider, we
        can move this class to some general location.

  Auth: 23-Apr-2003, Ron MacKenzie
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

---------------------------------------------------------------------------

  Mod:
       04-May-2003, Ron MacKenzie (ronm).
          Add exception throwing.

============================================================================*/
// General Includes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream.h> 
#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

// Includes for tcp/ip socket code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

// webUtil includes
#include "webUtil.h"

// webUtil_error is the exception that webUtil throws.

dpChads::webUtil_error::webUtil_error(const char* str) {
  strcpy(message, str);
}

dpChads::webUtil::webUtil() {
  // cout << "entering webUtil Constructor" << endl;
}

// ***************************************************************************
// openURL class
// Given a URL, open that page and return contents in user supplied buffer.
//
// To open this URL: http://www.somehost.com/path/file.html, 
// pass these parameters:
//
// 1) The HOST: www.somehost.com  (null terminated string)
// 2) The PATH: /path/file.html   (null terminated string)
//
// The user is responsible to ensure that userBuff is large enough.
// Throw webUtil_error exception if anything fails.
// ***************************************************************************

void dpChads::webUtil::openURL(const char* host_p, const char* path_p, 
			       const char* userBuff_p) throw (webUtil_error)
{   

	int s, n;
	char buf[2048];
	char errbuf[256];
	struct sockaddr_in sin;
        struct sockaddr saddr;
	struct hostent *hp;
	struct servent *sp;
        void * void_p;

	/*
	 *  Get the IP address of the host. 
	 */

	hp = gethostbyname(host_p);
	if (hp == NULL) {
          sprintf(errbuf, "Gethost failed in webUtil, errno is: %s\n",strerror(errno));
          throw webUtil_error(errbuf);
	}

	/*
	 *  Create an IP-family socket on which to make the connection
	 */

	s = socket(hp->h_addrtype, SOCK_STREAM, 0);
	if (s < 0) {
          sprintf(errbuf, "Socket failed in webUtil, errno is: %s\n",strerror(errno));
          throw webUtil_error(errbuf);
	}

	/*
	 *  Create a "sockaddr_in" structure which describes the remote
	 *  IP address we want to connect to (from gethostbyname()) and
	 *  the remote TCP port number (from getservbyname()).
	 */

	sin.sin_family = hp->h_addrtype;
	memcpy(&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(80);
        void_p = &sin;  // casting problem.

	/*
	 *  Connect to that address...
	 */

	if (connect(s,(sockaddr *)void_p, sizeof (sin)) < 0) {
          sprintf(errbuf, "Connect failed in webUtil, errno is: %s\n",strerror(errno));
          throw webUtil_error(errbuf);
	}

	// Create the commands to send to the http server.

	char out_str[100];
        char str1[100];
        sprintf(str1, "GET %s HTTP/1.0", path_p);
        // char str1[] = 
	   "GET /grp/cd/soft/slaconly/aida/NameServerDEV.ior HTTP/1.0";
	char str2[] = "From: esd@slac.stanford.edu";  // dummy email addr
        char str3[] = "User-Agent: HTTPTool/1.0";
        char str4[] = "                                                       ";
        sprintf(out_str, "%s\n%s\n%s\n%s\n\n",
                str1,str2,str3,str4);
        int len = strlen(str1)+strlen(str2)+strlen(str3)+strlen(str4)+5;        

        // printf("About to send this: \n%s ", out_str);
        // printf("Length is: %d\n", len);

        int nsnd;
	nsnd = send(s, out_str, len, 0);
        if (nsnd <= 0) {
          sprintf(errbuf, "Send failed in webUtil, errno is: %s\n",strerror(errno));
          throw webUtil_error(errbuf);
	} 
	// printf ("%s %d\n", "Send done. sent", nsnd);

	/*
	 *  Now go into a loop, reading data from the network, 
         *  writing it to the user buffer.
         *  TCP/IP may give it to us in chunks (one chunk per recv).
         */

	char* bigbuf_p = (char *) userBuff_p;
	int total_bytes = 0;

	while ((n = recv(s, buf, sizeof(buf), 0  )) > 0) {
	// printf ("\n\n%s %d\n", "*******Recv done, got this many bytes", n); 
	// fwrite(buf, n, 1, stdout);

            memcpy(bigbuf_p, buf, n);
            bigbuf_p += n;
	    total_bytes += n;

	    // if (!fgets(buf, sizeof(buf), stdin)) break;
	    // socket_write(s, buf, strlen(buf));
	    // send(s, buf, strlen(buf), 0);
	}

	if (n < 0) {
          sprintf(errbuf, 
		  "Recv failed in webUtil, errno is: %s\n",strerror(errno));
          throw webUtil_error(errbuf);
	}

        return;
}










