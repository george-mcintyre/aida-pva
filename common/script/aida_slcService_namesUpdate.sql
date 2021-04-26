--==============================================================
--
--  Abs:   Updates names in the Aida directory database schema
--         for SLC Database device names. 
--
--  Name:  aida_slcService_namesUpdate.sql                        
--
--  Rem:   This script merges rows of the table irmisdb.secndx,
--         which is the table of known prim.micro.unit.secn "device"
--         is the SLC Database, into the tables of the Aida directory
--         schema (primarily aida_names). This allows the Aida system
--         to access those named elements through the SLC DB "Data
--         Provider".
--
--         There is a pattern for these Aida directory update operations:
--
--	   0) Preliminaries. Create variable definitions.
--	   1) Add newly contributed names
--	   1.1) connect new contributed names to aida_directory 
--	   2) Mark names no longer contributed
--	   3) Unmark names now again contributed where last update 
--			had marked them for Delete.
--	   4) Remove those now not contributed twice in a row, and cleanup. 
-- 
--  Usage: @aida_slcService_namesUpdate.sql
--
--  Auth:  27-jan-2005, Greg White (greg).
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--         25-Feb-2014, Judy Rock (jrock)
--             secndx lives in irmisdb now - changed nlcdev to irmisdb
--         24-Feb-2010, Greg White (greg)
--             Remove Alter Table ststements, which can result in this script's
--             dependency on other DML processes on the same schema. 
--         04-Dec-2007, Bob Hall (rdh)
--             Modified by also deleting the KLYS/SBST primary KPHR 
--             instance/attribute rows from the aida_secndx table.
--             Rows for these signals will now be added instead during
--             aida_names/aida_directory processing for the Aida
--             Klystron server. 
--         03-Oct-2007, Bob Hall (rdh)
--             Modified by also deleting the KLYS/SBST primary PCON
--             and ACON instance/attribute rows from the aida_secndx
--             table.  Rows for these signals will now be added instead
--             during aida_names/aida_directory processing for the Aida
--             Klystron server. 
--         23-Aug-2007, Bob Hall (rdh)
--             Modified by deleting the KLYS/SBST primary PDES
--             and PHAS instance/attribute rows from the aida_secndx
--             table.  Rows for these signals will now be added instead
--             during aida_names/aida_directory processing for the Aida
--             Klystron server. 
--         15-Aug-2007, Bob Hall (rdh)
--             Fixed the problem of marking ASTS channel signals
--             for deletion by failing to account for records in
--             table aida_slc_db_asts.
--         20-Apr-2006, Bob Hall (rdh)
--             Modified by adding the insertion of aida_names records
--             from the table aida_slc_db_asts, which contains ASTS
--             primary/micro/channel name and pseudo-secondary
--             instance/attribute pairs.
--	   28-jan-2005, Greg White (greg)
--             Changed ltrim(unit) to rtrim(unit) in inst_map variable.
--             Replaced update stmt in 2) with insert to temp table, followed
--             by update.
--
--============================================================== 

/*
 * 0) Preliminaries: Definitions, variables etc.
 */
/* These define the mapping of the column values of a contributor's
   table to values of aida_names.instance and aida_names.attribute. That is,
   how do you get from the names known by a contributor to AIDA names.
   */

define inst_map = rtrim(rtrim(prim)||':'||rtrim(micro)||':'||ltrim(unit));
define attr_map = rtrim(secn);

/* Define slc_contributor set as comma delimited set of values of
   aida_services.id which identify services which respond to names
   being managed by this script.  Eg, chads_contributor_set would be
   (3,10,11,12,13). There is only one service which responds to SLC
   Database queries, which is 'SLC'. For hint try select id,name from
   aida_services where name like 'SLC%'; This list MUST BE EXHAUSTIVE,
   that is, it must list ALL the service_ids of services whose names
   are being contributed by this script.  */

define slc_contributor_set = (8);

/* define slc_contributor_set = (select id from aida_services where
name like 'SLC');*/


/* The service id is the service to which NEW names will be attached.
   There should be one of these definitions for every element in the
   contributor set above. Thye're used only in the section 1.1
   where new names are added to the aida_directory table */

define slc_service_id = 8;
/* define slc_service_id = (select id from aida_services where name = 'SLC');*/

/* As a precaution, set any existing 'N'ew rows to 'S'teady. This is
   important because, at the end of this procedure, we'll add each row
   marked 'N'ew to aida_directory as belonging to this service. So we
   must not have any left over from other contributors.  */

update aida_names a
	set state = 'S' where 
		a.state = 'N';

/* Create an indexed version of irmisdb.secndx, named aida_secndx, to
   make updating faster. The 2 fields of aida-secndx are named
   instance and attribute, just as their equivalents in aida_names
   are. Their values are populated using the defined maps above, so
   for instance instances are of the form prim:micr:unit, so we don't
   have to use the maps in statements after this one. We use a
   predefined table description, but truncate it and reporoduce it
   with each execution.  
*/
prompt Creating the indexed version of irmisdb secndx, named aida_secndx
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;
/* 
truncate table aida_secndx;
drop index inst_idx;
drop index attr_idx;
*/
delete from aida_secndx;

insert into aida_secndx
	select &inst_map, &attr_map from irmisdb.secndx;

