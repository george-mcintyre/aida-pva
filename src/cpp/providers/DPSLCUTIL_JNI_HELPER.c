/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
================================================================================

  Abs:  Java JNI interface helper routines so we can compile with normal
        options and not deal with the case issue for the Java* interface
        routines.for the Aida dpSlcUtil data provider. This can be
        compiled with our normal cmp command. Called by first level routine
        in dpslcutil_jni.c

  Name: dpslcutil_jni_helper.c

  Proto: None

  Auth: 03-Nov-2006, Bob Hall (rdh)
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

--------------------------------------------------------------------------------

  Mod:
        07-Aug-2007, Bob Hall (rdh)
           Modified to process requests for setting a BGRP
           variable value.
        24-Jun-2007, Bob Hall (rdh)
           Modified to process requests for deactivating/
           reactivating a specified TRIG device.
        06-Feb-2007, Bob Hall (rdh)
           Added routine DPSLCUTIL_MKB_ACCESSENABLED to check
           whether Aida multiknob operations are currently
           permitted.

==============================================================================*/

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <string.h>               /* strlen memcpy */
#include <stdlib.h>               /* malloc */
#include <ctype.h>                /* isalnum, ispunct */
#include <descrip.h>              /* descriptor definitions */
#include <starlet.h>              /* for sys$asctim() */

#include "slctxt:slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "descr.h"                /* descriptor macros */
#include "slctxt:slcparm.h"       /* MAX_MICR_COUNT */
#include "slctxt:slc_types.h"     /* prim_name_ts, micr_name_ts, unit_name_ts */
#include "slctxt:msg_proto.h"     /* for standalone_init, mpg_one */
#include "slctxt:err_proto.h"     /* for err_translate */
#include "slctxt:dbgetc.h"        /* useful database macros and prototypes */
#include "slctxt:rt_dcxp_structures.h" /* data and device structures */
#include "slctxt:knob_param.h"    /* for KNOB_INIT, KNOB_ROT, KNOB_END */
#include "slctxt:descr.h"         /* passing descriptors for strings */
#include "slctxt:mkbhst_struct.h" /* for mkbhst_data_ts */
#include "slctxt:tmgetc.h"        /* tmget, t_val_is_active, tmpcd */

#define RGBM_INCLUDE_BGRPSTRUCTS
#include "slctxt:rgbm_defines.hc"          /* Various BGRP constants       */
#include "slctxt:msghead.hc"               /* msgheader_ts                 */
#include "slctxt:mpg_reqreply_struc.hc"    /* mpg_bgrp_setvars_replyh_ts   */
#include "slctxt:mpgfunc.hc"               /* MPG_BGRP_VARSET              */
#include "slctxt:sysutil_proto.h"

/*
** Define prototypes of crrutil functions since the prototype
** for mkb_crrdrvr in crrutil_proto.h is incorrect.
*/
vmsstat_t mkb_set_knobfile( const struct dsc$descriptor * );

vmsstat_t mkb_crrdrvr(int4 *, int4 *, float *, ...);

vmsstat_t mkbhst_get_knobdef( mkbhst_data_ts *mkbhst_data_ps);

void mkb_initialise();

int chk_aida_access(const struct dsc$descriptor *,
        int *,
        int *);

vmsstat_t tm_deactivate(int * prim,
        int * micro,
        int * unit,
        int * i_beam_num);

vmsstat_t tm_reactivate(int * prim,
        int * micro,
        int * unit,
        int * i_beam_num);

/*
** Define symbols
*/
#define CRRKNOB_NUM 1
#define PARSE_ONLY -1 /* only parse the knob */
#define PRIM_LEN_NAME 4
#define MICR_LEN_NAME 4
#define SECN_LEN_NAME 4
#define MAX_UNIT_NUM_LEN 5
#define MAX_DEVICE_STRING_LEN 20

#define MEMORY_ALLOCATE_FAIL     2
#define NO_BGRP_NAMES            4
#define BGRP_NAME_NOT_FOUND      6
#define NO_BGRP_VARIABLES        8
#define BGRP_VARIABLE_NOT_FOUND 10

/*
#define DEBUG
*/

/*
#define STANDALONE_TEST
*/

/*
** Static variables
*/
static int curr_knob;

/* Indication of whether specified mutliknob file has the
   "absolute" flag set (otherwise it is a relative multiknob
   file.
------------------------------------------------------------ */
static int mkb_absolute_multiknob_file;

static int mkb_num_devices; /* Number of devices in multiknob file */

/* Array of device names (primary/micro/unit/secondary)
   contained in the specified multiknob file.  Each
   device name is MAX_DEVICE_STRING_LEN characters long
   and the entire array is terminated by a '\0'
   character.
------------------------------------------------------- */
static char *mkb_device_names = NULL;

/* Array of secondary values for devices in the specified
   multiknob.
--------------------------------------------------------- */
static float *mkb_secn_values = NULL;

/*
** Static Function Prototype(s)
*/

void JNI_ERR_UTILTRANSLATE (const unsigned long int* errcode_p,
        struct dsc$descriptor* msgdsc_ps);

static void parseDeviceName(char *slcname,
        prim_name_ts *prim_ps,
        micr_name_ts *micr_ps,
        unit_name_ts *unit_ps);


