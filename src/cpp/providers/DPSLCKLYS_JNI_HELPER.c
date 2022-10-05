/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
================================================================================

  Abs:  Java JNI interface helper routines so we can compile with normal
        options and not deal with the case issue for the Java* interface
        routines.for the Aida dpSlcKlys data provider. This can be
        compiled with our normal cmp command. Called by first level routine
        in dpslcklys_jni.c

  Name: dpslcklys_jni_helper.c

  Proto: None

  Auth: 01-May-2007, Bob Hall (rdh)
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

--------------------------------------------------------------------------------

  Mod:  (newest to oldest)
        19-Jun-2008, Bob Hall (rdh)
           Changed the TERS argument to klystrim_one
           from 0 to REFINT4_1(TRUE_FORTRAN).  This
           was done to attempt to fix the intermittent
           problem (e.g., 1 in 30 attempts) of returning
           an error code when calling klystrim_one.
        04-Dec-2007, Bob Hall (rdh)
           Modified DPSLCKLYS_SETTRIMPHASE to process KLYS
           secondary values as well as PDES values.
        28-Sep-2007, Bob Hall (rdh)
           Added DPSLCKLYS_SETCONFIG routine.
        27-Sep-2007, Bob Hall (rdh)
           Renamed the DPSLCKLYS_TRIMPHASE routine to
           DPSLCKLYS_SETTRIMPHASE and added a new parameter
           indicating whether a trim operation should be
           performed.
        21-Aug-2007, Bob Hall (rdh)
           Added DPSLCKLYS_DBGETKLYSFLOAT routine.
        07-Aug-2007, Bob Hall (rdh)
           Improved error handling.
        13-Jun-2007, Bob Hall (rdh)
           Added DPSLCKLYS_TRIMPHASE routine.

==============================================================================*/

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <string.h>               /* strlen memcpy */
#include <stdlib.h>               /* malloc */
#include <ctype.h>                /* isalnum, ispunct */
#include <descrip.h>              /* descriptor definitions */
#include <starlet.h>              /* for sys$asctim() */
#include <lib$routines.h>         /* LIB$DATE_TIME */

#include "slctxt:slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "slctxt:ref.h"           /* REFINT4_1 */
#include "descr.h"                /* descriptor macros */
#include "slctxt:slcparm.h"       /* MAX_MICR_COUNT */
#include "slctxt:slc_types.h"     /* prim_name_ts, micr_name_ts, unit_name_ts */
#include "slctxt:sysutil_proto.h" /* for cvt_vms_to_ieee_flt */
#include "slctxt:dbgetc.h"        /* useful database macros and prototypes */
#include "slctxt:msg_proto.h"     /* for standalone_init */
#include "slctxt:err_proto.h"     /* for err_translate */
#include "slctxt:dbgetc.h"        /* useful database macros and prototypes */
#include "slctxt:descr.h"         /* passing descriptors for strings */
#include "slctxt:klys_proto.h"    /* for klystrim_one */

typedef struct
{
	int2 list_length;
	int2 data_length;
	int2 data;
} KLYS_STATUS_TS;

vmsstat_t linac_klys_stat(void* const list_ps,
		int4* const micr_p,
		int4* const unit_p,
		int4* const beam_p,
		float* const dgrp_p);

vmsstat_t tm_deactivate_klys(long int* micro,
		long int* unit,
		long int* i_beam_num,
		long int* i_notify);

vmsstat_t tm_reactivate_klys(long int* micro,
		long int* unit,
		long int* i_beam_num,
		long int* i_notify);

int chk_aida_access(const struct dsc$descriptor*,
		int*,
		int*);

/*
** Define symbols
*/

/*
#define DEBUG
*/

/*
#define STANDALONE_TEST
*/

/*
** Static variables
*/

/* Micro and unit stored from parseDeviceName.
---------------------------------------------- */
char global_micro[5] = "    ";
char global_unit[5] = "    ";

