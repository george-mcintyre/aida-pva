set serveroutput on size 1000000
/*
--==============================================================
--
--  Abs:   Populate the Aida database tables for the Aida
--         utility server. 
--
--  Name:  set_aida_slc_utility.sql
--
--  Rem:   This script populates Aida database tables for the Aida
--         utility server from instance/attribute pairs read from
--         a text file.  The text file name is "aida_utility_load.dat" 
--         and is located in the /nfs/slac/g/cd/oracle/aida directory.
--         This directory is associated with the Oracle "create
--         directory" symbol AIDA_ORACLE, which is used for supplying
--         the directory parameter to the UTL_FILE.FOPEN routine.
--         The input text file is generated from running the Perl script
--         create_aida_utility_load_file.pl using the input file
--         "trignames.dmp".  This file is generated in the VMS NFS_NDB
--         directory by the VMS command file slccom:slcdb2oracle.submit,
--         which is run at time of a DB Install.  The VMS NFS_NDB
--         directory is mapped to the UNIX /mccdev/nfs_slcdb directory,
--         which is visible from such UNIX machines as slcs2.
--         
--         The Aida database is populated by adding new rows to the
--         aida_directory and aida_names tables.  Rows may also be
--         deleted from these tables if the Aida instance/attribute
--         pairs that were formerly in the "aida_utility_load.dat"
--         file are no longer in the new version of this file.  The Oracle
--         tables aida_old_slc_utility and aida_new_slc_utility are used
--         by this routine to hold id numbers during processing.  These
--         tables are used as temporary storage space for this script.
--
--  Auth:  27-Jul-2007, Bob Hall (rdh)
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--    27-Apr-2009, Bob Hall
--       Introduced log messages.
--
--==============================================================
*/

declare

    status            integer;
    service_num       integer;
    row_found         integer;
    name_id_num       integer;
    dir_entry_found   integer;
    old_name_id_num   integer;
    new_name_id_num   integer;
    first_read        integer;
    more_old_rows     integer;
    more_new_rows     integer;
    num_name_inserts  integer;
    num_dir_inserts   integer;
    num_name_deletes  integer;
 
    err_num           number;
    errcode           number;
    start_total       number;
    time_diff         number;

    out_file          UTL_FILE.file_type;
    in_file           UTL_FILE.file_type;

    service_name      varchar2(50);
    err_msg           varchar2(100);
    instance          varchar2(200);
    attribute         varchar2(200);
    sql_stmt_1        varchar2(200);
    sql_stmt_2        varchar2(200);
    sql_stmt_3        varchar2(200);
    daytime           varchar2(100);
    diff_calc         varchar2(100);
    user_name         varchar2(50);

    type slc_utility_ids_type is ref cursor;
    slc_utility_ids      slc_utility_ids_type;

    type old_slc_utility_ids_type is ref cursor;
    old_slc_utility_ids  old_slc_utility_ids_type;

    type new_slc_utility_ids_type is ref cursor;
    new_slc_utility_ids  new_slc_utility_ids_type;

/* The PL/SQL routine delete_slc_utility_id is used by the main routine
   to delete the row in the Oracle aida_directory table associated
   with the specified id number and SLC utility server service number.
   It will also delete the row in the aida_names table associated with
   the specified id number if there are no other rows in the
   aida_directory table that refer to the id number.
-------------------------------------------------------------------- */
procedure delete_slc_utility_id (slc_utility_id in integer,
                              service_num in integer) is

    adir_entry_found integer;

begin

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('entering delete_slc_utility_id with id = ' ||
        to_char(slc_utility_id));
    */

    execute immediate 'delete from aida_directory where name_id = ' ||
        to_char(slc_utility_id) || ' and service_id = ' || to_char(service_num);

    execute immediate 'select count(*) from aida_directory where name_id = ' ||
        to_char(slc_utility_id) into adir_entry_found;

    if (adir_entry_found = 0) then

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('deleting row from aida_names for id = ' ||
            to_char(slc_utility_id));
        */

        execute immediate 'delete from aida_names where id = ' ||
            to_char(slc_utility_id);

    end if;
