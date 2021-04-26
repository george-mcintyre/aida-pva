--==============================================================
--
--  Abs:   Gets AIDA metadata for all the matching AIDA names (INSTANCE/ATTRIBUTE pairs)
--         matching an SQLplus patterns for the INSTANCE and ATTRIBUTE.
--
--  Name:  aidalistIA_meta.sql                        
--
--  Rem:   
--
--  Usage: 
--   SQL> @aidalistIA_meta '<LIKE-expression>' '<LIKE-expression>'  
--   SQL> @aidalistIA_meta 'BPMS:PR02:8%' 'TWISS' 
--                                       - shows all IA for PR02 BPMS 
--                                         with 8* unit number and 
--                                         attribute TWISS. 
--   > sqlplus -S AIDADEV/<passwd> @aidalistIA_meta BPMS:PR02:8% TWISS
--
--  Auth:  14-Nov-2004, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
-- 16-May-2005, Greg White (greg)
-- Remove returning TRANSFORM, - causing "rows will be truncated" msg.
-- 10-May-2005, Greg White (greg)
-- Set "set head off" so clean output for scripts or matlab
-- 29-Nov-2004, Greg White (greg)
-- Remove exit, to stop exiting when executed inside a sql session.
--
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

select n.instance, n.attribute, s.name, n.transform from 
aida_names n, aida_directory d, aida_services s where 
n.instance like '&1' and n.attribute like '&2' and n.id = d.name_id and d.service_id = s.id;

-- End of script
