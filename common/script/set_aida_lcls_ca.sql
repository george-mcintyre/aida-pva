set serveroutput on size unlimited 

set timing on
set linesize 130
set pagesize 0

/*
--====================================================================================
--
--  Abs:   Populate the Aida database tables for the Aida LCLS
--         Channel Access server.
--
--  Name:  set_aida_lcls_ca.sql
--
--  Rem:   This script populates Aida database tables for the Aida
--         LCLS Channel Access server from PVs contained in rows
--         of the irmisdb Oracle account table curr_pvs.
--
--         The Aida database is populated by adding new rows to the
--         aida_directory and aida_names tables.  Rows may also be
--         deleted from these tables if LCLS PVs that were formerly
--         in the curr_pvs table are no longer in this table.  The
--         Oracle tables aida_old_lcls_ca and aida_new_lcls_ca are
--         used by this routine to hold id numbers during processing.
--         These tables are used as temporary storage space for this
--         script.
--
--  Note:  This script should be invoked from an Oracle SQLPLUS prompt while logged on
--         either as AIDAPROD@SLACPROD or AIDADEV@SLACPROD or AIDA@MCCQA.
--
--  Example Usage:
--  =============
--  Launch SQLPLUS from a directory in which this script resides, logging on as AIDA@MCCQA.
--  At the SQLPLUS use the "start" command and enter the script name followed by parameters "Aida servicename" and either a "Y" or "N", indicating whether to "commit" or "not commit" the data.
--  These two parameter values should be separated by a single space character and quote characters are *not* used to delineate these values.
--
--      SQL> start set_aida_lcls_ca EPICS_lcls Y
--   or
--      SQL> start set_aida_lcls_ca EPICS_lcls N
--
--  Auth:  17-Apr-2007, Bob Hall (rdh)
--  Rev:   09-Feb-2010, Elie Grunhaus
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--         04-Apr-2014  Poonam Pandey
--         Modified code in multiple places for better exception handling and also to continue with the insert into AIDA_DIRECTORY
--         despite any exceptions. The calling script "lcls_ca_populate_aida_database.sh" has also been modified to email the errors if any.
--         28-Jan-2013, Shashi Divekar
--         Changed "set serveroutput on size 1000000" to "set serveroutput on size unlimited"  
--         15-Sep-2011, Bob Hall
--            Added the population of FACET PVs.
--         09-May-2011, Elie Grunhaus
--         Replaced the SQL*PLUS directive "size unlimited" with what it was originally "size 1000000" because the former is not compatible with the 9.0.1 version of SQL*Plus being invoked by the "parent" script that runs this script.
--         27-Apr-2011, Elie Grunhaus
--           Created logic to retrieve IRMISDB.CURR_PVS data from MCCO instead of MCCQA.
--           This data retrieval would necessarily utilize a dblink from either SLACPROD or from MCCQA, depending upon where this script is logged on at the time.
--         20-Apr-2011, Elie Grunhaus
--            Replaced Jeff Du's data synchronization code with my "SQL Differencing" package CONTROLS_GLOBAL.SYNCH_MAIN_PKG.SYNCH_DATA.
--            Also replaced the SQLPLUS directive "set serveroutput on size 1000000" with "set serveroutput on size unlimited" so as to prevent error "ORU-10027".
--         07-Apr-2010, Elie Grunhaus
--            Added support for un-expected errors such as an Oracle instance shutdown or disconnect.
--            An "un-expected" error code of -7777 is returned to the calling module.  This calling module
--            should be coded to appropriately handle this code.
--         19-Mar-2010, Elie Grunhaus
--            Modified so as to remove all "hard parsing" of DML and SELECT statements.
--         16-Mar-2010, Elie Grunhaus
--            Reversed the "if-then-else" logic used within the exception handler surrounding the retrieval from view V_PROCESS_STATUS
--            so that Oracle does not get confused between the local versus the outer exception handlers and throws a "Used Defined" exception.
--            Also, added a criterion to check the STAGE column to the SELECT query used agaianst view V_PROCESS_STATUS.
--         10-Mar-2010, Bob Hall
--            Modified writes to the log file so that a "errcode: n" log message
--            is the last log message written.  This is needed so that the invoking
--            lcls_ca_populate_aida_database.sh script can find the "errcode: n" log
--            message to determine the success/failure error code.
--         03-Mar-2010, Elie Grunhaus
--            Added "p_commit" parameter to allow testing without an immediate commit.
--            Generalized the logic so as to remove hard-coding with respect to dblinks.
--            Commented "alter table" statements to prevent DDL commit.
--            Added ability to write out to ASCII log file via package CONTROLS_GLOBAL.WRITE_LOG.
--            Added support for tables CONTROLS_GLOBAL.DATA_VALIDATION_TAB/DATA_VALIDATION_AUDIT and package CONTROLS_GLOBAL.MCC_AR_PKG.
--            Commented out all DDL statements such as drop/create index and truncate table and replaced these with SQL DELETE statements.
--         02-Jul-2009, Bob Hall
--            Replaced dynamically created insert statement for
--            inserting into the aida_new_lcls_ca table with
--            a statement using a bind variable.  This is more
--            efficient for Oracle since it prevents "hard parses".
--            The previous dynamically created insert statement
--            caused an out-of-memory condition in the Oracle
--            library cache (ORA-20090 and ORA-04031) due to so
--            many "hard parses" (requiring Oracle to build a
--            query plan each time).  Currently there there are
--            over 373,000 LCLS PVs and a row is inserted into
--            the aida_new_lcls_ca table for each.
--         08-Apr-2009, Bob Hall (rdh) and Elie Grunhaus (egrunhau)
--            Substantially rewritten to improve performance.
--            Previously, processing approximately 370,000
--            LCLS system records from the irmisdb account
--            table curr_pv required 4.5 hours.  Elie Grunhaus
--            started this rewrite and Bob Hall completed it
--            while consulting with Elie to obtain ideas to
--            improve performance.
--
--====================================================================================
*/
declare

    errcode                      number;
    err_time                     varchar2(100);
    v_errmsg                     varchar2(500);

    v_daytime                    varchar2(100);
    v_start_total                number;
    v_start                      number;
    v_diff                       number;
    v_diff_calc                  varchar2(100);

    V_PROC                       VARCHAR2(100);
    V_COMMIT                     varchar2(1);

    V_SQLERRMSG                  VARCHAR2(500);
    V_STMT                       VARCHAR2(1000);
    V_STATUS_CHK                 VARCHAR2(30);
    V_OK_TO_PRINT                BOOLEAN;
    V_CURR_RETVAL                NUMBER;
    V_PROCESS_GRP_NO             NUMBER;

    V_TGT_INSTANCE_NM            VARCHAR2(30) := 'MCCO';
    V_TGT_SCHEMA_NM              VARCHAR2(30) := 'AIDA';
    V_INIT_LOG_FLAG              VARCHAR2(1)  := 'N';
    V_TEST_ROWS                  NUMBER       := NULL;

    V_DB_NAME                    VARCHAR2(100);
    V_LOG_FILE                   VARCHAR2(100);

    /* POINTS TO LINUX DIRECTORY /nfs/slac/g/cd/log/oracle */
    V_DIR               CONSTANT VARCHAR2(30)  := 'CONTROL_LOGS';

    V_BUFFER            CONSTANT NUMBER        := 1024;
    V_SYSTEM_NAME_1     CONSTANT VARCHAR2(20)  := 'LCLS';
    V_SYSTEM_NAME_2     CONSTANT VARCHAR2(20)  := 'FACET';
    V_INSTANCE_DO_SYNCH  CONSTANT VARCHAR2(20)  := 'MCCQA';
    V_INSTANCE_NO_SYNCH CONSTANT VARCHAR2(20)  := 'SLACPROD';
    V_TRGT_SCHEMA       CONSTANT VARCHAR2(30)  := 'IRMISDB';
    V_IRMIS_PROCESS_NM  CONSTANT VARCHAR2(120) := 'ALL_DATA_UPDATE';
    V_IRMIS_STAGE       CONSTANT VARCHAR2(120) := 'FINISH';
    V_TRGT_SCHEMA_TABLE CONSTANT VARCHAR2(30)  := 'CURR_PVS';
    V_REMOTE_HOST       CONSTANT VARCHAR2(20)  := 'MCCO';
    V_SQ                CONSTANT VARCHAR2(1)   := CHR(39);
    V_CRLF              CONSTANT VARCHAR2(1)   := CHR(10);

    V_UNEXPECTED_ERROR_CODE   CONSTANT  NUMBER := -7777;


    V_PROCESS_STATUS_ERROR       EXCEPTION;
    EXEC_ERROR                   EXCEPTION;