void JNI_ERR_KLYSTRANSLATE(const unsigned long int* errcode_p,
		struct dsc$descriptor* msgdsc_ps);

/*
** Static Function Prototype(s)
*/
static void parseName(char* slcname,
		int4u* prim_pi,
		int4u* micr_pi,
		int4u* lunit_pi,
		int4u* secn_pi);

static void parseDeviceName(char* slcname,
		prim_name_ts* prim_ps,
		micr_name_ts* micr_ps,
		unit_name_ts* unit_ps);

/* ================================================================================

  Abs:  The "standalone_init" for the AIDA Klystron data server.

  Name: DPSLCKLYS_DB_INIT

  Args: void

  Rem:  Calls standalone_init to map the SLC database, initialize
        the message service, etc., for standalone executables in
        the SLC control system.

  Side: Connects this executable to the SLC control system.

  Ret:  vmsstat_t of standalone_init

==============================================================================*/

vmsstat_t DPSLCKLYS_DB_INIT()
{
	vmsstat_t status;
	$DESCRIPTOR (PROCESS_NAME, "AIDA_DPSLCKLYS");

/*---------------------------------------------------*/

	status = standalone_init(&PROCESS_NAME, &((long)(TRUE)),
			NULL, &((long)(FALSE)), &((long)(FALSE)));
	return status;
}

/* ================================================================================

  Abs:  Get the current value for the specified floating point
        signal.

  Name: DPSLCKLYS_GETKLYSFLOAT

  Args:
        slcname                      The specified floating point signal,
          Use:   string              identified by primary/micro/unit/
          Type:  char *              secondary.
          Acc:   read-only
          Mech:  reference

        retfloat                     The returned current value of the
          Use:   float pointer       specified signal.
          Type:  float *
          Acc:   write-only
          Mech:  reference

  Rem:  This routine obtains the current value for the specified
        floating point signal (e.g., the PDES or PHAS secondary
        of a klystron or subbooster).

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCKLYS_DBGETKLYSFLOAT(char* slcname,
		float* retfloat)
{
	vmsstat_t iss;

	int2u one = 1;

	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;

	DBLIST(analog_ps,
	float);

/*---------------------------------------------------*/


#ifdef DEBUG
	fprintf(stderr, "entering DPSLCKLYS_DBGETKLYSFLOAT\n");
	fprintf(stderr, "slcname = %s\n", slcname);
#endif

	iss = 1;

	/* First parse the primary/micro/unit/secondary string
	   into its components.  Then get the current value of
	   this signal.  Finally, convert this value to IEEE
	   floating point format and return it.
	------------------------------------------------------ */
	parseName(slcname, &prim_i, &micr_i, &unit_i, &secn_i);

	DBCLEAR(analog_ps);

	iss = DBgetC(((void*)(&analog_ps)), prim_i, micr_i,
			unit_i, secn_i, NULL);
	if (!SUCCESS(iss)) {
		goto egress;
	}

	*retfloat = analog_ps->dat[0];

	cvt_vms_to_ieee_flt(retfloat, retfloat, &one);

egress:

	DBFREE(analog_ps);

	return iss;
}