/* ================================================================================

  Abs:  The "standalone_init" for the AIDA Utility data server.

  Name: DPSLCUTIL_DB_INIT

  Args: void

  Rem:  Calls standalone_init to map the SLC database, initialize
        the message service, etc., for standalone executables in
        the SLC control system.

  Side: Connects this executable to the SLC control system.

  Ret:  vmsstat_t of standalone_init

==============================================================================*/

vmsstat_t DPSLCUTIL_DB_INIT ()
{
    vmsstat_t status;
    $DESCRIPTOR (PROCESS_NAME, "AIDA_DPSLCUTIL");

/*---------------------------------------------------*/

    status = standalone_init ( &PROCESS_NAME, &((long)(TRUE)),
            NULL, &((long)(FALSE)), &((long)(FALSE)));
    return status;
}

/* ============================================================================`

  Abs:  Determine whether Aida multiknob operations are permitted

  Name: DPSLCUTIL_MKB_ACCESSENABLED

  Args: void

  Rem:  Returns a flag indicating whether Aida multiknob
        operations are currently permitted.

  Side: None

  Ret:  True flag value if access is currently enabled,
        false flag value if access is currently disabled.

==============================================================================*/

int DPSLCUTIL_MKB_ACCESSENABLED ()
{
    int status;
    int enabled_flag;
    int seconds_to_expire;

    char multiknob_access_logical_name[] = "AIDA_ACCESS_MULTIKNOB";

    DESCR_DECLARE;

/*---------------------------------------------------*/

    status = chk_aida_access(DESCRN1( ((void *)
                    (multiknob_access_logical_name)) ),
            &enabled_flag,
            &seconds_to_expire);

    return enabled_flag;
}

/* ================================================================================

  Abs:  Perform multiknob assign, rotate, and deassign operations.

  Name: DPSLCUTIL_DO_MKB

  Args:
        mkb_file                       Multiknob file name.
          Use:   string
          Type:  char *
          Acc:   read-only
          Mech:  reference

        relative_delta_array           Array of one element containing
          Use:   float pointer         relative value knob rotation
          Type:  float *               value.
          Acc:   read-only
          Mech:  value


        file_num_devices               Number of devices obtained from
          Use:   integer pointer       the specified multiknob file.
          Type:  int *
          Acc:   write-only
          Mech:  reference

  Rem:  Performs multiknob assign, rotate, and deassign operations
        for a specified multiknob file and relative delta knob
        rotation value.

  Side: Stores an array of device name strings in dynamically
        allocated pointer mkb_device_names, which is static.  Also
        stores an array of device name secondary values in
        dynamically allocated pointer mkb_secn_values, which is
        also static.  Both the mkb_device_names and mkb_secn_values
        dynamic arrays must be later freed (e.g., by a call to
        DPSLCUTIL_MKB_CLEANUP).  Also stores the number of
        device names in the specified multiknob file in static
        integer variable mkb_num_devices and an indication of
        whether the the specified multiknob file is "absolute"
        in static variable mkb_absolute_mulitknob_file.

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCUTIL_DO_MKB (char *mkb_file,
        float *relative_delta_array,
        int *file_num_devices)
{
    vmsstat_t status;
    vmsstat_t iss;

    int i;
    int j;
    int device_name_index; /* Current index into mkb_device_names */
    int num_leading_blanks;  /* Number of leading blanks in front of unit */
    int num_unit_digits;     /* Number of digits in unit */

    int4  parse_only;

    int4u crrknob_num; /* crr knob number */
    int4u knobfunc;    /* knob function code */
    int4u dummy_zero;

    float relative_delta_value; /* specified value */

    int4u prim;
    int4u micr;
    int4u unit;
    int4u secn;

    char primary[PRIM_LEN_NAME + 1];
    char micro[MICR_LEN_NAME + 1];
    char secondary[SECN_LEN_NAME + 1];
    char cur_device_name[MAX_DEVICE_STRING_LEN + 1];
    char unit_buf[MAX_UNIT_NUM_LEN + 1];   /* Unit number buffer */

    mkbhst_data_ts mkbhst_data; /* for retrieving data parsed by MKB routines */

    DBLIST(db_list_ps, int2u); /* Database list data struct */
    DBLIST(db_float_data_ps, float); /* Database float data */

    DESCR_DECLARE;

/*---------------------------------------------------*/

#ifdef DEBUG
    fprintf(stderr, "entering DPSLCUTIL_DO_MKB\n");
    fprintf(stderr, "mkb_file = %s\n", mkb_file);
    fprintf(stderr, "relative_delta_array[0] = %f\n",
        relative_delta_array[0]);
