/*
      **MEMBER**=SLCLIBS:AIDASHRLIB

================================================================================

  Abs:  Acquires Buffered BPM data for AIDA DpSlcBuffAcq data provider.

  Name: dpslcbuffacq_jni_helper.c

  Rem:  DPSLCBUFFACQ_JNI.C routines use routines defined here rather than
        calling into VMS shareables directly so we can compile with normal
        options and not deal with the case issue for the Java* interface
        routines. This can be compiled with our normal cmp command.

  Debug: To compile and run as a standalone for debugging,
        Change the #define DEBUG 0 to #define DEBUG 1
            and compile, link and run as follows:
        MCCDEV> define slctxt [], slc_txt
        MCCDEV> c dpslcbuff_jni_helper.c
        MCCDEV> testshrx/define/default bpmshr (for universal bpm_loadref)
        Use a dpslcbuff_jni_helper.opt, derived from aidashr.opt, but adding
        two lines:
            at the top put just "DPSLCBUFFACQ_JNI_HELPER"
            and add "aidashr/shareable"
        MCCDEV> lkdev dpslcbuff_jni_helper dpslcbuff_jni_helper.opt/opt map deb
        MCCDEV> run/debug dpslcbuff_jni_helper.dev

  Proto: None

  Auth: 04-Apr-2011, Greg White
  Rev:

--------------------------------------------------------------------------------

  Mod:  13-Jun-2012, Greg White
        Added GAPM
        30-Jun-2011, Greg White
        Added TORO, KLYS, SBST

==============================================================================*/

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <string.h>               /* strlen memcpy */
#include <stdlib.h>               /* malloc */
#include <stsdef.h>               /* $VMS_STATUS..; status manipulation */
#include <ssdef.h>                /* $SS_ vms standard status codes */
#include <descrip.h>              /* descriptor definitions */
#include <math.h>                 /* sin() etc */
#include <time.h>                 /* time() */

#include <libdtdef.h>             /* testing */
#include <lib$routines.h>         /* testing */

#include "bpmodef.hc"             /* BPM subsys error codes */
#include "opdef.hc"               /* OP_BAD_ALLOC */
#include "modldef.h"              /* MODL_ status codes */
#include "slc_descr.h"            /* VMS DESCRIPTOR management */
#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "slc_types.h"            /* micr_unit_ts */
#include "dbgetc.h"               /* DBLIST, DLIST_DECLARE, protos */
#include "ref.h"                  /* passing by reference macros */
#include "descr.h"                /* passing descriptors for strings */
#include "process_parm.h"         /* SELF_ID */
#include "bpmparamv.h"            /* MAX_DGRP_BPMS */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpm_buff_common.h"      /* MAX_BUFF_MEAS  */
#include "bpm_list_strucs.h"      /* bpm_clist_header_struc */
#include "bpm_ulist_struc.h"      /* struc bpm_ulist_struc */
#include "bpmstruc.hc"            /* bpms_data_ts - main bpm data structure*/
#include "bpmstat.hc"             /* HSTA_XONLY, HSTA_YONLY */
#include "wire_req_struc.h"       /* For wire_req_struc */
#include "status.hc"              /* STAT_GOOD, STAT_OK */
#include "bpmd_hsta.h"            /* BPMD_HSTA_STORED_BEAM_FID */
#include "bpmdif_bits.h"          /* Measbits, eg BDF_NODIFF */
#include "bpmutil_proto.h"        /* bpm utility prototypes */
#include "msg_proto.h"            /* for general_init */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "err_proto.h"            /* for err_translate */
#include "err_facility_p.h"
#include "dpslcbpm.h"             /* Borrow from suite include file of dpslcbpm */
#include "dpslcbuff.h"            /* Suite include file */

#define WS_PUID_USER_DATA_BYTES 4 /* size of each pulse id */
#define AIDA_CLIENT_TIMEOUT     30 /* The time, in seconds, after which an AIDA client will timeout */
#define CLIENT_TIMEOUT_EPS      10 /* Time in seconds margin for error when checking suff rate.
                                      This is large because there is a big margin given by the
                                      BPM sw for teh micros to "prep". So roughly, @30 Hz, nrpos = 600
                                      seems ok, but nrpos = 700 is not. 30Hz * (30-10) = 600. */

