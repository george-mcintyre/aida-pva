/*
        **CMS**=C_INC
==============================================================================

  Abs:  Module specific include file for DPSLCBUFF* modules. Those modules
        define the Aida Buffered BPM Data Provider.

  Name: DPSLCBUFF.H

  Rem:  See http://www.slac.stanford.edu/grp/cd/soft/aida/slcBpmDataProvider.html

  Prev: slctypes.h

  Auth: 05-Apr-2011, Greg White
  Rev:

--------------------------------------------------------------------------------

  Mod:
        DD-MMM-YYYY, My Name:
           Changed such and such to so and so. etc. etc.

==============================================================================*/

#ifndef DPSLCBUFF_H
#define DPSLCBUFF_H

/*
** Typed Data Definitions
*/
struct device_name_s
{
	prim_name_ts prim_s;
	micr_name_ts micr_s;
	unit_name_ts unit_s;
};
typedef struct device_name_s DEVICE_NAME_TS;

struct buff_data_table_s
{
	char devicename_c[NAME_SIZE + 1];  /* BPM name in prim:micro:unit syntax, null term */
	ws_puid_data_ts pulseData_s;        /* long, pulse id */
	ws_bpm_data_ts data_s;          /* The buffered data for this pulse */
};
typedef struct buff_data_table_s BUFF_DATA_TABLE_TS;

/*
** External Functions Defined
*/

vmsstat_t DPSLCBUFF_INIT(void);

void DPSLCBUFF_SETMODE(void);

vmsstat_t DPSLCBUFF_ACQINIT(int4u Mdevices, int4u nPulses);

vmsstat_t DPSLCBUFF_ACQ(
		const int4u Mdevices,
		const DEVICE_NAME_TS deviceNames_as[MAX_DGRP_BPMS],
		const char* dgrp_name_c,
		const int4u bpmd,
		const int4u nPulses
);

vmsstat_t DPSLCBUFF_ACQ_WITH_TIMEOUT(
		const int4u Mdevices,
		const DEVICE_NAME_TS deviceNames_as[MAX_DGRP_BPMS],
		const char* dgrp_name_c,
		const int4u bpmd,
		const int4u nPulses,
		const int4u timeout
);

vmsstat_t DPSLCBUFF_ACQTERM(void);

int4u DPSLCBUFF_GETTABLEMROWS(void);

int4u DPSLCBUFF_GETNAMES(char* names_ac[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETPULSEIDS(unsigned long pulseids_ap[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETXS(float xs_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETYS(float ys_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETTMITS(float tmits_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETSTATS(unsigned short stats_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

int4u DPSLCBUFF_GETGOODMEASES(unsigned short goodmeases_a[MAX_DGRP_BPMS * MAX_BUFF_MEAS]);

#endif

