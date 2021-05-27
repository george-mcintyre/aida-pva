/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
================================================================================

  Abs:  Java JNI interface helper routines so we can compile with normal
        options and not deal with the case issue for the Java* interface
        routines.for the Aida dpServer data provider. This can be
        compiled with our normal cmp command. Called by first level routine
        in dpserver_jni.c

  Name: dpserver_jni_helper.c

  Proto: None

  Auth: 10-Aug-2004, Bob Hall (rdh)
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
#include <descrip.h>              /* descriptor definitions */
#include <starlet.h>              /* for sys$asctim() */

#include <libdtdef.h>             /* testing */
#include <lib$routines.h>         /* testing */

#include "slctxt:slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "slctxt:msg_proto.h"     /* for standalone_init */
#include "slctxt:sysutil_proto.h" /* for cvt_vms_to_ieee_flt */
#include "slctxt:err_proto.h"     /* for err_translate */
#include "slctxt:hstb_proto.h"    /* for hstb_gather_data */
#include "slctxt:dbgetc.h"        /* useful database macros and prototypes */
#include "slctxt:rt_dcxp_structures.h" /* data and device structures */
#include "slctxt:msg_proto.h"     /* standalone_init  */

#define SAMPLE_TIME_LEN 6
#define NUM_ELEMENTS_PER_SAMPLE 7
#define HSTB_MAX_YR_PTS 42000

/*
#define DEBUG
*/

/*
** Local methods
*/

static void LCL_JNI_HISTPARSENAME (char *name, int4u *prim_pi, int4u *micr_pi,
                                      int4u *lunit_pi,  int4u *secn_pi,
                                      char *channel_name);

vmsstat_t SERVER_DB_INIT ()
{
   vmsstat_t status;
   $DESCRIPTOR (PROCESS_NAME, "Server");
/*---------------------------------------------------*/
   status = standalone_init ( &PROCESS_NAME, &((long)(TRUE)),
       NULL, &((long)(FALSE)), &((long)(FALSE)));
   return status;
}

