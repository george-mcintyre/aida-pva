/*   **CMS**=C_INC   */

/*==============================================================================

  Abs:  Common macros and typedef's for VAX & micro "C" code.

  Name: slc_macr.hc  (Used to be slc_macros.hc)

  Prev: stdio.h for true and false (included).

  Auth: 01-Aug-1990   R. Sass (RCS)

--------------------------------------------------------------------------------

  Mod: 02-Feb-1998    Greg White (GREG)
                      Included stdio.h
       11-Oct-1996    Ed Miller:
                      Eliminate log2 (conflicts with decc$rtldef.tlb  module math.h)
       06-Jun-1996    K  Luchini (LUCHINI):
                      change ! in CLRBITS to ~
       05-Jun-1996    K. Luchini (LUCHINI):
                      and GETBITS and CLRBITS macro
       06-Feb-1996    K. Luchini (LUCHINI) add macros MATCH_FOUND 
                      and MATCH_NOT_FOUND
       04-May-1994    D. Van Olst (VANOLST) add BOOL_TO_LOGICAL,
                         LOGICAL_TO_BOOL; also, check to see if MAX and MIN
                         are already defined and if so #undef them before #def
       04-May-1994    D. Van Olst (VANOLST) add BOOLEAN_DEF to help out
                         slc_macros.h 
       04-May-1994    D. Van Olst (VANOLST) add log4,log2,log1, etc.        
       22-Jul-1993    S. Allison (SAA) add NEAREST_INT* macros                
       03-Nov-1992    T. Lahey (TEL) Move DOS_PTR from ams_plat.h to this file
       24-Aug-1992    D. Van Olst (VANOLST) add VAX_DEV_NAME, VAX_PROD_NAME 
       21-Aug-1990    K. Krauter (KEK) add BOOLEAN
       01-Aug-1991    K. Krauter (KEK) add int1u
       13-Aug-1991    K. Krauter (KEK) support BOOL for 68K micros too
       20-jul-1992    D. Van Olst (VANOLST) move timestamp_ta to this file.
       21-jul-1992    G. R. White (GREG) add BOOL #define for gendefc.h
       29-Jul-1992    R. MacKenzie (RONM) The logic here used to be in 
                       slc_macros.hc.  This file was created for
                       DOS portability reasons (ie 8 char. file name limit).
        3-Aug-1992    03-Aug-1992 Gregory R. White (GREG)
                      Added #ifndef SLC_MACR_HC, #define SLC_MACR_HC
==============================================================================*/

#ifndef SLC_MACR_HC
#define SLC_MACR_HC

#include <stdio.h>


/* **  Check status return from standard error codes ** */

#define     SUCCESS( status)    ((status)&1)
 
/* ** MIN and MAX ** */

#ifdef MAX
#undef MAX
#endif
#define MAX(a,b) (((a)>(b))?(a):(b))      /* MAXIMUM of two expressions */

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) (((a)<(b))?(a):(b))      /* MINIMUM of two expressions */
 
/* 
**  Macro to be used with the find_index_c or find_index functions 
**  available on the VAX and micro.  These these functions 
**  search an array for a match to a supplied key.  If the key
**  is not found in the search string the following is returned:
**     in the "C" version a -1 is returned,
**     in the assembly version a 0 is returned,
**  Therefore, modify the usage of this macro depending upon 
**  which version of the function was called.
*/
#define MATCH_FOUND( index )     ((index)==-1) ? FALSE_F : TRUE_T
#define MATCH_NOT_FOUND( index ) ((index)==-1) ? TRUE_T  : FALSE_F

/*
** The macro GETBITS returns the right adjusted n-bit
** field of x that begins at position p. Assuming that
** both n and p are positive.
*/
#define GETBITS(x,p,n) ( ((x)>>( ((p)+1(n))) & ~(~0 << (n)) )

/*
** The macro CLRBITS returns a value x which has
** set to zero all bits in x that were set in the
** mask, and leaving all other bits in x unchanged.
*/
#define CLRBITS(x,mask)  ( (x) & (~mask) )

/* 
   Typedef's for various flavors of integers. Done in anticipation of the
   switch to RMX3 when the definition of int will change. We try to
   make a typedef category for each class of parameter, guessing at
   this time which will stay as 2 byte integers and which can float to
   4 bytes at RMX III time. These are also used by VAX code for message
   definition etc.

*/

typedef short             int2;           /* An I*2 */
typedef unsigned short    int2u;          /* An unsigned I*2 */
typedef long int          int4;           /* An I*4 */
typedef unsigned long int int4u;          /* An unsigned I*4 */
typedef unsigned char     int1u;          /* For unsigned byte arithmetic */
typedef unsigned char     charu;          /* For unsigned byte arithmetic */

/* Give VMS style status its own type */

typedef int4u           vmsstat_t;      /* VMS type status stays 32 bits */

