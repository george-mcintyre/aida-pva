/*
==============================================================================
 
  Abs:  This contains the main for the Aida Channel Archiver Data Source.

  Name: dpChadsServer.cc
        run        - function which is run by main.
        main       - the entry point for the server.

  Proto: None

  Rem: 
       This server can be run in two modes.  One to fetch real history 
       data from channel archiver and return that.  The other to just
       return static test data.  The later is useful for testing (and timing)
       the aida infrastructure.

       The second argument ("lcls" or "nonlcls") is not used and is
       supplied so that the common/tool/warmst script (called by the
       procmanger script) can determine whether a process invoking
       the dpChadsServer executable is for the LCLS Aida Channel Archiver
       data server running on a machine or not.

       Start this server like this to cause a real fetch of history data:
       $AIDADEVROOT/bin/@sys/dpChadsServer (lcls | nonlcls)
        -ORBconfig /afs/slac/package/aida/slaconly/errEvent.conf&

       Start this server like this to return static test data :
       $AIDADEVROOT/bin/@sys/dpChadsServer (lcls | nonlcls)
        -ORBconfig /afs/slac/package/aida/slaconly/errEvent.conf
        NUM_TEST 123 &

       NUM_TEST 122 says to return 122 history elements of static data.

       This server does all it's own Orbacus (Corba) ORB operations.
       It does not call any DA utilities for that purpose.

       This test history server is an EPICS extension.

       This module was originally adapted from the Orbacus Hello demo
       program to create the Chads Server in /afs/slac/package/aida.
       Then, an intermediate version was created in ~ronm/dev/chads which
       partially worked with AIDA NewLabour release (in 7/2002).  Then, this
       version was created to work with the GrenAIDA NewLabour release.
       See the IDL file for more discussion about functionality.

  Auth: 10-Mar-2003, Ron MacKenzie
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)
 
------------------------------------------------------------------------------
 
  Mod:  (newest to oldest)
        10-Mar-2007, Bob Hall (rdh)
          Modified to process the second argument ("lcls" or "nonlcls").
        24-Apr-2003, Ron MacKenzie (ronm)
	  Invoke method in aidaUtil class to find the IOR and to register with
          the aida name server.
	   
============================================================================*/

#include <OB/CORBA.h>
#include <OB/Properties.h>
#include <OB/BootManager.h>
#include "dpChads_impl.h"
#include "daNameServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream.h>     // for cin

#include "err.h"

#include "chadsErrlog.h" // chads error logger.
#include "except.h"    // ESD Exceptions here
#include "aidaUtil.h"

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

// #ifdef HAVE_STD_IOSTREAM
// using namespace std;
// #endif

