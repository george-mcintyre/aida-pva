#ifndef ___webUtil_h__
#define ___webUtil_h__

/*
==============================================================================

  Abs:  This is the header file for the web Utility Class.

  Name: webUtil.h

  Rem:  
      Method: openURL
      Given a URL, open that page and return contents in user supplied buffer.

      To open this URL: 
      http://www.somehost.com/path/file.html, pass these parameters:

       1) The HOST: www.somehost.com (null terminated string)
       2) The PATH: /path/file.html  (null terminated string)

       The user is responsible to ensure that userBuff is large enough.

       Some day, if anyone ever writes another C++ Data Provider, we
       can move this class to some general location (you'll want to change
       the namespace below if you do that).

  Auth: 24-Apr-2003, Ron MacKenzie (RONM):
  Rev:  dd-mmm-19yy, Tom Slick (TXS):

------------------------------------------------------------------------------

  Mod:
        dd-mmm-yyyy, person
          reason

============================================================================*/

#include <iostream.h>

namespace dpChads
{
 // webUtil_error is an exception that webUtil throws.

 class webUtil_error {

   public:
     char message[256];
     webUtil_error(const char*);
     // friend
     // ostream& operator<<(ostream& string, const dpChads::webUtil_error& we );

 };

 class webUtil {

 // private:

   public:
     webUtil(); 
     void dpChads::webUtil::openURL(const char* host_p, 
				 const char* path_p, const char* userBuff) throw (webUtil_error);
 };

}


#endif

