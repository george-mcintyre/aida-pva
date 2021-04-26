--==============================================================
--
--  Abs:   Allows you to change the "transform" associated with 
--         an AIDA Instance//attribute name.
--
--  Name:  update_IAT.sql                        
--
--  Rem:   An Aida TRANSFORM is the query that is actally sent to 
--         the aida data provider server for the instance attribute. 
--         If the Transform for the instance//attribute in AIDA_NAMES
--         is null (as it is for the vst majority), then the query 
--         AIDA makes to the aida server is the instance//attribute
--         itself.
-- 
--  Usage: @update_IAT <instance> <attribute> <transform>
--   e.g.: @update_IAT 'LCLS' 'BSA.elements.byZ' 'SELECT ELEMENT, BEAMLINE, AREA, LINACZ_M, ROOT_NAME, SOURCE FROM LCLS_INFRASTRUCTURE.V_LCLS_BSA ORDER BY LINACZ_M'
--
--  Auth:  17-Jun-2008, Greg White (greg)
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--        dd-mmm-yyyy, Whoever (user): 
--            Whatever. 
--
--============================================================== 

update aida_names 
set transform = '&3'
where instance = '&1' and attribute = '&2';

-- End of script