/* ================================================================================

  Abs:  Get the status code for the specified klystron on the
        specified beam code.

  Name: DPSLCKLYS_GETSTATUS

  Args:
        slcname                      The specified klystron, identified
          Use:   string              by a primary:micro:unit string.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        beam                         A string containing the specified
          Use:   string              beam code number.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        dgrp                         A string containing a display
          Use:   string              group name or "NULL" if a
          Type:  char *              display group was not specified.
          Acc:   read-only
          Mech:  reference

        klys_status                  The output klystron status code.
          Use:   short pointer
          Type:  short *
          Acc:   write-only
          Mech:  reference

  Rem:  This routine obtains the status code for the specified
        klystron on the specified beam code.  If the specified
        display group is not equal to "NULL", the specified
        klystron must belong to this display group.  If the
        the specified display group is equal to "NULL", the
        specified klystron must belong to the display group
        "LIN_KLYS".  The status code contains bit code values
        specified by the slctxt:linklysta.txt file.  If the
        LINKLYSTA_ACCEL bit is set, the klystron is in the
        accelerate (activated) state.  If the LINKLYSTA_STANDBY
        bit is set, the klystron is in the standby (deactivated)
        state.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCKLYS_GETSTATUS(char* slcname,
		char* beam,
		char* dgrp,
		short* klys_status)
{
	vmsstat_t iss;

	int4 beam_i;

	prim_name_ts prim_s;            /* Device primary */
	micr_name_ts micr_s;            /* Device micro */
	unit_name_ts unit_s;            /* Device unit */

	dgrp_name_ta dgrp_a;
	dgrp_name_ta* dgrp_pa = NULL;

	KLYS_STATUS_TS klys_status_s;   /* Standard list */

/*---------------------------------------------------*/
#ifdef DEBUG
	fprintf(stderr, "entering DPSLCKLYS_GETSTATUS\n");
	fprintf(stderr, "slcname = %s\n", slcname);
	fprintf(stderr, "beam = %s\n", beam);
	fprintf(stderr, "dgrp = %s\n", dgrp);
#endif

	iss = 1;

	/* Convert the beam code from a string to an integer.
	----------------------------------------------------- */
	sscanf((const char*)beam, "%d", &beam_i);

	if (strcmp(dgrp, "NULL") != 0) {
		strncpy(dgrp_a, dgrp, 8);
		dgrp_pa = &dgrp_a;
	}

	/* Parse the specified klystron primary:micro:unit string
	   to primary, micro, unit fields.
	--------------------------------------------------------- */
	parseDeviceName(slcname, &prim_s, &micr_s, &unit_s);

	/* Call linac_klys_stat to get the status code for the
	   specified klystron on the specified beam code.  If
	   the display group is "NULL", it is assumed that the
	   specified klystron belongs to the "LIN_KLYS" display
	   group.
	------------------------------------------------------- */
	klys_status_s.list_length = 3;
	klys_status_s.data_length = 0;

	iss = linac_klys_stat((void*)&klys_status_s,
			(void*)micr_s._a,
			(void*)unit_s._a,
			&beam_i,
			(void*)dgrp_pa);
	if (!SUCCESS(iss)) {
		fprintf(stderr, "return from linac_klys_stat = %x\n", iss);
		goto egress;
	}

#ifdef DEBUG
	fprintf(stderr, "klys_status_s.data = %x\n", klys_status_s.data);
#endif

	*klys_status = klys_status_s.data;

egress:

	return iss;
}

/* ============================================================================`

  Abs:  Determine whether Klystron Aida access is enabled

  Name: DPSLCKLYS_ACCESSENABLED

  Args: void

  Rem:  Returns a flag indicating whether Klystron Aida
        access is currently enabled.

  Side: None

  Ret:  True flag value if access is currently enabled,
        false flag value if access is currently disabled.

==============================================================================*/
int DPSLCKLYS_ACCESSENABLED()
{
	int status;
	int enabled_flag;
	int seconds_to_expire;

	char klystron_access_logical_name[] = "AIDA_ACCESS_KLYSTRON";

	DESCR_DECLARE;

/*---------------------------------------------------*/

	status = chk_aida_access(DESCRN1(((void*)
					(klystron_access_logical_name))),
			&enabled_flag,
			&seconds_to_expire);

#ifdef DEBUG
	fprintf(stderr, "chk_aida_access enabled_flag = %d\n",
		enabled_flag);
#endif

	return enabled_flag;
}