/* Delete KLYS/SBST primary PDES, KPHR, PHAS, PCON, and ACON rows from
   aida_secndx since these will be processed by the Aida Klystron
   server, not the Aida SLC Database server.
*/
delete from aida_secndx where instance like 'KLYS:%' and attribute = 'PDES';
delete from aida_secndx where instance like 'KLYS:%' and attribute = 'KPHR';
delete from aida_secndx where instance like 'KLYS:%' and attribute = 'PHAS';
delete from aida_secndx where instance like 'KLYS:%' and attribute = 'PCON';
delete from aida_secndx where instance like 'KLYS:%' and attribute = 'ACON';

delete from aida_secndx where instance like 'SBST:%' and attribute = 'PDES';
delete from aida_secndx where instance like 'SBST:%' and attribute = 'KPHR';
delete from aida_secndx where instance like 'SBST:%' and attribute = 'PHAS';
delete from aida_secndx where instance like 'SBST:%' and attribute = 'PCON';
delete from aida_secndx where instance like 'SBST:%' and attribute = 'ACON';

/*
create index inst_idx on aida_secndx ( instance );
create index attr_idx on aida_secndx ( attribute );
*/

/*
 * 1) Add newly contributed names.
 */
prompt Adding Newly Contributed Names
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/* First "in" is so that same contributor can contribute to more than
   one service, eg, same contributor contributes to all Chads services
   Can take 1.5hr!
*/
insert into aida_names( state, instance, attribute)
	select 'N', instance, attribute from aida_secndx
		minus select 'N', instance, attribute from 
			aida_names a, aida_directory d 
		where d.service_id in &slc_contributor_set and
                      d.name_id = a.id;

/*
 * Add aida_name records from the table aida_slc_db_asts, which contains
 * ASTS primary/micro/channel name and pseudo-secondary instance/attribute
 * pairs.
 */
insert into aida_names (state, instance, attribute)
    select 'N', instance, attribute from aida_slc_db_asts
        minus select 'N', instance, attribute from
            aida_names a, aida_directory d
        where d.service_id in &slc_contributor_set and
            d.name_id = a.id;

/*
 * Update the directory table aida_directory for new names in aida_names, 
 * to connect the new name to the service that manages that name's values.
 */
prompt Updating Directory:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/*
   For each aida_names.state = 'N'ew row in aida_names, create a new row 
   in aida_directory to associate the new name (by its aida_names.id) with 
   the service_id of the service
   which is responsible for it.
   Cleanup by setting all 'N' to 'S'. Note, when we exit, there must not be
   any N rows.
*/

insert into aida_directory( name_id, service_id) 
	select id, &slc_service_id from aida_names a 
		where a.state = 'N';
update aida_names a
	set state = 'S' where 
		a.state = 'N';

/* 
 * 2) Mark names no longer contributed.
 */
prompt Marking Names No Longer Contributed:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/* This sets the "state" to 'M'arked for Delete, of each element of
   aida_names whose state is presently 'S'teady, but which is not now
   present in the contributor's table (that is, has same contributor,
   and does not appear in the contributor). */

delete from aida_names_temp;
insert into aida_names_temp
	select * from aida_names where 	
		state = 'S' and 
		id in 
			(select id from aida_names, aida_directory where 
				id = aida_directory.name_id and 
				aida_directory.service_id in 
					&slc_contributor_set) and 
		(instance, attribute) in 
			(select instance, attribute from aida_names minus
		        ((select instance, attribute from aida_secndx) union
                         (select instance, attribute from aida_slc_db_asts)));

update aida_names
	set state = 'M' where id in (select id from aida_names_temp);


/* 
 * 3) Re-mark the names 'New' which are now again contributed, where, 
 *    before this run they were marked for Delete.
 */
prompt Un-mark the Names Now Contributed Again:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/*
   This sets the "state" to 'S'teady for each element of aida_names whose state
   is presently 'D'elete, which is present in the contributor's
   table (that is, has same contributor, and does appear in the contributor).
   NOTE: It's not set to 'N' since all existing Ns are later added to 
   aida_directory, and these names are already in aida_directory.
*/
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names, aida_directory 
			where aida_names.id = aida_directory.name_id and
			      aida_directory.service_id in 
				&slc_contributor_set) and
		exists ( select * from aida_secndx s 
			where s.instance = a.instance and
			      s.attribute = a.attribute);


/* 
 * 4) Remove those now not contributed twice in a row:
 */
prompt Removing Old Names:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/*
   i) Remove 'D'eleted from aida_names. There's a foregin key constraint on 
      aida_directory which checks for matching rows in aida_names; so disable it.
   ii) Remove all from aida_directory which now don't have an entry 
       in aida_names.
   iii) Set those 'M'arked for delete to 'D'elete.
*/
-- alter table aida_directory disable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

delete from aida_names where
	state = 'D' and 
	id in ( select id from aida_names, aida_directory 
		where aida_names.id = aida_directory.name_id and
		      aida_directory.service_id in &slc_contributor_set);

delete from aida_directory where name_id not in ( select id from aida_names);

-- alter table aida_directory enable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

update aida_names 
	set state = 'D' where state = 'M'; 



prompt Procedure Ends:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