#define nDEBUG 1

static const char* UNABLETOCOPYPULSEIDDATA = "Unable to copy pulseid data out of acq structure.";
static const char* UNABLETOCOPYBPMDATA = "Unable to copy bpm data out of acq structure.";
static const char* UNABLETOALLOCMEM = "Unable to allocate memory in DPSLCBUFFACQ_ACQINIT";
static const char* UNABLETOACQPULSEDATA = "Unable to acquire pulse data, 0 rows acquired";
static const char* MAXFEASNRPOS_MSG =
		"At %d rate, AIDA will timeout (%d sec) before %d have been acquired. Try NRPOS <= %d";

static const int2u ROGUE_TYPE = 0;
static const int2u BPM_TYPE = 1;
static const int2u TORO_TYPE = 2;
static const int2u KLYS_TYPE = 3;
static const int2u SBST_TYPE = 4;
static const int2u GAPM_TYPE = 5;
static const int2u PRIM_LEN = 4;
/* static const char* BPMS_PRIM = "BPMS";
static const char* TORO_PRIM = "TORO";
static const char* KLYS_PRIM = "KLYS";
static const char* SBST_PRIM = "SBST";
*/

/*
** Statics.
*/
static const $DESCRIPTOR( process_name,
"AIDA_DPSLCBUFF");

static BOOL development_node = FALSE_F;   /* Will be set true if the service executable
					     is found to be running on a production node.
					     Used to device whether to return real
					     or fake data */

static DEVICE_NAME_TS* names_as = NULL;   /* The BPMs to acquire */
static int4u Mdevices = 0;                /* Num of elements in names_as */
static int4u Mrows = 0;                   /* Rows in returned table, Mbpm x NRPOS */

static BUFF_DATA_TABLE_TS* buff_data_as = NULL;

/* Static Protos */
static void issue_dbg(char* message);
static void issue_err(char* message);

vmsstat_t DPSLCBUFF_INIT()
{
	REF_DECLARE;
	DESCR_DECLARE;
	vmsstat_t status;
	const struct msginit msg_init_s = { 1,    /* init msg service */
										1 };  /* init slcnet */

	/* Join the SLC control system */
	status = standalone_init(&process_name,

			REFINT4U_1(TRUE),
			&msg_init_s,
			REFINT4U_3(FALSE),
			REFINT4U_4(TRUE));
	if (!$VMS_STATUS_SUCCESS(status))
		fprintf(stderr, "\n\nDpslcbuff Standalone_init failed\n");

	DPSLCBUFF_SETMODE();

	return status;
}

void DPSLCBUFF_SETMODE()
{
	DESCR_DECLARE;

	/*
         * Allow second entry to allow 2nd executable to override.
	 * Which node are we on, prod or dev? If dev, we're going to avoid
	 * trying to get real data later.
	 */
	char node_type[12];                /* "DEVELOPMENT" or "PRODUCTION" */
	slc_node(DESCRA1(node_type));     /* Acquires node type */
	development_node =
			(BOOL)(strncmp(node_type, VAX_DEV_NAME, strlen(VAX_DEV_NAME)) == 0);
}

vmsstat_t DPSLCBUFF_ACQINIT(int4u Mdevices, int4u nPulses)
{
	names_as = malloc(MAX_DGRP_BPMS * MAX_BUFF_MEAS);
	if (names_as == NULL) {
		fprintf(stderr, UNABLETOALLOCMEM);
		return OP_BAD_ALLOC;
	}

	buff_data_as = (BUFF_DATA_TABLE_TS*)
			malloc(Mdevices * nPulses * sizeof(BUFF_DATA_TABLE_TS));
	if (buff_data_as == NULL) {
		fprintf(stderr, UNABLETOALLOCMEM);
		return OP_BAD_ALLOC;
	}

	return OP_OKOK;
}

