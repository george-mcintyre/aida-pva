/*   **CMS**=C_INC   */

/*=======================================================================

  Abs:  General macros to make life under VAX C easier.

  Name: slc_macros.h

  Prev: <descrip.h> (if descriptor macros used; now is explicitly included)
        <string.h>  (if descriptor macros used; now is explicitly included)
        <string.h>  - also if NULL_TO_PAD, PAD_TO_NULL, or MEMSTR is used

  Auth:  7-feb-1990, Lou Sanchez-Chopitea (LSC):

------------------------------------------------------------------------

  Mod:
        09-Feb-2001, Ken Underwood (KKU)
           Add BUILDANDFILLDESC_F macro
        28-Jul-2000, Ed Miller
           (long) cast for TO_ACTIVE_CONSOLE; include <descrip.h>,<string.h>
        11-Aug-1994, D. Van Olst (VANOLST)
           Add more BITS_PER*, BYTES_PER* macros. Check BOOLEAN_DEF
           before typedefing BOOLEAN in case SLC_MACR.HC was already
           included.
        24-Nov-1993, H. Shoaee (HAMID)
           Add REALLOC macro to get around the VAX C compiler bug causing
           an error condition if the memory pointer to realloc is NULL.
        23-Mar-1993, G. Sherwin (GREGS)
           Add MEMSTR
        06-nov-1992, D. Van Olst (VANOLST)
           Add NOT_FOUND macro
        01-oct-1992, D. Van Olst (VANOLST)
           Add TIME* macros.
        27-AUG-1992, D. Van Olst (VANOLST)
           Add PAGES
        06-AUG-1992, D. Van Olst (VANOLST)
           Add BYTES_PER_PAGE
        18-JUN-1992, D. Van Olst (VANOLST)
           Do not loop past beginning of string in PAD_TO_NULL
        18-JUN-1992, D. Van Olst (VANOLST)
           Add NULL_TO_PAD and PAD_TO_NULL
        20-MAY-1992, D. Van Olst (VANOLST)
           Add VAL macro.
        13-Sep-1990, K. Krauter (KEK)
           Add BUILDANDFILLDESC
        01-Aug-1990, R. Sass (RCS)
           Split out VAX/micro common definitions into slctxt:slc_macros.hc
        28-Mar-1990, R. Hall (RDH):
           Added TO_ACTIVE_CONSOLE and BYTES_PER_WORD.
        23-Mar-1990, F. Rouse (FR):
           Add DBALLOC, MAILALLOC, BOOLEAN and MIN/MAX
        26-feb-1990, Lou Sanchez-Chopitea (LSC):
           Fix computation of WORDS to round up, use shift for efficiency.
        20-feb-1990, Lou Sanchez-Chopitea (LSC):
           Add fstring_ts (fortran string) and WORDS.
        13-feb-1990, Lou Sanchez-Chopitea (LSC):
           Fix typo in string type.

========================================================================*/

#ifndef SLC_MACROS_H
#define SLC_MACROS_H

#include <descrip.h>    /* needed especially if dsc$descriptor defined as dsc$descriptor_s */
#include <string.h>

#define TO_ACTIVE_CONSOLE (long)1  /* For use in COWRITE to active COW */
                                   /*   or CALF. */
#define NOT_FOUND        -1    /* Quite often the index of an item
                                  that is not present in an array is
                                  represented by a -1 */

#define GS_PAGE_BC       8192  /* GS_PAGE_BC gives number of bytes in an     */
                               /*  Alpha machine page, for determining Vax   */
                               /*  or Alpha global section exact length as   */
                               /*  integral multiple of Alpha machine pages. */
                               /*  Also defined in SLCPARM.TXT.              */

#define BYTES_PER_PAGE    512  /* The number of bytes in a Vax page (on    */
                               /*  the Alpha, this is known as a pagelet). */

#define BYTES_PER_WORD      2  /* The number of bytes in a word.     */
#define BYTES_PER_LONGWORD  4  /* The number of bytes in a longword. */
#define BYTES_PER_QUADWORD  8  /* The number of bytes in a quadword. */

#define BITS_PER_BYTE       8  /* The number of bits in a byte.      */
#define BITS_PER_WORD      16  /* The number of bits in a word.      */
#define BITS_PER_LONGWORD  32  /* The number of bits in a longword.  */
#define BITS_PER_QUADWORD  64  /* The number of bits in a quadword.  */

/*
**  Size in bits of an object.
*/
#define BITS(i)         ((sizeof(i) * BITS_PER_BYTE)
/*
**  Size in words of an object. Fractions are rounded up.
*/
#define WORDS(i)        ((sizeof(i)+BYTES_PER_WORD-1)/BYTES_PER_WORD)
/*
**  Size in longwords of an object. Fractions are rounded up.
*/
#define LONGWORDS(i)    ((sizeof(i)+BYTES_PER_LONGWORD-1)/BYTES_PER_LONGWORD)
/*
**  Size in quadwords of an object. Fractions are rounded up.
*/
#define QUADWORDS(i)    ((sizeof(i)+BYTES_PER_QUADWORD-1)/BYTES_PER_QUADWORD)
/*
** Size in pages of an object. Fractions are rounded up.
*/
#define PAGES(i)        ((sizeof(i)+BYTES_PER_PAGE-1)/BYTES_PER_PAGE )