#endif

    status = 1;

    *file_num_devices = 0;

    relative_delta_value = relative_delta_array[0];

    /* Perform needed multiknob initialization.
    ------------------------------------------- */
    mkb_initialise();

    /* Call mkb_set_knobfile for specifying the multiknob
       file.
    ----------------------------------------------------- */
    iss = mkb_set_knobfile(DESCRN1( ((void *) (mkb_file)) ) );
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from mkb_set_knobfile = %x\n", iss);
        status = iss;
        goto egress;
    }

    crrknob_num = CRRKNOB_NUM;

    /* Just parse the multiknob file as a prerequisite
       to the later call to mkbhst_get_knobdef.  If we
       perform the actual assign operation instead of
       just parsing, error messages may be generated
       because the multiknob file is absolute (which
       we cannot determine until mkbhst_get_knobdef
       is called).
    --------------------------------------------------- */
    knobfunc = KNOB_INIT;

    dummy_zero = 0;
    parse_only = PARSE_ONLY;

    iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
            &relative_delta_value, &dummy_zero, &parse_only);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from parse only KNOB_INIT mkb_crrdrvr = %x\n",
                iss);
        status = iss;
        goto egress;
    }

    /* Get the multiknob file contents, stored in structure
       instance mkbhst_data.  Store an indication of whether
       the absolute_knob_function element of this structure
       indicates that the specified multiknob file has the
       "absolute" flag set.  If this "absolute" flag is set,
       perform the  deassign operation and exit.  Otherwise,
       other elements of this structure will be used later
       in extracting the device names contained in the file.
    -------------------------------------------------------- */
    iss = mkbhst_get_knobdef(&mkbhst_data);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from mkbhst_get_knobdef = %x\n", iss);
        status = iss;

        knobfunc = KNOB_END;

        iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
                &relative_delta_value);

        goto egress;
    }

    mkb_absolute_multiknob_file = mkbhst_data.absolute_knob_function;

    if (mkb_absolute_multiknob_file)
    {
        fprintf(stderr, "found an absolute knob file\n");

        knobfunc = KNOB_END;

        iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
                &relative_delta_value);

        goto egress;
    }

    /* Perform the assign operation using function code
       KNOB_INIT.
    --------------------------------------------------- */
    knobfunc = KNOB_INIT;

    iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
            &relative_delta_value);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from KNOB_INIT mkb_crrdrvr = %x\n", iss);
        status = iss;
        goto egress;
    }

    /* Perform the relative knob rotation specified by the
       knob relative delta value for the devices specified
       by the multiknob file.
    ------------------------------------------------------ */
    knobfunc = KNOB_ROT;

    iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
            &relative_delta_value);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from KNOB_ROT mkb_crrdrvr = %x\n", iss);
        status = iss;
        goto egress;
    }

    /* Perform the deassign operation using function code
       KNOB_END.
    ----------------------------------------------------- */
    knobfunc = KNOB_END;

    iss = mkb_crrdrvr(&crrknob_num, &knobfunc,
            &relative_delta_value);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from KNOB_END mkb_crrdrvr = %x\n", iss);
        status = iss;
        goto egress;
    }

    /* Return the number of devices in the specified multiknob
       file.
    ---------------------------------------------------------- */
    mkb_num_devices = mkbhst_data.ndevice_fetched;
    *file_num_devices = mkb_num_devices;

    if (mkb_num_devices > 0)
    {
        /* The dynamically allocated arrays mkb_device_names and
           mkb_secn_values must be freed after they have been
           returned to the Java layer.
        -------------------------------------------------------- */
        mkb_device_names = (char *) malloc((mkb_num_devices *
                MAX_DEVICE_STRING_LEN) + 1);

        mkb_secn_values = (float *) malloc((mkb_num_devices) *
                sizeof(float));

        /* Form the mkb_device_names array of strings.  Each device
           name is formed and stored into the next location of
           this string.  Each device name occupies
           MAX_DEVICE_STRING_LEN characters.  The device name
           consists of the primary, micro, unit, and secondary
           fields, separated by colons.  If a device name
           requires less than MAX_DEVICE_STRING_LEN characters
           (i.e., the unit number is less than the maximum of
           5 characters), the device name is right padded with
           null characters.
        ----------------------------------------------------------- */
        for (i = 0; i < mkb_num_devices; i++)
        {
            device_name_index = 0;

            memcpy(&cur_device_name[device_name_index],
                    &(mkbhst_data.dev_list[i][0]), PRIM_LEN_NAME);
            device_name_index += PRIM_LEN_NAME;

            strncpy(cur_device_name + device_name_index, ":", 1);
            device_name_index++;

            memcpy(&cur_device_name[device_name_index],
                    &(mkbhst_data.dev_list[i][1]), MICR_LEN_NAME);
            device_name_index += MICR_LEN_NAME;

            strncpy(cur_device_name + device_name_index, ":", 1);
            device_name_index++;

            sprintf(unit_buf, "%5d", mkbhst_data.dev_list[i][2]);
            unit_buf[MAX_UNIT_NUM_LEN] = '\0';

            num_leading_blanks = strspn(unit_buf, " ");
            num_unit_digits = MAX_UNIT_NUM_LEN - num_leading_blanks;

            strncpy(cur_device_name + device_name_index,
                    unit_buf + num_leading_blanks, num_unit_digits);
            device_name_index += num_unit_digits;

            strncpy(cur_device_name + device_name_index, ":", 1);
            device_name_index++;

            memcpy(&cur_device_name[device_name_index],
                    &(mkbhst_data.dev_list[i][3]), SECN_LEN_NAME);
            device_name_index += SECN_LEN_NAME;

            for (j = 0; j < num_leading_blanks; j++)
            {
                cur_device_name[device_name_index] = '\0';
                device_name_index++;
            }

            cur_device_name[MAX_DEVICE_STRING_LEN] = '\0';

#ifdef DEBUG
            fprintf(stderr, "cur_device_name = %s\n", cur_device_name);
#endif

            memcpy(mkb_device_names + (i * MAX_DEVICE_STRING_LEN),
                    cur_device_name, MAX_DEVICE_STRING_LEN);
        }

        *(mkb_device_names + (mkb_num_devices * MAX_DEVICE_STRING_LEN) + 1) =
                '\0';

#ifdef DEBUG
        fprintf(stderr, "mkb_device_names = %s\n", mkb_device_names);
#endif
    }


    /* Form a database list of device names needed to retrieve the
       specified secondary (e.g., BDES) using DBlistC.  Then call
       DBlistC to retrieve these secondary values and store them
       in the static float array mkb_secn_values.
    -------------------------------------------------------------- */
    DBCLEAR(db_list_ps);
    DBCLEAR(db_float_data_ps);

    for (i = 0; i < mkb_num_devices; i++)
    {
        memcpy(&prim, &(mkbhst_data.dev_list[i][0]),
                PRIM_LEN_NAME);

        memcpy(&micr, &(mkbhst_data.dev_list[i][1]),
                MICR_LEN_NAME);

        unit = mkbhst_data.dev_list[i][2];

        memcpy(&secn, &(mkbhst_data.dev_list[i][3]),
                SECN_LEN_NAME);

        iss = DBlistC((void *) &db_list_ps, prim, micr, unit,
                secn, NULL);
        if (!SUCCESS(iss))
        {
            fprintf(stderr, "return from call %d to DBlistC = %x\n", i, iss);
            status = iss;
            goto egress;
        }
    }

    if (mkb_num_devices > 0)
    {
        iss = DBlgetC((void *) &db_float_data_ps,
                (void *) &db_list_ps);
        if (!SUCCESS(iss))
        {
            fprintf(stderr, "return from DBlgetC = %x\n", iss);
            status = iss;
            goto egress;
        }

        for (i = 0; i < mkb_num_devices; i++)
        {
            *(mkb_secn_values + i) = db_float_data_ps->dat[i];

#ifdef DEBUG
            fprintf(stderr, "db_float_data_ps->dat[%d] = %f\n",
                i, db_float_data_ps->dat[i]);
#endif
        }
    }