----------------------

PROCEDURE ADD_TO_DATA_VALIDATION_TAB (P_STAGE          IN VARCHAR2,
                                      P_STATUS         IN VARCHAR2,
                                      P_MSG            IN VARCHAR2,
                                      P_PROCESS_GRP_NO IN OUT NUMBER,
                                      P_ERRMSG         IN OUT VARCHAR2)
IS

  V_PROC1    VARCHAR2(100);
  V_STAGE    VARCHAR2(120);
  V_STATUS   VARCHAR2(30);
  V_MSG      VARCHAR2(500);
  V_DV_ID    INTEGER;

  V_TAB_PROCESS_NM    CONSTANT    VARCHAR2(120) := 'SET_AIDA_LCLS_CA';

BEGIN

  V_PROC1  := 'ADD_TO_DATA_VALIDATION_TAB';
  V_STAGE  := SUBSTR(TRIM(P_STAGE),1,120);
  V_STATUS := SUBSTR(TRIM(P_STATUS),1,30);
  V_MSG    := SUBSTR(TRIM(P_MSG),1,500);

  P_ERRMSG := NULL;

  --------

  -- IF PASSED-IN P_PROCESS_GRP_NO IS NULL, THEN RETRIEVE A NEW VALUE FROM SEQUENCE.
  IF ( P_PROCESS_GRP_NO IS NULL ) THEN
    BEGIN
    SELECT DATA_VALIDATION_TAB_PGN_SEQ.NEXTVAL
      INTO P_PROCESS_GRP_NO
      FROM DUAL;
    EXCEPTION
      WHEN OTHERS THEN
        P_ERRMSG := SUBSTR('SEQUENCE ERROR ('||V_PROC1||') [PROCESS_GRP_NO='||NVL(TO_CHAR(P_PROCESS_GRP_NO),'NULL')||']: '||SQLERRM,1,500);
    END;
  END IF;

  --------

  BEGIN
  INSERT INTO DATA_VALIDATION_TAB (INST,
                                   SCHEMA_NM,
                                   PROCESS_NM,
                                   STAGE,
                                   STATUS_CHK,
                                   MSG,
                                   CREATED_BY,
                                   PROCESS_GRP_NO)
                           VALUES (V_DB_NAME,
                                   USER,
                                   V_TAB_PROCESS_NM,
                                   V_STAGE,
                                   V_STATUS,
                                   V_MSG,
                                   USER,
                                   P_PROCESS_GRP_NO);

  EXCEPTION
    WHEN OTHERS THEN
      P_ERRMSG := SUBSTR('INSERT ERROR ('||V_PROC1||') [PROCESS_GRP_NO='||NVL(TO_CHAR(P_PROCESS_GRP_NO),'NULL')||']: '||SQLERRM,1,500);
  END;

  --------

  RETURN;


EXCEPTION
  WHEN OTHERS THEN
    P_ERRMSG := SUBSTR('OTHERS ERROR ('||V_PROC1||') [PROCESS_GRP_NO='||NVL(TO_CHAR(P_PROCESS_GRP_NO),'NULL')||']: '||SQLERRM,1,500);

END ADD_TO_DATA_VALIDATION_TAB;

----------------------

function set_aida_lcls_ca(p_service_name in varchar2,
                          p_commit       in varchar2 default 'N') return number
is

    v_proc2                      varchar2(100);
    v_sqlcode                    number;

    v_start_total_ca             number;

    v_service_name               varchar2(50);

    v_cnt                        pls_integer;
    v_found                      boolean;

    v_arr_limit                  pls_integer := 1000;

    user_name                    varchar2(50);

    service_num                  number;
    name_id_num                  number;

    num_name_inserts             integer;
    num_dir_inserts              integer;
    num_name_deletes             integer;
    num_old_id_table_rows        integer;
    num_new_id_table_rows        integer;

    V_DBLINK                     VARCHAR2(200);
    V_SRC                        VARCHAR2(200);

    -----------------

    aida_new_insert_stmt       constant varchar2(200) := 'insert into aida_new_lcls_ca values (:bind_var_1)';

    aida_names_select_stmt     constant varchar2(200) := 'select id from aida_names where instance = :bind_rec_nm1 and attribute = :bind_attribute1';

    aida_names_insert_stmt     constant varchar2(200) := 'insert into aida_names (instance, attribute, transform) values (:bind_rec_nm2, :bind_attribute2, :bind_transform)';

    aida_directory_insert_stmt constant varchar2(200) := 'insert into aida_directory (name_id, service_id, order_no) values (:bind_name_id, :bind_service_id, :bind_order_no)';

    v_rownum_max               constant integer       := 2;
    v_attribute                constant varchar2(3)   := 'VAL';
    v_order_no                 constant number        := 1;
    v_transform                constant varchar2(1)   := NULL;

    -----------------

    XC                           SYS_REFCURSOR;  -- USED TO DETERMINE IF DBLINK (IF USED) IS VALID.
    RC                           SYS_REFCURSOR;  -- USED BECAUSE, DEPENDING ON LOGIN, DEFINING QUERY MAY/MAY NOT USE A DBLINK.

    TYPE REC_NM_TYP IS TABLE OF VARCHAR2(200) INDEX BY PLS_INTEGER;
    REC_NM_ARR    REC_NM_TYP;
    
    -- Begin objects used to populate tables.
    cursor aida_dir_name_id_cur (c_service_name in varchar2)
    is
        select name_id
            from aida_directory
            where service_id = (select id
                                from aida_services
                                where name = c_service_name);

    type aida_dir_name_id_cur_typ is table of aida_dir_name_id_cur%rowtype index by pls_integer;

    aida_dir_name_id_arr aida_dir_name_id_cur_typ;
    -- End objects used to populate tables.

    -------------------------------------------------------------------------

    invalid_service_name_error   exception;
    null_service_name_error      exception;
    del_aida_old_lcls_ca_error   exception;
    del_aida_new_lcls_ca_error   exception;
    pop_aida_old_lcls_ca_error   exception;
    DBLINK_NOT_FOUND_ERROR       EXCEPTION;
    DBLINK_INVALID_ERROR         EXCEPTION;
    UNEXPECTED_INSTANCE_ERROR    EXCEPTION;
    EXEC_IMMED_ERROR             EXCEPTION;
    DEL_NAMES_ERROR              EXCEPTION;
    DEL_DIR_ERROR                EXCEPTION;

