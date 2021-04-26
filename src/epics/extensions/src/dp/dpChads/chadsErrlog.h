#ifndef ___chadsErrlog_h__
#define ___chadsErrlog_h__

/*
==============================================================================

  Abs:  This is the header file for the local Chads Error logger.

  Name: chadsErrlog.h

  Rem:  

  Auth: 10-Mar-2003, Ron MacKenzie (RONM):
  Rev:  dd-mmm-19yy, Tom Slick (TXS):

------------------------------------------------------------------------------

  Mod:
        dd-mmm-yyyy, person
          reason

============================================================================*/

#include "err.h"
#include "except.h" 

namespace dpChads
{
 class chadsErrlog {

 private:
  Err* myErr_p;

 public:
  chadsErrlog(const char* initmsg, bool first_time); 
  ~chadsErrlog();
  void log(const char* logmsg);
  void log(const CORBA::Exception &ex, const string &suppl);
 };
}


#endif

