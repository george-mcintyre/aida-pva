--==============================================================
--
--  Abs:   Updates AIDA schema with names implied by a new 
--         irmisdb.secndx table. 
--
--  Name:  aidaPmuNames_update.sql                        
--
--  Rem:   This sql script is run following an update of "irmisdb.secndx",
--         which holds the names of all prim.micro.unit.secn names in 
--         in the SLC DB. From that list, this script updates the
--         aida_names table (and associated tbls) with the names now
--         acquireable by the SLC and SlcModel Data Providers.
-- 
--  Usage: no args. This script does the COMMIT, so the subordinate scripts
--         can be run to completion in unit testing. 
--
--  Auth:  31-Jan-2005 Greg White (greg): 
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--        25-Feb-2014 Judy Rock (jrock)
--        secndx was moved to irmisdb from nlcdev - updated comments
--        02-Feb-2005 Greg White (greg)
--        Added commit
--
--============================================================== 


whenever sqlerror exit sql.sqlcode;

@aida_slcService_namesUpdate.sql

@aida_slcModelService_namesUpdate.sql

commit;

exit;