int
run(CORBA::ORB_ptr orb, int argc, char* argv[], long tempNumElems,
    dpChads::chadsErrlog*  err_p)
{
    const char* archiverSubsystem;
    char startMessage[128];

#ifdef DEBUG
    cout << "entering run in dpChadsServer" << endl;  
#endif

    //
    // Resolve Root POA.
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
    
    //
    // Get a reference to the POA manager
    //
    PortableServer::POAManager_var manager = rootPOA -> the_POAManager();
    
    // Get Child Persistent POA
    CORBA::PolicyList pl(2);
    pl.length(2);
    pl[0] = rootPOA -> create_lifespan_policy(PortableServer::PERSISTENT);
    pl[1] = rootPOA -> create_id_assignment_policy(
    					   PortableServer::USER_ID);
            
    // In chads, dpChads was dpCaHist in the next line.**WARNING!!!!****

    PortableServer::POA_var m_childPersistentPOA = 
          rootPOA->create_POA("dpChads", manager, pl);

    //
    // Create implementation object
    //
    // Here, we're calling this constructor which is not the one created
    // by IDL.  I put it in the impl file, just like George did in caHist_impl.
    // It doesn't do anything (it just returns).  But, then that's all the 
    // IDL generated (one argument) one does too. 
    // THEN, I added one more parameter (number of test (static) elements)
    // tempNumElms = 0 indicates to fetch and return real history data. 

    dpChads::DpChadsI_impl* DpChadsI_impl_p = 
      new dpChads::DpChadsI_impl(orb, m_childPersistentPOA, tempNumElems);

    PortableServer::ServantBase_var servant = DpChadsI_impl_p;

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId("hello");
    m_childPersistentPOA -> activate_object_with_id(oid, servant);

    dpChads::DpChadsI_var caHistO = DpChadsI_impl_p -> _this();

    CORBA::String_var s = orb -> object_to_string(caHistO);
    
    // Get Name Server IOR and Register Chads with AIDA name server.  


    try {
      aidaUtil au;
      au.register_with_name_service(orb, s, "Chads");
    }
    catch (const except::UnableToRegisterException &ex) {
      return EXIT_FAILURE;
    }


    strcpy(startMessage, "AIDA DpChadsServer ");
    
    archiverSubsystem = getenv("ARCHIVER_SUBSYSTEM");
    if (archiverSubsystem == NULL)
    {
        strcat(startMessage, "Unknown");
    }
    else
    {
        strcat(startMessage, archiverSubsystem);
    }

    strcat(startMessage, " Starting");

    err_p->log(startMessage);

    err_p->log("Server Ready");

    cout << "Registration of Chads Service with the Aida NameServer complete." << endl;
    cout << "dpChads Server Ready." << endl;  

    manager -> activate();
    orb -> run();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;

    class dpChads::chadsErrlog*  err_p = new dpChads::chadsErrlog("AIDA", true); 

    // On cmd line, user passed in possible indication of this being 
    // a server that doesn't want to really fetch data by setting
    // arg4 and arg5 to "NUM_TEST 1234" where 1234 is the number of
    // static test elements to return.  WE PASS THAT VALUE TO CONSTRUCTOR BELOW.

    long tempNumElems;
    if (argc > 5) 
      {
	sscanf (argv[5], "%d", &tempNumElems);  // we will pass this to constrtr.
#ifdef DEBUG
        cout << endl << 
          "Server detects that you want STATIC test data, not real history data." 
          << endl;
#endif
      }   
    else
      {
	tempNumElems = 0;         // Indicates doing history fetch.

#ifdef DEBUG
        cout << endl <<
	  "Server detects that you want to fetch real history data." << endl;
#endif
      }

    //
    // Create an argument array and fill it with argument 0 (the program name),
    // argument 2 (ORBInitRef), and argument 3 (the EventService string).
    //
    int num_args;
    int arg_len;
    char * arguments[3];

    num_args = 3;

    arg_len = strlen(argv[0]);
    arguments[0] = (char *) malloc(arg_len + 1);
    strcpy(arguments[0], argv[0]);

    arg_len = strlen(argv[2]);
    arguments[1] = (char *) malloc(arg_len + 1);
    strcpy(arguments[1], argv[2]);

    arg_len = strlen(argv[3]);
    arguments[2] = (char *) malloc(arg_len + 1);
    strcpy(arguments[2], argv[3]);


#ifdef DEBUG
    cout << "calling getDefaultProperities" << endl;
#endif

    OB::Properties_var properties = OB::Properties::getDefaultProperties();
    OB::ParseArgs(num_args, arguments, properties);
    properties -> setProperty("ooc.orb.conc_model", "threaded");

#ifdef DEBUG
    cout << "starting try block" << endl;
#endif

    try
    {
       orb = CORBA::ORB_init(num_args, arguments);
       status = run(orb, num_args, arguments, tempNumElems, err_p);
    }
    catch(const CORBA::Exception& ex)
    {
	cerr << ex << endl;
        err_p->log(ex, " in Main " );
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
	    cerr << ex << endl;        
            err_p->log(ex, " while doing destroy() " );
	    status = EXIT_FAILURE;
	}
    }

    return status;
}