begin
    v_sqlcode := V_UNEXPECTED_ERROR_CODE;  -- If returned, this code indicates an un-expected error occurred such as an Oracle instance shutdown or disconnect.

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Starting function set_aida_lcls_ca');
    END IF;
    dbms_output.put_line('Starting function set_aida_lcls_ca');

    v_proc2 := 'set_aida_lcls_ca';


    -- Begin check parameters.

    -- v_service_name must be non-NULL and validated against table aida_services.
    -- If NULL, raise an error.

    v_service_name := substr(trim(p_service_name), 1, 50);

    if (v_service_name is not NULL) then

        -- Validate against table aida_services.

        begin
        select id
            into service_num
            from aida_services
            where name = v_service_name;

        exception
            when no_data_found then
                service_num := 0;
        end;

        if (service_num = 0) then
            v_errmsg := substr('Error: ('||v_proc2||'): parameter p_service_name = '||v_service_name||' not found in table aida_services.', 1, 500);
            raise invalid_service_name_error;
        end if;

    else

        v_errmsg := substr('Error: (' || v_proc2 || '): NULL parameter p_service_name', 1, 500);
        raise null_service_name_error;

    end if;  -- if (v_service_name is not NULL) then

    ---------

    select decode(substr(upper(trim(p_commit)),1,1), NULL, 'N', 'N', 'N', 'Y', 'Y', 'N')
        into V_COMMIT
        from dual;

    -- End check parameters. 

    ----------------------

    -- Begin processing.
  
    v_start_total_ca := dbms_utility.get_time;

    select user
      into user_name
      from dual;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Log for populating database for Aida LCLS Channel Access server.  User name: ' || user_name);
    END IF;
    dbms_output.put_line('Log for populating database for Aida LCLS Channel Access server.  User name: ' || user_name);


    -- Get starting date and time.
    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
      into v_daytime
      from dual;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Begin function set_aida_lcls_ca at: ' || v_daytime);
    END IF;
    dbms_output.put_line('Begin function set_aida_lcls_ca at: ' || v_daytime);
 
    ------------

    /* BEGIN COMMENT DROP INDEX AND TRUNCATE TABLE DUE TO POTENTIAL ERRORS RESULTING FROM IMPLICIT COMMITS
    -- Drop the aida_old_lcls_ca_index index and truncate the aida_old_lcls_ca table. 
    begin
    execute immediate 'drop index aida_old_lcls_ca_index';
    exception
        when others then
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE('Failed to drop index aida_old_lcls_ca_index');
            END IF;
            dbms_output.put_line('Failed to drop index aida_old_lcls_ca_index');
    end;

    execute immediate 'truncate table aida_old_lcls_ca';
    END COMMENT DROP INDEX AND TRUNCATE TABLE DUE TO POTENTIAL ERRORS RESULTING FROM IMPLICIT COMMITS */

    ------------

    -- Delete table aida_old_lcls_ca.
    v_start := dbms_utility.get_time;

    begin
    delete from aida_old_lcls_ca;
    v_cnt := sql%rowcount;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Deleted ' || to_char(v_cnt) || ' aida_old_lcls_ca rows.');
    END IF;
    dbms_output.put_line('Deleted ' || v_cnt || ' aida_old_lcls_ca rows.');
     
    exception
        when others then
            v_errmsg := substr('Error (' || v_proc2 || '): delete from aida_old_lcls_ca =>'||sqlerrm, 1, 500);
            raise del_aida_old_lcls_ca_error;
    end;

    ----------------------

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to delete aida_old_lcls_ca rows: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to delete aida_old_lcls_ca rows: ' || v_diff_calc);

    ------------

    /* BEGIN COMMENT TRUNCATE TABLE DUE TO POTENTIAL ERRORS RESULTING FROM IMPLICIT COMMITS
    -- Truncate the aida_new_lcls_ca table.
    execute immediate 'truncate table aida_new_lcls_ca';
    END COMMENT TRUNCATE TABLE DUE TO POTENTIAL ERRORS RESULTING FROM IMPLICIT COMMITS */

    ------------

    -- Delete table aida_new_lcls_ca.
    v_start := dbms_utility.get_time;

    begin
    delete from aida_new_lcls_ca;
    v_cnt := sql%rowcount;
    dbms_output.put_line('Deleted ' || v_cnt || ' aida_new_lcls_ca rows.');

    exception
        when others then
            v_errmsg := substr('Error (' || v_proc2 || '): delete from aida_new_lcls_ca => '||sqlerrm, 1, 500);
            raise del_aida_new_lcls_ca_error;
    end;

    ------------

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to delete aida_new_lcls_ca rows: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to delete aida_new_lcls_ca rows: ' || v_diff_calc);

    ------------

    -- Begin populating table aida_old_lcls_ca.
    v_start := dbms_utility.get_time;

    num_old_id_table_rows := 0;

    open aida_dir_name_id_cur(v_service_name);

    loop

        fetch aida_dir_name_id_cur
            bulk collect
            into aida_dir_name_id_arr
            limit v_arr_limit;

        num_old_id_table_rows := num_old_id_table_rows + aida_dir_name_id_arr.count;

        -- Bulk insert into table.
        begin
        forall q in 1 .. aida_dir_name_id_arr.count
            insert into aida_old_lcls_ca values aida_dir_name_id_arr(q);
        exception
           when others then
               v_errmsg := substr('Error ('||v_proc2||'): during bulk insert into aia_old_lcls_ca for service_name = '||v_service_name||' => '||sqlerrm, 1, 500);
               raise pop_aida_old_lcls_ca_error;
        end;

        exit when aida_dir_name_id_cur%notfound;

    end loop; 

    close aida_dir_name_id_cur;

    ------------

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Number of aida_old_lcls_ca rows: ' || to_char(num_old_id_table_rows));
    END IF;
    dbms_output.put_line('Number of aida_old_lcls_ca rows: ' || to_char(num_old_id_table_rows));

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to populate aida_old_lcls_ca table: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to populate aida_old_lcls_ca table: ' || v_diff_calc);

    ---------

    /* BEGIN COMMENT CREATE INDEX
    -- Create the aida_old_lcls_ca_index index for the populated aida_old_lcls_ca table.
    v_start := dbms_utility.get_time;

    execute immediate 'create index aida_old_lcls_ca_index on aida_old_lcls_ca (name_id)';

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to create index aida_old_lcls_ca_index: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to create index aida_old_lcls_ca_index: ' || v_diff_calc);
    END COMMENT CREATE INDEX */

    ---------

    -- Process rows from the irmisdb account Oracle table curr_pvs.  The "distinct"
    -- keyword in the select statement allows the avoidance of having to check whether
    -- an id number is not already in the aida_new_lcls_ca table, which greatly aids
    -- performance.

    v_start := dbms_utility.get_time;

    num_name_inserts := 0;
    num_dir_inserts := 0;
    num_new_id_table_rows := 0;


    -- ELIE: BEGIN ADD

    -- LOGIC: REGARDLESS IF THE LOGON INSTANCE IS SLACPROD OR MCCQA, RETRIEVE FROM IRMISDB SCHEMA RESIDING ON THE REMOTE MCCQA INSTANCE.

    IF ( (V_DB_NAME = V_INSTANCE_DO_SYNCH) OR (V_DB_NAME = V_INSTANCE_NO_SYNCH) ) THEN
    
      -- DETERMINE NAME OF DBLINK TO USE
      BEGIN
      SELECT DB_LINK
        INTO V_DBLINK
        FROM USER_DB_LINKS
       WHERE USERNAME = V_TRGT_SCHEMA
         AND HOST     = V_REMOTE_HOST
         AND ROWNUM   < v_rownum_max;
    
      EXCEPTION
        WHEN NO_DATA_FOUND THEN
          V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): DBLINK NOT FOUND FOR V_DB_NAME='||V_DB_NAME||', V_TRGT_SCHEMA='||V_TRGT_SCHEMA||', AND V_REMOTE_HOST='||V_REMOTE_HOST,1,500);
          RAISE DBLINK_NOT_FOUND_ERROR;
      END;
    
      IF (V_OK_TO_PRINT) THEN
        CONTROLS_GLOBAL.WRITE_LOG.WRITE('CURRENTLY LOGGED ONTO '||V_DB_NAME||' AND WILL ACCESS '||V_TRGT_SCHEMA||'@'||V_REMOTE_HOST||' USING DBLINK "'||V_DBLINK||'".');
      END IF;
      DBMS_OUTPUT.PUT_LINE('CURRENTLY LOGGED ONTO '||V_DB_NAME||' AND WILL ACCESS '||V_TRGT_SCHEMA||'@'||V_REMOTE_HOST||' USING DBLINK "'||V_DBLINK||'".');

      V_SRC := SUBSTR(V_TRGT_SCHEMA||'.'||V_TRGT_SCHEMA_TABLE||'@'||V_DBLINK,1,200);

      -- CHECK THAT DBLINK IS VALID.
      BEGIN
      OPEN XC FOR 'SELECT 1 FROM '||V_SRC||' WHERE ROWNUM < '||to_char(v_rownum_max);
      LOOP
        EXIT WHEN XC%NOTFOUND;
        FETCH XC INTO V_CNT;
      END LOOP;
      CLOSE XC;
      EXCEPTION
        WHEN OTHERS THEN
          V_ERRMSG := SUBSTR('ERROR ('||V_PROC2||'): DBLINK "'||V_DBLINK||'" IS INVALID FOR V_DB_NAME='||V_DB_NAME||', V_TRGT_SCHEMA='||V_TRGT_SCHEMA||', AND V_REMOTE_HOST='||V_REMOTE_HOST||'['||SQLERRM||']',1,500);
          CLOSE XC;
          RAISE DBLINK_INVALID_ERROR;
      END;

    ELSE

      V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): UN-EXPECTED LOGON INSTANCE: V_DB_NAME='||V_DB_NAME,1,500);
      RAISE UNEXPECTED_INSTANCE_ERROR;
      
    END IF;  -- IF ( (V_DB_NAME = V_INSTANCE_DO_SYNCH) OR (V_DB_NAME = V_INSTANCE_NO_SYNCH) ) THEN

    -----------

    OPEN RC FOR 'SELECT DISTINCT TRIM(REC_NM) AS REC_NM FROM '||V_SRC||' WHERE SYSTEM = '||V_SQ||V_SYSTEM_NAME_1||V_SQ||' OR SYSTEM = '||V_SQ||V_SYSTEM_NAME_2||V_SQ;

    loop  -- BEGIN OUTER LOOP

        fetch RC
        bulk collect
        into REC_NM_ARR
        limit v_arr_limit;

        for q in 1 .. REC_NM_ARR.count
        loop
        
          /*
          IF (V_OK_TO_PRINT) THEN
            CONTROLS_GLOBAL.WRITE_LOG.WRITE('REC_NM_ARR('||TO_CHAR(Q)||')='||REC_NM_ARR(Q));  -- ELIE: DEBUG ONLY
          END IF;
          DBMS_OUTPUT.PUT_LINE('REC_NM_ARR('||Q||')='||REC_NM_ARR(Q));  -- ELIE: DEBUG ONLY
          */

          -- Check whether there are any rows in the aida_names table containing
          -- the current record name as an instance and with the attribute 'VAL'.
          -- If no rows were found, insert a row into this table containing these
          -- instance and attribute names.

          begin
          select 1
              into v_cnt
              from aida_names
              where instance  = REC_NM_ARR(q)
                and attribute = v_attribute
                and rownum = 1;

          exception
            when no_data_found then
              v_cnt := 0;
            when others then
              v_cnt := -1;
  -- Poonam - Added more exception handling.
              v_sqlcode := sqlcode;
              v_errmsg := substr('Error (' || v_proc2 || ') => ' || sqlerrm, 1, 500);
              IF (V_OK_TO_PRINT) THEN
                CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
              END IF;
              ROLLBACK;
              return v_cnt;
  -- Poonam
          end;

          /* ELIE COMMENTED
          if (v_cnt = 0) then
              execute immediate 'insert into aida_names (instance, attribute, transform) ' ||
              'values (''' ||  REC_NM_ARR(q) || ''', ''' ||
              v_attribute || ''', NULL)';

              num_name_inserts := num_name_inserts + 1;
          end if;
          */

          /* BEGIN ELIE ADD */
          if ( v_cnt = 0 ) then

            begin
            execute immediate aida_names_insert_stmt using REC_NM_ARR(q), v_attribute, v_transform;
            exception
              when others then
                V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): execute immediate '||aida_names_insert_stmt||' using '||REC_NM_ARR(q)||', '||v_attribute||' and '||nvl(v_transform,'NULL')||' =>'||sqlerrm,1,500);
                raise exec_immed_error;
            end;

            num_name_inserts := num_name_inserts + 1;

          end if;
          /* END ELIE ADD */


          --
          -- Obtain the id field in the aida_names table for the current instance/attribute pair
          -- and insert this id into the aida_new_lcls_ca table.
          --

          /* ELIE COMMENTED
          execute immediate 'select id from aida_names where instance = ''' ||
              REC_NM_ARR(q) || ''' and attribute = ''' ||
              v_attribute || '''' into name_id_num;
          */

          /* BEGIN ELIE ADD */
          begin
          execute immediate aida_names_select_stmt into name_id_num using REC_NM_ARR(q), v_attribute;
          exception
            when others then
              V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): execute immediate '||aida_names_select_stmt||' using '||REC_NM_ARR(q)||' and '||v_attribute||' =>'||sqlerrm,1,500);
              raise exec_immed_error;
          end;
          /* END ELIE ADD */

          -------

          begin
          execute immediate aida_new_insert_stmt using name_id_num;
          exception
            when others then
              V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): execute immediate '||aida_new_insert_stmt||' using '||to_char(name_id_num)||' =>'||sqlerrm,1,500);
              raise exec_immed_error;
          end;

          -------

          num_new_id_table_rows := num_new_id_table_rows + 1;

          --
          -- Determine if there are any rows in the aida_directory table
          -- containing the aida_names id number and the service id number.
          -- If there were no such rows, insert a row into this table
          -- containing these two id numbers.
          --
 /* Poonam - Changed the original logic to only check with NAME_ID, as another Unique Constraint exists on NAME_ID + ORDER_NO.
            The original PK also exists on NAME_ID + SERVICE_ID. 
             This has been done to STOP the Duplicate data that was coming into AIDFA_DIRECTORY for wrong services.
          select 1
            into v_cnt
            from aida_directory
            where name_id = name_id_num
            and service_id = service_num;
*/

          begin
          select 1
            into v_cnt
            from aida_directory
            where name_id = name_id_num 
            and rownum = 1;

          exception
            when no_data_found then
            v_cnt := 0;
            when others then
            v_cnt := -1;
  -- Poonam - Added more exception handling.
              v_sqlcode := sqlcode;
              v_errmsg := substr('Error (' || v_proc2 || ') => ' || sqlerrm, 1, 500);
              IF (V_OK_TO_PRINT) THEN
                CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
              END IF;
              ROLLBACK;
     return v_cnt;
  -- Poonam
          end;

          -------

          /* ELIE COMMENTED
          if (v_cnt = 0) then
            execute immediate 'insert into aida_directory (name_id, service_id, order_no) values (' ||
            to_char(name_id_num) || ', ' || to_char(service_num) || ', 1)';
            num_dir_inserts := num_dir_inserts + 1;
          end if;
          */


          /* BEGIN ELIE ADD */
          if ( v_cnt = 0 ) then

            begin
            execute immediate aida_directory_insert_stmt using name_id_num, service_num, v_order_no;
            exception
              when others then
                V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): execute immediate '||aida_directory_insert_stmt||' using '||to_char(name_id_num)||', '||to_char(service_num)||' and '||to_char(v_order_no)||' =>'||sqlerrm,1,500);
  -- Poonam - Added more exception handling.
  --          Removed the raise error, as we want to insert the data (and not abort) and email the errors to controls-software-reports
            --    raise exec_immed_error;
            v_sqlcode := sqlcode;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
  -- Poonam
            end;

            num_dir_inserts := num_dir_inserts + 1;

          end if;
          /* END ELIE ADD */

        end loop;

        exit when RC%notfound;

    end loop;  -- END OUTER LOOP

    CLOSE RC;

    -- ELIE: END ADD

    ---------

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Inserted ' || to_char(num_name_inserts) || ' rows into aida_names');
    END IF;
    dbms_output.put_line('Inserted ' || to_char(num_name_inserts) || ' rows into aida_names');

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Inserted ' || to_char(num_dir_inserts) || ' rows into aida_directory');
    END IF;
    dbms_output.put_line('Inserted ' || to_char(num_dir_inserts) || ' rows into aida_directory');

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Number of aida_new_lcls_ca rows: ' || to_char(num_new_id_table_rows));
    END IF;
    dbms_output.put_line('Number of aida_new_lcls_ca rows: ' || to_char(num_new_id_table_rows));

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to process rows from irmisdb curr_pvs table: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to process rows from irmisdb curr_pvs table: ' || v_diff_calc);
  
    ---------

    -- Delete the rows that must be deleted from the aida_names and aida_directory
    -- tables.  First, get the number of rows that will be deleted from the aida_names
    -- table by finding the number of rows in the aida_old_lcls_ca table that are
    -- not in the aida_new_lcls_ca table.  Then temporarily disable the
    -- FK_AIDA_DIRECTORY_NAMES constraint that does not allow aida_names entries
    -- to be deleted while there are references to them in the aida_directory
    -- table.  Next, delete the rows in the aida_names table that have id values
    -- that are in the aida_old_lcls_ca table but not in aida_new_lcls_ca table.
    -- Finally, delete the rows in the aida_directory table that do not refer
    -- to rows in the aida_names table.  These should be the references to rows
    -- in the aida_names table that have been deleted.

    v_start := dbms_utility.get_time;

    select count(*) into num_name_deletes from
        (select name_id from aida_old_lcls_ca
        minus
        select name_id from aida_new_lcls_ca);


    /* (ELIE ADD) ==========================================================================================
    NOTE:
    THE FOLLOWING "DELETE FROM AIDA_NAMES" STATEMENT WOULD NORMALLY VIOLATE THE FK CONSTRAINT ON AIDA_DIRECTORY.
    HOWEVER, THIS VIOLATION DOES *NOT* OCCUR BECAUSE THE FK CONSTRAINTS ON AIDA_DIRECTORY ARE "DEFERRED WITH
    DEFAULT OF INITIALLY DEFERRED" VIA THE STATEMENTS:

      ALTER TABLE AIDA_DIRECTORY DROP CONSTRAINT FK_AIDA_DIRECTORY_AIDA_NAMES;

      ALTER TABLE AIDA_DIRECTORY ADD CONSTRAINT FK_AIDA_DIRECTORY_AIDA_NAMES FOREIGN KEY (NAME_ID) REFERENCES AIDA_NAMES (ID) DEFERRABLE INITIALLY DEFERRED;

    ALSO, THE ALTER STATEMENTS WILL AUTOMATICALLY COMMIT ANY PREVIOUS DML STATEMENTS WHICH WOULD MAKE ANY SUBSEQUENT ROLLBACK USELESS.

    THIS IS WHY I HAVE COMMENTED OUT THE FOLLOWING TWO "ALTER TABLE" STATEMENTS.
    ===================================================================================================== */

    -- execute immediate 'alter table aida_directory disable constraint FK_AIDA_DIRECTORY_AIDA_NAMES';  -- ELIE COMMENTED

    begin
    delete from aida_names
        where id in
        (select name_id from aida_old_lcls_ca
        minus
        select name_id from aida_new_lcls_ca);
    exception
      when others then
        V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): delete from aida_names using MINUS operator. =>'||sqlerrm,1,500);
        raise del_names_error;
    end;

    ---------

    begin
    delete from aida_directory where name_id not in (select id from aida_names);
    exception
      when others then
        V_ERRMSG := SUBSTR('ERROR (' || V_PROC2 || '): delete from aida_directory using name_id not in clause. =>'||sqlerrm,1,500);
        raise del_dir_error;
    end;

    ---------

    -- execute immediate 'alter table aida_directory enable constraint FK_AIDA_DIRECTORY_AIDA_NAMES';  -- ELIE COMMENTED

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Deleted ' || to_char(num_name_deletes) || ' rows from aida_names');
    END IF;
    dbms_output.put_line('Deleted ' || to_char(num_name_deletes) || ' rows from aida_names');

    v_diff := dbms_utility.get_time - v_start;
    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Time to delete rows from aida_names: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Time to delete rows from aida_names: ' || v_diff_calc);

    ---------

    -- PROCESS COMMIT BEHAVIOR
    if (V_COMMIT = 'N') then
        rollback;
        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('Rollback Done');
        END IF;
        dbms_output.put_line('Rollback Done');
    elsif (V_COMMIT = 'Y') then
        commit;
        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('Commit Done');
        END IF;
        dbms_output.put_line('Commit Done');
    end if;

    ---------

    v_diff := dbms_utility.get_time - v_start_total_ca;

    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
        into v_daytime
        from dual;

    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('End function set_aida_lcls_ca at : ' || v_daytime);
    END IF;
    dbms_output.put_line('End function set_aida_lcls_ca at : ' || v_daytime);

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Total function set_aida_lcls_ca time: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Total function set_aida_lcls_ca time: ' || v_diff_calc);

    v_sqlcode := 0;  -- Indicates success.

    return v_sqlcode;

