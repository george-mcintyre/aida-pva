/*
==============================================================================

  Abs:  aida Utility Class.

  Name: aidaUtil.cc

  Proto: aidaUtil.h

  Rem:  This class does all it's own logging of errors using the Err class.
        So, if you call it, you need to handle exceptions it throws, but
        don't need to log exceptions (unless you really want to).

        Some day, if anyone ever writes another C++ Data Provider, we
        can move this class to some general location.

  Auth: 23-Apr-2003, Ron MacKenzie
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

---------------------------------------------------------------------------

  Mod:
        31-Jan-2008, Bob Hall (rdh)
           Fixed problem in properly terminating the IOR string to
           exclude extraneous characters (starting with one or more
           control characters).
        11-Apr-2005, Bob Hall (rdh)
           Modified to use the new C++ getInstance Err method.
        06-May-2004, Bob Hall (rdh)
           Modified to form name registered with AIDA name server as
           the concatenation of a string with the contents of environment
           variable ARCHIVER_SUBSYSTEM, which is set by sourcing
           epicsSetup* in the startup file (its value will be "nlcta",
           "pack", or "pepii").  This modification will allow one process
           of the history server to run for each archiver subsystem
           (that is, for each archiver data source).


============================================================================*/
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
#include <OB/CORBA.h>
#include <OB/Properties.h>
#include <OB/BootManager.h>
#include "daNameServer.h"
#include "err.h"
#include "except.h"
#include "aidaUtil.h"
#include "webUtil.h"

#define MAX_HIST_SERVER_NAME_LEN 20

aidaUtil::aidaUtil() {
#ifdef DEBUG
    cout << "entering aidaUtil Constructor" << endl;
#endif

    error_p = Err::getInstance();                          // Error logger.
}
aidaUtil::~aidaUtil() {
#ifdef DEBUG
  cout << "entering aidaUtil Destructor" << endl;
#endif
}

// Arguments are:
// 1) The orb
// 2) The string representation of the object we're resistering.
// 3) The name of the service we're resistering.

void aidaUtil::register_with_name_service(CORBA::ORB_ptr orb,
						   CORBA::String_var s,
						   const char* service)
                              throw (except::UnableToRegisterException) {

    int i;
    int found_non_alnum;
    int url_validate_offset;
    int url_strlen;

    // Get the URL of the AIDA name server from an environment variable.

#ifdef DEBUG
    cout << "entering register_with_name_service\n";
#endif

    const char* url = getenv("AIDA_NAMESERVER_IOR_URL");
    if (url == NULL) {

      cout << "AIDA_NAMESERVER_IOR_URL environment variable not set\n";

      error_p->log(except::UnableToRegisterException(), 
		   " AIDA_NAMESERVER_IOR_URL environ. varb. not set");
      throw except::UnableToRegisterException();
    }

    char* host_p = strstr(url, "//");
    host_p += 2;
    char* endhost_p = strstr(url, "edu");
    endhost_p += 3; 

    char path[100];
    strcpy (path, endhost_p);
    
    *endhost_p = 0;  // null terminate the host part.

    // Fetch the IOR of the name server from the URL for use below.
    // Here's an example of what we're passing to openURL.
    // char host[] = "www.slac.stanford.edu";
    // char path[] = "/grp/cd/soft/slaconly/aida/NameServerDEV.ior";

    char urlBuf[1000];
    try {
      dpChads::webUtil::webUtil wUtil;
      wUtil.openURL (host_p, path, urlBuf);
    }
    catch (dpChads::webUtil_error& we) {
      cerr << we.message << endl;

      cout << "Unable to register with name server\n";

      error_p->log(except::UnableToRegisterException(), we.message );
      throw except::UnableToRegisterException();
    }

    char* url_p = strstr(urlBuf, "IOR:");
    if (url_p == NULL) {

      cout << "Fetched URL didn't contain IOR\n";

      error_p->log(except::UnableToRegisterException(), 
		   " Fetched URL didn't contain IOR");
      throw except::UnableToRegisterException();
    }
    
    // For some reason, the IOR returned has extaneous characters at the
    // end, starting with one or more control characters.  Perhaps this
    // is caused by the web interface.  Terminate the IOR string after
    // the "IOR:" prefix when encountering the first non alphanumeric
    // character, if any.

    url_strlen = strlen(url_p);
    url_validate_offset = strlen("IOR:");
    i = 0;
    found_non_alnum = 0;

    while ((i < url_strlen - url_validate_offset) && (!found_non_alnum))
    {
        if (isalnum(*(url_p + url_validate_offset + i)))
        {
            i++;
        }
        else
        {
            found_non_alnum = 1;
            *(url_p + url_validate_offset + i) = '\0';
        }
    }

#ifdef DEBUG
    cout << "\n\n******* BACK FROM webUtil... IOR IS*************** \n";
    printf("%s\n",url_p);
    cout << "**** IT's STRLEN is" << endl;
    printf ("%d\n", strlen(url_p));
#endif

    // Resigister the chads 'service' with the name server..

    // First, form the name registered with the AIDA name server by
    // concatenating a string with the name of the archiver subsystem
    // associated with this process.

#ifdef DEBUG
    cout << "getting ARCHIVER_SUBSYSTEM environment variable\n";
#endif

    const char* archiverSubsystem = getenv("ARCHIVER_SUBSYSTEM");
    if (archiverSubsystem == NULL) {

      cout << "Did not find translation for ARCHIVER_SUBSYSTEM environment variable\n";

      error_p->log(except::UnableToRegisterException(), 
		   " Did not find translation for ARCHIVER_SUBSYSTEM environment variable");
      throw except::UnableToRegisterException();
    }

#ifdef DEBUG
    cout << "ARCHIVER_SUBSYSTEM = " << archiverSubsystem << "\n";
#endif

    char histServerName[MAX_HIST_SERVER_NAME_LEN + 1];
    strcpy(histServerName, "Chads_");
    strcat(histServerName, archiverSubsystem);

    try {
      CORBA::Object_var nsWideObj = orb->string_to_object(url_p);
      daNameServer::DaNameServerI_ptr ns_p = 
	daNameServer::DaNameServerI::_narrow(nsWideObj);
      ns_p->StoreObjRef(histServerName, s);
    }
    catch (const CORBA::Exception& ex) {
#ifdef DEBUG
      cout << "caught CORBA exception " << ex << endl;
#endif
      error_p->log(except::UnableToRegisterException(), ex._to_string());
      throw except::UnableToRegisterException();
      // cerr << ex << endl;
    }
    catch (...) {
#ifdef DEBUG
      cout << "caught miscellaneous exception " << endl;
#endif
      error_p->log(except::UnableToRegisterException(), 
		   "MISC exception while narrowing Name Server IOR" );
      throw except::UnableToRegisterException();
    }

    cout << "Successful registration with Aida Name Server for " << histServerName << endl;

    return;

  }







