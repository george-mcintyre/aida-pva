set serveroutput on size unlimited 
/*************************************************************/
/*  This script populates Aida database tables from Channel  */
/*  Archiver instance/attribute pairs read from text files.  */
/*  The text files are the following files located in the    */
/*  /nfs/slac/g/cd/oracle/aida directory:                    */
/*    1.  converted_arch_pvs_lcls.dat                        */
/*    2.  converted_arch_pvs_facet.dat                       */
/*    3.  converted_arch_pvs_pepii.dat                       */
/*    4.  converted_arch_pvs_nlcta.dat                       */
/*  This directory is associated with the Oracle "create     */
/*  directory" symbol AIDA_ORACLE, which is used for         */
/*  supplying the directory parameter to the UTL_FILE.FOPEN  */
/*  routine.  Each input text file is generated from running */
/*  the Perl script convert_archiver_config_files.pl with    */
/*  the associated subsystem argument (lcls, facet, pepii,   */
/*  or nlcta).  That script obtains the channel names from   */
/*  the list of PV channel names that are currently being    */
/*  archived for each subsystem.                             */
/*                                                           */
/*  The Aida database is populated by adding new rows to the */
/*  aida_directory and aida_names tables.  Rows may also be  */
/*  deleted from these tables if the Channel Archiver        */
/*  instance/attribute pairs that were formerly in the       */
/*  "converted_arch_pvs_<subsystem>.dat" file (where         */
/*  "<subsystem>" is replaced by lcls, facet, pepii, or      */
/*  nlcta) currently being processed are no longer in the    */
/*  new version of this file.  The Oracle tables             */
/*  aida_old_arch and aida_new_arch are used by this         */
/*  routine to hold id numbers during processing.  These     */
/*  tables are used as temporary storage space for this      */
/*  script.                                                  */
/*                                                           */
/*  Auth:  Bob Hall, 17-Apr-2007                             */
/*                                                           */
/*  Mods: (Latest to oldest)                                 */
/*     02-Feb-2011, Bob Hall                                 */
/*        Added support for the new FACET Aida Channel       */
/*        Archiver server.                                   */
/*     25-Jun-2010, Bob Hall                                 */
/*        Modified to use bind variables in Oracle           */ 
/*        statements.                                        */
/*     23-Apr-2009, Bob Hall                                 */
/*        Introduced log messages.                           */
/*                                                           */
/*************************************************************/

declare

    status                      integer;
    service_num                 integer;
    row_found                   integer;
    name_id_num                 integer;
    dir_entry_found             integer;
    old_name_id_num             integer;
    new_name_id_num             integer;
    first_read                  integer;
    more_old_rows               integer;
    more_new_rows               integer;
    num_name_inserts            integer;
    num_dir_inserts             integer;
    num_name_deletes            integer;
    num_old_id_table_rows       integer;
 
    err_num           number;
    errcode           number;
    start_total       number;
    start_time        number;
    time_diff         number;

    out_file          UTL_FILE.file_type;
    in_file           UTL_FILE.file_type;

    v_arr_limit       pls_integer := 1000;

    aida_services_select_stmt            constant varchar2(200) := 'select id from aida_services where name = :bind_service_name';

    aida_names_select_count_stmt         constant varchar2(200) := 'select count(*) from aida_names where instance = :bind_instance and attribute = :bind_attribute';

    aida_names_select_stmt               constant varchar2(200) := 'select id from aida_names where instance = :bind_instance and attribute = :bind_attribute';

    aida_names_insert_stmt               constant varchar2(200) := 'insert into aida_names (instance, attribute, transform) values (:bind_rec_nm2, :bind_attribute2, :bind_transform)';

    aida_new_arch_select_count_st        constant varchar2(200) := 'select count(*) from aida_new_arch where name_id = :bind_name_id';

    aida_new_arch_insert_stmt            constant varchar2(200) := 'insert into aida_new_arch values (:bind_name_id_num)';

    aida_dir_select_count_stmt           constant varchar2(200) := 'select count(*) from aida_directory where name_id = :bind_name_id and service_id = :bind_service_id';

    aida_directory_insert_stmt           constant varchar2(200) := 'insert into aida_directory (name_id, service_id, order_no) values (:bind_name_id, :bind_service_id, :bind_order_no)';

    v_transform                          constant varchar2(1)   := NULL;
    v_order_no                           constant number        := 1;

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

    type arch_ids_type is ref cursor;
    arch_ids      arch_ids_type;

    type old_arch_ids_type is ref cursor;
    old_arch_ids  old_arch_ids_type;

    type new_arch_ids_type is ref cursor;
    new_arch_ids  new_arch_ids_type;

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

    pop_aida_old_lcls_ca_error   exception;