/*
** Name:  DPSLCBUFF_ACQ_WITH_TIMEOUT
**
** Abs: Acquires buffered "bpm" (or any other pulse-by-pulse device) data over
**      a number of pulses. This is the primary funciton of Dpslcbuff service.
**
** Args:   Mdevices              Input. Number of elements of bpms to read
**         deviceNames_as        Input. Names of devices to acquire.
**         dgrp_name_c           Input. Name of DGRP that was in the AIDA query. It
**                               must be found to match the BPMD following. There is a
**                               m:1 relation between BPMD and dgrp (each bpmd implies
**                               1 dgrp, but >1 bpmd may be implied by a dgrp).
**         bpmd                  Input. The measurement definition under whch the
**                               pulse data acq should be made.
**         Npulses               Input. The number of pulses over which the acq
**                               should be made.
**         timeout               Input. The timeout value
*/
vmsstat_t DPSLCBUFF_ACQ_WITH_TIMEOUT(
		const int4u Mdevices,
		const DEVICE_NAME_TS deviceNames_as[MAX_DGRP_BPMS],
		const char* dgrp_name_c,
		const int4u bpmd,
		const int4u nPulses,
		const int4u timeoutSecs
)
{
	REF_DECLARE;
	DESCR_DECLARE;
	vmsstat_t iss = BPMO_OKOK;                    /* Status */
	char err_c[BUFSIZ] = { 0 };                   /* Error text buff, initialized to rogue val */
	char bpmd_dgrp_name[DGRPNAME_LEN];          /* Name of dgrp implied by BPMD */
	struct bpm_ulist_struc ulist_s = { 0 };         /* Struct used by BPM API */
	struct wire_req_struc wire_req_s = { 0 };

	ulist_s.max_len = WORDS(ulist_s);
	ulist_s.sorted = 0;
	ulist_s.curr_len = 0;

#if (DEBUG)
	fprintf(stderr,"Args: bpmd %d\tdgrp_name_c %s\t nPulses %d",
		bpmd, dgrp_name_c, nPulses);
#endif
	/*
	 * Check that the given dgrp goes with the given bpmd
	 */
	iss = bpmd_dgrp(bpmd, (unsigned long*)bpmd_dgrp_name);
	issue_dbg("After bpmd_dgrp call");
	if (!$VMS_STATUS_SUCCESS(iss)) goto egress;
	if (strncmp(dgrp_name_c, bpmd_dgrp_name, strlen(dgrp_name_c)) != 0) {
		iss = OP_BADARGS;
		sprintf(err_c, "Given BPMD %d does not match given DGRP", bpmd);
		goto egress;
	}

	issue_dbg("After bpmd_dgrp call");
	if (!development_node) {
		/*
		 * Select the given bpmd, so proceeding acqs are on that bpmd.
		 * NOTE: Don't call bpm_select_util only if the BPMD necessary
		 * for the last acquisition was different to this acq (using
		 * what_is_selected_bpmd).  Experientially, that seems to have
		 * lead to bpm acq timeouts, which we hypothesize were caused by
		 * the "meas prep" conducted by bpm_acq after a reselection not
		 * being done. That is, the following block was only done if
		 */
		int2 bpmd_tmp = bpmd;
		iss = bpmd_select_util(&bpmd_tmp, NULL, NULL, NULL, NULL, NULL);
		if (!$VMS_STATUS_SUCCESS(iss)) {
			sprintf(err_c,
					"Bad status from BPMD Selection, valid BPMD %d?", bpmd);
			issue_err(err_c);
			goto egress;
		}


		/*
		 * Check there is beam rate on this bpmd measurement definition.
                 */
		float rate = 0;
		iss = bpm_get_beam_rate(&rate);
		if (!$VMS_STATUS_SUCCESS(iss)) goto egress;
		if (rate <= 0.0) {
			iss = BPMO_NORATE;
			goto egress;
		}
		/*
		 * Check the number of acquisitions requested will not result in AIDA client
		 * timeout at this rate.
		 */
		int maxPulsesFeasible = rate * (timeoutSecs - CLIENT_TIMEOUT_EPS);
		if (nPulses > maxPulsesFeasible) {
			iss = BPMO_LOWRATE;
			sprintf(err_c,
					MAXFEASNRPOS_MSG,
					rate, timeoutSecs, nPulses, maxPulsesFeasible);
			issue_err(err_c);
			goto egress;
		}

		/*
		 * Fill in the BPM ulist - the list of bpms for which to make
		 * the acq, in the API of the Buffered Acq software, then sort
		 * the list prior to making the acq.
                 */
		for (int i = 0; i < Mdevices; i++) {
			iss = bpm_unit_list(&ulist_s,
					(int4u*)deviceNames_as[i].prim_s._a,
					(int4u*)deviceNames_as[i].micr_s._a,
					&deviceNames_as[i].unit_s._i,
					NULL,
					bpmd_dgrp_name);
			if (!$VMS_STATUS_SUCCESS(iss)) goto egress;
		}
		if (!$VMS_STATUS_SUCCESS(bpm_sort_list(&ulist_s)))
			goto egress;

		/*
		 *  Set up the so called "wire req", that specifies the
		 *  params of the buffered acq
		 */
		wire_req_s.nrpos = nPulses;
		wire_req_s.n_avg = 1;
		wire_req_s.pulsid_flag = TRUE_FORTRAN;
		wire_req_s.wire_unit = 0;
		memset(&wire_req_s.wire_micro, ' ', 4);
		memset(&wire_req_s.scan_mode, ' ', 4);
		memset(&wire_req_s.type, ' ', 4);
		wire_req_s.mag_setting_supplied = FALSE_FORTRAN;

		/* Ok, do the acq */
		iss = wire_acq(&wire_req_s, &ulist_s, NULL);
		if (!$VMS_STATUS_SUCCESS(iss)) {
			sprintf(err_c,
					"Unsuccessful wire_acq on BPMD %d for %d pulses",
					bpmd, nPulses);
			issue_err(err_c);
			goto egress;
		}

		/*
		 * copy out the Pulse_id data. We only need to do this once, because the
		 * pulse-ids will be the same for all bpms.
		 */
		ws_puid_data_ts raw_puid_buff_as[MAX_BUFF_MEAS];
		iss = wire_copy_data(raw_puid_buff_as,
				&((const unsigned long)('RPID')),
				(const unsigned long*)deviceNames_as[0].micr_s._a,
				REFINT4_1(1),
				REFINT4_2(sizeof(ws_puid_data_ts) * MAX_BUFF_MEAS));
		if (!$VMS_STATUS_SUCCESS(iss)) {
			issue_err(UNABLETOCOPYPULSEIDDATA);
			goto egress;
		}

		/*
		 * Copy the data out for each bpm
		 */
		int irow = 0;
		ws_bpm_data_ts bpms_buff_as[MAX_BUFF_MEAS];
		ws_toro_data_ts toro_buff_as[MAX_BUFF_MEAS];
		ws_klys_data_ts klys_buff_as[MAX_BUFF_MEAS];
		ws_sbst_data_ts sbst_buff_as[MAX_BUFF_MEAS];
		ws_gapm_data_ts gapm_buff_as[MAX_BUFF_MEAS];

		for (int iDevice = 0; iDevice < Mdevices; iDevice++) {
			int devType = ROGUE_TYPE;
			if (deviceNames_as[iDevice].prim_s._i == BPMS_PRIM)
				devType = BPM_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == TORO_PRIM)
				devType = TORO_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == KLYS_PRIM)
				devType = KLYS_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == SBST_PRIM)
				devType = SBST_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == GAPM_PRIM)
				devType = GAPM_TYPE;

			if (devType == BPM_TYPE) {
				/* Get the buffered data (all pulses) for each bpm */
				iss = wire_copy_data(bpms_buff_as,
						(const unsigned long*)deviceNames_as[iDevice].prim_s._a,
						(const unsigned long*)deviceNames_as[iDevice].micr_s._a,
						REFINT4_1(deviceNames_as[iDevice].unit_s._i),
						REFINT4_2(sizeof(ws_bpm_data_ts) * MAX_BUFF_MEAS));
				if (!$VMS_STATUS_SUCCESS(iss)) {
					issue_err(UNABLETOCOPYBPMDATA);
					goto egress;
				}

				/* Iterate through the data for the Bpm acqured above, adding a row to
				   the output table for each pulse */
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid =
							raw_puid_buff_as[jPulse].pulseid;
					buff_data_as[irow].data_s.x = bpms_buff_as[jPulse].x;
					buff_data_as[irow].data_s.y = bpms_buff_as[jPulse].y;
					buff_data_as[irow].data_s.tmit = bpms_buff_as[jPulse].tmit;
					buff_data_as[irow].data_s.stat = bpms_buff_as[jPulse].stat;
					buff_data_as[irow].data_s.goodmeas = bpms_buff_as[jPulse].goodmeas;
					irow++;

				} /* For each pulse */
			} /* if BPM type */

			else if (devType == TORO_TYPE) {
				/* Get the buffered data (all pulses) for each toroid */
				iss = wire_copy_data(toro_buff_as,
						(const unsigned long*)deviceNames_as[iDevice].prim_s._a,
						(const unsigned long*)deviceNames_as[iDevice].micr_s._a,
						REFINT4_1(deviceNames_as[iDevice].unit_s._i),
						REFINT4_2(sizeof(ws_toro_data_ts) * MAX_BUFF_MEAS));
				if (!$VMS_STATUS_SUCCESS(iss)) {
					issue_err(UNABLETOCOPYBPMDATA);
					goto egress;
				}

				/* Iterate through the data for the Bpm acqured above, adding a row to
				   the output table for each pulse */
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid =
							raw_puid_buff_as[jPulse].pulseid;
					buff_data_as[irow].data_s.x = 0.0;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = toro_buff_as[jPulse].tmit;
					buff_data_as[irow].data_s.stat = toro_buff_as[jPulse].stat;
					buff_data_as[irow].data_s.goodmeas = toro_buff_as[jPulse].goodmeas;
					irow++;

				} /* For each pulse */

			} /* if TORO type */

			else if (devType == KLYS_TYPE) {
				/* Get the buffered data (all pulses) for each klys */
				iss = wire_copy_data(klys_buff_as,
						(const unsigned long*)deviceNames_as[iDevice].prim_s._a,
						(const unsigned long*)deviceNames_as[iDevice].micr_s._a,
						REFINT4_1(deviceNames_as[iDevice].unit_s._i),
						REFINT4_2(sizeof(ws_klys_data_ts) * MAX_BUFF_MEAS));
				if (!$VMS_STATUS_SUCCESS(iss)) {
					issue_err(UNABLETOCOPYBPMDATA);
					goto egress;
				}

				/* Iterate through the data for the device acqured above, adding a row to
				   the output table for each pulse */
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid =
							raw_puid_buff_as[jPulse].pulseid;
					buff_data_as[irow].data_s.x = klys_buff_as[jPulse].phas;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = klys_buff_as[jPulse].stat;
					buff_data_as[irow].data_s.goodmeas = 0;
					irow++;

				} /* For each pulse */

			} /* if KLYS type */

			else if (devType == SBST_TYPE) {
				/* Get the buffered data (all pulses) for each sbst */
				iss = wire_copy_data(sbst_buff_as,
						(const unsigned long*)deviceNames_as[iDevice].prim_s._a,
						(const unsigned long*)deviceNames_as[iDevice].micr_s._a,
						REFINT4_1(deviceNames_as[iDevice].unit_s._i),
						REFINT4_2(sizeof(ws_sbst_data_ts) * MAX_BUFF_MEAS));
				if (!$VMS_STATUS_SUCCESS(iss)) {
					issue_err(UNABLETOCOPYBPMDATA);
					goto egress;
				}

				/* Iterate through the data for the device acqured above, adding a row to
				   the output table for each pulse */
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid =
							raw_puid_buff_as[jPulse].pulseid;
					buff_data_as[irow].data_s.x = sbst_buff_as[jPulse].phas;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = sbst_buff_as[jPulse].stat;
					buff_data_as[irow].data_s.goodmeas = 0;
					irow++;

				} /* For each pulse */

			} /* if SBST type */

			else if (devType == GAPM_TYPE) {
				/* Get the buffered data (all pulses) for each gapm */
				iss = wire_copy_data(gapm_buff_as,
						(const unsigned long*)deviceNames_as[iDevice].prim_s._a,
						(const unsigned long*)deviceNames_as[iDevice].micr_s._a,
						REFINT4_1(deviceNames_as[iDevice].unit_s._i),
						REFINT4_2(sizeof(ws_gapm_data_ts) * MAX_BUFF_MEAS));
				if (!$VMS_STATUS_SUCCESS(iss)) {
					issue_err(UNABLETOCOPYBPMDATA);
					goto egress;
				}

				/* Iterate through the data for the device acqured above, adding a row to
				   the output table for each pulse */
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid =
							raw_puid_buff_as[jPulse].pulseid;
					buff_data_as[irow].data_s.x = gapm_buff_as[jPulse].data;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = gapm_buff_as[jPulse].stat;
					buff_data_as[irow].data_s.goodmeas = gapm_buff_as[jPulse].goodmeas;
					irow++;

				} /* For each pulse */

			} /* if GAPM type */

		} /* For each device */

		/* Record the number of rows in output array of data structures, buff_data_as */
		Mrows = irow;

	} /* end on production. */
	else /* on development */
	{
		/* seed a pseduo-pulseid just for fake data. */
		srand(time(NULL));
		int ppulseid0 = 10000 + rand() % 90000;

		/* Create fake data, for each BPM, for each pusle id adding a row to
		   the output table for each pulse */
		int irow = 0;

		for (int iDevice = 0; iDevice < Mdevices; iDevice++) {

			int devType = ROGUE_TYPE;
			if (deviceNames_as[iDevice].prim_s._i == BPMS_PRIM)
				devType = BPM_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == TORO_PRIM)
				devType = TORO_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == KLYS_PRIM)
				devType = KLYS_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == SBST_PRIM)
				devType = SBST_TYPE;
			else if (deviceNames_as[iDevice].prim_s._i == GAPM_PRIM)
				devType = GAPM_TYPE;

			if (devType == BPM_TYPE) {
				int ppulseid = ppulseid0;
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid = ppulseid++;
					buff_data_as[irow].data_s.x = sin((float)rand());
					buff_data_as[irow].data_s.y = cos((float)rand());
					buff_data_as[irow].data_s.tmit = 1.0e-10;
					buff_data_as[irow].data_s.stat = 0x0001;
					buff_data_as[irow].data_s.goodmeas = 0x000F;
					irow++;

				} /* For each pulse */
			} /* end if BPM type */

			else if (devType == TORO_TYPE) {
				int ppulseid = ppulseid0;
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid = ppulseid++;
					buff_data_as[irow].data_s.x = 0.0;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 1.0e-10 + (rand() / (float)RAND_MAX) / 1.0e-11;
					buff_data_as[irow].data_s.stat = 0x0001;
					buff_data_as[irow].data_s.goodmeas = 0x000F;
					irow++;

				} /* For each pulse */
			} /* end if TORO type */

			else if (devType == KLYS_TYPE) {
				int ppulseid = ppulseid0;
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid = ppulseid++;
					buff_data_as[irow].data_s.x = 180.0 + rand() / (float)RAND_MAX;;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = 0x0001;
					buff_data_as[irow].data_s.goodmeas = 0x000F;
					irow++;

				} /* For each pulse */

			} /* if KLYS type */

			else if (devType == SBST_TYPE) {
				int ppulseid = ppulseid0;
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid = ppulseid++;
					buff_data_as[irow].data_s.x = 90.0 + random() / (float)RAND_MAX;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = 0x0001;
					buff_data_as[irow].data_s.goodmeas = 0x000F;
					irow++;

				} /* For each pulse */
			} /* end if SBST */
			else if (devType == GAPM_TYPE) {
				int ppulseid = ppulseid0;
				for (int jPulse = 0; jPulse < nPulses; jPulse++) {
					sprintf(buff_data_as[irow].devicename_c, "%4.4s:%4.4s:%d",
							deviceNames_as[iDevice].prim_s._a,
							deviceNames_as[iDevice].micr_s._a,
							deviceNames_as[iDevice].unit_s._i);
					buff_data_as[irow].pulseData_s.pulseid = ppulseid++;
					buff_data_as[irow].data_s.x = random() / (float)RAND_MAX;
					buff_data_as[irow].data_s.y = 0.0;
					buff_data_as[irow].data_s.tmit = 0.0;
					buff_data_as[irow].data_s.stat = 0x0001;
					buff_data_as[irow].data_s.goodmeas = 0x000F;
					irow++;

				} /* For each pulse */
			} /* end if GAPM */
		} /* For each device */

		/* Record the number of rows in output array of data structures, buff_data_as */
		Mrows = irow;

	} /* on development */

