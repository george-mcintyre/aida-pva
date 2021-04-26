--==============================================================
--
--  Abs:   Adds a Service to those known by an Aida Network's
--         Directory Service.
--
--  Name:  add_service.sql         
--
--  Rem:   This will not be run often, only when a new Aida Service
--          such as 'Test Service' is added to an Aida network. 
--
--         Note that the AIDA_SERVICE.ID field will be filled in 
--         automatically by a trigger, it should not be specified.
--
--  Usage: 
--   SQL> @add_service <name> <description> 
--   SQL> @add_service 'Test' ''AIDA Test Server (Hardcoded tests)' 
--
--  Auth:  28-Nov-2004, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--
--============================================================== 
DEFINE name='&1'
DEFINE descr='&2'
insert into AIDA_SERVICES (NAME, DESCRIPTION) VALUES 
(
    '&name','&descr'
);