/* The PL/SQL routine delete_slc_hist_id is used by the main routine
   to delete the row in the Oracle aida_directory table associated
   with the specified id number and SLC History service number.  It
   will also delete the row in the aida_names table associated with
   the specified id number if there are no other rows in the
   aida_directory table that refer to the id number.
-------------------------------------------------------------------- */
procedure delete_arch_id (arch_id in integer,
                          service_num in integer) is

    adir_entry_found integer;

    aida_directory_delete_stmt           constant varchar2(200) := 'delete from aida_directory where name_id = :bind_name_id and service_id = :bind_service_num';

    aida_names_delete_stmt               constant varchar2(200) := 'delete from aida_names where id = :bind_id';

    aida_dir_select_count_stmt_2         constant varchar2(200) := 'select count(*) from aida_directory where name_id = :bind_name_id';

begin

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('entering delete_arch_id with id = ' ||
        to_char(arch_id));
    */

    execute immediate aida_directory_delete_stmt using arch_id, service_num;

    execute immediate aida_dir_select_count_stmt_2 into adir_entry_found using arch_id;

    if (adir_entry_found = 0) then

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('deleting row from aida_names for id = ' ||
            to_char(arch_id));
        */

        execute immediate aida_names_delete_stmt using arch_id;

    end if;
end;

procedure set_aida_channel_archiver (subsystem_name in varchar2) is