end;

begin

    start_total := dbms_utility.get_time;

    select user
        into user_name
        from dual;

    dbms_output.put_line('Log for populating database for Aida SLC Utility server.  User name: ' || user_name);

    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;

    dbms_output.put_line('Begin processing at: ' || daytime);

    /* Truncate any existing rows from the Oracle tables aida_old_slc_utility
       and aida_new_slc_utility used as storage space in this script.
    ---------------------------------------------------------------------- */
    execute immediate 'truncate table aida_old_slc_utility';

    execute immediate 'truncate table aida_new_slc_utility';

    /* Retrieve the service id number from the aida_services table
       associated with the "SLCUtil" data provider service.
    -------------------------------------------------------------- */
    service_name := 'SLCUtil';

    execute immediate 'select id from aida_services where name = ''' ||
        service_name || '''' into service_num;

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('service_num = ' || service_num);
    */

    /* Populate the aida_old_slc_utility table with all of the name_id
       numbers in rows of the aida_directory associated with the
       "SLCUtil" data provider service.
    --------------------------------------------------------------- */
    sql_stmt_1 := 'select name_id from aida_directory where service_id =  ' ||
        to_char(service_num);

    status := 1;
    open slc_utility_ids for sql_stmt_1;
    while (status = 1) loop
        fetch slc_utility_ids into name_id_num;
        exit when slc_utility_ids%NOTFOUND;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('existing name_id_num = ' || to_char(name_id_num));
        */

        execute immediate 'insert into aida_old_slc_utility values (' ||
            to_char(name_id_num) || ')';

    end loop;

    /* Open the file "aida_utility_load.dat" for reading.  This file
       is located in the directory /nfs/slac/g/cd/oracle/aida (associated
       with the Oracle "create directory" symbol AIDA_ORACLE).  The file
       contains the Aida utility server instance/attribute pairs.  There
       are two consecutive lines in this file for each signal: one
       containing the Aida instance name and the other containing the
       Aida attribute name.
    --------------------------------------------------------------------- */
    in_file := UTL_FILE.FOPEN('AIDA_ORACLE',
                           'aida_utility_load.dat',
                           'r',
                           201);

    /* Read each instance/attribute two line signal entry in the input
       file.  Exit the loop when the read of the first line for the
       entry (the instance) fails due to no more data in the file.
    ------------------------------------------------------------------ */
    num_name_inserts := 0;
    num_dir_inserts := 0;
    status := 1;
    while (status = 1) loop
        BEGIN
            UTL_FILE.GET_LINE(in_file,
                              instance,
                              200);

        EXCEPTION
            WHEN NO_DATA_FOUND then
                BEGIN
                    /***DEBUG
                    DBMS_OUTPUT.PUT_LINE('no data found after instance');
                    */
                    EXIT;
                END;

            WHEN others then
                NULL;

        END;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('instance = ' || instance);
        */

        UTL_FILE.GET_LINE(in_file,
                          attribute,
                          200);

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('attribute = ' || attribute);
        */

        /* Retrieve the number of rows in the aida_names table containing the
           just read instance and attribute names.  If no rows were found,
           insert a row into this table containing these instance and attribute
           names.
        ----------------------------------------------------------------------- */
        execute immediate 'select count(*) from aida_names where instance = ''' ||
            instance || ''' and attribute = ''' || attribute || '''' into row_found;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('row_found = ' || to_char(row_found));
        */

        if (row_found = 0) then

            /***DEBUG
            DBMS_OUTPUT.PUT_LINE('inserting new row into aida_names');
            */

            execute immediate 'insert into aida_names (instance, attribute, transform) ' ||
                'values (''' ||  instance || ''', ''' ||
                attribute || ''', NULL)';

            num_name_inserts := num_name_inserts + 1;

        end if;

        /* Retrieve the id for the newly inserted or previously existing row in
           the aida_names table having the previously read instance and attribute
           names.  If this id number is not already in the aida_new_slc_utility table
           (which could occur if the input file has more than one occurance of
           an instance/attribute pair), insert this id number into the
           aida_new_slc_utility table, which will contain the aida_names id number
           or each signal instance/attribute pair in the input file. 
        -------------------------------------------------------------------------- */
        execute immediate 'select id from aida_names where instance = ''' ||
            instance || ''' and attribute = ''' || attribute || '''' into name_id_num;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('name_id_num = ' || to_char(name_id_num));
        */

        execute immediate 'select count(*) from aida_new_slc_utility where name_id = ' ||
            to_char(name_id_num) into row_found;

        if (row_found = 0) then

            execute immediate 'insert into aida_new_slc_utility values (' ||
                to_char(name_id_num) || ')';

        end if;

        /* Determine if there are any rows in the aida_directory table containing
           the aida_names id number and SLCUtil service id number.
           If there were no such rows, insert a row into this table containing
           these two id numbers.
        ------------------------------------------------------------------------- */ 
        execute immediate 'select count(*) from aida_directory where name_id = ' ||
            to_char(name_id_num) || ' and service_id = ' ||
            to_char(service_num) into dir_entry_found;

        if (dir_entry_found = 0) then

            /***DEBUG
            DBMS_OUTPUT.PUT_LINE('inserting new row into aida_directory');
            */

            execute immediate 'insert into aida_directory (name_id, service_id, order_no) values (' ||
                to_char(name_id_num) || ', ' || to_char(service_num) || ', 1)';

            num_dir_inserts := num_dir_inserts + 1;

        end if;

    end loop;


    UTL_FILE.FCLOSE(in_file);

    dbms_output.put_line('Inserted ' || to_char(num_name_inserts) || ' rows into aida_names');
    dbms_output.put_line('Inserted ' || to_char(num_dir_inserts) || ' rows into aida_directory');

    /* The Aida database has now been populated with information associated
       with the instance/attribute pairs in the input file just processed.
       Now rows in the aida_directory table (and possibly the aida_names
       table) will be deleted if there are Aida utility server entries in the
       database that are not associated with instance/attribute pairs in
       the new instance of the input file just processed.  The method of
       detecting the aida_names id numbers associated with the rows to be
       deleted is to compare the sorted contents of the aida_old_slc_utility
       and aida_new_slc_utility processing tables.  Any aida_names id number
       in the aida_old_slc_utility table that is not in the aida_new_slc_utility
       table is an id associated with a Aida utility server entry that is in the
       Aida database tables but is not associated with an instance/attribute
       pair in the input file just processed.  The aida_directory table
       row associated with such a aida_names id number will be deleted by
       calling the routine delete_slc_utility_id (the associated aida_names
       row may be deleted also).
    ------------------------------------------------------------------------ */
       
    sql_stmt_2 := 'select name_id from aida_old_slc_utility order by name_id';

    open old_slc_utility_ids for sql_stmt_2;

    sql_stmt_3 := 'select name_id from aida_new_slc_utility order by name_id';

    open new_slc_utility_ids for sql_stmt_3;

    first_read := 1;
    more_old_rows := 1;
    more_new_rows := 1;
    num_name_deletes := 0;
    status := 1;

    /* The loop will be exited when there are either no more entries in the
       aida_old_slc_utility table that have not been processed or there are no
       more entries in the aida_new_slc_utility table that have not been
       processed.
    ----------------------------------------------------------------------- */
    while (status = 1) loop

        /* If this the first time through the loop, read an aida_names id
           from both the aida_old_slc_utility and aida_new_slc_utility sorted
           tables.
        ----------------------------------------------------------------- */
        if (first_read = 1) then

            first_read := 0;

            fetch old_slc_utility_ids into old_name_id_num;
            if (old_slc_utility_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

            fetch new_slc_utility_ids into new_name_id_num;
            if (new_slc_utility_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        end if;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('old name_id_num = ' || to_char(old_name_id_num));
        DBMS_OUTPUT.PUT_LINE('new name_id_num = ' || to_char(new_name_id_num));
        */

        /* The current aida_names id numbers from the sorted aida_old_slc_utility
           and aida_new_slc_utility tables are compared.  If they are equal, read
           an aida_names id from the next entries in the sorted
           aida_old_slc_utility and aida_new_slc_utility tables.
        ---------------------------------------------------------------------- */
        if (old_name_id_num = new_name_id_num) then

            fetch old_slc_utility_ids into old_name_id_num;
            if (old_slc_utility_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

            fetch new_slc_utility_ids into new_name_id_num;
            if (new_slc_utility_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        /* If the current aida_names id number from the sorted
           aida_old_slc_utility table is less than the current aida_names id
           number from the aida_new_slc_utility table, then there is
           not a match in the aida_new_slc_utility table for the current
           aida_names id number from the aida_old_slc_utility table.
           Therefore, this aida_names id number is not associated with
           a instance/attribute pair in the input file just processed
           and the Aida utility server information in the Aida database for this
           id may be deleted.  After calling delete_slc_utility_id to do
           this deletion, get the next current aida_old_slc_utility
           aida_names id number from the sorted table.
        ------------------------------------------------------------------ */
        elsif (old_name_id_num < new_name_id_num) then

            delete_slc_utility_id(old_name_id_num, service_num);

            num_name_deletes := num_name_deletes + 1;

            fetch old_slc_utility_ids into old_name_id_num;
            if (old_slc_utility_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

        /* If the current aida_names id number from the sorted
           aida_old_slc_utility table is greater than the current aida_names
           id number from the aida_new_slc_utility table, get the next
           current aida_new_slc_utility aida_names id number from the
           sorted table.
        ----------------------------------------------------------------- */
        elsif (old_name_id_num > new_name_id_num) then

            fetch new_slc_utility_ids into new_name_id_num;
            if (new_slc_utility_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        end if;
    end loop;

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('exited from delete check loop');
    */

    /* If there are any remaining entries from the sorted
       aida_old_slc_utility table that have not been processed,
       loop through them and delete the Aida utility server 
       information in the Aida database associated with
       each by calling routine delete_slc_utility_id.
    -------------------------------------------------------- */
    if (more_old_rows = 1) then

        status := 1;
        while (status = 1) loop

            delete_slc_utility_id(old_name_id_num, service_num);

            num_name_deletes := num_name_deletes + 1;

            fetch old_slc_utility_ids into old_name_id_num;
            if (old_slc_utility_ids%NOTFOUND) then
                exit;
            end if;

        end loop;

    end if;

    dbms_output.put_line('Deleted ' || to_char(num_name_deletes) || ' rows from aida_names');

    time_diff := dbms_utility.get_time - start_total;

    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;

    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('End processing at : ' || daytime);
    dbms_output.put_line('Total processing time: ' || diff_calc);

    errcode := 0;
    dbms_output.put_line('set_aida_slc_utility errcode: ' || to_char(errcode));

    execute immediate 'commit';

    exception
        when NO_DATA_FOUND then
            DBMS_OUTPUT.PUT_LINE('no data found error');
            UTL_FILE.FCLOSE(in_file);

            errcode := 1;
            dbms_output.put_line('set_aida_slc_utility errcode: ' || to_char(errcode));

            execute immediate 'rollback';

        when OTHERS then
            DBMS_OUTPUT.PUT_LINE('error occurred');

            err_num := SQLCODE;
            DBMS_OUTPUT.PUT_LINE('err_num = ' || to_char(err_num));

            err_msg := substr(SQLERRM, 1, 100);
            DBMS_OUTPUT.PUT_LINE('err_msg = ' || err_msg);

            UTL_FILE.FCLOSE(in_file);

            errcode := 1;
            dbms_output.put_line('set_aida_slc_utility errcode: ' || to_char(errcode));

            execute immediate 'rollback';

end;
/
exit;
