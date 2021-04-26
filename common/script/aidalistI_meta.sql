--==============================================================
--
--  Abs:   Gets all the matching AIDA INSTANCEs
--         matching a SQLplus patterns for the INSTANCE.
--
--  Name:  aidalistI.sql                        
--
--  Rem:   
--
--  Usage: 
--   SQL> @aidalistI_meta '<LIKE-expression>' 
--   SQL> @aidalistI_meta 'BPMS:PR02:8%' 
--   > sqlplus -S AIDADEV/<passwd> @aidalistI_meta BPMS:PR02:8%
--
--  Auth:  28-Oct-2009, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--============================================================== 
set head off;
set feed off;
set pagesize 10000;
set linesize 1024;
-- wrap off makes sure each each returned row is a single printed record.
set wrap off;
set verify off;

column instance format A20
column attribute format A20
column name format A10
-- column transform format A200

select distinct n.instance, n.attribute, s.name, n.transform from aida_names n, aida_directory d, aida_services s where n.instance like '&1' and n.id = d.name_id and d.service_id = s.id;

-- End of script