begin


    /* Truncate any existing rows from the Oracle tables aida_old_arch
       and aida_new_arch used as storage space in this script.
    ---------------------------------------------------------------------- */
    execute immediate 'truncate table aida_old_arch';

    execute immediate 'truncate table aida_new_arch';

    /* Retrieve the service id number from the aida_services table
       associated with the appropriate "Chads" data provider service.
    -------------------------------------------------------------- */
    service_name := 'Chads_' || subsystem_name;

    execute immediate aida_services_select_stmt into service_num using service_name;

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('service_num = ' || service_num);
    */

    /* Populate the aida_old_arch table with all of the name_id
       numbers in rows of the aida_directory associated with the
       "Chads" data provider service.
    --------------------------------------------------------------- */
    num_old_id_table_rows := 0;

    open aida_dir_name_id_cur(service_name);

    loop

        fetch aida_dir_name_id_cur
            bulk collect
            into aida_dir_name_id_arr
            limit v_arr_limit;

        num_old_id_table_rows := num_old_id_table_rows + aida_dir_name_id_arr.count;

        -- Bulk insert into table.
        begin
        forall q in 1 .. aida_dir_name_id_arr.count
            insert into aida_old_arch values aida_dir_name_id_arr(q);
        exception
           when others then
               raise pop_aida_old_lcls_ca_error;
        end;

        exit when aida_dir_name_id_cur%notfound;

    end loop;

    close aida_dir_name_id_cur;

    /* Open the file "converted_device_seq.dat" for reading.  This file
       is located in the directory /nfs/slac/g/cd/oracle/aida (associated
       with the Oracle "create directory" symbol AIDA_ORACLE).  The file
       contains the channel archiver instance/attribute pairs.  There
       are two consecutive lines in this file for each signal: one
       containing the Aida instance name and the other containing the
       Aida attribute name.
    --------------------------------------------------------------------- */
    in_file := UTL_FILE.FOPEN('AIDA_ORACLE',
                           'converted_arch_pvs_' || subsystem_name || '.dat',
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
        execute immediate aida_names_select_count_stmt into row_found using instance, attribute;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('row_found = ' || to_char(row_found));
        */

        if (row_found = 0) then

            /***DEBUG
            DBMS_OUTPUT.PUT_LINE('inserting new row into aida_names');
            */

            execute immediate aida_names_insert_stmt using instance, attribute, v_transform;

            num_name_inserts := num_name_inserts + 1; 

        end if;

        /* Retrieve the id for the newly inserted or previously existing row in
           the aida_names table having the previously read instance and attribute
           names.  If this id number is not already in the aida_new_arch table
           (which could occur if the input file has more than one occurance of
           an instance/attribute pair), insert this id number into the
           aida_new_arch table, which will contain the aida_names id number
           or each signal instance/attribute pair in the input file. 
        -------------------------------------------------------------------------- */
        execute immediate aida_names_select_stmt into name_id_num using instance, attribute;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('name_id_num = ' || to_char(name_id_num));
        */

        execute immediate aida_new_arch_select_count_st into row_found using name_id_num;

        if (row_found = 0) then

            execute immediate aida_new_arch_insert_stmt using name_id_num;

        end if;

        /* Determine if there are any rows in the aida_directory table containing
           the aida_names id number and Chads service id number.
           If there were no such rows, insert a row into this table containing
           these two id numbers.
        ------------------------------------------------------------------------- */ 
        execute immediate aida_dir_select_count_stmt into dir_entry_found using name_id_num, service_num;

        if (dir_entry_found = 0) then

            /***DEBUG
            DBMS_OUTPUT.PUT_LINE('inserting new row into aida_directory');
            */

            execute immediate aida_directory_insert_stmt using name_id_num, service_num, v_order_no;

            num_dir_inserts := num_dir_inserts + 1;

        end if;

    end loop;


    UTL_FILE.FCLOSE(in_file);

    dbms_output.put_line('Inserted ' || to_char(num_name_inserts) || ' rows into aida_names');
    dbms_output.put_line('Inserted ' || to_char(num_dir_inserts) || ' rows into aida_directory');

    /* The Aida database has now been populated with information associated
       with the instance/attribute pairs in the input file just processed.
       Now rows in the aida_directory table (and possibly the aida_names
       table) will be deleted if there are channel archiver entries in the
       database that are not associated with instance/attribute pairs in
       the new instance of the input file just processed.  The method of
       detecting the aida_names id numbers associated with the rows to be
       deleted is to compare the sorted contents of the aida_old_arch
       and aida_new_arch processing tables.  Any aida_names id number
       in the aida_old_arch table that is not in the aida_new_arch
       table is an id associated with a channel archiver entry that is in the
       Aida database tables but is not associated with an instance/attribute
       pair in the input file just processed.  The aida_directory table
       row associated with such a aida_names id number will be deleted by
       calling the routine delete_arch_id (the associated aida_names
       row may be deleted also).
    ------------------------------------------------------------------------ */
       
    sql_stmt_2 := 'select name_id from aida_old_arch order by name_id';

    open old_arch_ids for sql_stmt_2;

    sql_stmt_3 := 'select name_id from aida_new_arch order by name_id';

    open new_arch_ids for sql_stmt_3;

    first_read := 1;
    more_old_rows := 1;
    more_new_rows := 1;
    num_name_deletes := 0;
    status := 1;

    /* The loop will be exited when there are either no more entries in the
       aida_old_arch table that have not been processed or there are no
       more entries in the aida_new_arch table that have not been
       processed.
    ----------------------------------------------------------------------- */
    while (status = 1) loop

        /* If this the first time through the loop, read an aida_names id
           from both the aida_old_arch and aida_new_arch sorted
           tables.
        ----------------------------------------------------------------- */
        if (first_read = 1) then

            first_read := 0;

            fetch old_arch_ids into old_name_id_num;
            if (old_arch_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

            fetch new_arch_ids into new_name_id_num;
            if (new_arch_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        end if;

        /***DEBUG
        DBMS_OUTPUT.PUT_LINE('old name_id_num = ' || to_char(old_name_id_num));
        DBMS_OUTPUT.PUT_LINE('new name_id_num = ' || to_char(new_name_id_num));
        */

        /* The current aida_names id numbers from the sorted aida_old_arch
           and aida_new_arch tables are compared.  If they are equal, read
           an aida_names id from the next entries in the sorted
           aida_old_arch and aida_new_arch tables.
        ---------------------------------------------------------------------- */
        if (old_name_id_num = new_name_id_num) then

            fetch old_arch_ids into old_name_id_num;
            if (old_arch_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

            fetch new_arch_ids into new_name_id_num;
            if (new_arch_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        /* If the current aida_names id number from the sorted
           aida_old_arch table is less than the current aida_names id
           number from the aida_new_arch table, then there is
           not a match in the aida_new_arch table for the current
           aida_names id number from the aida_old_arch table.
           Therefore, this aida_names id number is not associated with
           a instance/attribute pair in the input file just processed
           and the channel archiver information in the Aida database for this
           id may be deleted.  After calling delete_arch_id to do
           this deletion, get the next current aida_old_arch
           aida_names id number from the sorted table.
        ------------------------------------------------------------------ */
        elsif (old_name_id_num < new_name_id_num) then

            delete_arch_id(old_name_id_num, service_num);

            num_name_deletes := num_name_deletes + 1;

            fetch old_arch_ids into old_name_id_num;
            if (old_arch_ids%NOTFOUND) then
                more_old_rows := 0;
                exit;
            end if;

        /* If the current aida_names id number from the sorted
           aida_old_arch table is greater than the current aida_names
           id number from the aida_new_arch table, get the next
           current aida_new_arch aida_names id number from the
           sorted table.
        ----------------------------------------------------------------- */
        elsif (old_name_id_num > new_name_id_num) then

            fetch new_arch_ids into new_name_id_num;
            if (new_arch_ids%NOTFOUND) then
                more_new_rows := 0;
                exit;
            end if;

        end if;
    end loop;

    /***DEBUG
    DBMS_OUTPUT.PUT_LINE('exited from delete check loop');
    */

    /* If there are any remaining entries from the sorted
       aida_old_arch table that have not been processed,
       loop through them and delete the channel archiver
       information in the Aida database associated with
       each by calling routine delete_arch_id.
    -------------------------------------------------------- */
    if (more_old_rows = 1) then

        status := 1;
        while (status = 1) loop

            delete_arch_id(old_name_id_num, service_num);

            num_name_deletes := num_name_deletes + 1;

            fetch old_arch_ids into old_name_id_num;
            if (old_arch_ids%NOTFOUND) then
                exit;
            end if;

        end loop;

    end if;

    dbms_output.put_line('Deleted ' || to_char(num_name_deletes) || ' rows from aida_names');

    execute immediate 'commit';

    exception
        when NO_DATA_FOUND then
            DBMS_OUTPUT.PUT_LINE('no data found error');
            UTL_FILE.FCLOSE(in_file);

            execute immediate 'rollback';

        when pop_aida_old_lcls_ca_error then

            DBMS_OUTPUT.PUT_LINE('pop_aida_old_lcls_ca_error');
            execute immediate 'rollback';

        when OTHERS then
            DBMS_OUTPUT.PUT_LINE('error occurred');

            err_num := SQLCODE;
            DBMS_OUTPUT.PUT_LINE('err_num = ' || to_char(err_num));

            err_msg := substr(SQLERRM, 1, 100);
            DBMS_OUTPUT.PUT_LINE('err_msg = ' || err_msg);

            UTL_FILE.FCLOSE(in_file);

            execute immediate 'rollback';
end;

begin

    start_total := dbms_utility.get_time;

    select user
        into user_name
        from dual;

    dbms_output.put_line('Log for populating database for Aida Channel Archiver server.  User name: ' || user_name);

    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;

    dbms_output.put_line('Begin processing at: ' || daytime);

    start_time := dbms_utility.get_time;
    dbms_output.put_line('Begin processing for lcls at: ' || daytime);

    set_aida_channel_archiver('lcls');

    time_diff := dbms_utility.get_time - start_time;
    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('Time for processing lcls: ' || diff_calc);
    dbms_output.put_line('-----------------------------------------');

    start_time := dbms_utility.get_time;
    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;
    dbms_output.put_line('Begin processing for facet at: ' || daytime);

    set_aida_channel_archiver('facet');

    time_diff := dbms_utility.get_time - start_time;
    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('Time for processing facet: ' || diff_calc);
    dbms_output.put_line('-----------------------------------------');

    start_time := dbms_utility.get_time;
    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;
    dbms_output.put_line('Begin processing for pepii at: ' || daytime);

    set_aida_channel_archiver('pepii');

    time_diff := dbms_utility.get_time - start_time;
    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('Time for processing pepii: ' || diff_calc);
    dbms_output.put_line('-----------------------------------------');

    start_time := dbms_utility.get_time;
    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;
    dbms_output.put_line('Begin processing for nlcta at: ' || daytime);

    set_aida_channel_archiver('nlcta');

    time_diff := dbms_utility.get_time - start_time;
    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('Time for processing nlcta: ' || diff_calc);
    dbms_output.put_line('-----------------------------------------');

    time_diff := dbms_utility.get_time - start_total;

    select to_char(sysdate, 'DD-MON-YYYY HH24:MI:SS')
        into daytime
        from dual;

    diff_calc := to_char(round((time_diff / 100), 2)) || ' seconds';
    dbms_output.put_line('End processing at : ' || daytime);
    dbms_output.put_line('Total processing time: ' || diff_calc);

    errcode := 0;
    dbms_output.put_line('set_aida_channel_archiver errcode: ' || to_char(errcode));
end;
/
exit;
