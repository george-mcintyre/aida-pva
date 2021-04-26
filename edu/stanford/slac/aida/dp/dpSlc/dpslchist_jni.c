/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
================================================================================
 
  Abs:  Java JNI interface for the Aida dpSlcHist data provider. This must be
        compiled with specific options to be used with jni which is what
        the JNI attribute accomplishes.

        opts = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION " + -
         "/names=(short,as_is) /reentrancy=multithread"
 
  Name: dpslchist_jni.c

  Proto: None

  Auth: 10-Aug-2004, Bob Hall (rdh) 
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)
 
--------------------------------------------------------------------------------
 
  Mod:  (newest to oldest)  
        DD-MMM-YYYY, My Name:
           Changed such and such to so and so. etc. etc.
 
==============================================================================*/

// Needed to shorten dpslchisti_.h to dpslchi.h because of
// VAX limitation of file name length.   
#include "slctxt:edu_stanford_slac_aida_dp_dpslc_dpslchi.h" /* Generated by javah */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include "slctxt:slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "slctxt:msg_proto.h"     /* for standalone_init */

#define NUM_ELEMENTS_PER_SAMPLE 7

/*
#define DEBUG
*/

/*
** Routines in dpslchist_jni_helper.c to call into SLC shareables
*/
extern vmsstat_t SLCHIST_DB_INIT (void);
extern vmsstat_t JNI_GETHISTSAMPLES (const char *, const char *,
    const char *, float **);

extern void      JNI_ERR_HISTTRANSLATE (const unsigned long int*,
                                     struct dsc$descriptor*);
/*
** Local helper routines
*/
static void lcl_throwexcept (JNIEnv *env, jobject this, vmsstat_t istatus, 
                 char *exceptname);

/*                                    
** Init for database access.
*/                                  
JNIEXPORT void JNICALL 
                     Java_edu_stanford_slac_aida_dp_dpSlc_DpSlcHistI_1impl_DbInit
                     (JNIEnv *env, jobject this)
{
   vmsstat_t status;
/*---------------------------------------------------*/
   printf ("DbInit entered\n");
   if (!SUCCESS(status = SLCHIST_DB_INIT()))
       lcl_throwexcept (env, this, status, "ServerInitialisationException");
   return;
}

/*
** GetHistSamples
*/
JNIEXPORT jfloatArray JNICALL 
                Java_edu_stanford_slac_aida_dp_dpSlc_DpSlcHistI_1impl_GetHistSamples
                     (JNIEnv *env, jclass this, jstring dbname,
                     jstring stime, jstring etime)
{
   float *val_p = NULL;
   int num_elements;
   vmsstat_t status = 0;
   jfloatArray lcl_floata;
/*---------------------------------------------------*/
   const char *pvname = (*env)->GetStringUTFChars(env, dbname, 0);

   const char *starttime = (*env)->GetStringUTFChars(env, stime, 0);
   const char *endtime = (*env)->GetStringUTFChars(env, etime, 0);

#ifdef DEBUG
   printf ("You have reached GetHistSamples in the shareable, \n");
   printf ("  with name arguments = %s %s %s\n", pvname,
       starttime, endtime);
#endif

   /*
    * Call the C routine JNI_GETHISTSAMPLES to get the data samples
    * for the specified signal name during the interval specified
    * by the start and end times.
    */ 
   if (!SUCCESS(status = JNI_GETHISTSAMPLES (pvname, starttime, endtime,
       &val_p)))
   {
       lcl_throwexcept (env, this, status, "UnableToGetDataException");
   }

#ifdef DEBUG
   printf("number of samples returned = %d\n", (int) *val_p);
#endif

   /*
    * The number of elements in the returned float array must be
    * calculated.  The number of samples returned is stored in the
    * first (0) element.  Each sample has 7 associated array elements:
    * one data value and six timestamp fields (year, month, day,
    * hour, minute, and second.
    */ 
   num_elements = (((int) *val_p) * NUM_ELEMENTS_PER_SAMPLE) + 1; 
   lcl_floata = (*env)->NewFloatArray(env, num_elements);

   (*env)->SetFloatArrayRegion(env, lcl_floata, 0, num_elements, 
                             (jfloat *)val_p); 
   free (val_p); 
   return lcl_floata;
}                                                    

/*
***************** Local routines ********************
*/

/*
** Throw exception back to java
*/
static void lcl_throwexcept (JNIEnv *env, jobject this, vmsstat_t istatus, 
                             char *exceptname)
{
   char errmsg[256] = { '\0' };
   char errname[256] = "edu/stanford/slac/except/";
   $DESCRIPTOR(MESSAGE, errmsg);
   struct dsc$descriptor errmsg_dsc = 
            {256,DSC$K_DTYPE_T, DSC$K_CLASS_S, (char *)&errmsg};
   jclass newExcept;
/*---------------------------------------------------*/
   /*
   ** Translate error and throw exception to Java server code.
   */
   JNI_ERR_HISTTRANSLATE (&istatus, &errmsg_dsc);
   (*env)->ExceptionClear(env);
   strcat (errname, exceptname);
   newExcept = (*env)->FindClass(env, errname);
   if (newExcept == 0)
       printf ("Can't find %s!\n",exceptname);
   else {
       (*env)->ThrowNew(env, newExcept, errmsg_dsc.dsc$a_pointer);
   } 
   return;
}