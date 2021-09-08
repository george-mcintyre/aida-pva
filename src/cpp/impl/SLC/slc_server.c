/**
 * SLC Database Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <string.h>
#include <stdlib.h>

#include <descrip.h>              /* for definition of $DESCRIPTOR  */
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

#include "aida_server_helper.h"
#include "slc_server.h"


/*
 * Local Static
 */
static vmsstat_t LCL_JNI_PARSENAME(const char* uri, int4u* prim_pi, int4u* micr_pi, int4u* lunit_pi, int4u* secn_pi, char* astsChannelName);
static const $DESCRIPTOR( process_name, "AidaDbIf");

/*
* Implementation for cm log requires process name so needs to be implemented in service impl.
 */
void issue_err( char* message )
{
	DESCR_DECLARE;
	REF_DECLARE;
	char msg_c[BUFSIZ];

	strcpy( msg_c, "DpSlcBuff: ");            /* Prepend Aida process name to msg */
	strcat( msg_c, message );                 /* Add passed in message */

	// TODO find impl of err_text
	err_text( REFINT4U_1(OP_MSGTXT), DESCRN1(msg_c), &process_name );
}

/**
 * Initialise the service
 * @param env to be used to throw exceptions using throw()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status = standalone_init(&process_name, &((long)(TRUE)), NULL, &((long)(FALSE)), &((long)(FALSE)));

	if (!SUCCESS(status)) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "Failed to initialise Aida Service");
	}

	printf("Aida Service Initialised\n");
}

/**
 * Get channel configuration
 * @param channelName
 * @param env to be used to throw exceptions using throw()
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName)
{
	Config config;
	memset(&config, 0, sizeof(config));

	// Return the config
	return config;
}

/**
 * Get a boolean
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments)
{
	int item = 0;

	DB_GET_SCALAR(int, "Unable to get boolean");

	// Return the item
	return item;
}

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments)
{
	char item = 0x0;

	DB_GET_SCALAR(char, "Unable to get byte");

	// Return the item
	return item;
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using throw()
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments)
{
	short item = 0;

	DB_GET_SCALAR(short int, "Unable to get short");

	// Return the item
	return item;
}

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments)
{
	int item = 0;

	DB_GET_SCALAR(int, "Unable to get integer");

	// Return the item
	return item;
}

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments)
{
	long item = 0;

	DB_GET_SCALAR(long, "Unable to get long");

	// Return the item
	return item;
}

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments.  To set a value
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments)
{
	float item = 0;

	DB_GET_SCALAR(float, "Unable to get float");

	unsigned short one = 1;
	cvt_vms_to_ieee_flt(&item, &item, &one);

	// Return the item
	return item;
}

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments)
{
	double item = 0.0;

	DB_GET_SCALAR(double, "Unable to get double");

	// TODO Find a way to convert doubles before return
	//	unsigned short one = 1;
	//	cvt_vms_to_ieee_flt(&item, &item, &one);

	// Return the item
	return item;
}

/**
 * Get a string.  Allocate memory for string and it will be freed for you by framework
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments)
{
	char* item;

	DESCR_DECLARE; /* pass-by-descrip macros. */
	DB_LIST_DEFINE_VARS_AND_PARSE_NAME(char)
	int ssize;              /* String size */

	// If the pseudo-secondary is CTRL, STAT, or VSTS, call
	// asts_color later to obtain the text, color, and flag
	// substrings from these bit patterns.
	int isColorCall = (secn_i == 'CTRL') || (secn_i == 'STAT') || (secn_i == 'VSTA');
	char color[9];
	char flag[2];
	char text[9];

	// If name parse is successful
	if (SUCCESS(status)) {
		if (strcmp(astsChannelName, "        ") != 0) {
			if (isColorCall) {
				DB_GET_LIST_ASTS_COLOR
			} else {
				DB_GET_LIST
				if (SUCCESS(status)) {
					DB_GET_LIST_ASTS_C
				}
			}
		} else {
			status = DBlistC(((void*)(&dbListDataBuffer)), prim_i, micr_i, unit_i, secn_i, NULL);
			if (SUCCESS(status)) {
				DB_GET_LIST_C
			}
		}

		// Create results to be returned
		if (SUCCESS(status)) {
			if (isColorCall) {
				color[8] = '\0';
				flag[1] = '\0';
				text[8] = '\0';

				// Allocate space for a string having a length of 18
				// characters.  First, fill this area with blanks.
				// Then copy the text substring into the string
				// starting at character 0, the color substring into
				// the string starting at character 9, and the flag
				// substring starting at character 18.  This layout
				// is assumed by the Java "get Any" method, which
				// calls this routine for ASTS channels with either
				// a CTRL, STAT, or VSTA pseudo-secondary.
				ssize = (sizeof(text) - 1) + (sizeof(color) - 1) + (sizeof(flag) - 1) + 2;

				item = malloc(ssize + 1); // Caller must free it
				memset(item, ' ', ssize);
				strncpy(item, text, strlen(text));

				int offset = (sizeof(text) - 1) + 1;
				strncpy(item + offset, color, strlen(color));

				offset = offset + (sizeof(color) - 1) + 1;
				strcpy(item + offset, flag);
			} else {
				ssize = DBCNT(dbDataReturnBuffer) * 2;
				item = malloc(ssize + 1);
				memcpy (item, &dbDataReturnBuffer->dat[0], ssize + 1);
				*(item + ssize) = '\0';  // Null terminate the string
			}
		}
	}

	DBFREE(dbListDataBuffer);
	DBFREE(dbDataReturnBuffer);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get string");
	}

	// Return the item
	return item;
}

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(int, "Unable to get boolean array")

	// Return the boolean array
	return itemArray;
}

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(char, "Unable to get byte array")

	// Return the byte array
	return itemArray;
}

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(short int, "Unable to get short array")

	// Return the short array
	return itemArray;
}

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(int, "Unable to get integer array")

	// Return the integer array
	return itemArray;
}

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(long, "Unable to get long array")

	// Return the long array
	return itemArray;
}

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(float, "Unable to get float array")
	// TODO Find a way to convert the floats
	//	unsigned short one = 1;
	//	cvt_vms_to_ieee_flt(&itemArray, &itemArray, &one);

	// Return the float array
	return itemArray;
}

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array itemArray;

	DB_GET_ARRAYS(double, "Unable to get double array")
	// TODO Find a way to convert doubles before return
	//	unsigned short one = 1;
	//	cvt_vms_to_ieee_flt(&itemArray, &itemArray, &one);

	// Return the double array
	return itemArray;
}

