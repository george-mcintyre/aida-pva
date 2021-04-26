#ifndef ___aidaUtil_h__
#define ___aidaUtil_h__

/*
==============================================================================

  Abs:  This is the header file for the aida Utility Class.

  Name: aidaUtil.h

  Rem:  Some day, if anyone ever writes another C++ Data Provider, we
        can move this class to some general location (you'll want to change
	the namespace below if you do that).

  Auth: 24-Apr-2003, Ron MacKenzie (RONM):
  Rev:  dd-mmm-19yy, Tom Slick (TXS):

------------------------------------------------------------------------------

  Mod:
        dd-mmm-yyyy, person
          reason

============================================================================*/

#include "err.h"
#include "except.h" 
#include "chadsErrlog.h"                  // local error loggger

 class aidaUtil {

 private:
  class Err* error_p;

 public:
  aidaUtil(); 
  ~aidaUtil();
  void register_with_name_service(CORBA::ORB_ptr orb,
			          CORBA::String_var s,
		        	  const char* service)
     throw (except::UnableToRegisterException);
 };

#endif