egress:
	/* Sanity check the number of rows output, and if
	   necessary set a generic return status */
	if (Mrows == 0) {
		char msg_c[BUFSIZ];
		if ($VMS_STATUS_SUCCESS(iss)) iss = OP_BADDATA;
		if (err_c[0] != 0)
			sprintf(msg_c, "%s: %s", UNABLETOACQPULSEDATA, err_c);
		else
			sprintf(msg_c, "%s", UNABLETOACQPULSEDATA);

		issue_err(msg_c);
	}

	return iss;
}

/*
** Name:  DPSLCBUFF_ACQ
**
** Abs: Acquires buffered "bpm" (or any other pulse-by-pulse device) data over
**      a number of pulses. This is the primary funciton of Dpslcbuff service.
**
** Args:   Mdevices              Input. Number of elements of bpms to read
**         deviceNames_as        Input. Names of devices to acquire.
**         dgrp_name_c           Input. Name of DGRP that was in the AIDA query. It
**                               must be found to match the BPMD following. There is a
**                               m:1 relation between BPMD and dgrp (each bpmd implies
**                               1 dgrp, but >1 bpmd may be implied by a dgrp).
**         bpmd                  Input. The measurement definition under whch the
**                               pulse data acq should be made.
**         Npulses               Input. The number of pulses over which the acq
**                               should be made.
*/
vmsstat_t DPSLCBUFF_ACQ(
		const int4u Mdevices,
		const DEVICE_NAME_TS deviceNames_as[MAX_DGRP_BPMS],
		const char* dgrp_name_c,
		const int4u bpmd,
		const int4u nPulses
)
{
	return DPSLCBUFF_ACQ_WITH_TIMEOUT(Mdevices, deviceNames_as, dgrp_name_c, bpmd, nPulses, AIDA_CLIENT_TIMEOUT);
}