/* ================================================================================

  Abs:  Set the specified klystron to be deactivated or reactivated
        on a specified beam code.

  Name: DPSLCKLYS_SETDEACTORREACT

  Args:
        slcname                      The specified klystron, identified
          Use:   string              by a primary:micro:unit string.
          Type:  char *
          Acc:   read-only
          Mech:  reference

        deact_or_react_flag          A flag indicating whether the specified
          Use:   integer             klystron is to be deactivated or
          Type:  int                 reactivated:
          Acc:   read-only              0 => deactivated
          Mech:  value                  1 => reactivated

        beam                         A string containing the specified
          Use:   string              beam code number.
          Type:  char *
          Acc:   read-only
          Mech:  reference

  Rem:  This routine either deactivates or reactivates the specified
        klystron on the specified beam code depending on whether
        an input flag indicates that a deactivate or reactivate
        operation is desired.  Before calling this routine, the
        dpslcklysi_impl.java routine checks to make sure that the
        specified klystron is in an appropriate state ("accelerate"
        for a deactivate operation or "standby" for a reactivate
        operation) before this routine is called.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCKLYS_SETDEACTORREACT(char* slcname,
		int deact_or_react_flag,
		char* beam)
{
	vmsstat_t iss;

	int4 beam_i;
	int4 notify = 0;

	prim_name_ts prim_s;            /* Device primary */
	micr_name_ts micr_s;            /* Device micro */
	unit_name_ts unit_s;            /* Device unit */

/*---------------------------------------------------*/

#ifdef DEBUG
	fprintf(stderr, "entering DPSLCKLYS_SETDEACTORREACT\n");
	fprintf(stderr, "slcname = %s\n", slcname);
	fprintf(stderr, "deact_or_react_flag = %d\n", deact_or_react_flag);
	fprintf(stderr, "beam = %s\n", beam);
#endif

	iss = 1;

	/* Convert the beam code from a string to an integer.
	----------------------------------------------------- */
	sscanf((const char*)beam, "%d", &beam_i);

	/* Parse the specified klystron primary:micro:unit string
	   to primary, micro, unit fields.
	--------------------------------------------------------- */
	parseDeviceName(slcname, &prim_s, &micr_s, &unit_s);

	/* Perform the deactivate or reactivate operation depending
	   on the input deact_or_react_flag setting.
	----------------------------------------------------------- */
	if (deact_or_react_flag == 0) {
		iss = tm_deactivate_klys((void*)micr_s._a,
				(void*)unit_s._a,
				&beam_i,
				&notify);
		if (!SUCCESS(iss)) {
			fprintf(stderr, "return from tm_deactivate_klys = %x\n", iss);
			goto egress;
		}
	} else {
		iss = tm_reactivate_klys((void*)micr_s._a,
				(void*)unit_s._a,
				&beam_i,
				&notify);
		if (!SUCCESS(iss)) {
			fprintf(stderr, "return from tm_reactivate_klys = %x\n", iss);
			goto egress;
		}
	}

egress:
	return iss;
}

/* ================================================================================


  Abs:  Sets the specified subbooster or klystron configuration
        secondary PCON or ACON value.

  Name: DPSLCKLYS_SETCONFIG

  Args:
        pmu_name                     The specified subbooster or klystron,
          Use:   string              identified by a primary:micro:unit
          Type:  char *              string.
          Acc:   read-only
          Mech:  reference

        config_value_array           Array of one element containing
          Use:   float *             the specified PCON or ACON value.
          Type:  float
          Acc:   read-only
          Mech:  reference

        secn                         The specified subbooster or klystron
          Use:   string              secondary ("PCON" or "ACON").
          Type:  char *
          Acc:   read-only
          Mech:  reference


  Rem:  This routine sets the PCON or ACON secondary value of a specified
        subbooster or klystron.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCKLYS_SETCONFIG(char* pmu_str,
		float* config_value_array,
		char* secn)
{
	vmsstat_t iss;

	prim_name_ts prim_s;            /* Device primary */
	micr_name_ts micr_s;            /* Device micro */
	unit_name_ts unit_s;            /* Device unit */

	int4u prim_i;
	int4u micr_i;
	int4u unit_i;
	int4u secn_i;

	DBLIST(float_ps,
	float);

