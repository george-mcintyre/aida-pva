/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
================================================================================
 
  Abs:  Java JNI interface helper routines so we can compile with normal 
        options and not deal with the case issue for the Java* interface 
        routines.for the Aida dpSlc data provider. This can be
        compiled with our normal cmp command. Called by first level routine
        in dpslc_jni.c
 
  Name: dpslc_jni_helper.c

  Proto: None

  Auth: 05-Nov-2002, Robert C. Sass
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)
 
--------------------------------------------------------------------------------
 
  Mod:  (newest to oldest)  
        DD-MMM-YYYY, My Name:
           Changed such and such to so and so. etc. etc.
 
==============================================================================*/

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <string.h>               /* strlen memcpy */
#include <stdlib.h>               /* malloc */
#include <ctype.h>                /* isalnum, ispunct */

#include "slctxt:slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "slctxt:msg_proto.h"     /* for standalone_init */
#include "slctxt:sysutil_proto.h" /* for cvt_vms_to_ieee_flt */
#include "slctxt:err_proto.h"     /* for err_translate */
#include "slctxt:dbgetc.h"        /* useful database macros and prototypes */

/*
** Local methods
*/

static void LCL_JNI_PARSENAME (char *name, int4u *prim_pi, int4u *micr_pi, 
                                      int4u *lunit_pi,  int4u *secn_pi);

vmsstat_t DB_INIT ()
{
   vmsstat_t status;
   $DESCRIPTOR (PROCESS_NAME, "AidaDbIf");
/*---------------------------------------------------*/
   status = standalone_init ( &PROCESS_NAME, &((long)(TRUE)),
       NULL, &((long)(FALSE)), &((long)(FALSE)));
   return status;
}