/* 
** Give FORTRAN-style LOGICALs their own types:
**
** The LOGICAL*4 type used in FORTRAN must sometimes be used in C code
** when calling FORTRAN routines (and vice-versa).
**
** LOGICAL*4 is not quite the same thing as vmsstat_t; it is like
** vmsstat_t in that the low-order bit determines TRUE/FALSEness, but
** unlike vmsstat_t in that you can't call err_send with it.
**
** LOGICAL*4 is not quite the same thing as BOOL; it is like BOOL in that
** it represents TRUE/FALSE, but unlike BOOL in that the values used to
** represent specific values of TRUE and FALSE differ.               
**
** The picky or careful can use log4 to represent a FORTRAN LOGICAL*4
** to avoid confusion.  (One might argue that log4u should be used
** but since FORTRAN uses -1 for TRUE signed is probably safer). 
*/
typedef char            log1;                   /* same as FORTRAN LOGICAL*1  */
    /* NOTE:  log2 conflicts with usage in  decc$rtldef.tlb  module math.h */
/* typedef int2            log2;  */                 /* same as FORTRAN LOGICAL*2 */
typedef int4            log4;                   /* same as FORTRAN LOGICAL*4 */

#define TRUE_FORTRAN   (-1)                     /* same as FORTRAN .TRUE.    */
#define FALSE_FORTRAN  (0)                      /* same as FORTRAN .FALSE.   */
                                                /* (note that we are         */
                                                /* deliberately not defining */
                                                /* the size of the TRUE_     */
                                                /* FORTRAN constant.  This   */
                                                /* is because FORTRAN sets   */
                                                /* all the bits in a TRUE    */
                                                /* logical.  So TRUE_FORTRAN */
                                                /* will work with log1, log2,*/
                                                /* and log4 correctly.       */

#define IS_TRUE_FORTRAN(logical)  ((logical)&1) /* Test a FORTRAN LOGICAL    */
                                                /* (same as SUCCESS).        */

/* ** The boolean type ** */
/*
 * The boolean type on the vax must take into account that TRUE and
 * FALSE are defined as constants and cannot be enum elements.
 * Thus the *_B thing.
 *
 * The boolean type on the 68K micro must take into account that the
 * SCI-provided file psos.hv already defines BOOLEAN as a char.
 *
 * The boolean type on the 386 micro must take into account that the
 * intel-provided include file RMXC.H pre-defines BOOLEAN as a unsigned
 * char but can't redefine TRUE_B and FALSE_B. Sigh.
 *
 * New code intended to run on the VAX and micro should use type BOOL
 * with members TRUE_T and FALSE_F
 */
#ifndef BOOLEAN_DEF
#ifdef TRUE
#ifdef FALSE
#ifndef BOOLEAN
#define BOOLEAN_DEF
typedef enum {FALSE_B = FALSE, TRUE_B = TRUE} BOOLEAN;
#endif /* BOOLEAN */
#endif /* TRUE */
#endif /* FALSE */
#endif /* BOOLEAN_DEF */

#ifndef BOOL_DEF

typedef enum {FALSE_F = 0, TRUE_T = 1} BOOL;

/* Macro to turn a FORTRAN logical into a BOOL. */
#define LOGICAL_TO_BOOL( logical )    \
   ( IS_TRUE_FORTRAN((logical)) ? TRUE_T : FALSE_F )
   
/* Macro to turn a BOOL into a FORTRAN logical. */
#define BOOL_TO_LOGICAL( theBOOL )      \
   ( ( TRUE_T == (theBOOL) ) ? TRUE_FORTRAN : FALSE_FORTRAN )

#define BOOL_DEF
#endif /* BOOL_DEF */

/*
** Define a general VMS timestamp type.
*/
#define NUM_TIMESTAMP_ITEMS  2
typedef int4u timestamp_ta [NUM_TIMESTAMP_ITEMS];

/* 
** The following macros will convert a float or double to the nearest
** 2 or 4 byte signed integer.
*/

#define NEAREST_INT4(A) (int4)(((A) > 0.0) ? ((A) + 0.5) : ((A) - 0.5))
#define NEAREST_INT2(A) (int2)(((A) > 0.0) ? ((A) + 0.5) : ((A) - 0.5))
 
/* The function slc_node() returns one of these two: */

#define VAX_PROD_NAME   "PRODUCTION"           /* VAX production  node name*/
#define VAX_DEV_NAME    "DEVELOPMENT"          /* VAX development node name*/

/*
** Compatibility between VMS and DOS platforms
*/

#ifdef SLC_DOS
#define DOS_PTR  far           /* holds FAR (for pointers) on DOS */
#else
#define DOS_PTR                /* if not DOS, far pointer is set to blank */
#endif

#endif /* SLC_MACR_HC */
                                                                                                                                                                                                                  