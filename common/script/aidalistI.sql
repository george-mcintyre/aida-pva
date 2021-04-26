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
--   SQL> @aidalistI '<LIKE-expression>' 
--   SQL> @aidalistI 'BPMS:PR02:8%' 
--   > sqlplus -S AIDADEV/<passwd> @aidalistI.sql BPMS:PR02:8%
--
--  Auth:  02-Feb-2005, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
-- 10-May-2005 Greg White (greg)
-- Changed to "set head off" so clean output for scripts or matlab
--============================================================== 
set head off;
set feed off;
set pagesize 10000;
set wrap off;
set verify off;


select distinct INSTANCE
from AIDA_NAMES 
where INSTANCE like '&1';

-- End of script