/* **  Descriptor macros
   **      (from CMS Callable Routines Reference p B-12) ** */

#define     BUILDDESC(name)     \
struct dsc$descriptor name = { 0, DSC$K_DTYPE_T, DSC$K_CLASS_D, 0}
#define     BUILDDESC_F(name)     \
struct dsc$descriptor name = { 0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0}
#define     FILLDESC(name,str)  \
    (name).dsc$w_length = strlen(str); \
    (name).dsc$a_pointer = (str)
#define     FILLDESC_F(name,str)  \
    (name).dsc$w_length = sizeof(str); \
    (name).dsc$a_pointer = (str)
#define     BUILDANDFILLDESC(name, str)     \
struct dsc$descriptor name =  \
    { sizeof(str)-1, DSC$K_DTYPE_T, DSC$K_CLASS_S, (str)}
#define     BUILDANDFILLDESC_F(name, str) \
struct dsc$descriptor name =  \
    { sizeof(str), DSC$K_DTYPE_T, DSC$K_CLASS_S, (str)}

typedef     struct dsc$descriptor   fstring_ts;

/* ... Space Allocation Macros ... */

#include    "netparm.hc"         /*  NETHEADERLEN                      */

#define MAILALLOC(size)     \
         malloc((size + NETHEADERLEN)*2)

#define DBALLOC(ptr, size)        \
          ptr = (short *) malloc((size + 2)*BYTES_PER_WORD); \
          *ptr= (size + 2);     \
          *(ptr+1) = 0

/*
**  Fix for C compiler bug in the realloc routine
*/

#define REALLOC(ptr, size)        \
            (ptr) ? realloc( (ptr), (size) ) : malloc( size )


/* ... Boolean Type ... */

#include    <stdio.h>                   /*  TRUE + FALSE                      */

#ifndef BOOLEAN_DEF
#define BOOLEAN_DEF
typedef enum {FALSE_B = FALSE, TRUE_B = TRUE} BOOLEAN;
#endif

/*
**  Cast for passing 4 character arrays (or pointers to them)
**  to routines that expect a int4u by value. (This comes in handy
**  with routines that expect a primary, micro, secondary, etc.
**  as an int4u by value, and the value available for passing is
**  a 4 character array).
**  'name' should be a array of four char's, or a pointer to four chars.
*/

#define VAL(name)    ( * ((int4u *) (name))  )


/*
** Convert a NULL-terminated character array to one that is
** blank-padded, non-null terminated.
** (name) should be a pointer and (size) is the size in bytes of the array.
*/

#define NULL_TO_PAD(name, size)                                        \
         if (strlen((char *)(name)) < (size) )                         \
         {                                                             \
            memset( (char *)(name) + strlen((char *)(name)), ' ',      \
                    size - strlen((char *)(name)) );                   \
         }

/*
** Convert a blank padded character array that is not null terminated
** to one that is null terminated and not blank padded.
** (name) should be a pointer and (size) is the size in bytes of the array.
**
** NOTE: if the array has no blank characters at the end, the last
**       element of the array is still null-terminated.
*/

#define PAD_TO_NULL(name, size)                                        \
        {                                                              \
           char *p = (char *)(name) + size - 1;                        \
           for (*p = 0, p--;  (*p==' ')&&(p>=(char *)(name)); *p = 0, p-- ); \
        }

/*
** Convert a character array that is not null-terminated to one that is.
** (name_in) and (name_out) should be pointers, and (size) is the size
** in bytes of the array (name_in).  (name_out) must be dimensioned to
** be at least (size) + 1 byte.
*/

#define MEMSTR(name_in, size, name_out)                               \
        {                                                             \
           strncpy(name_out, name_in, size);                          \
           name_out[size] = '\0';                                     \
        }

/*
** Macro to determine if an absolute quadword time
** is after another absolute quadword time. Note behavior
** will be unpredictable if delta time(s) are used.
**
** This macro is useful if you need to call a VAX utility
** where the times must be in the correct order, or if
** you just need to know if one time is after another.
**
** (either the 1st most sig. longword must be gt the 2nd, or it must be equal
** and the 1st least sig. longword must be gt the 2nd).
*/

#define TIME1_IS_AFTER_TIME2(time1,time2)                                     \
 (                                                                            \
    ( ((int4u *)(time1))[1] > ((int4u *)(time2))[1]  )                        \
    ||                                                                        \
    (                                                                         \
       ( ((int4u *)(time1))[1] ==((int4u *)(time2))[1]  )                     \
       &&                                                                     \
       ( ((int4u *)(time1))[0] > ((int4u *)(time2))[0]  )                     \
    )                                                                         \
 )

/*
** Macro to determine if a quadword time is an absolute time.
** Examine the most-significant word to see if it is not negative
** (positive or zero). Evaluates to TRUE if value is absolute.
**
** (this macro just assumes (time) is a pointer to a quadword of memory.
** We re-cast just to make sure we know what we have (a pointer to
** a signed longword), and then we offset one longword so we are at the
** most significant longword. Then we check for non-negativity).
*/

#define TIME_IS_ABSOLUTE(time) ( ((int4 *)(time))[1] >= 0 )


/*
** Inverse of previous operation.
*/

#define TIME_IS_DELTA( time )   (!(TIME_IS_ABSOLUTE(time)))


/* Include definitions shared between VAX & Micro  */

#include "slc_macr.hc"

#endif
