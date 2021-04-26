--==============================================================
--
--  Abs:   Updates names in the Aida directory database schema
--         for XAL Model system names. 
--
--  Name:  aida_xalModelService_namesUpdate.sql                        
--
--  Rem:   There is a pattern for these Aida directory update operations:
--
--	   0) Preliminaries. Create variable definitions.
--	   1) Add newly contributed names
--	   1.1) connect new contributed names to aida_directory 
--	   2) Mark names no longer contributed
--	   3) Unmark names now again contributed where last update 
--			had marked them for Delete.
--	   4) Remove those now not contributed twice in a row, and cleanup. 
-- 
--  Usage: @aida_xalService_namesUpdate.sql
--
--  Auth:  07-Occ-2008, Greg White (greg).
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--         24-Feb-2010, Greg White (greg)
--             Remove Alter Table ststements, which can result in this script's
--             dependency on other DML processes on the same schema. 
--         03-Nov-2008, Greg White (greg)
--            Changed step 1 insert, so that identically named instances 
--            that shared the same attribute (eg 'twiss') from ANY 
--            contributor, would be subtracted. Ie, removed "and same contributor" 
--            clause. This will permit contributor scripts to run and not 
--            add duplicate model names for lexically identical instances, for
--            more than one contributor (ie both SLC model and XAL). That is,
--            if a device is modelled by SLC in the Directory Service, and the 
--            XAL contributor also has model for it, the XAL contributor won't
--            add an instance for itself or override the existing one, it'll just 
--            leave the device as being modelled by SLC.  
--
--            Also removing trailing 'quit'
--
--============================================================== 

/*
 * 0) Preliminaries: Definitions, variables etc.
 */
set pagesize 0

/* These define the mapping of the column values of a contributor's table to
   values of aida_names.instance and aida_names.attribute. That is, 
   how do you get from the names known by a contributor to AIDA names.
*/
define inst_map = instance;


/* Define contributor_set as comma delimited set of values of aida_services.id
   which identify services which respond to names being managed by this script.
   Eg, chads_contributor_set would be (3,10,11,12,13). There is only one
   service which responds to XAL Model database queries, which is 'Model'. For hint
   try select id, name from aida_services where name like 'Model%';
   This list MUST BE EXHAUSTIVE, that is, it must list ALL the service_ids of
   services whose names are being contributed by this script.
*/
define xalmodel_contributor_set = (202);

/* The service id is the service to which NEW names will be attached.
   There should be one of these definitions for every element in the
   contributor set above. Thye're used only in the last section 5)
   where new names are added to the aida_directory table 
*/ 
define xalmodel_service_id = 202; 

/* As a precaution, set any existing 'N'ew rows to 'S'teady. This is
   important because, at the end of this procedure, we'll add each row
   marked 'N'ew to aida_directory as belonging to this service. So we
   must not have any left over from other contributors.  
*/
update aida_names a
	set state = 'S' where 
		a.state = 'N';

/*
 * 1) Add newly contributed names.
 */
prompt Adding Newly Contributed Names
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;
/*
 * Using the view of EPICS PV device names of all modelled devices,
 * add each one as an instance, with attribute 'twiss', unless such
 * a row already existed. Then add each one as an instance, with attribute 
 * R, unless such a row already existed.
 */

/*
   Insert all names we have now (which will create duplicates for most, if not all), 
   then delete the ones aida_directory already knew about from previous runs, 
   leaving only the really new ones we haven't seen before.
 */
insert into aida_names( state, instance, attribute)
	select 'N', device_name, 'twiss' from machine_model.v_modelled_device_names
		minus select 'N', a.instance, 'twiss' from 
			      aida_names a, aida_directory d 
			where d.name_id = a.id and
                              a.attribute = 'twiss';
-- "and d.service_id in &xalmodel_contributor_set" removed 3/Nov/08

insert into aida_names( state, instance, attribute)
	select 'N', device_name, 'R' from machine_model.v_modelled_device_names
		minus select 'N', a.instance, 'R' from 
			      aida_names a, aida_directory d 
			where d.name_id = a.id and
                              a.attribute = 'R';
-- "and d.service_id in &xalmodel_contributor_set" removed 3/Nov/08.

/*
 * Update the directory table aida_directory for new names in aida_names, 
 * to connect the new name to the service that manages that name's values.
 */
prompt Updating Directory:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;

/*
   For each aida_names.state = 'N'ew row in aida_names, create a new
   row in aida_directory to
   associate the new name (by its aida_names.id) with the service_id
   of the service which is responsible for it.
   Cleanup by setting all 'N' to 'S'. Note, when we exit, there must not be
   any N rows.  
*/

insert into aida_directory( name_id, service_id) 
	select id, &xalmodel_service_id from aida_names a 
		where a.state = 'N';

prompt New names added to the directory:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;
select state, instance, attribute from aida_names where state = 'N';

update aida_names a
	set state = 'S' where 
		a.state = 'N';

/* 
 * 2) Mark names no longer contributed.
 */
prompt Marking Names No Longer Contributed:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;

/* This sets the "state" to 'M'arked for Delete, of each row of
   aida_names whose state is presently 'S'teady, but for which there is
   now no modelled epics device name. This sets the rows for all the 
   attributes of all those instances.
*/

update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        aida_directory.service_id in 
					&xalmodel_contributor_set) and
		not exists ( select * from machine_model.v_modelled_device_names where 
	                     a.instance = device_name );

/* Log those being marked for Delete */
select instance, attribute, state from aida_names where state = 'M';

/* 
 * 3) Un-mark the names (to 'S') which are now again contributed, where, 
 *    before this run they were marked for Delete.
 */
prompt Un-mark the Names Now Contributed Again:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;
/*
   This sets the "state" to 'S'teady for each element of aida_names whose state
   is presently 'D'elete, which is (now again) present in the contributor's
   table (that is, has same contributor, and does appear in the contributor).
*/
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names, aida_directory 
			where aida_names.id = aida_directory.name_id and
			      aida_directory.service_id in 
				&xalmodel_contributor_set) and
                exists ( select * from machine_model.v_modelled_device_names where 
	                     a.instance = device_name );

/* 
 * 4) Remove those now not contributed twice in a row:
 */
prompt Removing Old Names:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;
/*
   i) Remove 'D'eleted from aida_names. There's a foregin key constraint on 
      aida_directory which checks for matching rows in aida_names; so disable it.
   ii) Remove all from aida_directory which now don't have 
       an entry in aida_names.
   iii) Set those 'M'arked for delete to 'D'elete.
*/
-- alter table aida_directory disable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

delete from aida_names where
	state = 'D' and 
	id in ( select id from aida_names, aida_directory 
		where aida_names.id = aida_directory.name_id and
		      aida_directory.service_id in &xalmodel_contributor_set);

delete from aida_directory where name_id in ( 
	select name_id from aida_directory minus select id from aida_names); 

-- alter table aida_directory enable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

update aida_names 
	set state = 'D' where state = 'M'; 

/*
 * 5) Exit procedure. Should do integrity checks.
 */

prompt Procedure Ends:
select to_char(sysdate,'MMDDYYYY HH:MI:SS') from dual;