/*---------------------------------------------------*/

	iss = 1;

#ifdef DEBUG
	fprintf(stderr, "entering DPSLCKLYS_SETCONFIG\n");
	fprintf(stderr, "pmu_str = %s\n", pmu_str);
	fprintf(stderr, "config_value_array[0] = %f\n",
		config_value_array[0]);
	fprintf(stderr, "secn = %s\n", secn);
#endif

	/* Parse the specified klystron primary:micro:unit string
	   to primary, micro, unit fields.
	--------------------------------------------------------- */
	parseDeviceName(pmu_str, &prim_s, &micr_s, &unit_s);

	/* Call DBputC to set the configuration secondary (PCON
	   or ACON) value after calling DBgetC to allocate
	   memory.
	-------------------------------------------------------- */
	memcpy(&prim_i, &prim_s, sizeof(prim_i));
	memcpy(&micr_i, &micr_s, sizeof(micr_i));
	memcpy(&unit_i, &unit_s, sizeof(unit_i));
	memcpy(&secn_i, secn, sizeof(secn_i));

	DBCLEAR(float_ps);

	iss = DBgetC(((void*)&float_ps),
			prim_i,
			micr_i,
			unit_i,
			secn_i,
			NULL);
	if (!SUCCESS(iss)) {
		printf("error status returned from DBgetC = %x\n", iss);
		DBFREE(float_ps);
		goto egress;
	}

	float_ps->dat[0] = config_value_array[0];

	iss = DBputC(((void*)&float_ps),
			prim_i,
			micr_i,
			unit_i,
			secn_i,
			NULL);
	if (!SUCCESS(iss)) {
		printf("error status returned from DBputC = %x\n", iss);
		DBFREE(float_ps);
		goto egress;
	}

	DBFREE(float_ps);

egress:
	return iss;
}

/* ================================================================================


  Abs:  Sets the PDES or KPHR value of the specified subbooster or
        klystron and optionally trims the phase if the secondary
        is PDES.

  Name: DPSLCKLYS_SETTRIMPHASE

  Args:
        pmu_name                     The specified subbooster or klystron,
          Use:   string              identified by a primary:micro:unit
          Type:  char *              string.
          Acc:   read-only
          Mech:  reference

        secn                         The specified secondary (PDES or
          Use:   string              KPHR).
          Type:  char *
          Acc:   read-only
          Mech:  reference

        secn_value_array             Array of one element containing
          Use:   float *             the specified secondary value.
          Type:  float
          Acc:   read-only
          Mech:  reference

        trim                         The string "YES" or "NO", indicating
          Use:   string              whether a trim operation should be
          Type:  char *              performed.
          Acc:   read-only
          Mech:  reference

        phas_value                   Pointer to the PHAS secondary
          Use:   float *             readback value of the specified
          Type:  float *             subbooster or klystron after the
          Acc:   write-only          trim operation.
          Mech:  reference

  Rem:  This routine sets the PDES or KPHR secondary value of a
        specified subbooster or klystron and optionally also trims
        the phase if the secondary is PDES.  It then returns the
        PHAS secondary readback value for this device.

  Side: none

  Ret:  vmsstat_t value indicating usual error status.

==============================================================================*/