egress:

    return status;
}

/* ============================================================================`

  Abs:  Get the absolute multiknob file flag

  Name: DPSLCUTIL_MKB_GETABSFLAG

  Args: void

  Rem:  Gets a flag indicating whether the specified multiknob
        file is an absolute multiknob file.  This information
        is stored in static variable mkb_absolute_multiknob_file
        by a previous call to DPSLCUTIL_DO_MKB.

  Side: None

  Ret:  Value in static variable mkb_absolute_multiknob_file

==============================================================================*/

int DPSLCUTIL_MKB_GETABSFLAG ()
{
    return mkb_absolute_multiknob_file;
}

/* ============================================================================`

  Abs:  Get the multiknob number of devices.

  Name: DPSLCUTIL_MKB_GETNUMDEVICES

  Args: void

  Rem:  Gets the number of devices stored in static variable
        mkb_num_devices by a previous call to DPSLCUTIL_DO_MKB.
        This value represents the number of devices
        (primary/micro/unit/secondary) contained in the
        specified multiknob file.

  Side: None

  Ret:  Value in static variable mkb_num_devices.

==============================================================================*/

int DPSLCUTIL_MKB_GETNUMDEVICES ()
{
    return mkb_num_devices;
}

/* ============================================================================`

  Abs:  Get the multiknob device names.

  Name: DPSLCUTIL_MKB_GETNAMES

  Args:
        device_names                  String containing multiknob device
          Use:   string               names stored by a previous call to
          Type:  char *               DPSLCUTIL_DO_MKB.
          Acc:   write-only
          Mech:  reference

  Rem:  Gets the multiknob device names string stored in static
        variable mkb_device_names by a previous call to
        DPSLCUTIL_DO_MKB.  These are the device names contained
        in the specified multiknob file.

  Side: None

  Ret:  None

==============================================================================*/

void DPSLCUTIL_MKB_GETNAMES (char *device_names)
{
    int i;

    char cur_device_name[MAX_DEVICE_STRING_LEN + 1];

/*---------------------------------------------------*/

#ifdef DEBUG
    fprintf(stderr, "entering DPSLCUTIL_MKB_GETNAMES\n");

    for (i = 0; i < mkb_num_devices; i++)
    {
        memcpy(cur_device_name, mkb_device_names + (i * MAX_DEVICE_STRING_LEN),
            MAX_DEVICE_STRING_LEN);
        cur_device_name[MAX_DEVICE_STRING_LEN] = '\0';
        fprintf(stderr, "cur_device_name[%d] = %s\n", i, cur_device_name);
    }
#endif

    memcpy(device_names, mkb_device_names,
            (mkb_num_devices * MAX_DEVICE_STRING_LEN) + 1);

    return;
}

/* ============================================================================`

  Abs:  Get the multiknob secondary values.

  Name: DPSLCUTIL_MKB_GETSECNVALUES

  Args:
        values                         Secondary values stored by a
          Use:   float array           previous call to
          Type:  float *               DPSLCUTIL_DO_MKB.
          Acc:   write-only
          Mech:  reference

  Rem:  Gets the secondary values stored in static
        float array mkb_secn_values by a previous call to
        DPSLCUTIL_DO_MKB.  These are the secondary values
        for the devices in the specified multiknob file
        after the knob rotation has completed.

  Side: None

  Ret:  None

==============================================================================*/