vmsstat_t DPSLCBUFF_ACQTERM(void)
{
	free(names_as);
	free(buff_data_as);

	return OP_OKOK;
}

int4u DPSLCBUFF_GETTABLEMROWS()
{
	issue_dbg("Entered DPSLCBUFF_GETTABLEMROWS");
	issue_dbg("Exit DPSLCBUFF_GETTABLEMROWS");
	return Mrows;

}

int4u DPSLCBUFF_GETNAMES(char* names_ac[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	issue_dbg("Entered DPSLCBUFF_GETNAMES");
	for (int irow = 0; irow < Mrows; irow++)
		names_ac[irow] = buff_data_as[irow].devicename_c;
	issue_dbg("Exit DPSLCBUFF_GETNAMES");
	return Mrows;
}

int4u DPSLCBUFF_GETPULSEIDS(unsigned long pulseids_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		pulseids_a[irow] = buff_data_as[irow].pulseData_s.pulseid;
	return Mrows;
}

int4u DPSLCBUFF_GETXS(float xs_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		xs_a[irow] = buff_data_as[irow].data_s.x;
	return Mrows;
}

int4u DPSLCBUFF_GETYS(float ys_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		ys_a[irow] = buff_data_as[irow].data_s.y;
	return Mrows;
}

int4u DPSLCBUFF_GETTMITS(float tmits_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		tmits_a[irow] = buff_data_as[irow].data_s.tmit;
	return Mrows;
}

int4u DPSLCBUFF_GETSTATS(unsigned short stats_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		stats_a[irow] = buff_data_as[irow].data_s.stat;
	return Mrows;
}

int4u DPSLCBUFF_GETGOODMEASES(unsigned short goodmeases_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS])
{
	for (int irow = 0; irow < Mrows; irow++)
		goodmeases_a[irow] = buff_data_as[irow].data_s.goodmeas;
	return Mrows;
}

