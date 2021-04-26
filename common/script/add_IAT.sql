--==============================================================
--
--  Abs:   Inserts a new Instance/Attribute pair into AIDA
--
--  Name:  add_IAT.sql                        
--
--  Rem:   TRANSFORM is required by this SQL script. If the name you want to add has 
--         no transform use add_IA.sql instead.
--
--         **To remove an entry made with add_IAT (or add_IA), use remove_IA.sql**. This will
--         remove the entry made in aida_directory which was created by a trigger on insert 
--         into aida_names.
-- 
--  Usage: @add_IAT <service-id> <instance> <attribute> <transform>
--   e.g.: @add_IAT 5 "BPMS:IN20" "BSA.X1H.NAMES" "select unique instance from aida_names where instance like ''BPMS:IN20:%:X1H''"
--
--         To find the correct value of <service-id> use show_services.sql
--
--  Auth:  08-Jun-2008, Greg White (greg)
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
    '&2','&3','&4'
);

insert into AIDA_DIRECTORY (NAME_ID, SERVICE_ID) values 
(
	(select N.ID from AIDA_NAMES N where N.INSTANCE='&2' and N.ATTRIBUTE='&3'),
    &1
);

commit;

-- End of script