void DPSLCUTIL_MKB_GETSECNVALUES(float *values)
{

/*---------------------------------------------------*/

    memcpy(values, mkb_secn_values, mkb_num_devices * sizeof(float));

    return;
}

/* ============================================================================`

  Abs:  Cleanup for previous call to DPSLCUTIL_DO_MKB.

  Name: DPSLCUTIL_MKB_GETCLEANUP

  Args: void

  Rem:  This routine frees dynamic memory allocated by a previous call
        to routine DPSLCUTIL_DO_MKB.  A call to this routine must be
        made after the call to DPSLCUTIL_DO_MKB and subsequent calls to
        DPSLCUTIL_MKB_GETNAMES and DPSLCUTIL_MKB_GETSECNVALUES, which
        may be used to return information in the dynamic memory areas
        mkb_device_names and mkb_secn_values.

  Side: None

  Ret:  None

==============================================================================*/

void DPSLCUTIL_MKB_GETCLEANUP()
{

/*---------------------------------------------------*/

    if (mkb_device_names != NULL)
    {
        free(mkb_device_names);
        mkb_device_names = NULL;
    }

    if (mkb_secn_values != NULL)
    {
        free(mkb_secn_values);
        mkb_secn_values = NULL;
    }

    return;
}

/* ============================================================================`

  Abs:  Get the status code for the specified device on the
        specified beam code.

  Name: DPSLCUTIL_TRIG_GETSTATUS

  Args:
        slcname                      The specified device, identified
          Use:   string              by a primary:micro:unit string.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        beam                         An integer containing the specified
          Use:   integer             beam code number.
          Type:  int
          Acc:   read-only
          Mech:  valus

        trig_status                  The output status code (0 => deactivated,
          Use:   short pointer       1 => activated).
          Type:  short *
          Acc:   write-only
          Mech:  reference

  Rem:  This routine obtains the status code for the specified
        device on the specified beam code.  The returned device
        status is 0 if the device is deactivated or 1 if the
        device is activated.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCUTIL_TRIG_GETSTATUS (char *slcname,
        int beam,
        short *trig_status)
{
    struct
    {
        unsigned short tval_dh[2];
        unsigned long tval;
    } dtget;

    vmsstat_t iss;

    int beam_i;
    int activation_status;

    unsigned long pcd_type;

    prim_name_ts prim_s;            /* Device primary */
    micr_name_ts micr_s;            /* Device micro */
    unit_name_ts unit_s;            /* Device unit */

#ifdef DEBUG
    fprintf(stderr, "entering DPSLCUTIL_TRIG_GETSTATUS\n");
    fprintf(stderr, "slcname = %s\n", slcname);
    fprintf(stderr, "beam = %d\n", beam);
#endif

    iss = 1;

    beam_i = beam;

    /* Parse the specified device primary:micro:unit string
       to primary, micro, unit fields.
    --------------------------------------------------------- */
    parseDeviceName(slcname, &prim_s, &micr_s, &unit_s);

    dtget.tval_dh[0] = (sizeof(dtget.tval) / 2) + 2;
    dtget.tval_dh[1] = 0;

    /* Get the TMATRIX value for the specified  device and
       beam code.
    ------------------------------------------------------ */
    iss = tmget(dtget.tval_dh,
            (unsigned *) prim_s._a,
            (unsigned *) micr_s._a,
            (unsigned *) unit_s._a,
            (unsigned *) &beam_i);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from tmget = %x\n", iss);
        goto egress;
    }

#ifdef DEBUG
    fprintf(stderr, "tval = %d\n", dtget.tval);
#endif

    /* Get the PCD type of the specified device.
    -------------------------------------------- */
    iss = tmpcd((unsigned *) prim_s._a,
            (unsigned *) micr_s._a,
            (unsigned *) unit_s._a,
            &pcd_type);
    if (!SUCCESS(iss))
    {
        fprintf(stderr, "return from tmpcd = %x\n", iss);
        goto egress;
    }

#ifdef DEBUG
    fprintf(stderr, "pcd_type = %d\n", pcd_type);
#endif

    /* Determine whether or not the specified device is
       activated on the specified beam code and return
       a flag code to indicate this status.
    --------------------------------------------------- */
    activation_status = t_val_is_deact((unsigned *) &dtget.tval,
            (unsigned *) &pcd_type,
            NULL);

#ifdef DEBUG
    fprintf(stderr, "activation_status = %d\n", activation_status);
#endif

    if (SUCCESS(activation_status))
    {
        *trig_status = 0;
    }
    else
    {
        *trig_status = 1;
    }

egress:
    return iss;
}

/* ============================================================================`

  Abs:  Determine whether Aida trigger operations are permitted

  Name: DPSLCUTIL_TRIG_ACCESSENABLED

  Args: void

  Rem:  Returns a flag indicating whether Aida trigger
        operations are currently permitted.

  Side: None

  Ret:  True flag value if access is currently enabled,
        false flag value if access is currently disabled.

==============================================================================*/

int DPSLCUTIL_TRIG_ACCESSENABLED ()
{
    int status;
    int enabled_flag;
    int seconds_to_expire;

    char trigger_access_logical_name[] = "AIDA_ACCESS_TRIGGER";

    DESCR_DECLARE;

/*---------------------------------------------------*/

    status = chk_aida_access(DESCRN1( ((void *)
                    (trigger_access_logical_name)) ),
            &enabled_flag,
            &seconds_to_expire);

    return enabled_flag;
}