-- Poonam - Added individual return v_sqlcode for each exception, so it can return more accurate error message.
--          Earlier it was not returning the exact error message and where it took place.
    exception
        when invalid_service_name_error then
            v_sqlcode := -20010;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20010, v_errmsg);
    return v_sqlcode;

        when null_service_name_error then
            v_sqlcode := -20020;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20020, v_errmsg);
    return v_sqlcode;

        when del_aida_old_lcls_ca_error then
            v_sqlcode := -20030;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20030, v_errmsg);
    return v_sqlcode;

        when del_aida_new_lcls_ca_error then
            v_sqlcode := -20040;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20040, v_errmsg);
    return v_sqlcode;

        when pop_aida_old_lcls_ca_error then
            v_sqlcode := -20050;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20050, v_errmsg);
    return v_sqlcode;

        WHEN DBLINK_NOT_FOUND_ERROR THEN
            v_sqlcode := -20060;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20060, V_ERRMSG);
    return v_sqlcode;

        WHEN DBLINK_INVALID_ERROR THEN
            v_sqlcode := -20070;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20070, V_ERRMSG);
    return v_sqlcode;

        WHEN UNEXPECTED_INSTANCE_ERROR THEN
            v_sqlcode := -20080;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20080, V_ERRMSG);
    return v_sqlcode;

        WHEN EXEC_IMMED_ERROR THEN
            v_sqlcode := -20090;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20090, V_ERRMSG);
    return v_sqlcode;

        WHEN DEL_NAMES_ERROR THEN
            v_sqlcode := -20100;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20100, V_ERRMSG);
    return v_sqlcode;

        WHEN DEL_DIR_ERROR THEN
            v_sqlcode := -20110;
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- RAISE_APPLICATION_ERROR(-20110, V_ERRMSG);
    return v_sqlcode;

        when others then
            v_sqlcode := sqlcode;
            v_errmsg := substr('Error (' || v_proc2 || ') => ' || sqlerrm, 1, 500);
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
            END IF;
            ROLLBACK;
            -- raise_application_error(-20120, v_errmsg);

    return v_sqlcode;

