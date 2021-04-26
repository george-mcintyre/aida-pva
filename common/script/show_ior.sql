--==============================================================
--
--  Abs:   Gets the CORBA Interoperable Object Reference (IOR) of
--         the named AIDA service. This is what DaNameServer.getObjRef does.
--
--  Name:  show_ior.sql                        
--
--  Rem:   
--  Usage: @show_ior '<service-name>'
--   e.g.: @show_ior 'SLCModel'
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
--=============================================================
set serveroutput on size 1000000
set head off
set pagesize 0
set wrap on;
set verify off;

SELECT S.NAME, I.SOR
FROM AIDA_INTERFACES I, 
AIDA_SERVICE_INTERFACE SI, 
AIDA_SERVICES S
WHERE I.ID = SI.INTERFACE_ID
AND SI.SERVICE_ID = S.ID
AND S.NAME like '&1'
AND SI.GROUP_ID = 1;


-- End of script