/* ============================================================================`

  Abs:  Set the specified device to be deactivated or reactivated
        on a specified beam code.

  Name: DPSLCUTIL_TRIG_SETDEACTORREACT

  Args:
        slcname                      The specified device, identified
          Use:   string              by a primary:micro:unit string.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        deact_or_react_flag          A flag indicating whether the specified
          Use:   integer             device is to be deactivated or
          Type:  int                 reactivated:
          Acc:   read-only              0 => deactivated
          Mech:  value                  1 => reactivated

        beam                         An integer containing the specified
          Use:   integer             beam code number.
          Type:  int
          Acc:   read-only
          Mech:  value

  Rem:  This routine either deactivates or reactivates the specified
        device on the specified beam code depending on whether
        an input flag indicates that a deactivate or reactivate
        operation is desired.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCUTIL_TRIG_SETDEACTORREACT (char *slcname,
        int deact_or_react_flag,
        int beam)
{
    vmsstat_t iss;

    int beam_i;

    prim_name_ts prim_s;            /* Device primary */
    micr_name_ts micr_s;            /* Device micro */
    unit_name_ts unit_s;            /* Device unit */

/*---------------------------------------------------*/

#ifdef DEBUG
    fprintf(stderr, "entering DPSLCUTIL_TRIG_SETDEACTORREACT\n");
    fprintf(stderr, "slcname = %s\n", slcname);
    fprintf(stderr, "deact_or_react_flag = %d\n", deact_or_react_flag);
    fprintf(stderr, "beam = %d\n", beam);
#endif

    iss = 1;

    beam_i = beam;

    /* Parse the specified device primary:micro:unit string
       to primary, micro, unit fields.
    --------------------------------------------------------- */
    parseDeviceName(slcname, &prim_s, &micr_s, &unit_s);

    /* Perform the deactivate or reactivate operation depending
       on the input deact_or_react_flag setting.
    ----------------------------------------------------------- */
    if (deact_or_react_flag == 0)
    {
        iss = tm_deactivate((void *) prim_s._a,
                (void *) micr_s._a,
                (void *) unit_s._a,
                &beam_i);
        if (!SUCCESS(iss))
        {
            fprintf(stderr, "return from tm_deactivate = %x\n", iss);
            goto egress;
        }
    }
    else
    {
        iss = tm_reactivate((void *) prim_s._a,
                (void *) micr_s._a,
                (void *) unit_s._a,
                &beam_i);
        if (!SUCCESS(iss))
        {
            fprintf(stderr, "return from tm_reactivate = %x\n", iss);
            goto egress;
        }
    }

egress:
    return iss;
}

/* ============================================================================`

  Abs:  Determine whether Aida BGRP set variable operations are permitted

  Name: DPSLCUTIL_BGRP_ACCESSENABLED

  Args: void

  Rem:  Returns a flag indicating whether Aida BGRP set
        variable operations are currently permitted.

  Side: None

  Ret:  True flag value if access is currently enabled,
        false flag value if access is currently disabled.

==============================================================================*/

int DPSLCUTIL_BGRP_ACCESSENABLED ()
{
    int status;
    int enabled_flag;
    int seconds_to_expire;

    char bgrp_setvar_access_logical_name[] = "AIDA_ACCESS_BGRP_SETVAR";

    DESCR_DECLARE;

/*---------------------------------------------------*/

    status = chk_aida_access(DESCRN1( ((void *)
                    (bgrp_setvar_access_logical_name)) ),
            &enabled_flag,
            &seconds_to_expire);

    return enabled_flag;
}

/* ============================================================================`

  Abs:  Sets the specified variable of the specified BGRP to
        a specified character value.

  Name: DPSLCUTIL_BGRP_SETVAR

  Args:
        bgrp_name_c                  String specifying the BGRP name.
          Use:   string
          Type:  char *
          Acc:   read-only
          Mech:  reference

        var_name_c                   String specifying the BGRP variable
          Use:   string              name.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        new_value_char               String containing the new value
          Use:   string              character for the BGRP variable.
          Type:  char *
          Acc:   read-only
          Mech:  reference

  Rem:  This routine sets the specified variable of the specified
        BGRP to a specified character value.  This routine was
        adapted from the bgrp_setvars_promptset routine in
        ref_timshr:rgbm_bgrppanel.c.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCUTIL_BGRP_SETVAR(char *bgrp_name_c,
        char *var_name_c,
        char *new_value_char)
{
    mpg_bgrp_setvars_req_ts setvars_req_s;

    mpg_bgrp_setvars_replyh_ts *setvars_replyh_p;  /* Pointer to list of   */
    /*  BGRP variables      */
    /*  fetched from MPG.   */

    char           bgrp_namelist[BGRPS_ACTIVE_MAX][BGRP_NAME_SIZE];
    char           vars_namelist[BGRP_VARNAMS_MAX*4][BGRP_VAR_NAME_SIZE];
    char           new_value[4];
    char           bgrp_name[BGRP_NAME_SIZE];
    char           var_name[BGRP_VAR_NAME_SIZE];

    int found;

    unsigned i;
    unsigned j;
    unsigned bgrps_n;
    unsigned varx_beg;
    unsigned varx_lim;
    unsigned bgrp_ix;
    unsigned varx;
    unsigned varp_n;
    unsigned first_blank_pos;

    unsigned long  timeout;

    unsigned short reply_maxwc;
    unsigned short req_datwc;


    const unsigned short mpg_bgrp_varset_funcw     = MPG_BGRP_VARSET;

    static const char mgrp_name_c12[12] = "MGRP_NAME   ";
    static const char blank_4[4] = "    ";

    vmsstat_t      iss;


    /*----------------------------------------------*/

