--==============================================================
--
--  Abs:   Inserts a new Instance/Attribute pair into AIDA
--
--  Name:  add_IA.sql                        
--
--  Rem:   TRANSFORM is always set to NULL by this script.
--         If you want a non-null TRANSFORM, clone this script and add
--         an argument.
-- 
--  Usage: @add_IA <service-id> <instance> <attribute> 
--   e.g.: @add_IA 63 'BPMS:PR02:8032' 'TWISS'
--
--         To find the correct value of <service-id> use show_services.sql
--
--  Auth:  04-Oct-2004, Greg White (greg)
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--        dd-mmm-yyyy, Whoever (user): 
--            Whatever. 
--
--============================================================== 

insert into AIDA_NAMES (INSTANCE, ATTRIBUTE, TRANSFORM) VALUES 
(
    '&2','&3',NULL
);

insert into AIDA_DIRECTORY (NAME_ID, SERVICE_ID) values 
(
	(select N.ID from AIDA_NAMES N where N.INSTANCE='&2' and N.ATTRIBUTE='&3'),
    &1
);

commit;

-- End of script