vmsstat_t JNI_GETHISTSAMPLES (const char *pvname,
   const char *starttime, const char *endtime, float **retfloat)
{
   vmsstat_t status;
   vmsstat_t astatus;

   int4u   prim_i=0x20202020, micr_i=0x20202020, unit_i=0, secn_i=0x20202020;
   unsigned short one = 1;

   int month_num;
   int num_converted;
   int time_field;
   int i;
   int j;
   int found;
   int sample_times_offset;
   int year;
   int day;
   int hour;
   int minute;
   int second;
   int channel_name_len;

   int2 timelen;

   int4u lib_operation = LIB$K_MONTH_OF_YEAR;

   float data_pf[HSTB_MAX_YR_PTS];
   double hours_pd[HSTB_MAX_YR_PTS];

   struct hstb_data_info data_info_s;
   struct dev_structure dev_structure_s;

   struct dsc$descriptor_s fileid_s;
   struct dsc$descriptor_s begin_time_s;
   struct dsc$descriptor_s end_time_s;

   unsigned begin_time_ui[2];
   unsigned end_time_ui[2];

   char fileid_ac[24];
   char begin_time_ac[20];
   char end_time_ac[20];
   char data_sample_time_ac[24];
   char month_substr[3];
   char time_buffer[21];
   char testfield[5];
   char sample_time_month[4];
   char channel_name[9];

   $DESCRIPTOR(data_sample_time_s, data_sample_time_ac);

   char months[][4] = {"JAN", "FEB", "MAR", "APR",
                       "MAY", "JUN", "JUL", "AUG",
                       "SEP", "OCT", "NOV", "DEC"};


/*---------------------------------------------------*/
   LCL_JNI_HISTPARSENAME (pvname, &prim_i, &micr_i, &unit_i, &secn_i,
       channel_name);

#ifdef DEBUG
   printf("unit_i = %d\n", unit_i);
#endif

   /* Fill in the dev_structure structure that is a required
    * parameter to the hstb_gather_data routine that will be
    * called later.  The dev_structure structure is filled in
    * with the primary name, micro name, unit number, and
    * secondary name.
    */
   dev_structure_s.devs_u.dev_struct_s.unit_u.unit_i4 = unit_i;

   strncpy(
       dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8,
       (char *) &prim_i, 4);

   dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[4] = ' ';
   dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[5] = ' ';
   dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[6] = ' ';
   dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[7] = ' ';

   testfield[0] = dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[0];
   testfield[1] = dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[1];
   testfield[2] = dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[2];
   testfield[3] = dev_structure_s.devs_u.dev_struct_s.prim_u.dev_c8[3];
   testfield[4] = '\0';

#ifdef DEBUG
   printf("prim = %s\n", testfield);
#endif

   strncpy(
       dev_structure_s.devs_u.dev_struct_s.micr_u.micr_i4,
       (char *) &micr_i, 4);

   strncpy(
       dev_structure_s.devs_u.dev_struct_s.secn_u.secd_i4,
       (char *) &secn_i, 4);
   strncpy(
       dev_structure_s.devs_u.dev_struct_s.subd_u.sdev_c8,"        ",8);

   /*
    * Pad parts of dev structure with blanks and init class to 0
    */
   dev_structure_s.devs_u.dev_struct_s.type_si = 40;

   dev_structure_s.devs_u.dev_struct_s.secn_u.parm_c8[4] = ' ';
   dev_structure_s.devs_u.dev_struct_s.secn_u.parm_c8[5] = ' ';
   dev_structure_s.devs_u.dev_struct_s.secn_u.parm_c8[6] = ' ';
   dev_structure_s.devs_u.dev_struct_s.secn_u.parm_c8[7] = ' ';

   dev_structure_s.devs_u.dev_struct_s.micr_u.sarea_c8[4] = ' ';
   dev_structure_s.devs_u.dev_struct_s.micr_u.sarea_c8[5] = ' ';
   dev_structure_s.devs_u.dev_struct_s.micr_u.sarea_c8[6] = ' ';
   dev_structure_s.devs_u.dev_struct_s.micr_u.sarea_c8[7] = ' ';

   dev_structure_s.devs_u.dev_struct_s.class_i2 = 0;

   channel_name_len = strlen(channel_name);
   strncpy(
       dev_structure_s.devs_u.dev_struct_s.chan_u.chnl_c8,
           channel_name, channel_name_len);
   for (i = channel_name_len; i < 8; i++)
   {
       dev_structure_s.devs_u.dev_struct_s.chan_u.chnl_c8[i] = ' ';
   }

   /*  Initialize the file id descriptior.
    *  Hstb_gather_data checks this to see if have supplied a fileid,
    *  set to blank which is no fileid indicating for it to use the default file.
    */
   strncpy( fileid_ac, "                        ", 24 );
   fileid_s.dsc$a_pointer = fileid_ac;
   fileid_s.dsc$w_length = 24;
   fileid_s.dsc$b_dtype = DSC$K_DTYPE_T;
   fileid_s.dsc$b_class = DSC$K_CLASS_S;

   /* Parse the begin and end times and convert them to valid VAX
    * time strings.  Next, create descriptors from the VAX time
    * strings.  Finally, convert the begin and end descriptors into
    * VAX quadword times by calling cvt_time.
   */
   strncpy(begin_time_ac, starttime + 3, 2);
   begin_time_ac[2] = '-';

   strncpy(month_substr, starttime, 2);
   month_substr[2] = '\0';

   num_converted = sscanf(month_substr, "%d", &month_num);

   strncpy(begin_time_ac + 3, months[month_num - 1], 3);
   begin_time_ac[6] = '-';

   strncpy(begin_time_ac + 7, starttime + 6, 13);

   strncpy(end_time_ac, endtime + 3, 2);
   end_time_ac[2] = '-';

   strncpy(month_substr, endtime, 2);
   month_substr[2] = '\0';

   num_converted = sscanf(month_substr, "%d", &month_num);

   strncpy(end_time_ac + 3, months[month_num - 1], 3);
   end_time_ac[6] = '-';

   strncpy(end_time_ac + 7, endtime + 6, 13);

   begin_time_s.dsc$a_pointer = begin_time_ac;
   begin_time_s.dsc$w_length = 20;
   begin_time_s.dsc$b_dtype = DSC$K_DTYPE_T;
   begin_time_s.dsc$b_class = DSC$K_CLASS_S;

   end_time_s.dsc$a_pointer = end_time_ac;
   end_time_s.dsc$w_length = 20;
   end_time_s.dsc$b_dtype = DSC$K_DTYPE_T;
   end_time_s.dsc$b_class = DSC$K_CLASS_S;

   strncpy(time_buffer, begin_time_ac, 20);
   time_buffer[20] = '\0';

#ifdef DEBUG
   printf("begin time = %s\n", time_buffer);
#endif

   cvt_time(&begin_time_s, begin_time_ui);

   status = lib$cvt_from_internal_time(((void *)(&lib_operation)),
                                      ((void *)(&time_field)),
                                      begin_time_ui);
   if (!SUCCESS(status))
   {
       goto egress;
   }

#ifdef DEBUG
   printf("time_field = %d\n", time_field);
#endif

   cvt_time(&end_time_s, end_time_ui);

   /*
    * Request data from the history buffer.
    */
   status = hstb_gather_data(data_pf,
                             hours_pd,
                             ((void *) (&data_info_s)),
                             &dev_structure_s,
                             &fileid_s,
                             ((void *) (&begin_time_ui)),
                             ((void *) (&end_time_ui)) );

#ifdef DEBUG
   printf("return from hstb_gather_data = %x\n", status);
#endif

   if (!SUCCESS(status))
   {
       goto egress;
   }

#ifdef DEBUG
   printf("num_points = %d\n", data_info_s.npoints);
#endif

#ifdef DEBUG
   for (i = 0; i < data_info_s.npoints; i++)
   {
       printf("value[%d] = %f\n", i, *(data_pf + i));
   }
#endif

   /* Allocate space to hold the returned data sample information.  This
    * information contains:
    *   1.  The first element (0) contains the number of samples
    *       retrieved (n).
    *   2.  The second to (n + 1) elements contains the data sample
    *       values.
    *   3.  Following the data sample values are clusters of 6 timestamp
    *       element fields for each sample: year, month, day, hour, minute,
    *       and second.
    */

   /* retfloat must be freed by the caller */

   *retfloat = (float *) malloc(((data_info_s.npoints *
       NUM_ELEMENTS_PER_SAMPLE) + 1) *
       sizeof(float));

   /*
    * Store the number of samples retrieved.  This must be converted from
    * VMS floating point format to IEEE format to return to Java and
    * eventually the client.
    */

   *(*retfloat) = (float) data_info_s.npoints;

   cvt_vms_to_ieee_flt (*retfloat, *retfloat, &one);

   /*
    * For each sample, store the data value and convert to IEEE format.
    */
   for (i = 0; i < data_info_s.npoints; i++)
   {
       *(*retfloat + i + 1) = *(data_pf + i);

       cvt_vms_to_ieee_flt(*retfloat + i + 1, *retfloat + i + 1, &one);
   }

   /*
    * For each sample, next store the cluster of 6 timestamp elements:
    * year, month, day, hour, minute, and second.  To obtain these
    * elements, the sample timestamp in VMS quadword time format
    * must be converted to an ASCII string using sys$asctim and then
    * the individual fields must be extracted with sscanf.  The ASCII
    * month field is a 3 letter abbreviation for the month which we
    * want to translate to a month index (1-12).  Once all of the
    * timestamp elements are found, store them and convert each to
    * IEEE floating point format.
    */
   sample_times_offset = data_info_s.npoints + 1;

   for (i = 0; i < data_info_s.npoints; i++)
   {
       status = sys$asctim(&timelen, &data_sample_time_s,
           ((void *) &hours_pd[i]), 0);
       if (!SUCCESS(status))
       {
           goto egress;
       }

       data_sample_time_ac[23] = '\0';

       sscanf(data_sample_time_ac, "%d-%3s-%d %d:%d:%d", &day, sample_time_month,
           &year, &hour, &minute, &second);

       j = 0;
       found = 0;
       while ((j < 12) && (!found))
       {
           if (strcmp(sample_time_month, months[j]) == 0)
           {
               found = 1;
           }
           else
           {
               j++;
           }
       }

       month_num = j + 1;

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i)) = (float) year;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i),
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i), &one);

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 1) = (float) month_num;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 1,
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 1, &one);

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 2) = (float) day;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 2,
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 2, &one);

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 3) = (float) hour;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 3,
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 3, &one);

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 4) = (float) minute;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 4,
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 4, &one);

       *(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 5) = (float) second;
       cvt_vms_to_ieee_flt(*retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 5,
           *retfloat + sample_times_offset + (SAMPLE_TIME_LEN * i) + 5, &one);

   }

   egress:

