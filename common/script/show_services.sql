--==============================================================
--
--  Abs:   Show all the AIDA Data Provider Service names and IDs.
--
--  Name:  show_services.sql                        
--
--  Rem:   AIDA "Services" is a synonym for AIDA Data Provider. This
--         script displays the directory service IDs and names, of all
--         the AIDA Data Providers.
-- 
--  Usage: SQL> @show_services
--   e.g.: SQL> @show_services
--         > sqlplus {AIDADEV|AIDAPROD}/<password> @show_services
--
--  Auth:  03-Oct-2004, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--        dd-mmm-yyyy, Whoever (user): 
--            Whatever. 
--
--============================================================== 
set head on;
set wrap off;
set pagesize 1000;
select i.id, si.service_id, s.name from aida_interfaces i, aida_service_interface si, aida_services s where i.id=si.interface_id and si.service_id = s.id;
-- End of script