/**
 * Get a string array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	StringArray itemArray;

	itemArray.count = 0;

	// Not Implemented
	// Return empty string array
	return itemArray;
}

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	Table table;
	memset(&table, 0, sizeof(table));

	// Not implemented
	// Return empty table
	return table;
}

/**
 * Parse the URI into parts
 * @param uri
 * @param prim_pi
 * @param micr_pi
 * @param lunit_pi
 * @param secn_pi
 * @param astsChannelName
 * @return status
 */
static vmsstat_t LCL_JNI_PARSENAME(const char* uri, int4u* prim_pi, int4u* micr_pi, int4u* lunit_pi, int4u* secn_pi, char* astsChannelName) {
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

	status = 1;
	strcpy(astsChannelName, "        ");

	clen = strcspn(uri, ".:;");   /* Find # chars in primary */
	memcpy (prim_pi, uri, clen);
	cindex = ++clen;                  /* Get past delimiter */
	*micr_pi = *(unsigned long*)&uri[cindex];  /* Micro always 4 chars */

	/*
	 * Find the index to the unit or channel name and then find its
	 * length.
	 */
	cindex += 5;
	clen = strcspn(&uri[cindex], ".:;");

	num_converted = 0;
	if (clen <= 4) {
		/*
		 * Copy the unit number into a character string and then
		 * convert it into a number.
		 */
		memcpy (&unit_c, &uri[cindex], clen);
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
		memcpy(unit_or_channel_str, &uri[cindex], clen);
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

		status = DBgetC(((void*)(&name_list_ps)), 'ASTS', *micr_pi, 'ALL*', 'NAME', NULL);
		if (!SUCCESS(status)) {
			printf("error status returned from DBgetC = %d\n", status);
			DBFREE(name_list_ps);
			return status;
		}

		num_names = DBCNT(name_list_ps);
		names_a = DBPTR_FIRST(name_list_ps);

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
			strcpy(astsChannelName, unit_or_channel_str);

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
		}
	}

	cindex += ++clen;
	clen = strcspn(&uri[cindex], " .:;");   /* Find length of secondary */
	memcpy(secn_pi, &uri[cindex], clen);     /* secondary */
	return status;
}