static void issue_dbg(char* message)
{
#if (DEBUG)
	fprintf(stderr, message );
	fprintf(stderr, "\n");
#endif
}

static void issue_err(char* message)
{
	DESCR_DECLARE;
	REF_DECLARE;
	char msg_c[BUFSIZ];

	strcpy(msg_c, "DpSlcBuff: ");            /* Prepend Aida process name to msg */
	strcat(msg_c, message);                 /* Add passed in message */
	err_text(REFINT4U_1(OP_MSGTXT), DESCRN1(msg_c), &process_name);
}

#if (DEBUG)
/*=============================================================================

  Abs:  A main for dpslcbuff, allows you to run it standalone (AIDA not
		connected) for testing.

  Name: main

  Args: void

  Rem:  See module header for instructions.

  Side: None

  Ret:  0
==============================================================================*/

static int main()
{
	vmsstat_t iss = MODL_OKOK;
	int4u bpmd = 0;
	char dgrp_c[DGRPNAME_LEN+1];
	DEVICE_NAME_TS bpmnames_as[MAX_DGRP_BPMS];
	int4 nrpos = 0;

	/*
	 * Data initialization
	 */
	int4u MbpmIn = 3;
	char* bpms_ac[3] = {"BPMS:LI18:701", "BPMS:LI19:701", "GAPM:LI18:930" };
	for (int j=0;j<3;j++)
	{
		strcpy(bpmnames_as[j].prim_s._a, strtok(bpms_ac[j], ":"));
		strcpy(bpmnames_as[j].micr_s._a, strtok(NULL, ":"));
		bpmnames_as[j].unit_s._i = atoi(strtok(NULL,":"));
	}
	bpmd=57; /* NDRFACET */
	strcpy(dgrp_c,"NDRFACET");

	printf("Enter nrpos\n");
	/* scanf("%s %d %d", &dgrp_c, &bpmd, &nrpos ); */
	scanf("%d", &nrpos );

	/*
	 * Acquisition
	 */
	DPSLCBUFF_INIT();
	iss = DPSLCBUFF_ACQINIT( MbpmIn, nrpos );
	if (! $VMS_STATUS_SUCCESS(iss) )
	{
		printf("Error from acq init  - check err log");
		exit(1);
	}

	iss = DPSLCBUFF_ACQ(MbpmIn, bpmnames_as, dgrp_c, bpmd, nrpos );

	/*
	 * Data retrieval and printing
	 */
	if ( !$VMS_STATUS_SUCCESS(iss) )
	{
		printf("Error in DPSLCBUFF_ACQ");
		return iss;
	}
	else
	{
		int4u Mrows = 0;
		char *names_col_apc[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		unsigned long pulseids_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		float xs_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		float ys_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		float tmits_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		unsigned short stats_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];
		unsigned short goodmeases_col_a[MAX_DGRP_BPMS*MAX_BUFF_MEAS];

		Mrows = DPSLCBUFF_GETTABLEMROWS();
		DPSLCBUFF_GETNAMES( names_col_apc );
		DPSLCBUFF_GETPULSEIDS( pulseids_col_a );
		DPSLCBUFF_GETXS( xs_col_a );
		DPSLCBUFF_GETYS( ys_col_a );
		DPSLCBUFF_GETTMITS( tmits_col_a );
		DPSLCBUFF_GETSTATS (stats_col_a);
		DPSLCBUFF_GETGOODMEASES( goodmeases_col_a );


		printf("Device Name\tP ID\tX\tY\tTMIT\tSTAT\tGOOD\n");
		for (int i = 0; i<Mrows; i++)
		{
		printf("%s\t%d\t%g\t%g\t%g\t%4.4x\t%4.4x\n",
			   names_col_apc[i],
			   pulseids_col_a[i],
			   xs_col_a[i],
			   ys_col_a[i],
			   tmits_col_a[i],
			   stats_col_a[i],
			   goodmeases_col_a[i]);
		}
	}

	/*
	 * Termination
	 */
	iss = DPSLCBUFF_ACQTERM();
	return iss;
}
#endif
