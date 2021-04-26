--==============================================================
--
--  Abs:   Updates names in the Aida directory database schema
--         for EPICS CA Record names. 
--
--  Name:  aida_caService_namesUpdate.sql                        
--
--  Rem:   This script merges rows of the table pvudb.pvu_rec,
--         which is the table of known EPICS Channel Access Process Variables, 
--         into the tables of the Aida directory
--         schema (primarily aida_names). This allows the Aida system
--         to access those named elements through the dpCa "Data
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
--  Usage: @aida_caService_namesUpdate.sql
--
--  Auth:  10-mar-2005, Greg White (greg).
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--         24-Feb-2010, Greg White (greg)
--             Remove Alter Table ststements, which can result in this script's
--             dependency on other DML processes on the same schema. 
--
--============================================================== 

/*
 * 0) Preliminaries: Definitions, variables etc.
 */
/* These define the mapping of the column values of a contributor's
   table to values of aida_names.instance and aida_names.attribute. That is,
   how do you get from the names known by a contributor to AIDA names.
   */

define inst_map = trim(curr_rec_name);
define attr_map = "'VAL'";

/* Define ca_contributor set as comma delimited set of values of
   aida_services.id which identify services which respond to names
   being managed by this script.  Eg, chads_contributor_set would be
   (3,10,11,12,13). There is only one service which responds to CA
   queries, which is 'EPICS'. For hint try select id.name from
   aida_services where name like 'CA%'; This list MUST BE EXHAUSTIVE,
   that is, it must list ALL the service_ids of services whose names
   are being contributed by this script.  */

define ca_contributor_set = (6);

/* define ca_contributor_set = (select id from aida_services where
name like 'CA');*/


/* The service id is the service to which NEW names will be attached.
   There should be one of these definitions for every element in the
   contributor set above. They're used only in the section 1.1
   where new names are added to the aida_directory table */

define ca_service_id = 6;
/* define ca_service_id = (select id from aida_services where name = 'CA');*/

/* As a precaution, set any existing 'N'ew rows to 'S'teady. This is
   important because, at the end of this procedure, we'll add each row
   marked 'N'ew to aida_directory as belonging to this service. So we
   must not have any left over from other contributors.  */

update aida_names a
	set state = 'S' where 
		a.state = 'N';

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
	select 'N', &inst_map, &attr_map from pvudb.pvu_rec
		minus select 'N', instance, attribute from 
			aida_names a, aida_directory d 
		where d.service_id in &ca_contributor_set and
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
	select id, &ca_service_id from aida_names a 
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
					&ca_contributor_set) and 
		(instance, attribute) in 
			(select instance, attribute from aida_names minus
			 select &inst_map, &attr_map from pvudb.pvu_rec);

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
				&ca_contributor_set) and
		exists ( select * from pvudb.pvu_rec s 
			where &inst_map = a.instance and
			      &attr_map = a.attribute);

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
		      aida_directory.service_id in &ca_contributor_set);

delete from aida_directory where name_id not in ( select id from aida_names);

-- alter table aida_directory enable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

update aida_names 
	set state = 'D' where state = 'M'; 



prompt Procedure Ends:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;
