--==============================================================
--
--  Abs:   Updates names in the Aida directory database schema
--         for SLC Model system names. 
--
--  Name:  aida_slcModelService_namesUpdate.sql                        
--
--  Rem:   This script examines rows of the table aida_names
--         which is the table of known Aida names, looking for names
--         of SLC Database devices that have the 'TWSP' attribute. If 
--         any new ones are found, it creates new 'twiss' and 'R' attrbute
--         rows for them, assigned to the SLCModel data provicer. 
--         This allows the Aida system
--         to access those named elements through the SLC Model "Data
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
--         20-Jun-2011, Greg White (greg)
--             Add model for model types for which SLC has to go to file. Namely:
-- 	        Basic   String type
--	        QUAD x       QUAS, 
--     		SOLN x        SOLS, 
--		SEXT,        SXTS, 
--		BEND x        BNDS, 
--		SEPT,        SPTS, 
--		KICK
--             Those marked x were added previously. 
--             NOTE: that is mod meant changing the condition under which enties are added
--             from adding a twiss and R if there existed a 'BDES' secondary, to if there
--             exists a KMOD secondary. Those mags on a magnet string (RHS) have no BDES,
--             but they do have KMOD.
-- 
--         24-Feb-2010, Greg White (greg)
--             Remove Alter Table ststements, which can result in this script's
--             dependency on other DML processes on the same schema. 
--         03-Nov-2008, Greg White (greg)
--            Changed step 1 insert, so that identically named instances 
--            that shared the same attribute (eg 'twiss') from ANY 
--            contributor, would be subtracted. Ie, removed same contributor 
--            clause. This will permit contributor scripts to run and not 
--            add duplicate model names for lexically identical instances, for
--            more than one contributor (ie both SLC model and XAL).  
--         10-Apr-2007, Bob Hall (rdh)
--            Corrected to only insert rows in the aida_names
--            table for SLC database primary names QUAD, BEND,
--            or SOLN for SLC database names (e.g., those with
--            the attribute BDES).
--         28-Mar-2007, Bob Hall (rdh)
--            Added rows in the aida_names table with instances
--            that have SLC database primary names QUAD, BEND,
--            or SOLN (the attribute names are 'twiss' and 'R').
--            These new rows are used in obtaining TWISS and R
--            Matrix values from files in the Aida Model data
--            server.
--
--
--============================================================== 

/*
 * 0) Preliminaries: Definitions, variables etc.
 */
/* These define the mapping of the column values of a contributor's table to
   values of aida_names.instance and aida_names.attribute. That is, 
   how do you get from the names known by a contributor to AIDA names.
*/

define inst_map = instance;
-- define attr_map = 'twiss';   Not used since 2 atrributes 'twiss' and 'R'
--                              are added for each 'TWSP' attribute.

/* Define contributor_set as comma delimited set of values of aida_services.id
   which identify services which respond to names being managed by this script.
   Eg, chads_contributor_set would be (3,10,11,12,13). There is only one
   service which responds to SLC Database queries, which is 'SLC'. For hint
   try select id,name from aida_services where name like 'SLC%';
   This list MUST BE EXHAUSTIVE, that is, it must list ALL the service_ids of
   services whose names are being contributed by this script.
*/
define slcmodel_contributor_set = (63);

/* define slcmodel_contributor_set = (select id from aida_services 
   	where name like 'SLCModel%');*/

/* The service id is the service to which NEW names will be attached.
   There should be one of these definitions for every element in the
   contributor set above. Thye're used only in the last section 5)
   where new names are added to the aida_directory table 
*/ 
define slcmodel_service_id = 63; 
/* define slcmodel_service_id = (select
   id from aida_services where name = 'SLCModel');*/

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
/*
   Add an attribute= 'twiss' and 'R' row for every row whose attribute
   is 'TWSP', unless such a row already existed.  Also add an attribute=
   'twiss' and 'R' row for every instance that has the SLC database
   primary 'QUAD', 'BEND', or 'SOLN' and a SLC database secondary, such
   as BDES (these are needed for retrieving TWISS and R matrix data for
   these signals from a file).
     The "in" is so that the same contributor can contribute to more than
   one service, eg, same name contributor contributes to all Chads services 
*/

insert into aida_names( state, instance, attribute)
	select 'N', &inst_map, 'twiss' from aida_names s
		where s.attribute = 'TWSP' or
                      (s.attribute = 'KMOD' and
		       (s.instance like 'QUAD:%' or
                        s.instance like 'QUAS:%' or
                        s.instance like 'SOLN:%' or
                        s.instance like 'SOLS:%' or
                        s.instance like 'SEXT:%' or
                        s.instance like 'SXTS:%' or
                        s.instance like 'BEND:%' or
                        s.instance like 'BNDS:%' or
                        s.instance like 'SEPT:%' or
                        s.instance like 'SPTS:%' or
                        s.instance like 'KICK:%' ))
		minus select 'N', a.instance, 'twiss' from 
			      aida_names a, aida_directory d 
			where d.name_id = a.id and
                              a.attribute = 'twiss';
-- "and d.service_id in &slcmodel_contributor_set removed 03/Nov/08.