vmsstat_t DPSLCKLYS_SETTRIMPHASE(char* pmu_str,
		char* secn,
		float* secn_value_array,
		char* trim,
		float* phas_value)
{
	vmsstat_t iss;
	vmsstat_t iss_1;

	int2u one = 1;

	prim_name_ts prim_s;            /* Device primary */
	micr_name_ts micr_s;            /* Device micro */
	unit_name_ts unit_s;            /* Device unit */

	int4u prim_i;
	int4u micr_i;
	int4u unit_i;

	unsigned char cur_time[24];

	$DESCRIPTOR(cur_time_desc, cur_time);

	DBLIST(float_ps,
	float);
	REF_DECLARE;

/*---------------------------------------------------*/

	iss = 1;

#ifdef DEBUG
	fprintf(stderr, "entering DPSLCKLYS_SETTRIMPHASE\n");
	fprintf(stderr, "pmu_str = %s\n", pmu_str);
	fprintf(stderr, "secn = %s\n", secn);
	fprintf(stderr, "secn_value_array[0] = %f\n",
		secn_value_array[0]);
	fprintf(stderr, "trim = %s\n", trim);
#endif

	/* Parse the specified klystron primary:micro:unit string
	   to primary, micro, unit fields.
	--------------------------------------------------------- */
	parseDeviceName(pmu_str, &prim_s, &micr_s, &unit_s);

	if (strcmp(secn, "KPHR") == 0) {
		/* Call klystrim_one to set the KPHR secondary of the
		   subbooster or klystron to the specified value.
		----------------------------------------------------- */
		iss = klystrim_one((void*)&prim_s,
				(void*)&micr_s,
				(void*)&unit_s,
				&((unsigned long)('KPHR')),
				secn_value_array,
				0,
				REFINT4_1(TRUE_FORTRAN));
		if (!SUCCESS(iss)) {
			iss_1 = LIB$DATE_TIME(&cur_time_desc);
			cur_time[23] = '\0';
			fprintf(stderr, "%s return from klystrim_one = %x\n",
					cur_time, iss);
			goto egress;
		}
	} else if (strcmp(secn, "PDES") == 0) {
		if (strcmp(trim, "YES") == 0) {
			/* Call klystrim_one to trim the phase of the specified
			   subbooster or klystron to the specified PDES value.
			------------------------------------------------------- */
			iss = klystrim_one((void*)&prim_s,
					(void*)&micr_s,
					(void*)&unit_s,
					&((unsigned long)('PDES')),
					secn_value_array,
					0,
					REFINT4_1(TRUE_FORTRAN));
			if (!SUCCESS(iss)) {
				iss_1 = LIB$DATE_TIME(&cur_time_desc);
				cur_time[23] = '\0';
				fprintf(stderr, "%s return from klystrim_one = %x\n",
						cur_time, iss);
				goto egress;
			}
		} else {
			/* Call DBputC to set the PDES secondary value after
			   calling DBgetC to allocate memory.  No trim operation
			   is performed.
			-------------------------------------------------------- */
			memcpy(&prim_i, &prim_s, sizeof(prim_i));
			memcpy(&micr_i, &micr_s, sizeof(micr_i));
			memcpy(&unit_i, &unit_s, sizeof(unit_i));

			DBCLEAR(float_ps);

			iss = DBgetC(((void*)&float_ps),
					prim_i,
					micr_i,
					unit_i,
					'PDES',
					NULL);
			if (!SUCCESS(iss)) {
				printf("error status returned from DBgetC = %x\n", iss);
				DBFREE(float_ps);
				goto egress;
			}

			float_ps->dat[0] = secn_value_array[0];

			iss = DBputC(((void*)&float_ps),
					prim_i,
					micr_i,
					unit_i,
					'PDES',
					NULL);
			if (!SUCCESS(iss)) {
				printf("error status returned from DBputC = %x\n", iss);
				DBFREE(float_ps);
				goto egress;
			}
		}
	}

	/* Call DBgetC to get the PHAS secondary value of the
	   specified subbooster or klystron.
	----------------------------------------------------- */
	memcpy(&prim_i, &prim_s, sizeof(prim_i));
	memcpy(&micr_i, &micr_s, sizeof(micr_i));
	memcpy(&unit_i, &unit_s, sizeof(unit_i));

	DBCLEAR(float_ps);

	iss = DBgetC(((void*)&float_ps),
			prim_i,
			micr_i,
			unit_i,
			'PHAS',
			NULL);
	if (!SUCCESS(iss)) {
		printf("error status returned from DBgetC = %x\n", iss);
		DBFREE(float_ps);
		goto egress;
	}

	/* Convert the obtained PHAS secondary value to an IEEE
	   floating point value and return this to the caller.
	------------------------------------------------------- */
	*phas_value = float_ps->dat[0];

	DBFREE(float_ps);

	cvt_vms_to_ieee_flt(phas_value, phas_value, &one);

egress:
	return iss;
}