#ifdef DEBUG
    fprintf(stderr, "entering DPSLCUTIL_BGRP_SETVAR\n");
    fprintf(stderr, "bgrp_name_c = %s\n", bgrp_name_c);
    fprintf(stderr, "var_name_c = %s\n", var_name_c);
    fprintf(stderr, "new_value_char = %s\n", new_value_char);
#endif

    /* Copy the C null terminated strings for the input BGRP name and
       variable name to bgrp_name and var_name and pad to the right
       with blanks.
    ----------------------------------------------------------------- */
    strncpy(bgrp_name, bgrp_name_c, BGRP_NAME_SIZE);

    first_blank_pos = strlen(bgrp_name_c);
    for (i = first_blank_pos; i < BGRP_NAME_SIZE; i++)
    {
        bgrp_name[i] = ' ';
    }

    strncpy(var_name, var_name_c, BGRP_VAR_NAME_SIZE);

    first_blank_pos = strlen(var_name_c);
    for (i = first_blank_pos; i < BGRP_VAR_NAME_SIZE; i++)
    {
        var_name[i] = ' ';
    }

    reply_maxwc = (sizeof(*setvars_replyh_p) - sizeof(setvars_replyh_p->dat.vars) +
            sizeof(setvars_replyh_p->dat.vars[0]) * BGRPS_ACTIVE_MAX *
                    (BGRP_VARNAMS_MAX*4 + BGRP_IFCONSTS_MAX))/2;

    setvars_replyh_p = calloc(1, reply_maxwc*2);
    if (setvars_replyh_p)
    {
        memset(&setvars_req_s, 0, sizeof(setvars_req_s));
        timeout = 5000;
        req_datwc = sizeof(setvars_req_s)/2;
        if (SUCCESS(iss = mpg_one(&mpg_bgrp_varset_funcw, &timeout, &req_datwc,
                (unsigned short *) &setvars_req_s, &reply_maxwc,
                (unsigned short *) setvars_replyh_p)) &&
                SUCCESS(iss = setvars_replyh_p->dat.iss))
        {
            /* We assume 1st numerical variable is always MGRP_NAME, and */
            /* we exclude that one.                                      */

            for (bgrps_n = j = 0;  j < setvars_replyh_p->dat.bgrps_active_n;  ++j)
            {
                if (setvars_replyh_p->dat.bgrps[j].timers_n +
                        (setvars_replyh_p->dat.bgrps[j].varns_n ?
                         setvars_replyh_p->dat.bgrps[j].varns_n - 1 : 0) +
                        setvars_replyh_p->dat.bgrps[j].varas_n)
                {
                    memcpy(bgrp_namelist[bgrps_n], setvars_replyh_p->dat.bgrps[j].bgrp_name,
                            BGRP_NAME_SIZE);
                    ++bgrps_n;
                }
            }

            if (bgrps_n == 0)
            {
                iss = NO_BGRP_NAMES;
                goto egress;
            }

            /* Make sure that the specified BGRP name is in the
               list of valid BGRP names.
            --------------------------------------------------- */
            found = 0;
            i = 0;
            while ((i < bgrps_n) && (!found))
            {
                if (CMP_mem_ascii_nocase(bgrp_namelist[i],
                        bgrp_name, BGRP_NAME_SIZE) == 0)
                {
                    found = 1;
                }
                else
                {
                    i++;
                }
            }

            if (!found)
            {
                iss = BGRP_NAME_NOT_FOUND;
                goto egress;
            }

            memcpy(setvars_req_s.bgrp_name, bgrp_name, BGRP_NAME_SIZE);

            bgrp_ix = 0;
            varx_beg = 0;
            while ((bgrp_ix < bgrps_n) &&
                    CMP_mem_ascii_nocase(
                            setvars_replyh_p->dat.bgrps[bgrp_ix].bgrp_name,
                            bgrp_name, BGRP_NAME_SIZE) )
            {
                varx_beg += setvars_replyh_p->dat.bgrps[bgrp_ix].timers_n +
                        setvars_replyh_p->dat.bgrps[bgrp_ix].counters_n +
                        setvars_replyh_p->dat.bgrps[bgrp_ix].varns_n +
                        setvars_replyh_p->dat.bgrps[bgrp_ix].varas_n;
                ++bgrp_ix;
            }

            varx_lim = varx_beg + setvars_replyh_p->dat.bgrps[bgrp_ix].timers_n +
                    setvars_replyh_p->dat.bgrps[bgrp_ix].counters_n +
                    setvars_replyh_p->dat.bgrps[bgrp_ix].varns_n +
                    setvars_replyh_p->dat.bgrps[bgrp_ix].varas_n;

            for (varp_n = 0, varx = varx_beg;  varx < varx_lim;  ++varx)
            {
                if ((varx < varx_beg + setvars_replyh_p->dat.bgrps[bgrp_ix].timers_n ||
                        varx >= varx_beg + setvars_replyh_p->dat.bgrps[bgrp_ix].timers_n +
                                setvars_replyh_p->dat.bgrps[bgrp_ix].counters_n) )
                {
                    if (Find_index_ascii_nocase(setvars_req_s.var,
                            sizeof(setvars_req_s.var[0]),
                            setvars_req_s.vars_n,
                            setvars_replyh_p->dat.vars[varx].var_name,
                            BGRP_VAR_NAME_SIZE,
                            0) == -1)
                    {
                        if (CMP_mem_ascii_nocase(setvars_replyh_p->dat.vars[varx].var_name,
                                mgrp_name_c12, BGRP_VAR_NAME_SIZE))
                        {
                            memcpy(vars_namelist[varp_n],
                                    setvars_replyh_p->dat.vars[varx].var_name, BGRP_VAR_NAME_SIZE);
                            ++varp_n;
                        }
                    }
                }
            }

            if (varp_n == 0)
            {
                iss = NO_BGRP_VARIABLES;
                goto egress;
            }

            /* Make sure that the specified variable names is
               list of valid variables for the BGRP.
            ------------------------------------------------- */
            found = 0;
            i = 0;
            while ((i < varp_n) && (!found))
            {
                if (CMP_mem_ascii_nocase(vars_namelist[i], var_name,
                        BGRP_VAR_NAME_SIZE) == 0)
                {
                    found = 1;
                }
                else
                {
                    i++;
                }
            }

            if (!found)
            {
                iss = BGRP_VARIABLE_NOT_FOUND;
                goto egress;
            }

            memcpy(setvars_req_s.var[0].var_name, var_name,
                    BGRP_VAR_NAME_SIZE);

            memcpy(new_value, blank_4, 4);
            memcpy(new_value, new_value_char, 1);

            memcpy(&setvars_req_s.var[0].newval, new_value, 4);

            setvars_req_s.vars_n = 1;

            iss = mpg_one(&mpg_bgrp_varset_funcw, &timeout, &req_datwc,
                    (unsigned short *) &setvars_req_s, &reply_maxwc,
                    (unsigned short *) setvars_replyh_p);
            if (!SUCCESS(iss))
            {
                goto egress;
            }

            iss = setvars_replyh_p->dat.iss;
            if (!SUCCESS(iss))
            {
                goto egress;
            }
        }
        else
        {
            goto egress;
        }
    }
    else
    {
        iss = MEMORY_ALLOCATE_FAIL;
        goto egress_2;
    }