insert into aida_names( state, instance, attribute)
	select 'N', &inst_map, 'R' from aida_names s
		where s.attribute = 'TWSP' or
                      (s.attribute = 'KMOD' and
		       (s.instance like 'QUAD:%' or
                        s.instance like 'QUAS:%' or
                        s.instance like 'SOLN:%' or
                        s.instance like 'SOLS:%' or
                        s.instance like 'SEXT:%' or
                        s.instance like 'SXTS:%' or
                        s.instance like 'BEND:%' or
                        s.instance like 'BNDS:%' or
                        s.instance like 'SEPT:%' or
                        s.instance like 'SPTS:%' or
                        s.instance like 'KICK:%' ))
		minus select 'N', a.instance, 'R' from 
			      aida_names a, aida_directory d 
			where d.name_id = a.id and
                              a.attribute='R';
-- "and d.service_id in &slcmodel_contributor_set" removed 3/Nov/08.

/*
 * Update the directory table aida_directory for new names in aida_names, 
 * to connect the new name to the service that manages that name's values.
 */
prompt Updating Directory:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;

/*
   For each aida_names.state = 'N'ew row in aida_names, create a new
   row in aida_directory to
   associate the new name (by its aida_names.id) with the service_id
   of the service which is responsible for it.
   Cleanup by setting all 'N' to 'S'. Note, when we exit, there must not be
   any N rows.  */

insert into aida_directory( name_id, service_id) 
	select id, &slcmodel_service_id from aida_names a 
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
   aida_names whose state is presently 'S'teady, but but for which there is
   an existing row with the same instance, attribute='TWSP' which has
   been 'M'arked for delete. 

   IF THE NOT-EXISTS HERE PROVES VERY INEFFICIENT, SEE ~GREG/DEV/DOC/V.SQL
*/

update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        aida_directory.service_id in 
					&slcmodel_contributor_set) and
		not exists ( select * from aida_names s where 
				s.instance = a.instance and
                                s.attribute = 'TWSP' or
                                (s.attribute = 'KMOD' and
		                 ( s.instance like 'QUAD:%' or
                        	   s.instance like 'QUAS:%' or
                        	   s.instance like 'SOLN:%' or
                        	   s.instance like 'SOLS:%' or
                                   s.instance like 'SEXT:%' or
                                   s.instance like 'SXTS:%' or
                                   s.instance like 'BEND:%' or
                                   s.instance like 'BNDS:%' or
                                   s.instance like 'SEPT:%' or
                                   s.instance like 'SPTS:%' or
                                   s.instance like 'KICK:%' ))
                                );

/* Same as for 'twiss' but for 'R' attribute. */
update aida_names a
	set state = 'M' where
		a.state = 'S' and
		a.id in ( select id from aida_names s, aida_directory 
			  where s.id = aida_directory.name_id and
			        aida_directory.service_id in 
					&slcmodel_contributor_set) and
		not exists ( select * from aida_names s where 
				s.instance = a.instance and
                                s.attribute = 'TWSP' or
                                (s.attribute = 'KMOD' and
		                 ( s.instance like 'QUAD:%' or
                        	   s.instance like 'QUAS:%' or
                        	   s.instance like 'SOLN:%' or
                        	   s.instance like 'SOLS:%' or
                                   s.instance like 'SEXT:%' or
                                   s.instance like 'SXTS:%' or
                                   s.instance like 'BEND:%' or
                                   s.instance like 'BNDS:%' or
                                   s.instance like 'SEPT:%' or
                                   s.instance like 'SPTS:%' or
                                   s.instance like 'KICK:%' ))
                                );

/* 
 * 3) Un-mark the names (to 'S') which are now again contributed, where, 
 *    before this run they were marked for Delete.
 */
prompt Un-mark the Names Now Contributed Again:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;
/*
   This sets the "state" to 'S'teady for each element of alpha whose state
   is presently 'D'elete, which is (now again) present in the contributor's
   table (that is, has same contributor, and does appear in the contributor).
*/
update aida_names a
	set state = 'S' where
		state = 'D' and
		id in ( select id from aida_names, aida_directory 
			where aida_names.id = aida_directory.name_id and
			      aida_directory.service_id in 
				&slcmodel_contributor_set) and
		exists ( select * from aida_names s 
			where &inst_map = a.instance and
			      s.attribute = 'TWSP' or
                              (s.attribute = 'KMOD' and
		               ( s.instance like 'QUAD:%' or
                        	   s.instance like 'QUAS:%' or
                        	   s.instance like 'SOLN:%' or
                        	   s.instance like 'SOLS:%' or
                                   s.instance like 'SEXT:%' or
                                   s.instance like 'SXTS:%' or
                                   s.instance like 'BEND:%' or
                                   s.instance like 'BNDS:%' or
                                   s.instance like 'SEPT:%' or
                                   s.instance like 'SPTS:%' or
                                   s.instance like 'KICK:%' ))
                              );

/* 
 * 4) Remove those now not contributed twice in a row:
 */
prompt Removing Old Names:
select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;
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
		      aida_directory.service_id in &slcmodel_contributor_set);

delete from aida_directory where name_id in ( 
	select name_id from aida_directory minus select id from aida_names); 

-- alter table aida_directory enable constraint FK_AIDA_DIRECTORY_AIDA_NAMES; 

update aida_names 
	set state = 'D' where state = 'M'; 

/*
 * 5) Exit procedure. Should do integrity checks.
 */

prompt Procedure Ends:


select to_char(sysdate,'MMDDYYYY HH24:MI:SS') from dual;
