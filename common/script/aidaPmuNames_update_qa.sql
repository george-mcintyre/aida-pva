--==============================================================
--
--  Abs:   Updates AIDA schema with names implied by a new 
--         irmisdb.secndx table specifically for our QA instance.
--
--  Name:  aidaPmuNames_update_qa.sql                        
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
-- **This script does not sync MCCQA to MCCO. Rather that is left to run_pmu_load.sh,
-- once all SLC change, inlcuding hist, klystron etc, have been applied to mccqa.**
--
--  Auth:  11-Jan-2010 Greg White (greg): 
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--        25-Feb-2014 Judy Rock (jrock)
--        secndx was moved to irmisdb from nlcdev - updated comments
--
--
--============================================================== 


whenever sqlerror exit sql.sqlcode;

-- note _qa qualifier
@aida_slcService_namesUpdate_qa.sql

@aida_slcModelService_namesUpdate.sql

commit;

exit;