end set_aida_lcls_ca;

------------

begin

    V_PROC := 'SET_AIDA_LCLS_CA.SQL';

    v_start_total := dbms_utility.get_time;

    v_errmsg := NULL;

    V_SQLERRMSG := NULL;

    V_COMMIT := NULL;

    --------------

    -- DETERMINE LOGON INSTANCE NAME.
    SELECT SYS_CONTEXT ('USERENV','DB_NAME')
      INTO V_DB_NAME
      FROM DUAL;

    --------------

    -- ASSIGN LOG FILE NAME.
    V_LOG_FILE := 'set_aida_lcls_ca_'||lower(v_db_name)||'_'||lower(user)||'.log';

    --------------

    -- INITIALIZE LOGGING.
    BEGIN
    CONTROLS_GLOBAL.WRITE_LOG.INITIALIZE(IPATH => V_DIR, IFILE => V_LOG_FILE, IBUFFER => V_BUFFER, IMODE => 'W');
    V_OK_TO_PRINT := TRUE;
    EXCEPTION
      WHEN OTHERS THEN
        V_OK_TO_PRINT := FALSE;
        DBMS_OUTPUT.PUT_LINE('ERROR: LOGGING COULD NOT BE INITIALIZED => '||SQLERRM);
    END;

    --------------

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('LOGGED IN AS '||USER||'@'||V_DB_NAME);
    END IF;
    DBMS_OUTPUT.PUT_LINE('LOGGED IN AS '||USER||'@'||V_DB_NAME);
    DBMS_OUTPUT.PUT_LINE(' ');

    --------------

    -- Get starting date and time.
    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
        into v_daytime
        from dual;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('BEGIN PROCESSING AT: ' || v_daytime);
    END IF;
    DBMS_OUTPUT.PUT_LINE('BEGIN PROCESSING AT: ' || v_daytime);
    DBMS_OUTPUT.PUT_LINE(' ');

    --------------

    -- CHECK IF IRMIS CRAWLER UPDATES HAVE COMPLETED SUCCESSFULLY.  IF NOT, THEN ABORT THIS SCRIPT.
    BEGIN
    SELECT STATUS_CHK
      INTO V_STATUS_CHK
      FROM V_PROCESS_STATUS
     WHERE INST       = V_INSTANCE_DO_SYNCH
       AND SCHEMA_NM  = V_TRGT_SCHEMA
       AND PROCESS_NM = V_IRMIS_PROCESS_NM
       AND STAGE      = V_IRMIS_STAGE;

    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        v_errmsg := substr('ERROR ('||V_PROC||'): ROW FOR IRMISDB NOT FOUND IN V_PROCESS_STATUS', 1, 500);
        RAISE V_PROCESS_STATUS_ERROR;
      WHEN OTHERS THEN
        v_errmsg := substr('ERROR ('||V_PROC||'): V_PROCESS_STATUS RETRIEVAL CAUSED: '||SQLERRM, 1, 500);
        RAISE V_PROCESS_STATUS_ERROR;
    END;

    --------------

    IF ( V_STATUS_CHK IS NULL ) THEN
      v_errmsg := substr('ERROR ('||V_PROC||'): ROW FOR IRMISDB IN V_PROCESS_STATUS HAS NULL STATUS', 1, 500);
      RAISE V_PROCESS_STATUS_ERROR;
    ELSIF ( V_STATUS_CHK = 'FAILED' ) THEN
      v_errmsg := substr('ERROR ('||V_PROC||'): ROW FOR IRMISDB IN V_PROCESS_STATUS HAS "FAILED" STATUS', 1, 500);
      RAISE V_PROCESS_STATUS_ERROR;
    ELSIF ( V_STATUS_CHK = 'SUCCESS' ) THEN
      NULL;
    ELSE
      v_errmsg := substr('ERROR ('||V_PROC||'): ROW FOR IRMISDB IN V_PROCESS_STATUS CONTAINS INVALID STATUS='||V_STATUS_CHK, 1, 500);
      RAISE V_PROCESS_STATUS_ERROR;
    END IF;

    --------------

    -- RECORD START OF SCRIPT IN BOTH VALIDATION TABLE AND IN LOG FILE REGARDLESS OF THE LOGON INSTANCE (SLACPROD OR MCCQA).
    V_PROCESS_GRP_NO := NULL;

    ADD_TO_DATA_VALIDATION_TAB('Validation', NULL, 'Begins', V_PROCESS_GRP_NO, V_SQLERRMSG);

    IF ( V_SQLERRMSG IS NOT NULL ) THEN
      IF (V_OK_TO_PRINT) THEN
        CONTROLS_GLOBAL.WRITE_LOG.WRITE('1) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
      END IF;
      dbms_output.put_line('1) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
    END IF;

    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
      into v_daytime
      from dual;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('INVOKING FUNCTION SET_AIDA_LCLS_CA AT '||V_DAYTIME);
    END IF;
    DBMS_OUTPUT.PUT_LINE('INVOKING FUNCTION SET_AIDA_LCLS_CA AT '||V_DAYTIME);

    --------------

    -- CALL THE FUNCTION.
 -- errcode := set_aida_lcls_ca('EPICS_lcls', 'Y');
    errcode := set_aida_lcls_ca('&1', '&2');

    --------------

    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
      into v_daytime
      from dual;

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('RETURNED FROM FUNCTION SET_AIDA_LCLS_CA AT '||V_DAYTIME);
    END IF;
    DBMS_OUTPUT.PUT_LINE('RETURNED FROM FUNCTION SET_AIDA_LCLS_CA AT '||V_DAYTIME);

    --------------

    /* ===========================================================================================================================
    LOGIC:
    1) REGARDLESS OF ERRCODE, DATA IS ALWAYS WRITTEN TO TABLE CONTROLS_GLOBAL.DATA_VALIDATION_TAB (AND, VIA AN AUTONOMOUS TRIGGER,
       TO CONTROLS_GLOBAL.DATA_VALIDATION_AUDIT).
    2) IF (V_COMMIT = 'Y' *AND* V_DB_NAME = V_INSTANCE_DO_SYNCH), THEN A SYNCHRONIZATION BETWEEN MCCQA AND MCCO IS PERFORMED.
    3) IF (V_COMMIT = 'Y' *AND* V_DB_NAME = V_INSTANCE_NO_SYNCH), THEN A SYNCHRONIZATION IS *NOT* PERFORMED.
    4) IF (V_COMMIT = 'N'), THEN NO SYNCHRONIZATION IS PERFORMED REGARDLESS OF THE VALUE OF V_DB_NAME.
    =========================================================================================================================== */

    IF ( ERRCODE = 0 ) THEN

      -- SCRIPT INVOCATION WAS SUCCESSFUL.  RECORD STATUS IN BOTH VALIDATION TABLE AND IN LOG FILE.
      ADD_TO_DATA_VALIDATION_TAB('Validation', 'SUCCESS', 'Validation Completed Successfully', V_PROCESS_GRP_NO, V_SQLERRMSG);

      IF ( V_SQLERRMSG IS NOT NULL ) THEN
        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('2) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
        END IF;
        dbms_output.put_line('2) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
      END IF;

      IF (V_OK_TO_PRINT) THEN
        CONTROLS_GLOBAL.WRITE_LOG.WRITE('END SET_AIDA_LCLS_CA: SUCCESS (ERRCODE='||TO_CHAR(ERRCODE)||').');
      END IF;
      dbms_output.put_line('END SET_AIDA_LCLS_CA: SUCCESS');

      ------

      IF ( V_COMMIT = 'Y' ) THEN

        IF ( V_DB_NAME = V_INSTANCE_DO_SYNCH ) THEN

          -- SCRIPT WAS RUN WHILE LOGGED ON MCCQA, AND SO, A SYNCHRONIZATION PROCESS TO REFRESH MCCO SHOULD BE INVOKED.

          DBMS_OUTPUT.PUT_LINE(V_CRLF||V_CRLF||'***********************************');

          -- RECORD START OF SYNCHRONIZATION IN LOG FILE.
          IF (V_OK_TO_PRINT) THEN
            CONTROLS_GLOBAL.WRITE_LOG.WRITE('BEGIN AIDA SYNCH (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES.');
          END IF;
          dbms_output.put_line('BEGIN AIDA SYNCH (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES.');

          ------

          v_start := dbms_utility.get_time;

/* =========================================================================
  V_STMT := 'SELECT CONTROLS_GLOBAL.SYNCH_MAIN_PKG.SYNCH_DATA(:a,:b,:c,:d,:e) FROM DUAL';

  BEGIN
  EXECUTE IMMEDIATE V_STMT INTO V_CURR_RETVAL
                           USING V_TGT_INSTANCE_NM,
                                 V_TGT_SCHEMA_NM,
                                 V_PROCESS_GRP_NO,
                                 V_INIT_LOG_FLAG,
                                 V_TEST_ROWS;
  EXCEPTION
    WHEN OTHERS THEN
      V_ERRMSG := SUBSTR('ERROR ('||V_PROC||'): EXECUTE IMMEDIATE FOR "'||V_STMT||'" =>'||SQLERRM, 1, 500);
    RAISE EXEC_ERROR;
  END;
========================================================================= */

  V_CURR_RETVAL := CONTROLS_GLOBAL.SYNCH_MAIN_PKG.SYNCH_DATA(V_TGT_INSTANCE_NM,
                                                             V_TGT_SCHEMA_NM,
                                                             V_PROCESS_GRP_NO,
                                                             V_INIT_LOG_FLAG,
                                                             V_TEST_ROWS);

          v_diff := dbms_utility.get_time - v_start;
          v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

          IF (V_OK_TO_PRINT) THEN
            CONTROLS_GLOBAL.WRITE_LOG.WRITE('TIME TO SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||'): ' || v_diff_calc);
          END IF;
          dbms_output.put_line('TIME TO SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||'): ' || v_diff_calc);

          ----------

          IF ( V_CURR_RETVAL = 0 ) THEN

            -- AIDA SYNCHRONIZATION WAS SUCCESSFUL.  RECORD STATUS IN LOG FILE.
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE('END SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES: SUCCESS');
            END IF;
            dbms_output.put_line('END SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES: SUCCESS');

          ELSIF ( V_CURR_RETVAL != 0 ) THEN

            V_ERRMSG := SUBSTR(SQLERRM(V_CURR_RETVAL),1,500);

            -- AIDA SYNCHRONIZATION FAILED.  RECORD FAILED STATUS IN LOG FILE.
            IF (V_OK_TO_PRINT) THEN
              CONTROLS_GLOBAL.WRITE_LOG.WRITE('END SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES WITH ERROR: '||V_ERRMSG);
            END IF;
            dbms_output.put_line('END SYNCH AIDA (PROCESS_GRP_NO='||TO_CHAR(V_PROCESS_GRP_NO)||') OF MCCO WITH MCCQA DML CHANGES WITH ERROR: '||V_ERRMSG);

          END IF;  -- IF ( V_CURR_RETVAL = 0 ) THEN

          -----------
  
          DBMS_OUTPUT.PUT_LINE('***********************************');

        ELSIF ( V_DB_NAME = V_INSTANCE_NO_SYNCH ) THEN

          -- SCRIPT WAS INVOKED FROM SLACPROD, AND SO, SYNCHRONIZATION WAS NOT DONE.  RECORD STATUS IN LOG FILE.
          IF (V_OK_TO_PRINT) THEN
            CONTROLS_GLOBAL.WRITE_LOG.WRITE('SCRIPT WAS INVOKED FROM '||V_DB_NAME||' AND SO, SYNCHRONIZATION WAS *NOT* DONE.');
          END IF;
          dbms_output.put_line('SCRIPT WAS INVOKED FROM '||V_DB_NAME||' AND SO, SYNCHRONIZATION WAS *NOT* DONE.');
  
        END IF;  -- IF ( V_DB_NAME = V_INSTANCE_DO_SYNCH ) THEN

      ELSIF ( V_COMMIT = 'N' ) THEN

        -- COMMIT WAS NOT REQUESTED, AND SO, SYNCHRONIZATION WAS NOT DONE.  RECORD STATUS IN LOG FILE.
        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('COMMIT WAS NOT REQUESTED, AND SO, SYNCHRONIZATION WAS *NOT* DONE.');
        END IF;
        dbms_output.put_line('COMMIT WAS NOT REQUESTED, AND SO, SYNCHRONIZATION WAS *NOT* DONE.');

      END IF;  -- IF ( V_COMMIT = 'Y' ) THEN

    ELSIF ( ERRCODE != 0 ) THEN

      IF ( ERRCODE = V_UNEXPECTED_ERROR_CODE ) THEN
        V_ERRMSG := SUBSTR('UN-EXPECTED ERROR OCCURRED DURING INVOCATION OF '||V_PROC||' AT '||TO_CHAR(SYSDATE, 'DD-MON-YYYY HH:MI:SS PM'),1,500);
      END IF;

      -- SCRIPT INVOCATION FAILED.  RECORD FAILED STATUS IN BOTH VALIDATION TABLE AND IN LOG FILE.
      ADD_TO_DATA_VALIDATION_TAB('Validation', 'FAILED', v_errmsg, V_PROCESS_GRP_NO, V_SQLERRMSG);

      IF ( V_SQLERRMSG IS NOT NULL ) THEN
        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('3) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
        END IF;
        dbms_output.put_line('3) ERROR: INSERT INTO TABLE DATA_VALIDATION_TAB FAILED: ' || V_SQLERRMSG);
      END IF;

      IF (V_OK_TO_PRINT) THEN
        CONTROLS_GLOBAL.WRITE_LOG.WRITE('END SCRIPT WITH ERROR: '||v_errmsg);
      END IF;
      dbms_output.put_line('END SCRIPT WITH ERROR: '||v_errmsg);

    END IF;  -- IF ( ERRCODE = 0 ) THEN

    -----------

    v_diff := dbms_utility.get_time - v_start_total;

    select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
        into v_daytime
        from dual;

    v_diff_calc := to_char(round((v_diff / 100), 2)) || ' seconds';

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('End Total Processing at : ' || v_daytime);
    END IF;
    dbms_output.put_line('End Total Processing at : ' || v_daytime);

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.WRITE('Total Processing Time: ' || v_diff_calc);
    END IF;
    dbms_output.put_line('Total Processing Time: ' || v_diff_calc);

    -- Write error code to log file.  NO OTHER "dbms_output.put_line"
    -- STATEMENT SHOULD APPEAR AFTER THIS LINE!  The invoking
    -- lcls_ca_populate_aida_database.sh script expects the error
    -- code with the format "errcode: n" to be the last line written
    -- by this set_aida_lcls_ca.sql script.

    dbms_output.put_line('errcode: ' || TO_CHAR(ERRCODE));


    -----------

    IF (V_OK_TO_PRINT) THEN
      CONTROLS_GLOBAL.WRITE_LOG.TERMINATE;
    END IF;


EXCEPTION
    WHEN V_PROCESS_STATUS_ERROR THEN
        select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
          into v_daytime
          from dual;

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);
        END IF;
        dbms_output.put_line(V_CRLF||V_CRLF||'************************************************************************');
        dbms_output.put_line('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
        END IF;
        dbms_output.put_line(v_errmsg);
        dbms_output.put_line('************************************************************************');

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.TERMINATE;
        END IF;

    WHEN EXEC_ERROR THEN
        select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
          into v_daytime
          from dual;

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);
        END IF;
        dbms_output.put_line(V_CRLF||V_CRLF||'************************************************************************');
        dbms_output.put_line('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
        END IF;
        dbms_output.put_line(v_errmsg);
        dbms_output.put_line('************************************************************************');

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.TERMINATE;
        END IF;

    WHEN OTHERS THEN
        v_errmsg := SUBSTR(SQLERRM,1,500);

        select to_char(sysdate, 'DD-MON-YYYY HH:MI:SS PM')
          into v_daytime
          from dual;

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);
        END IF;
        dbms_output.put_line(V_CRLF||V_CRLF||'************************************************************************');
        dbms_output.put_line('THE FOLLOWING ERROR WAS DETECTED AT: '||V_DAYTIME);

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.WRITE(v_errmsg);
          CONTROLS_GLOBAL.WRITE_LOG.WRITE('************************************************************************');
        END IF;
        dbms_output.put_line(v_errmsg);
        dbms_output.put_line('************************************************************************');

        IF (V_OK_TO_PRINT) THEN
          CONTROLS_GLOBAL.WRITE_LOG.TERMINATE;
        END IF;

end;
/
exit;