#ifdef DEBUG
   printf("return from LCL_JNI_HISTPARSENAME status = %d\n", status);
#endif

   return status;
}

void JNI_ERR_HISTTRANSLATE (const unsigned long int* errcode_p,
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

static void LCL_JNI_HISTPARSENAME (char *slcname, int4u *prim_pi, int4u *micr_pi,
                                      int4u *lunit_pi,  int4u *secn_pi,
                                      char *channel_name)
{
    int  cindex, clen;  /* Char index and length */
    int  channel_name_len; /* Length of channel name */
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

    /*
    ** Some pv names have an additional channel name field after the
    ** primary, micro, unit, and secondary fields.  If the pv name
    ** has a channel name field, parse it and return it in the
    ** channel_name parameter.  Otherwise, return 8 blanks in
    ** the channel_name parameter.
    */
    cindex += ++clen;
    clen = strlen(slcname) - cindex;

#ifdef DEBUG
    printf("cindex = %d\n", cindex);
    printf("clen = %d\n", clen);
    printf("strlen(slcname) = %d\n", strlen(slcname));
#endif

    if (cindex > strlen(slcname))
    {
        strcpy(channel_name, "        ");
    }
    else
    {
        if (clen <= 8)
        {
            channel_name_len = clen;
        }
        else
        {
            channel_name_len = 8;
        }
        strncpy(channel_name, &slcname[cindex], channel_name_len);
        channel_name[channel_name_len] = '\0';
    }

#ifdef DEBUG
    printf("channel_name = %s\n", channel_name);
#endif

    return;
}
