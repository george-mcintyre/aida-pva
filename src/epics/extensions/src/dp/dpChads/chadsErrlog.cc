/*
==============================================================================

  Abs:  Local error logger so we can add throttling and other stuff to logging
        later if needed here instead of all over the place.

  Name: chadsErrlog.cc

  Proto: chadsErrlog.h

  Rem:

  Auth: 10-Apr-2003, Ron MacKenzie
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

---------------------------------------------------------------------------

  Mod:

============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "err.h"
#include "except.h"
#include "chadsErrlog.h"

// Error logging class.
// This local class is useful so we can add message throttling here later.

Err* myErr_p;

dpChads::chadsErrlog::chadsErrlog(const char* initmsg, bool first_time) {
#ifdef DEBUG
    cout << "entering chadsErrlog Constructor" << endl;
    cout << "creating err object" << endl;
#endif

    if (first_time)
    {
        myErr_p = Err::getInstance("DpChads");
    }
    else
    {
        myErr_p = Err::getInstance();
    }

#ifdef DEBUG
    cout << "exiting chadsErrlog Constructor" << endl;
#endif
  }

dpChads::chadsErrlog::~chadsErrlog() {
  cout << "entering chadsErrlog Destructor" << endl;

}


void dpChads::chadsErrlog::log(const char* logmsg) {
    myErr_p->log(logmsg);
  }
void dpChads::chadsErrlog::log(const CORBA::Exception &ex, const string &suppl) {
    myErr_p->log(ex, suppl);
  }


