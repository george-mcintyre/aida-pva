/*
      **MEMBER**=SLCLIBS:AIDASHRLIB
================================================================================

  Abs:  Java JNI interface helper routines so we can compile with normal
        options and not deal with the case issue for the Java* interface
        routines.for the Aida dpSlc data provider. This can be
        compiled with our normal cmp command. Called by first level routine
        in dpslc_jni.c

  Name: dpslc_jni_helper.c

  Debug: To compile and run as a standalone for debugging,
        1. Change the #define DEBUG 0 to #define DEBUG 1
            and compile, link and run as follows:
        2. MCCDEV> define slctxt [], slc_txt
        3. MCCDEV> c dpslc_jni_helper.c
        Use a dpslcbuff_jni_helper.opt, derived from aidashr.opt, but adding
        two lines:
            at the top put just "DPSLCBUFFACQ_JNI_HELPER"
            and add "aidashr/shareable"
        5. MCCDEV> lkdev dpslc_jni_helper dpslc_jni_helper.opt/opt map deb
        6. MCCDEV> run/debug dpslc_jni_helper.dev

  Proto: None

  Auth: 05-Nov-2002, Robert C. Sass
  Rev:  DD-MMM-YYYY, Reviewer's Name (.NE. Author's Name)

--------------------------------------------------------------------------------

  Mod:  (newest to oldest)
        21-Sep-2022, George McIntyre (sly)
           Added routine JNI_DBSETINT.
        04-Oct-2007, Bob Hall (rdh)
           Added routines JNI_DBACCESSENABLED and JNI_DBSETFLOAT.
        05-Sep-2007, Bob Hall (rdh)
           Enclosed debug printf statements with "#ifdef DEBUG"
           and "#endif".
        08-Jun-2006, Bob Hall (rdh)
           Added processing for returning meaningful strings
           for ASTS the channel pseudo-secondaries CTRL, STAT,
           and VSTA.
        06-Apr-2006, Bob Hall (rdh)
           Added support for ASTS channel name processing.
        14-Oct-2004, Bob Hall (rdh)
           Implemented many previously unimplemented functions.

==============================================================================*/

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <string.h>               /* strlen memcpy */
#include <stdlib.h>               /* malloc */
#include <ctype.h>                /* isalnum, ispunct */
#include <stsdef.h>               /* Macros for handling VMS status */
#include <ssdef.h>                /* VMS general status codes */

#include "opdef.hc"               /* General messages */
#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"            /* for standalone_init */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "err_proto.h"            /* for err_translate */
#include "dbgetc.h"               /* useful database macros and prototypes */
#include "db_types.hc"            /* for db_name_ta */
#include "ref.h"                  /* Pass by ref to fortran helpers */
#include "descr.h"                /* for pass by descr macros */
#include "util_proto.h"           /* asts_color */
#include "err_facility_p.h"

#define nDEBUG 1
#define MAIN_DEBUG 0

/*
 * External proto
 */
int chk_aida_access(const struct dsc$descriptor*,
		int*,
		int*);

/*
 * Local proto
 */
static void issue_err(char* message);
static vmsstat_t LCL_JNI_PARSENAME(char* name, int4u* prim_pi, int4u* micr_pi,
		int4u* lunit_pi, int4u* secn_pi,
		char* asts_channel_name);

/*
 * Local Static
 */
static const $DESCRIPTOR( process_name,
"AidaDbIf");
static BOOL development_node = FALSE_F;   /* Will be set true if the service exe
                                             is found to be running on a product
                                             Used to device whether to return re
                                             or fake data */

/*
 * Global external Function definitions
 */
vmsstat_t DB_INIT()
{
	vmsstat_t status;
/*---------------------------------------------------*/
	status = standalone_init(&process_name, &((long)(TRUE)),
			NULL, &((long)(FALSE)), &((long)(FALSE)));
	return status;
}

