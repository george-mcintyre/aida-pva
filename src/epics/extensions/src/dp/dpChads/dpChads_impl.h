#ifndef ___dpChads_impl_h__
#define ___dpChads_impl_h__

/*
==============================================================================

  Abs:  This is the header file for the AIDA Channel Archiver Data Provider.

  Name: dpChads_impl.h

  Rem:  It was generated initially by IDL (with the --impl switch).  
        Then, it was modified a little the same as George Modified
        caHist_impl.h in July 2002 (ading one constructor with 2 args). 
        

  Auth: 10-Mar-2003, Ron MacKenzie (RONM):
  Rev:  dd-mmm-19yy, Tom Slick (TXS):

------------------------------------------------------------------------------

  Mod:
        dd-mmm-yyyy, person
          reason

============================================================================*/


// Ronm took these out since not needed.  Created by IDL.
// #include <OB/Types_impl.h>
// #include <except_impl.h>

#include "chadsErrlog.h"                  /* local error loggger */
#include "IndexFile.h"
#define DP_HIST_MAX_ELEMENTS 1000000   /* remember, chArch uses repeat counts */
                    /* so, this max would usually be a lot more data points */

#include <dpChads_skel.h>

// #include "err.h"
// #include "except.h" 

// IDL:dp.aida.slac.stanford.edu/dpChads:1.0
//
namespace dpChads
{

//
// IDL:dp.aida.slac.stanford.edu/dpChads/DpChadsI:1.0
//
class DpChadsI_impl : virtual public POA_dpChads::DpChadsI,
                         virtual public PortableServer::RefCountServantBase
{
    DpChadsI_impl(const DpChadsI_impl&);
    void operator=(const DpChadsI_impl&);

private:
    PortableServer::POA_var poa_;
    CORBA::ORB_var orb_;            // ronm added.

    struct histData*       hdBig_p;
    struct histData*       hdStatic_p;
    class chadsErrlog*        cerr_p;     // error, exception logger..
    long num_test_elems;          // Used to indicate if not fetching history

public:    

    DpChadsI_impl(PortableServer::POA_ptr);
    // ronm added construct. below.
    DpChadsI_impl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, 
		     long numTestElms);;
    ~DpChadsI_impl();

    // ronm added.
    int DpChadsI_init();

    virtual PortableServer::POA_ptr _default_POA();

    //
    // IDL:dp.aida.slac.stanford.edu/dpChads/DpChadsI/get:1.0
    //
    virtual CORBA::Any* get(const char* target,
                            const CORBA::StringSeq& args)
        throw(except::UnableToGetDataException,
              CORBA::SystemException);

    //
    // IDL:dp.aida.slac.stanford.edu/dpChads/DpChadsI/status:1.0
    //
    virtual CORBA::Long status()
        throw(CORBA::SystemException);

    //
    // IDL:dp.aida.slac.stanford.edu/dpChads/DpChadsI/shutdown:1.0
    //
    virtual void shutdown()
        throw(CORBA::SystemException);
};

} // End of namespace dpChads

#endif