egress:

    free(setvars_replyh_p);

egress_2:

    return iss;
}

void JNI_ERR_UTILTRANSLATE (const unsigned long int* errcode_p,
        struct dsc$descriptor* msgdsc_ps)
{
    char *msg_p;
    int ii;

/*---------------------------------------------------*/

    err_translate (errcode_p, msgdsc_ps);
    msg_p = msgdsc_ps->dsc$a_pointer;

    /* Loop backwards through the message to find the first non-blank
       character and add null terminator.
    ----------------------------------------------------------------- */
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

static void parseDeviceName(char *slcname,
        prim_name_ts *prim_ps,
        micr_name_ts *micr_ps,
        unit_name_ts *unit_ps)
{
    int  cindex, clen;                          /* counters for lengths */
    char unit_c[5] = {'\0','\0','\0','\0','\0'};

    clen = strcspn (slcname, ".:;");           /* Find # chars in primary */
    memcpy (prim_ps, slcname, clen);
    cindex = ++clen;                           /* Get past delimiter */
    micr_ps->_i = *(unsigned long *) &slcname[cindex];
    /* Micro always 4 chars */
    cindex += 5;                               /* Index to unit */
    clen = strcspn (&slcname[cindex], ".:;/"); /* Find length of unit */
    memcpy (unit_c, &slcname[cindex], clen);   /* Copy unit into a char */
    sscanf ((const char *)unit_c, "%d", &unit_ps->_i);
    /* make unit numeric */
#ifdef DEBUG
    printf("cindex = %d\n", cindex);
    printf("clen = %d\n", clen);
    printf("strlen(slcname) = %d\n", strlen(slcname));
    printf("Prim %4.4s micro %4.4s unit %d\n",
        prim_ps->_a, micr_ps->_a, unit_ps->_i);
#endif

    return;
}

#ifdef STANDALONE_TEST
static int main()
{
    vmsstat_t status;

    char bgrp_name[BGRP_NAME_SIZE];
    char var_name[BGRP_VAR_NAME_SIZE];
    char new_value_char[1];

    /*** Logic begins */

    status = DPSLCUTIL_DB_INIT();
    {
        fprintf(stderr, "return from dpslcutil_db_init = %x\n", status);
        status = 0;
        return status;
    }

    strncpy(bgrp_name, "LCLS        ", 12);
    strncpy(var_name,  "T_CAV       ", 12);
    new_value_char[0] = 'Y';

    status = DPSLCUTIL_BGRP_SETVAR(bgrp_name, var_name,
        new_value_char);
    if (!SUCCESS(status))
    {
        fprintf(stderr, "return from dpslcutil_bgrp_setvars = %x\n", status);
        status = 0;
        return status;
    }

    return status;
}
#endif
