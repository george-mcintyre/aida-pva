--==============================================================
--
--  Abs:   Gets all the matching AIDA names (INSTANCE/ATTRIBUTE pairs)
--         matching an SQLplus patterns for the INSTANCE and ATTRIBUTE.
--
--  Name:  aidalistIA.sql                        
--
--  Rem:   
--
--  Usage: 
--   SQL> @aidalistIA '<LIKE-expression>' '<LIKE-expression>'  
--   SQL> @aidalistIA 'BPMS:PR02:8%' 'TWISS' 
--                                       - shows all IA for PR02 BPMS 
--                                         with 8* unit number and 
--                                         attribute TWISS. 
--   > sqlplus -S AIDADEV/<passwd> @aidalistIA.sql BPMS:PR02:8% TWISS
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
set wrap off;
set verify off;

select INSTANCE, ATTRIBUTE 
from AIDA_NAMES 
where INSTANCE like '&1' and ATTRIBUTE like '&2';

-- End of script