vmsstat_t JNI_DBGETBYTE (const char *pvname, unsigned char *retbyte)
{
   vmsstat_t status;
    DBLIST(byte_ps, unsigned char);
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/
   DBCLEAR(byte_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBgetC( ((void *)(&byte_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   *retbyte = byte_ps->dat[0];
egress:                 
   DBFREE(byte_ps);
   return status;
}

vmsstat_t JNI_DBGETSHORT (const char *pvname, short int *retshort)
{
   vmsstat_t status;
    DBLIST(short_ps, short int);
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/
   DBCLEAR(short_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBgetC( ((void *)(&short_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   *retshort = short_ps->dat[0];
egress:                 
   DBFREE(short_ps);
   return status;
}

vmsstat_t JNI_DBGETINT (const char *pvname, int *retint)
{
   vmsstat_t status;
    DBLIST(int_ps, int);
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/
   DBCLEAR(int_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBgetC( ((void *)(&int_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   *retint = int_ps->dat[0];
egress:                 
   DBFREE(int_ps);
   return status;
}

vmsstat_t JNI_DBGETFLOAT (const char *pvname, float *retfloat)
{
   vmsstat_t status;
    DBLIST(analog_ps, float);
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
    unsigned short one = 1;
/*---------------------------------------------------*/
   DBCLEAR(analog_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBgetC( ((void *)(&analog_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   *retfloat = analog_ps->dat[0];
   printf ("Data in VMS from PV %s is %f\n", pvname, *retfloat);
   cvt_vms_to_ieee_flt (retfloat, retfloat, &one);
egress:                 
   DBFREE(analog_ps);
   return status;
}

vmsstat_t JNI_DBGETCHAR (const char *pvname, unsigned char *retchar)
{
   vmsstat_t status;
    DBLIST(char_ps, char);
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/
   DBCLEAR(char_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBgetC( ((void *)(&char_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   *retchar = char_ps->dat[0];
egress:                 
   DBFREE(char_ps);
   return status;
}


vmsstat_t JNI_DBGETSTRING (const char *pvname, unsigned char **retstring)
{
   vmsstat_t status;
    DBLIST(stringlist_ps, int4u);
    DBLIST(stringdata_ps, char);
    int ssize;              /* String size */
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/

   DBCLEAR(stringlist_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);

   if (!SUCCESS(status = DBlistC( ((void *)(&stringlist_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;

   DBCLEAR (stringdata_ps);

   if (!SUCCESS(status = DBlgetC(((void *)(&stringdata_ps)), 
                                ((void *)(&stringlist_ps))))) goto egress;

   ssize = DBCNT(stringdata_ps) * 2;

   *retstring = malloc(ssize + 1); /* Caller must free it */

   memcpy (*retstring, &stringdata_ps->dat[0], ssize + 1);

   *(*retstring + ssize) = '\0';  /* Null terminate the string */

egress:

   DBFREE(stringlist_ps);
   DBFREE(stringdata_ps);
   return status;
}

vmsstat_t JNI_DBGETBYTEA (const char *pvname, unsigned char **retbytea)
{
   vmsstat_t status;
    DBLIST(bytealist_ps, int4u);
    DBLIST(byteadata_ps, char);
    int ssize;              /* Bytea size */
    int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
/*---------------------------------------------------*/
   DBCLEAR(bytealist_ps);
   LCL_JNI_PARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i);
   if (!SUCCESS(status = DBlistC( ((void *)(&bytealist_ps)), prim_i, micr_i,
                                unit_i, secn_i, NULL))) goto egress;
   DBCLEAR (byteadata_ps);
   if (!SUCCESS(status = DBlgetC(((void *)(&byteadata_ps)), 
                                ((void *)(&bytealist_ps))))) goto egress;
   ssize = DBCNT(byteadata_ps) * 2;
   *retbytea = malloc(ssize + 1); /* Caller must free it */
   memcpy (*retbytea, &byteadata_ps->dat[0], ssize + 1);
   *retbytea[ssize] = '\0';        /* Insure it's null terminated */
egress:                 
   DBFREE(bytealist_ps);
   DBFREE(byteadata_ps);
   return status;
}

void JNI_ERR_TRANSLATE (const unsigned long int* errcode_p,
                            struct dsc$descriptor* msgdsc_ps)
{
   char *msg_p;
   int ii;
/*---------------------------------------------------*/
   err_translate (errcode_p, msgdsc_ps);
   msg_p = msgdsc_ps->dsc$a_pointer;
   /*
   ** Loop backwards through the message to find the first non-blank
   ** character and add null terminator.
   */
   for( ii=msgdsc_ps->dsc$w_length-1; ii>=0; --ii)
   {
      if( isalnum(msg_p[ii]) || ispunct(msg_p[ii]) )
      {
         msg_p[ii+1] = '\0';
         break;
      }
   }
   return; 
}

/*
*********************** Local routines ************************
*/

static void LCL_JNI_PARSENAME (char *slcname, int4u *prim_pi, int4u *micr_pi, 
                                      int4u *lunit_pi,  int4u *secn_pi)
{
    int  cindex, clen;  /* Char index and length */
    char unit_c[5] = {'\0','\0','\0','\0','\0'};
/*---------------------------------------------------*/
    clen = strcspn (slcname, ".:;");   /* Find # chars in primary */ 
    memcpy (prim_pi, slcname, clen);
    cindex = ++clen;                  /* Get past delimiter */
    *micr_pi = *(unsigned long *) &slcname[cindex];  /* Micro always 4 chars */
    cindex += 5;                     /* Index to unit */
    clen = strcspn (&slcname[cindex], ".:;");    /* Find length of unit */ 
    memcpy (&unit_c, &slcname[cindex], clen);    /* Copy unit (1-4 chars) into a char */
    sscanf ((const char *)&unit_c, "%d", lunit_pi);           /* make numeric */
    cindex += ++clen;
    clen = strcspn (&slcname[cindex], " .:;");   /* Find length of secondary */ 
    memcpy(secn_pi, &slcname[cindex], clen);     /* secondary */
    return;
}