void JNI_ERR_KLYSTRANSLATE(const unsigned long int* errcode_p,
		struct dsc$descriptor* msgdsc_ps)
{
	char* msg_p;
	int ii;

/*---------------------------------------------------*/

	err_translate(errcode_p, msgdsc_ps, global_micro, global_unit);
	msg_p = msgdsc_ps->dsc$a_pointer;

	/* Loop backwards through the message to find the first non-blank
	   character and add null terminator.
	----------------------------------------------------------------- */
	for (ii = msgdsc_ps->dsc$w_length - 1; ii >= 0; --ii) {
		if (isalnum(msg_p[ii]) || ispunct(msg_p[ii])) {
			msg_p[ii + 1] = '\0';
			break;
		}
	}
	return;
}

/*
*********************** Local routines ************************
*/

static void parseName(char* slcname,
		int4u* prim_pi,
		int4u* micr_pi,
		int4u* lunit_pi,
		int4u* secn_pi)
{
	int cindex, clen;                          /* counters for lengths */
	int num_converted; /* Number of items converted by sscanf */

	char unit_c[5] = { '\0', '\0', '\0', '\0', '\0' };

	clen = strcspn(slcname, ".:;");           /* Find # chars in primary */
	memcpy (prim_pi, slcname, clen);
	cindex = ++clen;                           /* Get past delimiter */
	*micr_pi = *(unsigned long*)&slcname[cindex];
	/* Micro always 4 chars */
	cindex += 5;                               /* Index to unit */
	clen = strcspn(&slcname[cindex], ".:;/"); /* Find length of unit */

	num_converted = 0;
	if (clen <= 4) {
		/*
		 * Copy the unit number into a character string and then
		 * convert it into a number.
		 */
		memcpy (&unit_c, &slcname[cindex], clen);
		num_converted = sscanf((const char*)&unit_c, "%d", lunit_pi);
	}

	cindex += ++clen;
	clen = strcspn(&slcname[cindex], " .:;");   /* Find length of secondary */
	memcpy(secn_pi, &slcname[cindex], clen);     /* secondary */

	return;
}

static void parseDeviceName(char* slcname,
		prim_name_ts* prim_ps,
		micr_name_ts* micr_ps,
		unit_name_ts* unit_ps)
{
	int cindex, clen;                          /* counters for lengths */
	char unit_c[5] = { '\0', '\0', '\0', '\0', '\0' };

	clen = strcspn(slcname, ".:;");           /* Find # chars in primary */
	memcpy (prim_ps, slcname, clen);
	cindex = ++clen;                           /* Get past delimiter */
	micr_ps->_i = *(unsigned long*)&slcname[cindex];
	/* Micro always 4 chars */
	cindex += 5;                               /* Index to unit */
	clen = strcspn(&slcname[cindex], ".:;/"); /* Find length of unit */
	memcpy (unit_c, &slcname[cindex], clen);   /* Copy unit into a char */
	sscanf((const char*)unit_c, "%d", &unit_ps->_i);
	/* make unit numeric */

	/* Store global micro and unit strings for use in
	   JNI_ERR_KLYSTRANSLATE.
	---------------------------------------------------------------- */
	memcpy(global_micro, micr_ps->_a, 4);
	global_micro[4] = '\0';

	sprintf(global_unit, "%4d", unit_ps->_i);

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


	/*** Logic begins */

	status = DPSLCKLYS_DB_INIT();
	if (!SUCCESS(status))
	{
		fprintf(stderr, "return from dpslcklys_db_init = %x\n", status);
		status = 0;
		return status;
	}

	return status;
}
#endif
