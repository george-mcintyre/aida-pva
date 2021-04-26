--  NOTE: THIS SCRIPT NO LONGER DOES THE ALTER TABLE, SO IT NO LONGER DOES A COMMIT!!!
--  as of 11-Jan-2009.    
--
--  Abs:   Updates names in the Aida directory database schema
--         for names contributed by a the Symbols subschema. It should be run
--         after a new Symbols upload is done. 
--
--  Name:  aida_Symbols_namesUpdate.sql                        
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
--  Usage: @aida_Symbols_namesUpdate.sql
--
--  Auth:  21-Oct-2008, Greg White (greg).
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--       12-Jan-2009, Greg White (greg), Elie Grunhaus (elie)
--       Add Z and S from elements table. As part of this, in stage 3, 
--       replace the use of "exists" clause with use fromof IN, to avoid 
--       oracle disconnect (probably caused by excessive cache depletion).
--       See emails re "SLAC #147808" in subject.
--============================================================== 

/*
 * 0) Preliminaries: Definitions, variables etc.
 */
/* These define the mapping of the column values of a contributor's table to
   values of aida_names.instance and aida_names.attribute. That is, 
   how do you get from the names known by a contributor to AIDA names.
*/
define inst_map = instance;


/* Define contributor_set as comma delimited set of values of aida_services.id
   which identify services which respond to names being managed by this script.
   It is the set of services affected by the names contributed by this this contributor.
   Eg, chads_contributor_set would be (3,10,11,12,13). There is only one
   service which responds to changes to Symbols, it is 'Oracle'. 
*/
define symbols_contributor_set = (5);

/* The service id is the service to which NEW names will be attached.
   There should be one of these definitions for every element in the
   contributor set above. They're used only in the last section 5)
   where new names are added to the aida_directory table 
*/ 
define rdb_service_id = 5; 

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
set escape on

-- Add effective_length
insert into aida_names( state, instance, attribute, transform)
	select 'N', epics_channel_access_name, 
               'element.effective_length', '/select effective_length from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
        from lcls_infrastructure.v_lcls_elements_report where 
	     epics_channel_access_name is not null
		minus select 'N', 
                             a.instance, 
                             'element.effective_length', 
                             '/select effective_length from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
                      from aida_names a, aida_directory d 
		      where d.service_id in &symbols_contributor_set and
                      	      d.name_id = a.id and
			    a.attribute = 'element.effective_length';

-- Add Z
insert into aida_names( state, instance, attribute, transform)
	select 'N', epics_channel_access_name, 
               'element.Z', '/select linacz_m from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
        from lcls_infrastructure.v_lcls_elements_report where 
	     epics_channel_access_name is not null
		minus select 'N', 
                             a.instance, 
                             'element.Z', 
                             '/select linacz_m from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
                      from aida_names a, aida_directory d 
		      where d.service_id in &symbols_contributor_set and
                      	      d.name_id = a.id and
			    a.attribute = 'element.Z';

-- Add S for display
insert into aida_names( state, instance, attribute, transform)
	select 'N', epics_channel_access_name, 
               'element.S', '/select s_display from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
        from lcls_infrastructure.v_lcls_elements_report where 
	     epics_channel_access_name is not null
		minus select 'N', 
                             a.instance, 
                             'element.S', 
                             '/select s_display from lcls_infrastructure.v_lcls_elements_report where epics_channel_access_name = ''\&instance''' 
                      from aida_names a, aida_directory d 
		      where d.service_id in &symbols_contributor_set and
                      	      d.name_id = a.id and
			    a.attribute = 'element.S';

set escape off

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
	select id, &rdb_service_id from aida_names a 
		where a.state = 'N'; 
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
   now no modelled epics device name being contributed by this contributor for
   which the instance is the epics device name, and attribute is an attribute 
   for which the acquisition is handled by the rdb service. This sets the rows for all the 
   attributes of all those instances.
   This is quite clumsy: a better way woud have been to put a new row in aida_directory, that
   specifies the contributor of the aida_name row, along with, as it does now, the 
   data provider of the aida_name row.
*/

-- effective-length
update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        s.attribute = 'element.effective_length' and
			        aida_directory.service_id in 
					&symbols_contributor_set) and
		not exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );

-- Z names
update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        s.attribute = 'element.Z' and
			        aida_directory.service_id in 
					&symbols_contributor_set) and
		not exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );

-- S names
update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        s.attribute = 'element.S' and
			        aida_directory.service_id in 
					&symbols_contributor_set) and
		not exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );


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
-- effective_length
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names s, aida_directory d
			where s.id = d.name_id and
			      s.attribute = 'element.effective_length' and
			      d.service_id in 
				&symbols_contributor_set) and
                a.instance in (select v.epics_channel_access_name
                        from lcls_infrastructure.v_lcls_elements_report v );
/*
				&symbols_contributor_set) and
exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );
*/

-- Z
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names s, aida_directory d
			where s.id = d.name_id and
			      s.attribute = 'element.Z' and
			      d.service_id in 
				&symbols_contributor_set) and
                a.instance in (select v.epics_channel_access_name
                        from lcls_infrastructure.v_lcls_elements_report v );
/*
				&symbols_contributor_set) and
exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );
*/

-- S
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names s, aida_directory d
			where s.id = d.name_id and
			      s.attribute = 'element.S' and
			      d.service_id in 
				&symbols_contributor_set) and
                a.instance in (select v.epics_channel_access_name
                        from lcls_infrastructure.v_lcls_elements_report v );
/*
				&symbols_contributor_set) and
exists ( select * from lcls_infrastructure.v_lcls_elements_report where 
	                     a.instance = epics_channel_access_name );
*/


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
		      aida_directory.service_id in &symbols_contributor_set);

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
