--==============================================================
--
--  Abs:   Gets the AIDA Data Provider Service ID for a given 
--         Service name (eg, if given 'SLCModel', answer would be 63).
--
--  Name:  show_service_id.sql                        
--
--  Rem:   Given a service name, what's its ID? This is useful for 
--         associating new AIDA names (formally instane/attribute" pairs)
--         with a given service.
-- 
--  Usage: @show_service_id '<service-name>'
--   e.g.: @show_service_id 'SLCModel'
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

select ID from AIDA_SERVICES where (NAME='&1');

-- End of script