vmsstat_t JNI_DBGETBYTE(const char* pvname, unsigned char* retbyte)
{
	vmsstat_t status;
	DBLIST(byte_ps, unsigned char);
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(byte_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
#ifdef DEBUG
		printf("asts_channel_name = %s\n", asts_channel_name);
#endif

		status = DBget_astsC((void*)&byte_ps,
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}
	} else {
		if (!SUCCESS(status = DBgetC(((void*)(&byte_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;
	}

	*retbyte = byte_ps->dat[0];
egress:
	DBFREE(byte_ps);
	return status;
}

vmsstat_t JNI_DBGETSHORT(const char* pvname, short int* retshort)
{
	vmsstat_t status;
	DBLIST(short_ps, short int);
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(short_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
#ifdef DEBUG
		printf("asts_channel_name = %s\n", asts_channel_name);
#endif

		status = DBget_astsC((void*)&short_ps,
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}
	} else {
		if (!SUCCESS(status = DBgetC(((void*)(&short_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;
	}

	*retshort = short_ps->dat[0];
egress:
	DBFREE(short_ps);
	return status;
}

vmsstat_t JNI_DBGETINT(const char* pvname, int* retint)
{
	vmsstat_t status;
	DBLIST(int_ps, int);
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(int_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
#ifdef DEBUG
		printf("asts_channel_name = %s\n", asts_channel_name);
#endif

		status = DBget_astsC((void*)&int_ps,
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}
	} else {
		if (!SUCCESS(status = DBgetC(((void*)(&int_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;
	}

	*retint = int_ps->dat[0];
egress:
	DBFREE(int_ps);
	return status;
}

vmsstat_t JNI_DBGETFLOAT(const char* pvname, float* retfloat)
{
	vmsstat_t status;
	DBLIST(analog_ps, float);
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	unsigned short one = 1;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(analog_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
#ifdef DEBUG
		printf("asts_channel_name = %s\n", asts_channel_name);
#endif

		status = DBget_astsC((void*)&analog_ps,
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}
	} else {
		if (!SUCCESS(status = DBgetC(((void*)(&analog_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;
	}

	*retfloat = analog_ps->dat[0];

#ifdef DEBUG
	printf ("Data in VMS from PV %s is %f\n", pvname, *retfloat);
#endif

	cvt_vms_to_ieee_flt(retfloat, retfloat, &one);
egress:
	DBFREE(analog_ps);
	return status;
}

vmsstat_t JNI_DBGETCHAR(const char* pvname, unsigned char* retchar)
{
	vmsstat_t status;
	DBLIST(char_ps, char);
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(char_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
#ifdef DEBUG
		printf("asts_channel_name = %s\n", asts_channel_name);
#endif

		status = DBget_astsC((void*)&char_ps,
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}
	} else {
		if (!SUCCESS(status = DBgetC(((void*)(&char_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;
	}

	*retchar = char_ps->dat[0];
egress:
	DBFREE(char_ps);
	return status;
}

vmsstat_t JNI_DBGETSTRING(const char* pvname, unsigned char** retstring)
{
	vmsstat_t status;
	DESCR_DECLARE; /* pass-by-descrip macros. */

	DBLIST(stringlist_ps, int4u);
	DBLIST(stringdata_ps, char);
	int i;
	int offset;
	int ssize;              /* String size */
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
	char color[9];
	char flag[2];
	char text[9];
/*---------------------------------------------------*/
	DBCLEAR(stringlist_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
		/*
		** If the pseudo-secondary is CTRL, STAT, or VSTS, call
		** asts_color to obtain the text, color, and flag
		** substrings from these bit patterns.
		*/
		if ((secn_i == 'CTRL') ||
				(secn_i == 'STAT') ||
				(secn_i == 'VSTA')) {
			status = asts_color(&micr_i,
					&unit_i,
					asts_channel_name,
					DESCRA1(color),
					DESCRA2(flag),
					DESCRA3(text),
					NULL);
			if (!SUCCESS(status)) {
				goto egress;
			}

			color[8] = '\0';
			flag[1] = '\0';
			text[8] = '\0';

#ifdef DEBUG
			printf("text = %s\n", text);
		   printf("color = %s\n", color);
		   printf("flag = %s\n", flag);
#endif

			/*
			** Allocate space for a string having a length of 18
			** characters.  First, fill this area with blanks.
			** Then copy the text substring into the string
			** starting at character 0, the color substring into
			** the string starting at character 9, and the flag
			** substring starting at character 18.  This layout
			** is assumed by the Java "get Any" method, which
			** calls this routine for ASTS channels with either
			** a CTRL, STAT, or VSTA pseudo-secondary.
			*/
			ssize = (sizeof(text) - 1) +
					(sizeof(color) - 1) +
					(sizeof(flag) - 1) +
					2;

#ifdef DEBUG
			printf("ssize = %d\n", ssize);
#endif

			*retstring = malloc(ssize + 1); /* Caller must free it */

			for (i = 0; i < ssize; i++) {
				*(*retstring + i) = ' ';
			}

			strncpy(*retstring, text, strlen(text));

			offset = (sizeof(text) - 1) + 1;
			strncpy(*retstring + offset, color, strlen(color));

			offset = offset + (sizeof(color) - 1) + 1;
			strcpy(*retstring + offset, flag);

			return status;
		} else {
			status = DBlist_astsC(((void*)(&stringlist_ps)),
					micr_i,
					unit_i,
					secn_i,
					asts_channel_name);
			if (!SUCCESS(status)) {
				goto egress;
			}

			DBCLEAR(stringdata_ps);

			if (!SUCCESS(status = DBlget_astsC(((void*)(&stringdata_ps)),
					((void*)(&stringlist_ps)))))
				goto egress;
		}
	} else {
		if (!SUCCESS(status = DBlistC(((void*)(&stringlist_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;

		DBCLEAR(stringdata_ps);

		if (!SUCCESS(status = DBlgetC(((void*)(&stringdata_ps)),
				((void*)(&stringlist_ps)))))
			goto egress;
	}

	ssize = DBCNT(stringdata_ps) * 2;

	*retstring = malloc(ssize + 1); /* Caller must free it */

	memcpy(*retstring, &stringdata_ps->dat[0], ssize + 1);

	*(*retstring + ssize) = '\0';  /* Null terminate the string */

egress:

	DBFREE(stringlist_ps);
	DBFREE(stringdata_ps);
	return status;
}

vmsstat_t JNI_DBGETBYTEA(const char* pvname, unsigned char** retbytea)
{
	vmsstat_t status;
	DBLIST(bytealist_ps, int4u);
	DBLIST(byteadata_ps, char);
	int ssize;              /* Bytea size */
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(bytealist_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
		status = DBlist_astsC(((void*)(&bytealist_ps)),
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}

		DBCLEAR(byteadata_ps);
		if (!SUCCESS(status = DBlget_astsC(((void*)(&byteadata_ps)),
				((void*)(&bytealist_ps)))))
			goto egress;
	} else {
		if (!SUCCESS(status = DBlistC(((void*)(&bytealist_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;

		DBCLEAR(byteadata_ps);
		if (!SUCCESS(status = DBlgetC(((void*)(&byteadata_ps)),
				((void*)(&bytealist_ps)))))
			goto egress;
	}

	ssize = DBCNT(byteadata_ps) * 2;
	*retbytea = malloc(ssize + 1); /* Caller must free it */
	memcpy(*retbytea, &byteadata_ps->dat[0], ssize + 1);
	*(*retbytea + ssize) = '\0';        /* Null terminate the string */
egress:
	DBFREE(bytealist_ps);
	DBFREE(byteadata_ps);
	return status;
}

vmsstat_t JNI_DBGETSHORTA(const char* pvname, short int** retshorta)
{
	vmsstat_t status;
	DBLIST(shortalist_ps, int4u);
	DBLIST(shortadata_ps, short int);
	int i;

	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(shortalist_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
		status = DBlist_astsC(((void*)(&shortalist_ps)),
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}

		DBCLEAR(shortadata_ps);
		if (!SUCCESS(status = DBlget_astsC(((void*)(&shortadata_ps)),
				((void*)(&shortalist_ps)))))
			goto egress;
	} else {
		if (!SUCCESS(status = DBlistC(((void*)(&shortalist_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;

		DBCLEAR(shortadata_ps);
		if (!SUCCESS(status = DBlgetC(((void*)(&shortadata_ps)),
				((void*)(&shortalist_ps)))))
			goto egress;
	}

	/* retshorta must be freed by the caller */
	*retshorta = (short int*)malloc((DBCNT(shortadata_ps) + 1) * sizeof(short int));

	/*
	 * Store the number of array values.
	 */
	*(*retshorta) = (short int)DBCNT(shortadata_ps);

#ifdef DEBUG
	printf("number of values = %d\n", *(*retshorta));
#endif

	/*
	 * For each array value, store the value.
	 */
	for (i = 0; i < DBCNT(shortadata_ps); i++) {
		*(*retshorta + i + 1) = shortadata_ps->dat[i];

#ifdef DEBUG
		printf("value[%d] = %d\n", i, *(*retshorta + i + 1));
#endif

	}

egress:
	DBFREE(shortalist_ps);
	DBFREE(shortadata_ps);
	return status;
}

vmsstat_t JNI_DBGETINTA(const char* pvname, int** retinta)
{
	vmsstat_t status;
	DBLIST(intalist_ps, int4u);
	DBLIST(intadata_ps, int);
	int i;

	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(intalist_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
		status = DBlist_astsC(((void*)(&intalist_ps)),
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}

		DBCLEAR(intadata_ps);
		if (!SUCCESS(status = DBlget_astsC(((void*)(&intadata_ps)),
				((void*)(&intalist_ps)))))
			goto egress;
	} else {
		if (!SUCCESS(status = DBlistC(((void*)(&intalist_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;

		DBCLEAR(intadata_ps);
		if (!SUCCESS(status = DBlgetC(((void*)(&intadata_ps)),
				((void*)(&intalist_ps)))))
			goto egress;
	}

	/* retinta must be freed by the caller */
	*retinta = (int*)malloc((DBCNT(intadata_ps) + 1) * sizeof(int));

	/*
	 * Store the number of array values.
	 */
	*(*retinta) = (int)DBCNT(intadata_ps);

#ifdef DEBUG
	printf("number of values = %d\n", *(*retinta));
#endif

	/*
	 * For each array value, store the value.
	 */
	for (i = 0; i < DBCNT(intadata_ps); i++) {
		*(*retinta + i + 1) = intadata_ps->dat[i];

#ifdef DEBUG
		printf("value[%d] = %d\n", i, *(*retinta + i + 1));
#endif
	}

egress:
	DBFREE(intalist_ps);
	DBFREE(intadata_ps);
	return status;
}

vmsstat_t JNI_DBGETFLOATA(const char* pvname, float** retfloata)
{
	vmsstat_t status;
	DBLIST(floatalist_ps, int4u);
	DBLIST(floatadata_ps, float);
	int i;
	unsigned short one = 1;

	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
/*---------------------------------------------------*/
	DBCLEAR(floatalist_ps);
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	if (strcmp(asts_channel_name, "        ") != 0) {
		status = DBlist_astsC(((void*)(&floatalist_ps)),
				micr_i,
				unit_i,
				secn_i,
				asts_channel_name);
		if (!SUCCESS(status)) {
			goto egress;
		}

		DBCLEAR(floatadata_ps);
		if (!SUCCESS(status = DBlget_astsC(((void*)(&floatadata_ps)),
				((void*)(&floatalist_ps)))))
			goto egress;
	} else {
		if (!SUCCESS(status = DBlistC(((void*)(&floatalist_ps)), prim_i, micr_i,
				unit_i, secn_i, NULL)))
			goto egress;

		DBCLEAR(floatadata_ps);
		if (!SUCCESS(status = DBlgetC(((void*)(&floatadata_ps)),
				((void*)(&floatalist_ps)))))
			goto egress;
	}

	/* retfloata must be freed by the caller */
	*retfloata = (float*)malloc((DBCNT(floatadata_ps) + 1) * sizeof(float));

	/*
	 * Store the number of array values.  This must be converted from
	 * VMS floating point format to IEEE format to return to Java and
	 * eventually the client.
	 */
	*(*retfloata) = (float)DBCNT(floatadata_ps);

#ifdef DEBUG
	printf("number of values = %f\n", *(*retfloata));
#endif

	cvt_vms_to_ieee_flt(*retfloata, *retfloata, &one);

	/*
	 * For each array value, store the value and convert to IEEE format.
	 */
	for (i = 0; i < DBCNT(floatadata_ps); i++) {
		*(*retfloata + i + 1) = floatadata_ps->dat[i];

#ifdef DEBUG
		printf("value[%d] = %f\n", i, *(*retfloata + i + 1));
#endif

		cvt_vms_to_ieee_flt(*retfloata + i + 1, *retfloata + i + 1, &one);
	}

egress:
	DBFREE(floatalist_ps);
	DBFREE(floatadata_ps);
	return status;
}

int JNI_DBACCESSENABLED()
{
	int status;
	int enabled_flag;
	int seconds_to_expire;

	char db_access_logical_name[] = "AIDA_ACCESS_SLCDATABASE";

	DESCR_DECLARE;

/*---------------------------------------------------*/

	status = chk_aida_access(DESCRN1(((void*)
					(db_access_logical_name))),
			&enabled_flag,
			&seconds_to_expire);

#ifdef DEBUG
	fprintf(stderr, "chk_aida_access enabled_flag = %d\n",
		enabled_flag);
#endif

	return enabled_flag;
}

/*==================================================================================
 *
 * Abs: Sets a given SLC database tuple (prim,micro,unit,secn) to the given float
 *      value or array.
 *
 * Name: JNI_DBSETFLOAT
 *
 * Args: pvname.   The SLC database tuple whose value to set to data. Given in
 *                 syntax "<primary>:<micro>:<unit>:<secondary>".
 *                 Null terminated string given by reference.
 *       data      The floating point data values to which to set the pvname.
 *                 The number of float elements of the data array must be exactly
 *                 equal to the length of the secondary. Ie, If where the
 *                 secondary is described as <n>R4 (eg 1R4), N must be the
 *                 length of data. Specifically, if the pvname identifies a 1R4
 *                 secondary, then only 1 float would be expected in this array.
 *                 A float array (given by reference).
 *       length    The number of elements in teh array data above. Must be
 *                 equal to the size of the DB secondary. See data arg above.
 *                 An integer given by value.
 *
 *================================================================================*/
vmsstat_t JNI_DBSETFLOAT(const char* pvname, float* data, int length)
{
	vmsstat_t status = 1;
	DBLIST(float_ps, float);
	int elemSize;             /* Size_t (bytes) of a single data element. */
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
	const int WORDS_TO_FLOATSIZE = 2;

#ifdef DEBUG
	printf("entering JNI_DBSETFLOAT\n");
	printf("slc pvname = %s\n", pvname);
	for (int t=0;t<length;t++) printf("data[%d]=%g\n",t,data[t]);
#endif


	/* Parse the given AIDA name (called pvname for some bizarre reason) into
	   the four parts that together form an SLC Db pv tuple (primary, micro, unit, secn).
	*/
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	/* Call DBputC to set the specified primary/micro/unit/
	   secondary value to the data given.
	*/
	elemSize = sizeof(float);
	DBalloc((void*)&float_ps, length, elemSize);
	DBCLEAR(float_ps);
	memcpy(&(float_ps->dat), data, length * elemSize);

	/* Set cur to the length of dat, in 2-byte words.*/
	float_ps->cur = length * elemSize / WORDS_TO_FLOATSIZE;

	status = DBputC(((void*)&float_ps),
			prim_i,
			micr_i,
			unit_i,
			secn_i,
			NULL);
	if (!SUCCESS(status)) {
		printf("error status returned from DBputC = %x\n", status);
		DBFREE(float_ps);
		goto egress;
	}

	DBFREE(float_ps);

egress:
	return status;
}

/*==================================================================================
 *
 * Abs: Sets a given SLC database tuple (prim,micro,unit,secn) to the given int
 *      value or array of ints.
 *
 * Name: JNI_DBSETINT
 *
 * Args: pvname.   The SLC database tuple whose value to set to data. Given in
 *                 syntax "<primary>:<micro>:<unit>:<secondary>".
 *                 Null terminated string given by reference.
 *       data      The integer data values to which to set the pvname.
 *                 The number of int elements of the data array must be exactly
 *                 equal to the length of the secondary. Ie, If where the
 *                 secondary is described as <n>R4 (eg 1R4), N must be the
 *                 length of data. Specifically, if the pvname identifies a 1R4
 *                 secondary, then only 1 int would be expected in this array.
 *                 An int array (given by reference).
 *       length    The number of elements in the array data above. Must be
 *                 equal to the size of the DB secondary. See data arg above.
 *                 An integer given by value.
 *
 *================================================================================*/
vmsstat_t JNI_DBSETINT(const char* pvname, int* data, int length)
{
	vmsstat_t status = 1;
	DBLIST(int_ps, int);
	int elemSize;             /* Size_t (bytes) of a single data element. */
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020;
	char asts_channel_name[9];
	const int WORDS_TO_INTSIZE = 1;

#ifdef DEBUG
	printf("entering JNI_DBSETINT\n");
	printf("slc pvname = %s\n", pvname);
	for (int t=0;t<length;t++) printf("data[%d]=%d\n",t,data[t]);
#endif


	/* Parse the given AIDA name (called pvname for some bizarre reason) into
	   the four parts that together form an SLC Db pv tuple (primary, micro, unit, secn).
	*/
	status = LCL_JNI_PARSENAME(pvname, &prim_i, &micr_i, &unit_i, &secn_i,
			asts_channel_name);
	if (!SUCCESS(status)) {
		goto egress;
	}

	/* Call DBputC to set the specified primary/micro/unit/
	   secondary value to the data given.
	*/
	elemSize = sizeof(int);
	DBalloc((void*)&int_ps, length, elemSize);
	DBCLEAR(int_ps);
	memcpy(&(int_ps->dat), data, length * elemSize);

	/* Set cur to the length of dat, in 2-byte words.*/
	int_ps->cur = length * elemSize / WORDS_TO_INTSIZE;

	status = DBputC(((void*)&int_ps),
			prim_i,
			micr_i,
			unit_i,
			secn_i,
			NULL);
	if (!SUCCESS(status)) {
		printf("error status returned from DBputC = %x\n", status);
		DBFREE(int_ps);
		goto egress;
	}

	DBFREE(int_ps);

egress:
	return status;
}

void JNI_ERR_TRANSLATE(const unsigned long int* errcode_p,
		struct dsc$descriptor* msgdsc_ps)
{
	char* msg_p;
	int ii;
/*---------------------------------------------------*/
	err_translate(errcode_p, msgdsc_ps);
	msg_p = msgdsc_ps->dsc$a_pointer;
	/*
	** Loop backwards through the message to find the first non-blank
	** character and add null terminator.
	*/
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

static vmsstat_t LCL_JNI_PARSENAME(char* slcname,
		int4u* prim_pi,
		int4u* micr_pi,
		int4u* lunit_pi,
		int4u* secn_pi,
		char* asts_channel_name)
{
	int i;
	int cindex, clen;  /* Char index and length */
	int num_converted; /* Number of items converted by sscanf */
	int found;

	short i_name;
	short num_names;

	db_name_ta* names_a;

	vmsstat_t status;

	char unit_c[5] = { '\0', '\0', '\0', '\0', '\0' };
	char unit_or_channel_str[9];
	char cur_channel_name[9];

	struct
	{
		int2u max;       /* max # units */
		int2u cur;       /* cur # units */
		int2u unit_num;  /* unit number */
	} unit_s;

	DBLIST(name_list_ps, db_name_ta);
/*---------------------------------------------------*/

#ifdef DEBUG
	printf("slcname = %s\n", slcname);
#endif

	status = 1;
	strcpy(asts_channel_name, "        ");

	clen = strcspn(slcname, ".:;");   /* Find # chars in primary */
	memcpy(prim_pi, slcname, clen);
	cindex = ++clen;                  /* Get past delimiter */
	*micr_pi = *(unsigned long*)&slcname[cindex];  /* Micro always 4 chars */

	/*
	 * Find the index to the unit or channel name and then find its
	 * length.
	 */
	cindex += 5;
	clen = strcspn(&slcname[cindex], ".:;");

	num_converted = 0;
	if (clen <= 4) {
		/*
		 * Copy the unit number into a character string and then
		 * convert it into a number.
		 */
		memcpy(&unit_c, &slcname[cindex], clen);
		num_converted = sscanf((const char*)&unit_c, "%d", lunit_pi);
	}

	/*
	 * If the primary is ASTS, determine whether the third field is a unit
	 * number or a channel name.
	 */
	if (*prim_pi == 'ASTS') {
		/*
		 * Extract the unit number or channel name string.
		 */
		memcpy(unit_or_channel_str, &slcname[cindex], clen);
		for (i = clen; i < 8; i++) {
			unit_or_channel_str[i] = ' ';
		}
		unit_or_channel_str[8] = '\0';

		/*
		 * Call DBgetC to get all of the ASTS channel names for the specified
		 * micro.  Then search the returned list of ASTS channel names for
		 * one that matches the extracted unit number of channel name string.
		 */
		DBCLEAR(name_list_ps);

		status = DBgetC(((void*)(&name_list_ps)), 'ASTS', *micr_pi, 'ALL*',
				'NAME', NULL);
		if (!SUCCESS(status)) {
			printf("error status returned from DBgetC = %d\n", status);
			DBFREE(name_list_ps);
			return status;
		}

		num_names = DBCNT(name_list_ps);
		names_a = DBPTR_FIRST(name_list_ps);

#ifdef DEBUG
		printf("num_names = %d\n", num_names);
		printf("unit_or_channel_str = %s\n", unit_or_channel_str);

		memcpy(cur_channel_name, names_a[0], 8);
		cur_channel_name[8] = '\0';

		printf("first names_a = %s\n", cur_channel_name);
#endif

		found = 0;
		i_name = 0;
		while ((i_name < num_names) && (!found)) {
			memcpy(cur_channel_name, names_a[i_name], 8);
			cur_channel_name[8] = '\0';

			if (strcmp(cur_channel_name, unit_or_channel_str) == 0) {
				found = 1;
			} else {
				i_name++;
			}
		}

		DBFREE(name_list_ps);

		if (found) {
			/*
			 * A channel name has been found.  Return this channel name
			 * string to the caller.  Then call dbunits_asts to convert
			 * the ASTS channel name into a unit number.
			 */

#ifdef DEBUG
			printf("found channel name\n");
#endif

			strcpy(asts_channel_name, unit_or_channel_str);

			unit_s.max = sizeof(unit_s) / 2;
			unit_s.cur = 0;

			status = dbunits_asts((void*)(&unit_s),
					micr_pi,
					(char*)unit_or_channel_str);
			if (!SUCCESS(status)) {
				printf("error status returned from dbunits_asts = %d\n",
						status);
				return status;
			}

			*lunit_pi = unit_s.unit_num;

#ifdef DEBUG
			printf("converted ASTS unit number = %d\n", *lunit_pi);
#endif
		}
	}

	cindex += ++clen;
	clen = strcspn(&slcname[cindex], " .:;");   /* Find length of secondary */
	memcpy(secn_pi, &slcname[cindex], clen);     /* secondary */
	return status;
}

/*
 * Issues a given error message to SLC error log, which is passed on to cmlog.
 */
static void issue_err(char* message)
{
	DESCR_DECLARE;
	REF_DECLARE;
	char msg_c[BUFSIZ];

	strcpy(msg_c, "DpSlcBuff: ");            /* Prepend Aida process name to msg */
	strcat(msg_c, message);                 /* Add passed in message */
	err_text(REFINT4U_1(OP_MSGTXT), DESCRN1(msg_c), &process_name);
}

#if (MAIN_DEBUG)
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
	REF_DECLARE;
	DESCR_DECLARE;
	vmsstat_t iss = 0;

	size_t elemSize = 0;         /* Size a datum or array datum element */
	int nelem = 0;               /* Num of elements of data */
	DBLIST( float_ps, float );

	prim_name_ts prim_s;
	micr_name_ts micr_s;
	unit_name_ts unit_s;
	secn_name_ts secn_s;

	/*
	 * Standalone init
	 */

	vmsstat_t status;
	const struct msginit msg_init_s = { 1,    /* init msg service */
											1 };  /* init slcnet */

	/* Join the SLC control system */
	status = standalone_init( &process_name,
				  REFINT4U_1(TRUE),
				  &msg_init_s,
				  REFINT4U_3(FALSE),
				  REFINT4U_4(TRUE) );
	if ( ! $VMS_STATUS_SUCCESS( status ))
		fprintf(stderr,"\n\nTEST Aida slc Standalone_init failed\n");

	/*
	 * Which node are we on, prod or dev? If dev, we're going to avoid
	 * trying to get real data later.
	 */
	char node_type[12];                /* "DEVELOPMENT" or "PRODUCTION" */
	slc_node( DESCRA1(node_type));     /* Acquires node type */
	development_node =
		(BOOL)(strncmp(node_type, VAX_DEV_NAME, strlen(VAX_DEV_NAME)) == 0);


	/*
	  Test a single float write/read
	*/
	float datum[] = {700.23234};
	status = JNI_DBSETFLOAT("KLYS:LI03:21:ADES", datum, 1);
#if 0
	prim_s._i = 'KLYS';
	micr_s._i = 'LI03';
	unit_s._i = 21;
	secn_s._i = 'ADES';

	elemSize = sizeof(datum);
	nelem = 1;
	DBalloc( (void*)&float_ps, nelem, elemSize );
	DBCLEAR( float_ps );
	float_ps->dat[0] = datum;
	float_ps->cur=WORDS(datum);  /* Set cur to the length of dat, in 2-byte words.*/
	status = DBputC( (( void* ) &float_ps),
			 prim_s._i, micr_s._i, unit_s._i, secn_s._i, NULL);
	DBFREE( float_ps );
#endif

	if ( !SUCCESS(status) )
	{
		printf("Error from JNI_DBSETFLOAT = %x\n", status);
		return status;
	}

	/*
	  Test a float array write
	*/
	float data[] = {43.2, -234.4, 3.345e-3, 7.77e8, -0.1, -1.0};
	status = JNI_DBSETFLOAT("KLYS:LI03:21:POLY", data, 6);
#if 0
	prim_s._i = 'KLYS';
	micr_s._i = 'LI03';
	unit_s._i = 21;
	secn_s._i = 'POLY';

	elemSize = sizeof(float);
	nelem = sizeof(data)/elemSize;
	DBalloc( (void*)&float_ps, nelem, elemSize );
	DBCLEAR( float_ps );
	memcpy( &(float_ps->dat), data, nelem*elemSize );
	float_ps->cur=WORDS(data);  /* Set cur to the length of dat, in 2-byte words.*/
	status = DBputC( (( void* ) &float_ps),
			 prim_s._i, micr_s._i, unit_s._i, secn_s._i, NULL);
	DBFREE( float_ps );

#endif
	if ( !SUCCESS(status) )
	{
		printf("Error from JNI_DBSETFLOAT for array write = %x\n", status);
		return status;
	}


	return 0;
}
#endif
